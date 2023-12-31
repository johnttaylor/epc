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
#include "Cpl/System/Assert.h"
#include "Ajax/Logging/Api.h"
#include "mp/ModelPoints.h"

#define SECT_ "Ajax::Metrics::Api"

using namespace Ajax::Metrics;


///////////////////////////
Api::Api( Cpl::Dm::MailboxServer&  myMbox )
    : Cpl::Itc::CloseSync( myMbox )
    , Cpl::System::Timer( myMbox )
    , m_obFanPWM( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::fanPWMChanged )
    , m_obHeaterPWM( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::heaterPWMChanged )
    , m_obFailsafeAlert( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::failsafeAlertChanged )
    , m_opened( false )
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
    m_opened        = true;
    m_fanOn         = false;
    m_heaterOn      = false;
    m_runtimeMarker = Cpl::System::ElapsedTime::milliseconds();

    // Enable change notifications
    mp::cmdFanPWM.attach( m_obFanPWM );
    mp::cmdHeaterPWM.attach( m_obHeaterPWM );
    mp::failedSafeAlert.attach( m_obFailsafeAlert );

    // Start my interval timer
    Timer::start( OPTION_AJAX_METRICS_INTERVAL_TIME_MS );

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
    m_opened = false;
    Timer::stop();
    
    // Cancel change notifications
    mp::cmdFanPWM.detach( m_obFanPWM );
    mp::cmdHeaterPWM.detach( m_obHeaterPWM );
    mp::failedSafeAlert.detach( m_obFailsafeAlert );

    // Update run-time counters
    uint32_t now = Cpl::System::ElapsedTime::milliseconds();
    uint32_t pwm;
    if ( mp::cmdFanPWM.read( pwm ) )
    {
        processFanMetric( now, pwm, 0 );
    }
    if ( mp::cmdHeaterPWM.read( pwm ) )
    {
        processHeaterMetric( now, pwm, 0 );
    }
    mp::metricRunningTime.increment( Cpl::System::ElapsedTime::deltaMilliseconds( m_runtimeMarker, now ) );

    // NOTE: The application/shutdown-sequence is REQUIERD to 'flush' the Metrics record AFTER call close on the class
    msg.returnToSender();
}

///////////////////////////
void Api::expired() noexcept
{
    uint32_t now = Cpl::System::ElapsedTime::milliseconds();

    uint32_t pwm;
    if ( mp::cmdFanPWM.read( pwm ) && pwm > 0 && m_fanOn )
    {
        processFanMetric( now, pwm );
    }
    if ( mp::cmdHeaterPWM.read( pwm ) && pwm > 0 && m_heaterOn )
    {
        processHeaterMetric( now, pwm );
    }

    // Accumulate system-on time
    uint32_t delta = Cpl::System::ElapsedTime::deltaMilliseconds( m_runtimeMarker, now );
    if ( delta > OPTION_AJAX_METRICS_INTERVAL_TIME_MS )
    {
        m_runtimeMarker = now;
        mp::metricRunningTime.increment( delta );
        mp::metricFaultHeaterSafety.touch();    // Indirectly trigger a save of the Metrics record
    }

    // Restart the timer
    Timer::start( OPTION_AJAX_METRICS_INTERVAL_TIME_MS );
}


void Api::fanPWMChanged( Cpl::Dm::Mp::Uint32& mpPWM, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    uint32_t pwm;
    if ( mpPWM.readAndSync( pwm, clientObserver ) )
    {
        processFanMetric( Cpl::System::ElapsedTime::milliseconds(), pwm );
    }
}

void Api::heaterPWMChanged( Cpl::Dm::Mp::Uint32& mpPWM, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    uint32_t pwm;
    if ( mpPWM.readAndSync( pwm, clientObserver ) )
    {
        processHeaterMetric( Cpl::System::ElapsedTime::milliseconds(), pwm );
    }
}

void Api::failsafeAlertChanged( Ajax::Dm::MpAlert& mpAlert, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    if ( mpAlert.isNotValidAndSync( clientObserver ) == false )
    {
        // Increment metric count when Alert is raised (updating the MP triggers a save of the storage record)
        mp::metricFaultHeaterSafety.increment();
    }
}

void Api::processFanMetric( uint32_t currentElapsedTimeMs, uint32_t pwm, uint32_t intervalTime ) noexcept
{
    // transition to ON
    if ( !m_fanOn && pwm > 0 )
    {
        m_fanOn     = true;
        m_fanMarker = currentElapsedTimeMs;
    }

    else if ( m_fanOn )
    {
        // transition to OFF
        if ( pwm == 0 )
        {
            m_fanOn        = false;
            uint32_t delta = Cpl::System::ElapsedTime::deltaMilliseconds( m_fanMarker, currentElapsedTimeMs );
            mp::metricFanOnTime.increment( delta );
            mp::metricFaultHeaterSafety.touch();    // Indirectly trigger a save of the Metrics record
        }

        // Running            
        else
        {
            uint32_t delta = Cpl::System::ElapsedTime::deltaMilliseconds( m_fanMarker, currentElapsedTimeMs );
            if ( delta >= intervalTime )
            {
                m_fanMarker = currentElapsedTimeMs;
                mp::metricFanOnTime.increment( delta );
                mp::metricFaultHeaterSafety.touch();    // Indirectly trigger a save of the Metrics record
            }
        }
    }
}

void Api::processHeaterMetric( uint32_t currentElapsedTimeMs, uint32_t pwm, uint32_t intervalTime ) noexcept
{
    // transition to ON
    if ( !m_heaterOn && pwm > 0 )
    {
        m_heaterOn     = true;
        m_heaterMarker = currentElapsedTimeMs;
    }

    else if ( m_heaterOn )
    {
        // transition to OFF
        if ( pwm == 0 )
        {
            m_heaterOn     = false;
            uint32_t delta = Cpl::System::ElapsedTime::deltaMilliseconds( m_heaterMarker, currentElapsedTimeMs );
            mp::metricHeaterOnTime.increment( delta );
            mp::metricFaultHeaterSafety.touch();    // Indirectly trigger a save of the Metrics record
        }

        // Running            
        else
        {
            uint32_t delta = Cpl::System::ElapsedTime::deltaMilliseconds( m_heaterMarker, currentElapsedTimeMs );
            if ( delta >= intervalTime )
            {
                m_heaterMarker = currentElapsedTimeMs;
                mp::metricHeaterOnTime.increment( delta );
                mp::metricFaultHeaterSafety.touch();    // Indirectly trigger a save of the Metrics record
            }
        }
    }
}


