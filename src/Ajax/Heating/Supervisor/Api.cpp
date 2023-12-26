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

#define MAX_PWM     ((uint64_t)0xFFFF)

using namespace Ajax::Heating::Supervisor;

//////////////////////////////////
Api::Api( Cpl::Dm::MailboxServer&  myMbox ) noexcept
    : Cpl::Itc::CloseSync( myMbox )
    , Cpl::System::Timer( myMbox )
    , m_flcController( mp::flcConfig )
    , m_obHwSafety( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::hwSafetyChanged )
    , m_obHeatingEnabled( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::heatingEnabledChanged )
    , m_opened( false )
{
}


//////////////////////////////////
void Api::request( OpenMsg& msg )
{
    if ( !m_opened )
    {
        // Housekeeping
        m_opened                     = true;
        m_firstExecution             = true;
        m_temperatureSensorAvailable = true; // If there is no sensor actually available, the runHeatingAlgo() method will generate a evNoTempSensor event
        allOff();

        // Retrieve configuration value(s)
        m_maxCapacity = 1; // Default value if the MP is invalid (which should never happen)
        mp::maxHeatingCapacity.read( m_maxCapacity );

        // Initialize the Fuzzy Logic controller
        m_flcController.start();

        // Brute force my FSM (allows restarting)
        m_initialized=static_cast<int>(0U);
        initialize();

        // Subscribe to my MP
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
    // Run the algorithm (when the interval time has expired)
    scheduleAlgorithm();

    // Restart my interval timer - just needs to be 'faster' than the actual polling rate
    Timer::start( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS / 10 );
}

void Api::scheduleAlgorithm() noexcept
{
    uint32_t now = Cpl::System::ElapsedTime::milliseconds();

    // Initialize the interval (but only once)
    if ( m_firstExecution )
    {
        // Round down to the nearest interval boundary
        m_firstExecution = false;
        m_timeMarker     = (now / OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS) * OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS;
    }

    // Has the interval expired?
    if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timeMarker, OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS, now ) )
    {
        // Execute the algorithm
        m_timeMarker += OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS;
        intervalExpired();
    }
}


void Api::intervalExpired() noexcept
{
    // 'Run' the FSM
    generateEvent( FSM_NO_MSG );
}

void Api::hwSafetyChanged( Cpl::Dm::Mp::Bool& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    bool safetyTripped;
    if ( mp.readAndSync( safetyTripped, clientObserver ) )
    {
        if ( safetyTripped )
        {
            Ajax::Logging::logf( Ajax::Logging::AlertMsg::FAILED_SAFE, "RAISED" );
            mp::failedSafeAlert.raiseAlert();
            generateEvent( Fsm_evHiTemp );
        }
        else
        {
            Ajax::Logging::logf( Ajax::Logging::AlertMsg::FAILED_SAFE, "Cleared" );
            mp::failedSafeAlert.lowerAlert();
            generateEvent( Fsm_evSafeTemp );
        }
    }
}

void Api::heatingEnabledChanged( Cpl::Dm::Mp::Bool& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    bool enabled;
    if ( mp.readAndSync( enabled, clientObserver ) )
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
void Api::checkForSensor() noexcept
{
    // Poll the state of the temperature sensors
    if ( !mp::onBoardIdt.isNotValid() || !mp::remoteIdt.isNotValid() )
    {
        if ( !m_temperatureSensorAvailable ) // Prevent 'double' clear
        {
            m_temperatureSensorAvailable = true;
            Ajax::Logging::logf( Ajax::Logging::AlertMsg::NO_TEMPERATURE_SENSOR, "Cleared" );
            mp::sensorFailAlert.lowerAlert();
            generateEvent( Fsm_evSensorAvailable );
        }
    }
}

void Api::allOff() noexcept
{
    // Shut everything off
    heatOff();
    fanOff();
    m_sumCapacityRequest = 0; 
}

void Api::fanOff() noexcept
{
    mp::cmdFanPWM.write( 0 );
}

void Api::fanOn() noexcept
{
    mp::cmdFanPWM.write( getFanPWM() );
}

void Api::heatOff() noexcept
{
    mp::cmdHeaterPWM.write( 0 );
}


void Api::runHeatingAlgo() noexcept
{
    int32_t currentTemp;
    if ( getTemperature( currentTemp ) )
    {
        int32_t setpoint;
        if ( mp::heatSetpoint.read( setpoint ) )
        {
            // Run the FLC, accumulate the capacity requests, and clamp at max capacity
            int32_t delta         = m_flcController.calcChange( currentTemp, setpoint );
            m_sumCapacityRequest += delta;
            if ( m_sumCapacityRequest < 0 )
            {
                m_sumCapacityRequest = 0;
            }
            else if ( m_sumCapacityRequest > ((int32_t)m_maxCapacity) )
            {
                m_sumCapacityRequest = m_maxCapacity;
            }

            // Convert Capacity request to PWM
            uint32_t heaterPwm = (m_sumCapacityRequest * MAX_PWM) / m_maxCapacity;
            mp::cmdHeaterPWM.write( heaterPwm );

            // Set Fan speed (Note: Fan only runs when the heater is 'on')
            uint32_t fanPWM = 0;
            if ( heaterPwm > 0 )
            {
                fanPWM = getFanPWM();
            }
            mp::cmdFanPWM.write( fanPWM );

            CPL_SYSTEM_TRACE_MSG( SECT_, ("idt=%ld, setpt=%ld, err=%ld.  flc=%ld, capreq=%ld.  newHeaterPWM=%lu, newFanPWM=%lu",
                                           currentTemp,
                                           setpoint,
                                           setpoint - currentTemp,
                                           delta,
                                           m_sumCapacityRequest,
                                           heaterPwm,
                                           fanPWM) );
        }

        // No setpoint value for SOME reason 
        else
        {
            // This should NEVER happen -->give-up and reboot
            Cpl::System::FatalError::log( "HeatingAlgo: Setpoint MP is invalid" );
        }
    }

    // No sensor available
    else
    {
        if ( m_temperatureSensorAvailable ) // Prevent 'double' raise
        {
            m_temperatureSensorAvailable = false;
            Ajax::Logging::logf( Ajax::Logging::AlertMsg::NO_TEMPERATURE_SENSOR, "RAISED" );
            mp::sensorFailAlert.raiseAlert();
            generateEvent( Fsm_evNoTempSensor );
        }
    }
}


bool Api::isSensorAvailable() noexcept
{
    return m_temperatureSensorAvailable;
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

uint32_t Api::getFanPWM() noexcept
{
    uint32_t fanPWM             = MAX_PWM;
    Ajax::Type::FanMode fanMode = Ajax::Type::FanMode::eHIGH; // Default to high speed
    mp::fanMode.read( fanMode );
    switch ( fanMode )
    {
    case Ajax::Type::FanMode::eLOW:
        mp::fanLowPercentage.read( fanPWM );
        break;
    case Ajax::Type::FanMode::eMEDIUM:
        mp::fanMedPercentage.read( fanPWM );
        break;
    default:
        mp::fanHighPercentage.read( fanPWM );
        break;
    }
    return fanPWM;
}