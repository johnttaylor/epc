/*-----------------------------------------------------------------------------
* COPYRIGHT_HEADER_TO_BE_FILLED_LATER
*----------------------------------------------------------------------------*/
/** @file */


#include "Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Text/strip.h"
#include "Cpl/Text/atob.h"
#include "Driver/AIO/Eros/Thermistor.h"

using namespace Eros::Test::Thermistor;


void getDisplayIdt( int32_t srcTemp, int32_t& dstInteger, int32_t& dstFractional ) noexcept
{
    srcTemp += 5;   // Round 'hundredths' of degree to 'tenths'
    dstFractional = (srcTemp % 100) / 10;
    if ( dstFractional < 0 )
    {
        dstFractional *= -1;
    }
    dstInteger    = srcTemp / 100;
}

/////////////////////////////////////////////////////
Api::Api( Cpl::Container::SList<MAppApi>&    mappList,
          Cpl::Dm::MailboxServer&          myMbox,
          Cpl::Dm::Mp::ArrayInt32<2>&      mpDriverSamples,
          const char*                      name )
    : MApp_( mappList, name, DESCRIPTION, USAGE )
    , m_mpSample( mpDriverSamples )
    , m_obSample( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::changedADCSample )
{
}


void Api::intialize_() noexcept
{
    // Nothing needed
}

void Api::shutdown_() noexcept
{
    // Ensure I disconnect from the model point
    m_mpSample.detach( m_obSample );
}

bool Api::start_( char* args ) noexcept
{

    // Parse my command line args
    if ( m_started || !parse( args ) )
    {
        return false;
    }

    CPL_SYSTEM_TRACE_MSG( OPTION_CPL_MAPP_TRACE_SECTION, ("%s: Configuration: displayMs=%u ms",
                                                           m_name,
                                                           m_displayMs) );

    // Housekeeping
    m_started      = true;
    m_maxTemp      = INT32_MIN;
    m_minTemp      = INT32_MAX;
    m_sumTemp      = 0;
    m_maxADCBits   = 0;
    m_minADCBits   = UINT32_MAX;
    m_sumADCBits   = 0;
    m_numSamples   = 0;
    m_invalidData  = false;
    m_timeMarkerMs = Cpl::System::ElapsedTime::milliseconds();

    m_mpSample.attach( m_obSample );
    return true;
}

void Api::stop_() noexcept
{
    if ( m_started )
    {
        // Stop my timer
        m_mpSample.detach( m_obSample );

        // Housekeeping
        m_started = false;
    }
}

bool Api::parse( char* args ) noexcept
{
    // Default the arguments
    m_displayMs  = OPTION_EROS_TEST_THERMISTOR_DISPLAY_INTERVAL_MS;

    // No arguments
    const char* arg1 = Cpl::Text::stripSpace( args );
    if ( *arg1 == '\0' )
    {
        return true;
    }

    // Display time
    unsigned long value;
    const char*   endPtr;
    if ( !Cpl::Text::a2ul( value, arg1, 10, " ", &endPtr ) )
    {
        return false;
    }
    m_displayMs = (uint32_t) value;
    return true;
}

void Api::changedADCSample( Cpl::Dm::Mp::ArrayInt32<2>& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    // Read the latest sample
    int32_t vals[2];
    if ( mp.readAndSync( vals, 2, clientObserver ) )
    {
        m_invalidData = false;

        // Metrics
        int32_t  temperature = vals[Driver::AIO::Eros::Thermistor::IDX_TEMPERATURE];
        uint32_t adcBits     = (uint32_t) (vals[Driver::AIO::Eros::Thermistor::IDX_ADC_BITS]);
        if ( temperature < m_minTemp )
        {
            m_minTemp = temperature;
        }
        else if ( temperature > m_maxTemp )
        {
            m_maxTemp = temperature;
        }
        if ( adcBits < m_minADCBits )
        {
            m_minADCBits = adcBits;
        }
        else if ( adcBits > m_maxADCBits )
        {
            m_maxADCBits = adcBits;
        }

        // Running totals
        m_numSamples++;
        m_sumTemp    += temperature;
        m_sumADCBits += adcBits;

        uint32_t now = Cpl::System::ElapsedTime::milliseconds();
        if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timeMarkerMs, m_displayMs, now ) || m_numSamples == 1 )
        {
            m_timeMarkerMs = now;

            int32_t avgTemp = (int32_t)( m_sumTemp / m_numSamples);
            int32_t avgInteger, avgTenths;
            getDisplayIdt( avgTemp, avgInteger, avgTenths );

            int32_t tempInteger, tempTenths;
            getDisplayIdt( temperature, tempInteger, tempTenths );

            int32_t maxInteger, maxTenths;
            getDisplayIdt( m_maxTemp, maxInteger, maxTenths );

            int32_t minInteger, minTenths;
            getDisplayIdt( m_minTemp, minInteger, minTenths );
           
            uint32_t avgBits = (uint32_t)(m_sumADCBits / m_numSamples);
            CPL_SYSTEM_TRACE_MSG( OPTION_CPL_MAPP_TRACE_SECTION, ("%s: 'F= %02d.%d (%02d.%d %02d.%d %02d.%d), ADC= %4u (%4u %4u %4u)",
                                                                   m_name,
                                                                   tempInteger, tempTenths,
                                                                   avgInteger, avgTenths,
                                                                   minInteger, minTenths,
                                                                   maxInteger, maxTenths,
                                                                   adcBits,
                                                                   avgBits,
                                                                   m_minADCBits,
                                                                   m_maxADCBits) );
        }
    }
    else if ( !m_invalidData )
    {
        m_invalidData = true;
        CPL_SYSTEM_TRACE_MSG( OPTION_CPL_MAPP_TRACE_SECTION, ("%s: <data invalid>", m_name) );
    }
}

