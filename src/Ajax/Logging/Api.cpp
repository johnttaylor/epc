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

#include "Api.h"
#include "Cpl/Logging/Api.h"
 
void Ajax::Logging::logf( CriticalMsg msgId, const char* msgTextFormat, ... ) noexcept
{
    va_list ap;
    va_start( ap, msgTextFormat );
    Cpl::Logging::vlogf<CategoryId, CriticalMsg>( CategoryId::CRITICAL, msgId, msgTextFormat, ap );
    va_end( ap );
}

void Ajax::Logging::logf( WarningMsg msgId, const char* msgTextFormat, ... ) noexcept
{
    va_list ap;
    va_start( ap, msgTextFormat );
    Cpl::Logging::vlogf<CategoryId, WarningMsg>( CategoryId::WARNING, msgId, msgTextFormat, ap );
    va_end( ap );
}

void Ajax::Logging::logf( EventMsg msgId, const char* msgTextFormat, ... ) noexcept
{
    va_list ap;
    va_start( ap, msgTextFormat );
    Cpl::Logging::vlogf<CategoryId, EventMsg>( CategoryId::EVENT, msgId, msgTextFormat, ap );
    va_end( ap );
}

void Ajax::Logging::logf( AlertMsg msgId, const char* msgTextFormat, ... ) noexcept
{
    va_list ap;
    va_start( ap, msgTextFormat );
    Cpl::Logging::vlogf<CategoryId, AlertMsg>( CategoryId::ALERT, msgId, msgTextFormat, ap );
    va_end( ap );
}

void Ajax::Logging::logf( InfoMsg msgId, const char* msgTextFormat, ... ) noexcept
{
    va_list ap;
    va_start( ap, msgTextFormat );
    Cpl::Logging::vlogf<CategoryId, InfoMsg>( CategoryId::INFO, msgId, msgTextFormat, ap );
    va_end( ap );
}

void Ajax::Logging::logf( MetricsMsg msgId, const char* msgTextFormat, ... ) noexcept
{
    va_list ap;
    va_start( ap, msgTextFormat );
    Cpl::Logging::vlogf<CategoryId, MetricsMsg>( CategoryId::METRICS, msgId, msgTextFormat, ap );
    va_end( ap );
}