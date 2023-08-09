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

#include "Cpl/Logging/TimeApi.h"
#include "Cpl/System/ElapsedTime.h"


CplLoggingTime_T Cpl::Logging::now() noexcept
{
    return Cpl::System::ElapsedTime::precision().asFlatTime(); // Convert Precision time to 'flat' millisecond time as a uint64_t
}
