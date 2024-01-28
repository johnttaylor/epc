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

#include "Ajax/Main/appmain.h"      // FIXME: Must be first (because of the pimoroni graphic library stuffs)
#include "Ajax/Main/application.h"
#include "Ajax/Main/screens.h"
#include "mp/ModelPoints.h"
#include "Cpl/TShell/Cmd/User.h"
#include "TShellSecurity.h"
#include "Ajax/Heating/Supervisor/Api.h"
#include "Ajax/Heating/Io/Api.h"
#include "Ajax/Ui/StatusIndicator/Api.h"
#include "Ajax/Alerts/Summary.h"
#include "Ajax/Metrics/Api.h"

#include <stdio.h>

using namespace Ajax::Main;

// Only use/require the 'User' command for Production builds
//#if !defined(DEBUG_BUILD)
static TShellSecurity         security_( *g_sha512Ptr );
static Cpl::TShell::Cmd::User userCmd_( g_cmdlist, security_ );
//#endif

// Algorithm
static Ajax::Heating::Supervisor::Api  heatingAlgo_( g_appMbox );
static Ajax::Heating::Io::Api          heatingIo_( g_appMbox, g_heaterPWMDriver, g_fanPWMDriver, g_hwSafetyDriver );

// Alert summary
static Ajax::Dm::MpAlert* alerts_[Ajax::Type::Alert::NUM_ALERTS] ={
    &mp::failedSafeAlert,
    &mp::sensorFailAlert,
    &mp::remoteSensorFailAlert,
    &mp::postFailedAlert,
    &mp::notProvisionedAlert
};
Ajax::Alerts::Summary       alertSummary_( g_appMbox, alerts_ );

static Ajax::Metrics::Api   metrics_( g_appMbox );


// Screens...
Ajax::Ui::Home::Screen      Ajax::Main::g_homeScreen_( Ajax::Main::g_screenNav, g_graphics, Ajax::Main::g_uiMbox, mp::onBoardIdt );
Ajax::Ui::EditSetpt::Screen Ajax::Main::g_editSetptScreen_( Ajax::Main::g_screenNav, g_graphics );
Ajax::Ui::About::Screen     Ajax::Main::g_aboutScreen_( Ajax::Main::g_screenNav, g_graphics );
Ajax::Ui::Error::Screen     Ajax::Main::g_errorScreen_( g_graphics );

static Ajax::Ui::StatusIndicator::Api  statusIndicator_( Ajax::Main::g_uiMbox, Driver::PicoDisplay::Api::rgbLED() );

/////////////////////////////
void Ajax::Main::appvariant_initialize0()
{
    printf( "\nAJAX: appvariant_initialize0()\n" );
}

void Ajax::Main::appvariant_initializeModelPoints0()
{
    // Populate 'static' Model points
    Cpl::Text::FString<OPTION_AJAX_MAX_VERSION_LENGTH> tmpVer;
    tmpVer.format( "%s-%07lu", AJAX_SEMANTIC_VERSION_STR, BUILD_NUMBER );
    mp::fwVersion.write( tmpVer );
}

void Ajax::Main::appvariant_open0()
{
    // Check for Errors that can occurred during start-up
    if ( !mp::notProvisionedAlert.isNotValid() || !mp::postFailedAlert.isNotValid()  )
    {
        mp::errorScrPtr.write( &Ajax::Main::g_errorScreen_ );   // Trigger Error/UI-Halt screen
        mp::heatingMode.write( false );                         // Force the heating mode to be OFF
    }

    // Start-up the application
    metrics_.open();
    alertSummary_.open();
    statusIndicator_.open();
    heatingIo_.open();
    heatingAlgo_.open();
}

void Ajax::Main::appvariant_launchHomeScreen()
{
    mp::homeScrPtr.write( &g_homeScreen_ );
}

void Ajax::Main::appvariant_close0()
{
    heatingAlgo_.close();
    heatingIo_.close();
    statusIndicator_.close();
    alertSummary_.close();
    metrics_.close();
}