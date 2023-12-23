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


#include "MpAlertSummary.h"

///
using namespace Ajax::Dm;


///////////////////////////////////////////////////////////////////////////////
MpAlertSummary::MpAlertSummary( Cpl::Dm::ModelDatabase& myModelBase, const char* symbolicName )
    : ModelPointCommon_( myModelBase, symbolicName, &m_data, sizeof( m_data ), false )
{
}


///////////////////////////////////////////////////////////////////////////////
void MpAlertSummary::attach( Observer& observer, uint16_t initialSeqNumber ) noexcept
{
    attachSubscriber( observer, initialSeqNumber );
}

void MpAlertSummary::detach( Observer& observer ) noexcept
{
    detachSubscriber( observer );
}

const char* MpAlertSummary::getTypeAsText() const noexcept
{
    return "Ajax::Dm::MpAlertSummary";
}

void MpAlertSummary::setJSONVal( JsonDocument& doc ) noexcept
{
    JsonObject valObj  = doc.createNestedObject( "val" );
    valObj["count"]    = m_data.count;
    JsonArray arrayObj = valObj.createNestedArray( "active" );
    for ( unsigned i = 0; i < m_data.count; i++ )
    {
        arrayObj.add( m_data.activeAlerts[i]._to_string() );
    }
}

bool MpAlertSummary::fromJSON_( JsonVariant& src, LockRequest_T lockRequest, uint16_t& retSequenceNumber, Cpl::Text::String* errorMsg ) noexcept
{
    if ( src.is<JsonArray>() )
    {
        JsonArray list = src;
        Data      newData;
        for ( unsigned i=0; i < list.size(); i++ )
        {
            // Get the enum
            const char* alertenum = list[i];
            if ( alertenum != nullptr )
            {
                auto maybeValue = Ajax::Type::Alert::_from_string_nothrow( alertenum );
                if ( maybeValue )
                {
                    newData.activeAlerts[i] = *maybeValue;
                    newData.count++;
                }
            }
        }

        retSequenceNumber = write( newData, lockRequest );
        return true;
    }

    return false;
}
