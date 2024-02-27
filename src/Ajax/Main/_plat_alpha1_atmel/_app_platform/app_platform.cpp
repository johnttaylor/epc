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
#include "Ajax/Heating/Supervisor/Api.h"
#include "mp/ModelPoints.h"
#include "Driver/AIO/Ajax/Thermistor.h"
#include "Driver/AIO/HalSingleInput.h"
#include "Driver/AIO/Arduino/HalSingleInput.h"
#include <stdio.h>

using namespace Ajax::Main;

// Core heating Algorithm 
static Ajax::Heating::Supervisor::Api  heatingAlgo_( g_appMbox );

// The Ajax and Eros Applications have DIFFERENT Thermistor drivers
static Driver::AIO::Ajax::Thermistor    thermistor_( g_appMbox, PIN_ONBOARD_IDT, mp::onBoardIdt );


/////////////////////////////
void Ajax::Main::appvariant_platform_initialize0()
{
    driverAIOHalSingleInputArduino_initialize( PIN_ONBOARD_IDT, AR_DEFAULT );   // Use the internal VDDana 3.3V reference
    Driver_AIO_HalSingle_setADCSize( PIN_ONBOARD_IDT, 12 );
}

void Ajax::Main::appvariant_platform_initializeModelPoints0()
{
    // Nothing currently needed
}

void Ajax::Main::appvariant_platform_open0()
{
    thermistor_.open();
    heatingAlgo_.open();
}

void Ajax::Main::appvariant_platform_close0()
{
    heatingAlgo_.close();
    thermistor_.close();
}
