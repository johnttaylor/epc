/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2023  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "Api.h"
#include "Ajax/Logging/Api.h"
#include "mp/ModelPoints.h"
#include "Cpl/System/Trace.h"


#define SECT_ "Ajax::Heating::Io"


using namespace Ajax::Heating::Io;

//////////////////////////////////
Api::Api( Cpl::Dm::MailboxServer&  myMbox,
          Driver::DIO::Pwm&        driverHeaterPwm,
          Driver::DIO::Pwm&        driverFanPwm,
          Driver::DIO::In&         driverHwSafetyIn ) noexcept
    : Cpl::Itc::CloseSync( myMbox )
    , Cpl::System::Timer( myMbox )
    , m_obHeaterPWM( myMbox, *this, &Api::heaterPWMChanged )
    , m_obFanPWM( myMbox, *this, &Api::fanPWMChanged )
    , m_driverHeaterPwm( driverHeaterPwm )
    , m_driverFanPwm( driverFanPwm )
    , m_driverHwSafetyIn( driverHwSafetyIn )
    , m_opened( false )
{
}


//////////////////////////////////
void Api::request( OpenMsg& msg )
{
    if ( !m_opened )
    {
        // Housekeeping
        m_opened         = true;
        m_firstExecution = true;

        // Start processing
        expired();
    }

    msg.returnToSender();
}

void Api::request( CloseMsg& msg )
{
    if ( m_opened )
    {
        // Housekeeping
        m_opened = false;
        Timer::stop();
    
        // Put the heating system into a 'safe' state
        m_driverHeaterPwm.setDutyCycle( 0 );
        m_driverFanPwm.setDutyCycle( 0 );
        mp::hwSafetyLimit.setInvalid();
    }

    msg.returnToSender();
}

////////////////////////////////
void Api::heaterPWMChanged( Cpl::Dm::Mp::Uint32& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    uint32_t pwmVal;
    if ( mp.readAndSync( pwmVal, clientObserver ) )
    {
        m_driverHeaterPwm.setDutyCycle( pwmVal );
    }
}

void Api::fanPWMChanged( Cpl::Dm::Mp::Uint32& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    uint32_t pwmVal;
    if ( mp.readAndSync( pwmVal, clientObserver ) )
    {
        m_driverFanPwm.setDutyCycle( pwmVal );
    }
}

void Api::expired() noexcept
{
    uint32_t now = Cpl::System::ElapsedTime::milliseconds();

    // Initialize the interval (but only once)
    if ( m_firstExecution )
    {
        // Round down to the nearest interval boundary
        m_firstExecution = false;
        m_timeMarker     = (now / OPTION_AJAX_HEATING_IO_ALGO_INTERVAL_MS) * OPTION_AJAX_HEATING_IO_ALGO_INTERVAL_MS;
    }

    // Has the interval expired?
    if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timeMarker, OPTION_AJAX_HEATING_IO_ALGO_INTERVAL_MS, now ) )
    {
        // Execute the algorithm
        m_timeMarker += OPTION_AJAX_HEATING_IO_ALGO_INTERVAL_MS;

        // Read INPUT(s)
        bool hwSafety = m_driverHwSafetyIn.sample();
        mp::hwSafetyLimit.write( hwSafety );
    }

    // Restart my interval timer - just needs to be 'faster' than the actual polling rate
    Timer::start( OPTION_AJAX_HEATING_IO_ALGO_INTERVAL_MS / 4 );
}

