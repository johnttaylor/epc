#ifndef Ajax_Logging_CategoryId_h_
#define Ajax_Logging_CategoryId_h_
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
#include <stdint.h>



///
namespace Ajax {
///
namespace Logging {

/** This enumeration defines the various logging categories.

    Note: Each symbol must be less than 16 characters
           123456789 123456

    @param CategoryId           Enum

    @param CRITICAL             A sub-system/module encountered an unrecoverable error – but the overall System can still provide some level of functionality.
    @param WARNING              A recoverable error and/or an unexpected condition occurred
    @param EVENT                An ‘interesting’ event or action occurred
    @param ALERT                An alert condition or state was encounter (aka a UX Alert)
    @param INFO                 Informational.  Typically used for detailed troubleshooting and is not recommended to be enabled by default
    @param METRICS              Used to capture metrics to the device’s log file.  Expected usage is that the metrics are periodically written the log file.
 */
BETTER_ENUM( CategoryId, uint32_t
             , CRITICAL    = 0x0001
             , WARNING     = 0x0002
             , ALERT       = 0x0004
             , EVENT       = 0x0008
             , INFO        = 0x0010
             , METRICS     = 0x0020
);



};      // end namespaces
};
#endif  // end header latch
