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
        Ajax::Dm::MpAlert::Data alertInfo;
        if ( m_data.activeAlerts[i]->read( alertInfo ) )
        {
            arrayObj.add( alertInfo.name._to_string() );
        }
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
            // Get the MP name string
            const char* mpname = list[i];
            if ( mpname != nullptr )
            {
                // Lookup the MP (and validate it IS a MpAlert instance)
                ModelPoint* mpPtr = m_modelDatabase.lookupModelPoint( mpname );
                if ( mpPtr && strcmp( mpPtr->getTypeAsText(), "Ajax::Dm::MpAlert" ) == 0 )
                {
                    newData.activeAlerts[i] = (Ajax::Dm::MpAlert*) mpPtr;
                    newData.count++;
                }
            }
        }

        retSequenceNumber = write( newData, lockRequest );
        return true;
    }

    return false;
}
