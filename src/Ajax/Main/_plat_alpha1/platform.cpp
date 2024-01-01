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
#include "Driver/PicoDisplay/STM32/Api.h"
#include "Driver/NV/Onsemi/CAT24C512/Api.h"
#include "Driver/I2C/STM32/Master.h"
#include "Driver/DIO/Pwm.h"
#include <stdlib.h>

using namespace Ajax::Main;

static Driver::I2C::STM32::Master           i2cDriver_( BSP_I2C_HANDLE );
static Driver::NV::Onsemi::CAT24C512::Api   nvDriver_( i2cDriver_, BSP_I2C_ADDRESS_EEPROM );
Driver::NV::Api&                            Ajax::Main::g_nvramDriver = nvDriver_;

static DriverDioPwmSTM32Config_T            heaterPWMCfg_( PWM_HEATER_BLOCK_PTR, PWM_HEATER_CHANNEL );
Driver::DIO::Pwm                            Ajax::Main::g_heaterPWMDriver( heaterPWMCfg_ );
static DriverDioPwmSTM32Config_T            fanPWMCfg_( PWM_FAN_BLOCK_PTR, PWM_FAN_CHANNEL );
Driver::DIO::Pwm                            Ajax::Main::g_fanPWMDriver( fanPWMCfg_ );

/////////////////////////////
void Ajax::Main::platform_initialize0()
{
    // Platform init...
    i2cDriver_.start();
    g_heaterPWMDriver.start( 0 );
    g_fanPWMDriver.start( 0 );
    Driver::PicoDisplay::STM32::initialize();

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
}
