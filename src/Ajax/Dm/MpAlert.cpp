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
MpAlert::MpAlert( Cpl::Dm::ModelDatabase& myModelBase,
                  const char*             symbolicName,
                  Ajax::Type::Alert       alertIdentifer,
                  uint8_t                 priority )
    : ModelPointCommon_( myModelBase, symbolicName, &m_data, sizeof( m_data ), false )
    , m_data( alertIdentifer, priority )
{
}


///////////////////////////////////////////////////////////////////////////////
void MpAlert::hookSetInvalid() noexcept
{
    // Override parent implement -->Do NOT clear the priority or name fields
}

uint16_t MpAlert::raiseAlert( LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();
    Data newData( m_data );  
    uint16_t result = writeData( &newData, sizeof( Data ), lockRequest ); // set the MP to the valid state
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
    valObj["name"]     = m_data.name._to_string();
    valObj["priority"] = m_data.priority;
}

bool MpAlert::fromJSON_( JsonVariant& src, LockRequest_T lockRequest, uint16_t& retSequenceNumber, Cpl::Text::String* errorMsg ) noexcept
{
    if ( src.is<bool>() )
    {
        Data updatedData = m_data;
        if ( src.as<bool>() )
        {
            retSequenceNumber = write( updatedData, lockRequest );
            return true;
        }
    }

    if ( errorMsg )
    {
        errorMsg->format( "Invalid key/value for 'val' (only supports 'val:true')" );
    }
    return false;
}
