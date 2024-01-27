#ifndef Ajax_Logging_EventMsg_h_
#define Ajax_Logging_EventMsg_h_
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

/** This enumeration defines Logging message code for the EVENT category

        Note: Each error code symbol must be less than 32 characters
           123456789 123456789 12345678 12

    @param EventMsg                         Enum

    @param PLACE_HOLDER                     Place holder till I have a event log entry
 */
BETTER_ENUM( EventMsg, uint16_t
             , PLACE_HOLDER
);


};      // end namespaces
};
#endif  // end header latch
