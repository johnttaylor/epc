#ifndef Ajax_Heating_Io_test_api_h_
#define Ajax_Heating_Io_test_api_h_
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

#include "Driver/DIO/In.h"
#include "Driver/DIO/Pwm.h"
#include "Cpl/Io/Input.h"
#include "Cpl/Io/Output.h"
#include <stdint.h>

/// Executes the tests. A return of zero indicates all test passed
void runtests( Driver::DIO::In&  hwSafetyInDriver,
               Driver::DIO::Pwm& heaterPWMDriver,
               Driver::DIO::Pwm& fanPWMDriver,
               Cpl::Io::Input&    infd, 
               Cpl::Io::Output&   outfd );


#endif  // end header latch
