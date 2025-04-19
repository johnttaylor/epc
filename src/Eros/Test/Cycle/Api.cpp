/*-----------------------------------------------------------------------------
* COPYRIGHT_HEADER_TO_BE_FILLED_LATER
*----------------------------------------------------------------------------*/
/** @file */


#include "Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Text/strip.h"
#include "Cpl/Text/atob.h"

using namespace Eros::Test::Cycle;

/////////////////////////////////////////////////////
Api::Api( Cpl::Container::SList<MAppApi>& mappList,
          Cpl::Dm::MailboxServer&         myMbox,
          Driver::DIO::Pwm&               heaterPWMDriver,
          Driver::DIO::Pwm&               fanPWMDriver )
    : MApp_( mappList, MAPP_NAME, DESCRIPTION, USAGE )
    , Cpl::System::Timer( myMbox )
    , m_heaterDriver( heaterPWMDriver )
    , m_fanDriver( fanPWMDriver )
{
}


void Api::intialize_() noexcept
{
    // Nothing needed
}

void Api::shutdown_() noexcept
{
    // Ensure that everything is stopped
    Cpl::System::Timer::stop();
}

bool Api::start_( char* args ) noexcept
{

    // Parse my command line args
    if ( m_started || !parse( args ) )
    {
        return false;
    }

    CPL_SYSTEM_TRACE_MSG( OPTION_CPL_MAPP_TRACE_SECTION, ( "%s: Configuration: hpwm=%u fpwm=%u, onTime=%u, offTime=%lu, repeat=%u", m_name, m_heaterPWM, m_fanPWM, m_onTimeMs, m_offTimeMs, m_maxCycles ) );

    // Housekeeping
    m_started      = true;
    m_inOnCycle    = false;
    m_firstCycle   = true;
    m_cycleCount   = 0;
    m_timeMarkerMs = Cpl::System::ElapsedTime::milliseconds();

    // Execute duty cycle logic
    expired();
    return true;
}

void Api::stop_() noexcept
{
    if ( m_started )
    {
        // Stop my timer
        Cpl::System::Timer::stop();
        m_heaterDriver.setDutyCycle( 0 );
        m_fanDriver.setDutyCycle( 0 );

        // Housekeeping
        m_started = false;
    }
}

bool Api::parse( char* args ) noexcept
{
    // Default the arguments
    m_heaterPWM = 100;
    m_fanPWM    = 100;
    m_onTimeMs  = 60 * 1000;
    m_offTimeMs = 60 * 1000;
    m_maxCycles = 5;

    // No arguments
    const char* arg1 = Cpl::Text::stripSpace( args );
    if ( *arg1 == '\0' )
    {
        return true;
    }

    // Heater PWM time
    unsigned long value;
    const char*   endPtr;
    if ( !Cpl::Text::a2ul( value, arg1, 10, " ", &endPtr ) || value > 100 )
    {
        return false;
    }
    m_heaterPWM = value;

    // Fan PWM time
    const char* arg2 = Cpl::Text::stripSpace( endPtr );
    if ( *arg2 == '\0' )
    {
        return true;
    }
    if ( !Cpl::Text::a2ul( value, arg2, 10, " ", &endPtr ) || value > 100 )
    {
        return false;
    }
    m_fanPWM = value;

    // On time
    const char* arg3 = Cpl::Text::stripSpace( endPtr );
    if ( *arg3 == '\0' )
    {
        return true;
    }
    if ( !Cpl::Text::a2ul( value, arg3, 10, " ", &endPtr ) )
    {
        return false;
    }
    m_onTimeMs = value;

    // Off time
    const char* arg4 = Cpl::Text::stripSpace( endPtr );
    if ( *arg4 == '\0' )
    {
        return true;
    }
    if ( !Cpl::Text::a2ul( value, arg4, 10, " ", &endPtr ) )
    {
        return false;
    }
    m_offTimeMs = value;

    // Off time
    const char* arg5 = Cpl::Text::stripSpace( endPtr );
    if ( *arg5 == '\0' )
    {
        return true;
    }
    if ( !Cpl::Text::a2ul( value, arg5, 10, " ", &endPtr ) )
    {
        return false;
    }
    m_maxCycles = value;
    return true;
}

void Api::expired( void ) noexcept
{
    if ( m_cycleCount < m_maxCycles )
    {
        uint32_t now = Cpl::System::ElapsedTime::milliseconds();

        // ON CYCLE
        if ( m_inOnCycle )
        {
            // Transition to OFF cycle
            if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timeMarkerMs, m_onTimeMs, now ) )
            {
                m_inOnCycle    = false;
                m_timeMarkerMs = now;
                m_cycleCount++;
                m_heaterDriver.setDutyCycle( 0 );
                m_fanDriver.setDutyCycle( 0 );

                CPL_SYSTEM_TRACE_MSG( OPTION_CPL_MAPP_TRACE_SECTION, ( "OFF Cycle#%u. OffTime=%lu", m_cycleCount, m_offTimeMs ) );
            }
        }

        // ON CYCLE
        else
        {
            // Transition to ON cycle
            if ( m_firstCycle || Cpl::System::ElapsedTime::expiredMilliseconds( m_timeMarkerMs, m_offTimeMs, now ) )
            {
                m_inOnCycle    = true;
                m_timeMarkerMs = now;
                m_firstCycle   = false;
                m_heaterDriver.setDutyCycle( m_heaterPWM );
                m_fanDriver.setDutyCycle( m_fanPWM );

                CPL_SYSTEM_TRACE_MSG( OPTION_CPL_MAPP_TRACE_SECTION, ( "ON  Cycle#%u. HPWM=%d %, FPWM=%d %, OnTime=%lu", m_cycleCount + 1, m_heaterPWM, m_fanPWM, m_onTimeMs ) );
            }
        }

        // Restart my timer
        Cpl::System::Timer::start( OPTION_EROS_TEST_CYCLE_INTERVAL_MS );
    }
    else
    {
        CPL_SYSTEM_TRACE_MSG( OPTION_CPL_MAPP_TRACE_SECTION, ( "All Cycles completed (%u). To exit the test type: mapp stop %s", m_cycleCount, MAPP_NAME ) );
    }
}
