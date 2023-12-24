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


///////////////////////////
Api::Api( Cpl::Dm::MailboxServer&    myMbox,
          Driver::LED::RedGreenBlue& statusLED )
    : Cpl::Itc::CloseSync( myMbox )
    , m_ledDriver( statusLED )
    , m_obAlertSummary( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::alertSummaryChanged )
    , m_obHeaterPWM( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Api::heaterPWMChanged )
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
    mp::alertSummary.attach( m_obAlertSummary );
    mp::cmdHeaterPWM.attach( m_obHeaterPWM );
    m_ledDriver.setOff();

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
    mp::alertSummary.detach( m_obAlertSummary );
    mp::cmdHeaterPWM.detach( m_obHeaterPWM );
    m_ledDriver.setOff();

    msg.returnToSender();
}

///////////////////////////
void Api::alertSummaryChanged( Ajax::Dm::MpAlertSummary& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    if ( !mp.isNotValidAndSync( clientObserver ) )
    {
        setStatus();
    }
}

void Api::heaterPWMChanged( Cpl::Dm::Mp::Uint32& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    if ( !mp.isNotValidAndSync( clientObserver ) )
    {
        setStatus();
    }
}

///////////////////////////
void Api::setStatus() noexcept
{
    Ajax::Dm::MpAlertSummary::Data alertSummary;
    uint32_t                       heaterPWM;
    if ( mp::alertSummary.read( alertSummary ) && mp::cmdHeaterPWM.read( heaterPWM ) )
    {
        if ( alertSummary.count > 0 )
        {
            // At least one alert -->set to RED
            m_ledDriver.setRgb( 255, 0, 0 );
        }
        else if ( heaterPWM > 0 )
        {
            // Heater is on -->set to Blue
            m_ledDriver.setRgb( 0, 0, 255 );
        }
        else 
        {
            // Heater is off -->set to Green
            m_ledDriver.setRgb( 0, 255, 0 );
        }
    }
    
    // Cannot determine state -->set to White
    else
    {
        m_ledDriver.setRgb( 255, 255, 255 );
    }
}
