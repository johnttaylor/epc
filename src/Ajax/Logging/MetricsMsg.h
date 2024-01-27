#ifndef Ajax_Logging_MetricsMsg_h_
#define Ajax_Logging_MetricsMsg_h_
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

#include "Cpl/Type/enum.h"

///
namespace Ajax {
///
namespace Logging {

/** This enumeration defines Logging message code for the METRICS category

        Note: Each error code symbol must be less than 32 characters
           123456789 123456789 12345678 12

    @param MetricsMsg                       Enum

    @param POWER_ON                         The MCU was powered and/or was reset
    @param SHUTDOWN                         An orderly shutdown of the application was initiated
 */
BETTER_ENUM( MetricsMsg, uint16_t
             , POWER_ON
             , SHUTDOWN
);


};      // end namespaces
};
#endif  // end header latch
