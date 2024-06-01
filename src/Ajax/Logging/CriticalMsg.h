#ifndef Ajax_Logging_CriticalMsg_h_
#define Ajax_Logging_CriticalMsg_h_
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

/** This enumeration defines Logging message code for the CRITICAL category

        Note: Each error code symbol must be less than 32 characters
           123456789 123456789 12345678 12

    @param CriticalMsg                      Enum

    @param DRIVER                           An unrecoverable driver error occurred
    @param DIVIDE_BY_ZERO                   Calculation was aborted because the denominator was zero
    @param UX_ERROR                         An error occurred that prevented proper operation of the Display and/or user buttons
    @param FATAL_ERROR                      An Fatal error was logged using the CPL FatalError handler
 */
BETTER_ENUM( CriticalMsg, uint16_t
             , DRIVER
             , DIVIDE_BY_ZERO
             , UX_ERROR
             , FATAL_ERROR
);


};      // end namespaces
};
#endif  // end header latch
