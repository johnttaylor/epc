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

#include "Ajax/Main/_plat_alpha1/app_platform.h"
#include "Ajax/Main/appmain.h"
#include "Bsp/Api.h"
#include "mp/ModelPoints.h"
#include "Driver/AIO/Eros/Thermistor.h"
#include <stdio.h>

using namespace Ajax::Main;

// The Ajax and Eros Applications have DIFFERENT Thermistor drivers
static Driver::AIO::Eros::Thermistor    thermistor_( g_appMbox, AIN_THERMISTOR_PTR, mp::onBoardIdt );

/////////////////////////////
void Ajax::Main::appvariant_platform_initialize0()
{
    printf( "\nEROS: alpha1: appvariant_platform_initialize0()\n" );
}

void Ajax::Main::appvariant_platform_initializeModelPoints0()
{
    // Nothing currently needed
}

void Ajax::Main::appvariant_platform_open0()
{
    thermistor_.open();
}

void Ajax::Main::appvariant_platform_close0()
{
    thermistor_.close();
}