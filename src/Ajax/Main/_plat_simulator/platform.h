#ifndef Ajax_Main_plat_simulator_platform_h_
#define Ajax_Main_plat_simulator_platform_h_
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
/** @file

    This file defines platform specific interfaces when running on the simulator
 */

#include "docopt-cpp/docopt.h"
#include "docopt-cpp/docopt_value.h"

///
namespace Ajax {
///
namespace Main {


/// The simulator's command line arguments 
extern std::map<std::string, docopt::value> g_args;



};      // end namespaces
};
#endif  // end header latch