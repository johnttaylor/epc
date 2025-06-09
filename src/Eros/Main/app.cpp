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
#include "Eros/Main/screens.h"
#include "mp/ModelPoints.h"
#include "Driver/AIO/Eros/ModelPoints.h"
#include "Eros/TShell/Rgb.h"
#include "Eros/TShell/Pwm.h"
#include "Eros/TShell/HwSafety.h"
#include "Cpl/MApp/Manager.h"
#include "Cpl/MApp/Cmd.h"
#include "Eros/Test/Thermistor/Api.h"
#include "Eros/Test/Cycle/Api.h"
#include "Cpl/System/Trace.h"
#include <stdio.h>

using namespace Ajax::Main;

Eros::Ui::Home::Screen          Eros::Main::g_homeScreen( Ajax::Main::g_screenNav, g_graphics );
Eros::Ui::LcdTest::Screen       Eros::Main::g_lcdTextScreen( Ajax::Main::g_screenNav, g_graphics );

static Eros::TShell::Rgb        rgbCmd_( g_cmdlist, Driver::PicoDisplay::Api::rgbLED() );
static Eros::TShell::Pwm        pwmCmd_( g_cmdlist, Ajax::Main::g_heaterPWMDriver, Ajax::Main::g_fanPWMDriver );
static Eros::TShell::HwSafety   hwSafetyCmd_( g_cmdlist, Ajax::Main::g_hwSafetyDriver );

static Cpl::Container::SList<Cpl::MApp::MAppApi>   mappList_;
static Cpl::MApp::Manager                        mappManager_( g_appMbox, mappList_ );
static Cpl::MApp::Cmd                            mappCmd_( g_cmdlist, mappManager_ );

static Eros::Test::Thermistor::Api               thermistorTest_( mappList_, g_appMbox, mp::erosThermistorSample );
static Eros::Test::Cycle::Api                    cycleTest_( mappList_, g_appMbox, Ajax::Main::g_heaterPWMDriver, Ajax::Main::g_fanPWMDriver );

/////////////////////////////
void Ajax::Main::appvariant_initialize0()
{
    CPL_SYSTEM_TRACE_ENABLE_SECTION( OPTION_CPL_MAPP_TRACE_SECTION );
}

void Ajax::Main::appvariant_initializeModelPoints0()
{
    // Populate 'static' Model points
    Cpl::Text::FString<OPTION_AJAX_MAX_VERSION_LENGTH> tmpVer;
    tmpVer.format( "%s-%07lu", EROS_SEMANTIC_VERSION_STR, BUILD_NUMBER );
    mp::fwVersion.write( tmpVer );
}

void Ajax::Main::appvariant_open0()
{
    mappManager_.open();
}

void Ajax::Main::appvariant_launchHomeScreen()
{
    mp::homeScrPtr.write( &Eros::Main::g_homeScreen );
}

void Ajax::Main::appvariant_close0()
{
    mappManager_.close();
}