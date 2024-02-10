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

#include "colony_config.h"
#include "platform.h"
#include "Ajax/Main/platform.h"
#include "Ajax/Main/appmain.h"
#include "app_platform.h"
#include "Driver/NV/File/Cpl/Api.h"
#include "Driver/DIO/Pwm.h"
#include "Cpl/System/Trace.h"
#include "mp/ModelPoints.h"
#include "Ajax/Main/_plat_simulator/ModelPoints.h"
#include <stdlib.h>

using namespace Ajax::Main;

#ifndef DRIVER_NV_FILE_NAME     
#define DRIVER_NV_FILE_NAME         "eeprom.bin"
#endif
#ifndef DRIVER_NV_NUM_PAGES
#define DRIVER_NV_NUM_PAGES         512
#endif
#ifndef DRIVER_NV_BYTES_PER_PAGE
#define DRIVER_NV_BYTES_PER_PAGE    128
#endif

static Driver::NV::File::Cpl::Api   nvDriver_( DRIVER_NV_NUM_PAGES, DRIVER_NV_BYTES_PER_PAGE, DRIVER_NV_FILE_NAME );
Driver::NV::Api&                    Ajax::Main::g_nvramDriver = nvDriver_;

Driver::DIO::Pwm                    Ajax::Main::g_heaterPWMDriver( mp::mockedHeaterPwmOut );
Driver::DIO::Pwm                    Ajax::Main::g_fanPWMDriver( mp::mockedFanPwmOut );
Driver::DIO::In                     Ajax::Main::g_hwSafetyDriver( mp::mockedHwSafetyIn );
/////////////////////////////
void Ajax::Main::platform_initialize0()
{
    // Platform init...
    nvDriver_.start();
    g_heaterPWMDriver.start( 0 );
    g_fanPWMDriver.start( 0 );
    g_hwSafetyDriver.start();
    appvariant_platform_initialize0();
}

bool Ajax::Main::platform_runPOST()
{
    // Generate a POST failure when requested via the command line
    bool postFailed = false;
    if ( Ajax::Main::g_args["-e"].isBool() )
    {
        postFailed = g_args["-e"].asBool();
    }
    return !postFailed;
}

void Ajax::Main::platform_initializeModelPoints0()
{
    // Platform init...

    appvariant_platform_initializeModelPoints0();
}

void Ajax::Main::platform_open0()
{
    // Platform open...
    appvariant_platform_open0();
}

void Ajax::Main::platform_close0()
{
    appvariant_platform_close0();

    // Platform close...
}

int Ajax::Main::platform_exit( int exitCode )
{
    exit( exitCode );
}
