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


#define SECT_ "Ajax::Heating::Supervisor"

using namespace Ajax::Heating::Supervisor;

//////////////////////////////////
Api::Api( Cpl::Dm::MailboxServer& myMbox,
          size_t                   maxHeaterPWM,
          size_t                   maxFanPWM )
    : Cpl::Itc::CloseSync( myMbox )
    , Cpl::System::Timer( myMbox )
    , m_flcController( mp::flcConfig )
    , m_obRemoteIdt( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::remoteIdtChanged )
    , m_obOnboardIdt( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::onboardIdtChanged )
    , m_obHwSafety( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::hwSafetyChanged )
    , m_obHeatingEnabled( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::heatingEnabledChanged )
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
        m_remoteAvail  = false;
        m_onboardAvail = false;
        heatOff();

        // Initialize the Fuzzy Logic controller
        m_flcController.start();

        // Brute force my FSM (allows restarting)
        m_initialized=static_cast<int>(0U);
        initialize();

        // Subscribe to my MP
        mp::remoteIdt.attach( m_obRemoteIdt );
        mp::onBoardIdt.attach( m_obOnboardIdt );
        mp::hwSafetyLimit.attach( m_obHwSafety );
        mp::heatingMode.attach( m_obHeatingEnabled );

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

        // Cancel my subscriptions
        mp::remoteIdt.detach( m_obRemoteIdt );
        mp::onBoardIdt.detach( m_obOnboardIdt );
        mp::hwSafetyLimit.detach( m_obHwSafety );
        mp::heatingMode.detach( m_obHeatingEnabled );

        // Shutdown stuffs...
        heatOff();
        Timer::stop();
        m_flcController.stop();
    }

    msg.returnToSender();
}

////////////////////////////////
void Api::expired() noexcept
{
    // 'Run' the FSM
    generateEvent( FSM_NO_MSG );

    // Restart my interval timer
    // Note: There will be jitter in the software timer, but the FLC algorithm
    //       is not overly sensitive to jitter in its periodic interval.
    Timer::start( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS );
}

void Api::remoteIdtChanged( Cpl::Dm::Mp::Int32& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    int32_t notUsed;
    if ( mp.readAndSync( notUsed, m_obRemoteIdt ) )
    {
        // Only fire the event when transitioning to at least one working sensor
        if ( !m_remoteAvail && !m_onboardAvail )
        {
            generateEvent( Fsm_evSensorAvailable );
        }
        m_remoteAvail = true;
    }
    else
    {
        // I am transitioning to no sensors working...
        if ( m_remoteAvail && !m_onboardAvail )
        {
            generateEvent( Fsm_evNoTempSensor );
        }
        m_remoteAvail = false;
    }
}

void Api::onboardIdtChanged( Cpl::Dm::Mp::Int32& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    int32_t notUsed;
    if ( mp.readAndSync( notUsed, m_obOnboardIdt ) )
    {
        // Only fire the event when transitioning to at least one working sensor
        if ( !m_remoteAvail && !m_onboardAvail )
        {
            generateEvent( Fsm_evSensorAvailable );
        }
        m_onboardAvail = true;
    }
    else
    {
        // I am transitioning to no sensors working...
        if ( !m_remoteAvail && m_onboardAvail )
        {
            generateEvent( Fsm_evNoTempSensor );
        }
        m_onboardAvail = false;
    }
}

void Api::hwSafetyChanged( Cpl::Dm::Mp::Bool& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    bool safetyTripped;
    if ( mp.readAndSync( safetyTripped, m_obHwSafety ) )
    {
        if ( safetyTripped )
        {
            generateEvent( Fsm_evHiTemp );
        }
        else
        {
            generateEvent( Fsm_evSafeTemp );
        }
    }
}

void Api::heatingEnabledChanged( Cpl::Dm::Mp::Bool& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    bool enabled;
    if ( mp.readAndSync( enabled, m_obHeatingEnabled ) )
    {
        if ( enabled )
        {
            generateEvent( Fsm_evEnabled );
        }
        else
        {
            generateEvent( Fsm_evDisabled );
        }
    }
}

