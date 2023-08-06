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

#include "Ajax/Main/platform.h"
#include "app_platform.h"
#include <stdlib.h>

using namespace Ajax::Main;


/////////////////////////////
void Ajax::Main::platform_initialize0()
{
    // Platform init...

    appvariant_platform_initialize0();
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
