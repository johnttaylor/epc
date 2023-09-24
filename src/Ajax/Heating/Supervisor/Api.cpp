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

using namespace Ajax::Heating::Supervisor;

//////////////////////////////////
Api::Api( Cpl::Dm::MailboxServer& myMbox,
          Ajax::Heating::Flc::Api& heatingController,
          size_t                   maxHeaterPWM,
          size_t                   maxFanPWM )
    : Cpl::Itc::CloseSync( myMbox )
    , Cpl::System::Timer( myMbox )
    , m_flcController( heatingController )
    , m_maxHeaterPWM( maxHeaterPWM )
    , m_maxFanPWM( maxFanPWM )
    , m_opened( false )
{
}


//////////////////////////////////
void Api::request( OpenMsg& msg )
{
    if ( !m_opened )
    {
        // Housekeeping
        m_opened       = true;
        m_heaterOutPWM = 0;

        // Initialize the Fuzzy Logic controller
        m_flcController.start();

        // Brute force my FSM (allows restarting)
        m_initialized=static_cast<int>(0U);
        initialize();

        // TODO: Subscribe to Safety limit MP. -->add changeCb() -->generate evSafetyLimit event
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

        // Shutdown stuffs...
        Timer::stop();
        m_flcController.stop();
    }

    msg.returnToSender();
}

void Api::expired() noexcept
{
    // 'Run' the FSM
    generateEvent( FSM_NO_MSG );

    // Restart my interval timer
    // Note: There will be jitter in the software timer, but the FLC algorithm
    //       is not overly sensitive to jitter in its periodic interval.
    Timer::start( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS );
}


///////////////////////////
void Api::clearHiTempAlert() noexcept
{
    mp::failedSafeAlert.lowerAlert();
    Ajax::Logging::logf( Ajax::Logging::AlertMsg::FAILED_SAFE, "Cleared" );
}

void Api::clearSensorAlert() noexcept
{
    mp::sensorFailAlert.lowerAlert();
    Ajax::Logging::logf( Ajax::Logging::AlertMsg::NO_TEMPERATURE_SENSOR, "Cleared" );
}

void Api::heatOff() noexcept
{
    // Shut everything off
    mp::cmdHeaterPWM.write( 0 );
    mp::cmdFanPWM.write( 0 );
}

void Api::raiseHiTempAlert() noexcept
{
    mp::failedSafeAlert.raiseAlert();
    Ajax::Logging::logf( Ajax::Logging::AlertMsg::FAILED_SAFE, "RAISED" );
}

void Api::raiseSensorAlert() noexcept
{
    mp::sensorFailAlert.raiseAlert();
    Ajax::Logging::logf( Ajax::Logging::AlertMsg::NO_TEMPERATURE_SENSOR, "RAISED" );
}

void Api::runHeatingAlgo() noexcept
{
    int32_t currentTemp;
    if ( getTemperature( currentTemp ) )
    {
        int32_t setpoint;
        if ( mp::heatSetpoint.read( setpoint ) )
        {
            // Run the FLC and scale the output to the PWM range 
            int32_t delta = m_flcController.calcChange( currentTemp, setpoint );
            int32_t scaledDelta = (delta * m_maxHeaterPWM) / m_maxHeaterPWM;

            // Update commanded output
            m_heaterOutPWM += scaledDelta;
            if ( m_heaterOutPWM < 0 )
            {
                m_heaterOutPWM = 0;
            }
            else if ( m_heaterOutPWM > m_maxHeaterPWM )
            {
                m_heaterOutPWM = m_maxHeaterPWM;
            }
            mp::cmdHeaterPWM.write( m_heaterOutPWM );
        }
        
        // No setpoint value for SOME reason!
        else
        {
            // TODO: What?
        }
    }

}

bool Api::isSensorAvailable() noexcept
{
    int32_t notUsed;
    return getTemperature( notUsed );
}


////////////////////////////////////
bool Api::getTemperature( int32_t& idt ) noexcept
{
    // NOTE: Until the system actually has a working 'remote sensor' - the follow logic will always 'fail'
    if ( mp::remoteIdt.read( idt ) )
    {
        return true;
    }

    // Try the built-in sensor
    if ( mp::onBoardIdt.read( idt ) )
    {
        return true;
    }

    // If I get here, there is no available temperature sensor
    generateEvent( Fsm_evNoTempSensor );
    return false;
}
