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

#include "Ajax/Main/appmain.h"
#include "Ajax/Main/platform.h"
#include "app_platform.h"
#include "Bsp/Api.h"
#include "Driver/PicoDisplay/Arduino/Api.h"
#include "Driver/NV/Onsemi/CAT24C512/Api.h"
#include "Driver/I2C/Arduino/Master.h"
#include <stdlib.h>

using namespace Ajax::Main;

static Driver::I2C::Arduino::Master         i2cDriver_( I2C_BUS_DRIVER );
static Driver::NV::Onsemi::CAT24C512::Api   nvDriver_( i2cDriver_, BSP_I2C_ADDRESS_EEPROM );
Driver::NV::Api&                            Ajax::Main::g_nvramDriver = nvDriver_;


/////////////////////////////
void Ajax::Main::platform_initialize0()
{
    // Platform init...
    i2cDriver_.start();
    Driver::PicoDisplay::Arduino::initialize();

    appvariant_platform_initialize0();
}

bool Ajax::Main::platform_runPOST()
{
    // External EEPROM test -->can I communicate with it?
    uint8_t buffer;
    auto result = i2cDriver_.readFromDevice( BSP_I2C_ADDRESS_EEPROM, 1, &buffer );
    return result == Driver::I2C::Master::eSUCCESS;
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
    Bsp_Api_reset_MCU();
    return exitCode;  // Note: This should never execute
}
