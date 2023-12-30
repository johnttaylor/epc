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
/** @file */

#include "Thermistor.h"
#include "Ajax/Logging/Api.h"
#include "Cpl/System/Trace.h"
#include <stdint.h>
#include <stdlib.h>

#define SECT_ "Driver::AIO::Ajax"


using namespace Driver::AIO::Ajax;

#define ROUNDING_DIVIDER    100
#define ROUNDING_OFFSET     (ROUNDING_DIVIDER/2)


// Look-up table (one extra entry to simplify interpolation)
#define NUM_TABLE_ENTRIES   64
int32_t bitsToF_[NUM_TABLE_ENTRIES + 1] = // Values are in ten-thousands degrees 'F, i.e. has 4 fractional digits
{
    3000000,
    1613400,
    1450700,
    1357900,
    1292800,
    1242400,
    1201100,
    1166100,
    1135600,
    1108500,
    1084000,
    1061600,
    1040900,
    1021700,
    1003600,
    986600,
    970400,
    955000,
    940200,
    926000,
    912300,
    899000,
    886200,
    873600,
    861300,
    849300,
    837500,
    825900,
    814500,
    803200,
    792000,
    780900,
    769900,
    758900,
    748000,
    737000,
    726100,
    715000,
    704000,
    692800,
    681500,
    670100,
    658500,
    646700,
    634700,
    622300,
    609700,
    596700,
    583300,
    569400,
    554800,
    539700,
    523700,
    506700,
    488600,
    469100,
    447900,
    424400,
    398100,
    367800,
    331800,
    286900,
    225900,
    126100,
    0
};


////////////////////////////////////////////////
Thermistor::Thermistor( Cpl::Dm::MailboxServer &    myMbox,
                        DriverAIOHalSingleInput_T   adcDriver,
                        Cpl::Dm::Mp::Int32&         mpTemperatureOutput )
    : Cpl::Itc::CloseSync( myMbox )
    , Cpl::System::Timer( myMbox )
    , m_mpTempOut( mpTemperatureOutput )
    , m_adcDriver( adcDriver )
    , m_opened( false )
{
}


//////////////////////////////////
void Thermistor::request( OpenMsg& msg )
{
    if ( !m_opened )
    {
        // Housekeeping
        m_opened = true;

        // Start processing
        uint8_t numBits = Driver_AIO_HalSingle_setADCSize( m_adcDriver, OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION );
        if ( numBits != OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION )
        {
            // This Error is bad!  Fail the driver.
            ::Ajax::Logging::logf( ::Ajax::Logging::CriticalMsg::DRIVER, "Hardware platform does not support %d ADC Bites (%d)", OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION, numBits );
            m_mpTempOut.setInvalid();
            m_opened = false;
        }
        else
        {
            expired();
        }
    }

    msg.returnToSender();
}

void Thermistor::request( CloseMsg& msg )
{
    if ( m_opened )
    {
        // Housekeeping
        m_opened = false;

        // Shutdown stuffs...
        Timer::stop();
    }

    msg.returnToSender();
}

//////////////////////////////////
void Thermistor::expired( void ) noexcept
{

    // Take N raw samples
    uint32_t samples[OPTION_DRIVER_AIO_AJAX_NUM_SAMPLES_TO_AVG];
    uint32_t sum = 0;
    for ( unsigned i=0; i < OPTION_DRIVER_AIO_AJAX_NUM_SAMPLES_TO_AVG; i++ )
    {
        if ( !Driver_AIO_HalSingleInput_sample( m_adcDriver, samples[i] ) )
        {
            // Error -->log the error, mark the temperature as invalid, and stop further processing!
            ::Ajax::Logging::logf( ::Ajax::Logging::CriticalMsg::DRIVER, "ADC Read failed!" );
            m_mpTempOut.setInvalid();
            Timer::stop();
            return;
        }
        sum += samples[i];
    }

    // Average the samples and convert to a table index
    long   avg  = sum / OPTION_DRIVER_AIO_AJAX_NUM_SAMPLES_TO_AVG;
    ldiv_t r    = ldiv( avg, NUM_TABLE_ENTRIES );
    long   idx0 = r.quot;
    if ( idx0 >= NUM_TABLE_ENTRIES )
    {
        idx0 = NUM_TABLE_ENTRIES - 1;
    }

    // Check for open/shorted conditions
    if ( avg > OPTION_DRIVER_AIO_AJAX_ADC_MAX_RAIL_BITS || avg < OPTION_DRIVER_AIO_AJAX_ADC_MIN_RAIL_BITS )
    {
        m_mpTempOut.setInvalid();
    }

    // Convert bits to temperature
    else
    {
        // Note: Table entries have a HIGHER precision than the MP which in is hundredths of degree
        int32_t base = bitsToF_[idx0];
        int32_t val  = base +  (r.rem * (bitsToF_[idx0 + 1] - base) / NUM_TABLE_ENTRIES);
        m_mpTempOut.write( (val+ ROUNDING_OFFSET) / ROUNDING_DIVIDER );
    }

    // Restart the delay timer
    Timer::start( OPTION_DRIVER_AIO_AJAX_SAMPLE_TIME_MS );
}

