
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


// Core heating algorithm - extended with the House simulator
static Cpl::Dm::Mp::Bool                simEnable_( mp::g_modelDatabase, "simEnable" );
static Cpl::Dm::Mp::Double              simOdt_( mp::g_modelDatabase, "simOdt" );
static Ajax::Heating::Simulated::House  heatingAlgo_( g_appMbox, simEnable_, simOdt_ );


/////////////////////////////
void Ajax::Main::appvariant_platform_initialize0()
{
   // Nothing currently needed
}

void Ajax::Main::appvariant_platform_initializeModelPoints0()
{
    double odt = 70.0;
    odt = g_args["-o"].asLong();
    simOdt_.write( odt );
    simEnable_.write( true );
}

void Ajax::Main::appvariant_platform_open0()
{
    heatingAlgo_.open();
}

void Ajax::Main::appvariant_platform_close0()
{
    heatingAlgo_.close();
}
