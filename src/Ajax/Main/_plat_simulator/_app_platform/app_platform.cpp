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


#include "Ajax/Main/appmain.h"  // Must be first because of naming-collisions in the Pimoroni Graphics library
#include "Ajax/Main/_plat_simulator/platform.h"
#include "Ajax/Main/_plat_simulator/app_platform.h"
#include "Ajax/Heating/Simulated/House.h"
#include "mp/ModelPoints.h"
#include "Driver/AIO/Ajax/Thermistor.h"
#include <stdio.h>

using namespace Ajax::Main;



// The Ajax and Eros Applications have DIFFERENT Thermistor drivers
static Cpl::Dm::Mp::Uint32              mockADCBits_( mp::g_modelDatabase, "mockedADCBits" );
static Driver::AIO::Ajax::Thermistor    thermistor_( g_appMbox, mockADCBits_, mp::onBoardIdt );
static Ajax::Heating::Supervisor::Api   heatingAlgo_( g_appMbox );


/////////////////////////////
void Ajax::Main::appvariant_platform_initialize0()
{
    // Set initial ADC bits 
    uint32_t bits = g_args["-t"].asLong();
    if ( bits < 4096 && bits >= 0 )
    {
        mockADCBits_.write( bits );
    }
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
