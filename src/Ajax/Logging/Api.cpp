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


bool Cpl::Logging::getIDStrings( uint32_t             categoryNumericValue,
                                 uint16_t             messageIdNumericValue,
                                 Cpl::Text::String&   dstCategoryString,
                                 Cpl::Text::String&   dstMessageString ) noexcept
{
    switch ( categoryNumericValue )
    {
    case Ajax::Logging::CategoryId::CRITICAL:
        if ( Ajax::Logging::CriticalMsg::_is_valid( messageIdNumericValue ) )
        {
            dstCategoryString = (+Ajax::Logging::CategoryId::CRITICAL)._to_string();
            dstMessageString  = Ajax::Logging::CriticalMsg::_from_integral_unchecked( messageIdNumericValue )._to_string();
            return true;
        }
        break;
    case Ajax::Logging::CategoryId::WARNING:
        if ( Ajax::Logging::WarningMsg::_is_valid( messageIdNumericValue ) )
        {
            dstCategoryString = (+Ajax::Logging::CategoryId::WARNING)._to_string();
            dstMessageString  = Ajax::Logging::WarningMsg::_from_integral_unchecked( messageIdNumericValue )._to_string();
            return true;
        }
        break;
    case Ajax::Logging::CategoryId::EVENT:
        if ( Ajax::Logging::EventMsg::_is_valid( messageIdNumericValue ) )
        {
            dstCategoryString = (+Ajax::Logging::CategoryId::EVENT)._to_string();
            dstMessageString  = Ajax::Logging::EventMsg::_from_integral_unchecked( messageIdNumericValue )._to_string();
            return true;
        }
        break;
    case Ajax::Logging::CategoryId::INFO:
        if ( Ajax::Logging::InfoMsg::_is_valid( messageIdNumericValue ) )
        {
            dstCategoryString = (+Ajax::Logging::CategoryId::INFO)._to_string();
            dstMessageString  = Ajax::Logging::InfoMsg::_from_integral_unchecked( messageIdNumericValue )._to_string();
            return true;
        }
        break;
    case Ajax::Logging::CategoryId::METRICS:
        if ( Ajax::Logging::MetricsMsg::_is_valid( messageIdNumericValue ) )
        {
            dstCategoryString = (+Ajax::Logging::CategoryId::METRICS)._to_string();
            dstMessageString  = Ajax::Logging::MetricsMsg::_from_integral_unchecked( messageIdNumericValue )._to_string();
            return true;
        }
        break;
    case Ajax::Logging::CategoryId::ALERT:
        if ( Ajax::Logging::AlertMsg::_is_valid( messageIdNumericValue ) )
        {
            dstCategoryString = (+Ajax::Logging::CategoryId::ALERT)._to_string();
            dstMessageString  = Ajax::Logging::AlertMsg::_from_integral_unchecked( messageIdNumericValue )._to_string();
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}
