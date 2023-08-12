#ifndef Ajax_Logging_WarningMsg_h_
#define Ajax_Logging_WarningMsg_h_
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

/** This enumeration defines Logging message code for the WARNING category

        Note: Each error code symbol must be less than 32 characters
           123456789 123456789 12345678 12

    @param WarningMsg                       Enum

    @param LOGGING_OVERFLOW                 The Logging Queue overflowed (and has not recovered)
    @param OPEN_CLOSE                       Incorrect open()/close() semantics (e.g. call open() twice in row)
    @param OUT_OF_MEMORY                    A non-fatal out-of-memory condition was encounter
 */
BETTER_ENUM( WarningMsg, uint16_t
             , LOGGING_OVERFLOW
             , OPEN_CLOSE
             , OUT_OF_MEMORY
);


};      // end namespaces
};
#endif  // end header latch
