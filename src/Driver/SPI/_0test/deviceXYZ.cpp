/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2022  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "deviceXYZ.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Api.h"

#define SECT_	"_0test"

#define STOP_TEST()         for ( ;; )
using namespace Driver::SPI;

void runtests( Driver::SPI::Master & uut )
{
    CPL_SYSTEM_TRACE_MSG( SECT_, ("SPI Driver Test - expecting a ???? (or compatible) ???? peripheral device.") );

    // TODO: IMPLEMENT TESTS!!!!

    // Do not return once the test has completed
    CPL_SYSTEM_TRACE_MSG( SECT_, ("ALL TESTS COMPLETED SUCCESFULLY") );
    STOP_TEST();
}
