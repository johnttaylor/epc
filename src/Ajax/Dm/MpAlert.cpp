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


#include "MpAlert.h"

///
using namespace Ajax::Dm;


///////////////////////////////////////////////////////////////////////////////
MpAlert::MpAlert( Cpl::Dm::ModelDatabase& myModelBase, const char* symbolicName, uint8_t priority )
    : ModelPointCommon_( myModelBase, symbolicName, &m_data, sizeof( m_data ), false )
    , m_data( { priority, false } )
{
}


///////////////////////////////////////////////////////////////////////////////
void MpAlert::hookSetInvalid() noexcept
{
    // Override parent implement -->Do NOT clear the priority field
}

uint16_t MpAlert::raiseAlert( LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();
    Data newData( m_data.priority, false );
    uint16_t result = writeData( &newData, sizeof( Data ), lockRequest );
    m_modelDatabase.unlock_();

    return result;
}

uint16_t MpAlert::acknowledgeAlert( LockRequest_T lockRequest ) noexcept
{

    m_modelDatabase.lock_();
    Data newData( m_data.priority, true );
    uint16_t result = writeData( &newData, sizeof( Data ), lockRequest );
    m_modelDatabase.unlock_();

    return result;
}

///////////////////////////////////////////////////////////////////////////////
void MpAlert::attach( Observer& observer, uint16_t initialSeqNumber ) noexcept
{
    attachSubscriber( observer, initialSeqNumber );
}

void MpAlert::detach( Observer& observer ) noexcept
{
    detachSubscriber( observer );
}

const char* MpAlert::getTypeAsText() const noexcept
{
    return "Ajax::Dm::MpAlert";
}

void MpAlert::setJSONVal( JsonDocument& doc ) noexcept
{
    JsonObject valObj  = doc.createNestedObject( "val" );
    valObj["ack"]      = m_data.acked;
    valObj["priority"] = m_data.priority;
}

bool MpAlert::fromJSON_( JsonVariant& src, LockRequest_T lockRequest, uint16_t& retSequenceNumber, Cpl::Text::String* errorMsg ) noexcept
{
    if ( src.is<JsonObject>() )
    {
        Data updatedData = m_data;

        // Parse Fields
        if ( src["ack"].is<bool>() )
        {
            updatedData.acked = src["ack"];
        }
        if ( src["priority"].is<uint8_t>() )
        {
            updatedData.priority = src["priority"];
        }

        retSequenceNumber = write( updatedData, lockRequest );
        return true;
    }

    if ( errorMsg )
    {
        errorMsg->format( "Invalid key/value for 'val'" );
    }
    return false;
}
