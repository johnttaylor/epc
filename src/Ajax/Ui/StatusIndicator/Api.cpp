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

#include "Api.h"
#include "Cpl/System/Trace.h"
#include "Ajax/Logging/Api.h"
#include "mp/ModelPoints.h"

#define SECT_ "Ajax::Ui::StatusIndicator"

using namespace Ajax::Ui::StatusIndicator;

#define BASE_FLASH_INTERVAL     50              // 50ms
#define FLASH_2HZ_INTERVAL      (1000/2/2)      // 2Hz flash rate -->edges @4Hz      
#define FLASH_1HZ_INTERVAL      (1000/2)        // 1Hz flash rate -->edges @2Hz      
#define FLASH_QTR_HZ_INTERVAL   (1000*2)        // 1/4Hz flash rate -->edges @1/2Hz      

#define STATE_ON                0
#define STATE_HEATER_ON         1
#define STATE_ALERTS            2
#define STATE_INVALID           3

#define ONE_PERCENT_PWM         ((0xFFFF/100) + 1)


///////////////////////////
Api::Api( Cpl::Dm::MailboxServer&        myMbox,
          Driver::LED::RedGreenBlue&     statusLED,
          Ajax::ScreenMgr::Navigation&   scrMgr )
    : Cpl::Itc::CloseSync( myMbox )
    , Cpl::System::Timer( myMbox )
    , m_ledDriver( statusLED )
    , m_scrMgr( scrMgr )
{
}

void Api::request( OpenMsg& msg )
{
    if ( m_opened )
    {
        Ajax::Logging::logf( Ajax::Logging::WarningMsg::OPEN_CLOSE, "open: " SECT_ );
        msg.returnToSender();
        return;
    }

    // Housekeeping
    m_opened = true;
    m_ledDriver.setOff();
    expired();

    msg.returnToSender();
}

/// Shutdowns the component
void Api::request( CloseMsg& msg )
{
    if ( !m_opened )
    {
        Ajax::Logging::logf( Ajax::Logging::WarningMsg::OPEN_CLOSE, "close: " SECT_ );
        msg.returnToSender();
        return;
    }

    // Housekeeping
    m_opened         = false;
    m_firstExecution = true;
    m_state          = STATE_INVALID;
    m_ledDriver.setOff();

    msg.returnToSender();
}

///////////////////////////
void Api::expired() noexcept
{
    uint32_t now = Cpl::System::ElapsedTime::milliseconds();

    // Initialize the intervals (but only once)
    if ( m_firstExecution )
    {
        // Round down to the nearest interval boundary
        m_firstExecution      = false;
        m_timeMarker2Hz       = (now / FLASH_2HZ_INTERVAL) * FLASH_2HZ_INTERVAL;
        m_timeMarker1Hz       = (now / FLASH_1HZ_INTERVAL) * FLASH_1HZ_INTERVAL;
        m_timeMarkerQuarterHz = (now / FLASH_QTR_HZ_INTERVAL) * FLASH_QTR_HZ_INTERVAL;
        m_2HzOnCycle          = false;
        m_1HzOnCycle          = false;
        m_quarterHzOnCycle    = false;
    }

    // Check Halt state
    if ( !setHaltError( now ) )
    {
        // No Halt-error -->go process the nominal Status indicators
        setStatus( now );
    }

    // Restart my interval timer
    Timer::start( BASE_FLASH_INTERVAL );
}

///////////////////////////
bool Api::setHaltError( uint32_t now ) noexcept
{
    // Has the 2Hz interval expired?
    if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timeMarker2Hz, FLASH_2HZ_INTERVAL, now ) )
    {
        // Set the marker to the next interval
        m_timeMarker2Hz += FLASH_2HZ_INTERVAL;

        // Is there a 'hard-error'
        if ( m_scrMgr.isCurrentScreenHaltError() )
        {
            // Transition to off
            if ( m_2HzOnCycle )
            {
                m_ledDriver.setOff();
            }
            // Transition to on
            else
            {
                m_ledDriver.setRgb( 255, 0, 0 );
            }
            m_2HzOnCycle = !m_2HzOnCycle;

            // There can only be one 'source' for the RGB LED
            return true;
        }
    }

    return false;
}
void Api::setStatus( uint32_t now ) noexcept
{
    // Default to the LED solid white
    unsigned state = STATE_INVALID;

    // Determine my 'state' (but wait till the UI has transition OFF of the splash screen)
    if ( m_scrMgr.getCurrentScreen() != nullptr )
    {
        Ajax::Dm::MpAlertSummary::Data alertSummary;
        uint32_t                       heaterPWM;
        if ( mp::alertSummary.read( alertSummary ) && mp::cmdHeaterPWM.read( heaterPWM ) )
        {
            if ( alertSummary.count > 0 )
            {
                state = STATE_ALERTS;

                // On the initial transition to the state -->turn the LED on
                if ( m_state != state )
                {
                    m_ledDriver.setRgb( 255, 0, 0 );
                }
            }
            else if ( heaterPWM >= ONE_PERCENT_PWM / 2 ) // PWM must be at least >= 0.5%
            {
                state = STATE_HEATER_ON;

                // On the initial transition to the state -->turn the LED on
                if ( m_state != state )
                {
                    m_ledDriver.setRgb( 0, 0, 255 );
                }
            }
            else
            {
                state = STATE_ON;
            }

            m_state = state;
        }
    }

    // 1HZ
    if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timeMarker1Hz, FLASH_1HZ_INTERVAL, now ) )
    {
        // Set the marker to the next interval
        m_timeMarker1Hz += FLASH_1HZ_INTERVAL;

        // ALERT - flashing RED
        if ( state == STATE_ALERTS )
        {
            // Transition to off
            if ( m_1HzOnCycle )
            {
                m_ledDriver.setOff();
            }
            // Transition to on
            else
            {
                m_ledDriver.setRgb( 255, 0, 0 );
            }
            m_1HzOnCycle = !m_1HzOnCycle;
        }
    }

    // 1/4 HZ
    if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timeMarkerQuarterHz, FLASH_QTR_HZ_INTERVAL, now ) )
    {
        // Set the marker to the next interval
        m_timeMarkerQuarterHz += FLASH_QTR_HZ_INTERVAL;

        // HEATER ON - slow BLUE flash
        if ( state == STATE_HEATER_ON )
        {
            // Transition to off
            if ( m_quarterHzOnCycle )
            {
                m_ledDriver.setOff();
            }
            // Transition to on
            else
            {
                m_ledDriver.setRgb( 0, 0, 255 );
            }
            m_quarterHzOnCycle = !m_quarterHzOnCycle;
        }
    }

    // Unit on, but not heating
    if ( state == STATE_ON )
    {
        // Heater is off -->set to Green
        m_ledDriver.setRgb( 0, 255, 0 );
    }

    // Cannot determine state -->(should only happen when on the splash screen)
    else if ( state == STATE_INVALID )
    {
        m_ledDriver.setRgb( 255, 255, 255 );
    }
}
