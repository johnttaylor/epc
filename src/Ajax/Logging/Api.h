#ifndef Ajax_Logging_Api_h_
#define Ajax_Logging_Api_h_
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

#include "Ajax/Logging/CategoryId.h"
#include "Ajax/Logging/CriticalMsg.h"
#include "Ajax/Logging/WarningMsg.h"
#include "Ajax/Logging/EventMsg.h"
#include "Ajax/Logging/AlertMsg.h"
#include "Ajax/Logging/InfoMsg.h"
#include "Ajax/Logging/MetricsMsg.h"


///
namespace Ajax {
///
namespace Logging {


/*---------------------------------------------------------------------------*/
/// This method logs a CRITICAL log entry
void logf( CriticalMsg msgId, const char* msgTextFormat, ... ) noexcept;

/// This method logs a WARNING log entry
void logf( WarningMsg msgId, const char* msgTextFormat, ... ) noexcept;

/// This method logs a EVENT log entry
void logf( EventMsg msgId, const char* msgTextFormat, ... ) noexcept;

/// This method logs a ALERT log entry
void logf( AlertMsg msgId, const char* msgTextFormat, ... ) noexcept;

/// This method logs a INFO log entry
void logf( InfoMsg msgId, const char* msgTextFormat, ... ) noexcept;

/// This method logs a METRICS log entry
void logf( MetricsMsg msgId, const char* msgTextFormat, ... ) noexcept;

};      // end namespaces
};
#endif  // end header latch