///////////////////////////
void Api::clearHiTempAlert() noexcept
{
    if ( mp::failedSafeAlert.isNotValid() == false )
    {
        Ajax::Logging::logf( Ajax::Logging::AlertMsg::FAILED_SAFE, "Cleared" );
    }
    mp::failedSafeAlert.lowerAlert();
}

void Api::clearSensorAlert() noexcept
{
    if ( mp::sensorFailAlert.isNotValid() == false )
    {
        Ajax::Logging::logf( Ajax::Logging::AlertMsg::NO_TEMPERATURE_SENSOR, "Cleared" );
    }
    mp::sensorFailAlert.lowerAlert();
}

void Api::heatOff() noexcept
{
    // Shut everything off
    mp::cmdHeaterPWM.write( 0 );
    mp::cmdFanPWM.write( 0 );
}

void Api::raiseHiTempAlert() noexcept
{
    if ( mp::failedSafeAlert.isNotValid() )
    {
        Ajax::Logging::logf( Ajax::Logging::AlertMsg::FAILED_SAFE, "RAISED" );
    }
    mp::failedSafeAlert.raiseAlert();
}

void Api::raiseSensorAlert() noexcept
{
    if ( mp::sensorFailAlert.isNotValid() )
    {
        Ajax::Logging::logf( Ajax::Logging::AlertMsg::NO_TEMPERATURE_SENSOR, "RAISED" );
    }
    mp::sensorFailAlert.raiseAlert();
}

void Api::runHeatingAlgo() noexcept
{
    int32_t     currentTemp;
    if ( getTemperature( currentTemp ) )
    {
        int32_t setpoint;
        if ( mp::heatSetpoint.read( setpoint ) )
        {
            // Run the FLC and scale the output to the PWM range 
            int32_t delta       = m_flcController.calcChange( currentTemp, setpoint );
            int32_t scaledDelta = (delta * m_maxHeaterPWM) / m_maxHeaterPWM;

            // Update commanded Heater output
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

            // Set Fan speed (Note: Fan only runs when the heater is 'on')
            uint32_t newFanPWM = 0;
            if ( m_heaterOutPWM > 0 )
            {
                Ajax::Type::FanMode fanMode       = Ajax::Type::FanMode::eHIGH; // Default to high speed
                uint32_t            fanPercentage = 0;
                mp::fanMode.read( fanMode );
                switch ( fanMode )
                {
                case Ajax::Type::FanMode::eLOW:
                    mp::fanLowPercentage.read( fanPercentage );
                    break;
                case Ajax::Type::FanMode::eMEDIUM:
                    mp::fanMedPercentage.read( fanPercentage );
                    break;
                default:
                    mp::fanHighPercentage.read( fanPercentage );
                    break;
                }

                // Convert percentage to a actual PWM duty cycle value
                newFanPWM = fanPercentage * m_maxFanPWM) / 1000;
            }
            
            mp::cmdFanPWM.write( newFanPWM );
            CPL_SYSTEM_TRACE_MSG( SECT_, ("idt=%ld, setpt=%ld, err=%ld.  flc=%ld, scaled=%ld.  newHeaterPWM=%lu, newFanPWM=%lu", 
                                           currentTemp,
                                           setpoint,
                                           setpoint - currentTemp,
                                           delta, 
                                           scaledDelta, 
                                           m_heaterOutPWM, 
                                           newFanPWM) );
        }
        
        // No setpoint value for SOME reason 
        else
        {
            // This should NEVER happen -->give-up and reboot
            Cpl::System::FatalError::log( "HeatingAlgo: Setpoint MP is invalid" );
        }
    }
}

bool Api::isSensorAvailable() noexcept
{
    return m_remoteAvail || m_onboardAvail;
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
    return false;
}
