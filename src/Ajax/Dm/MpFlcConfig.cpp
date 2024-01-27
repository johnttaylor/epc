/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2022  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */


#include "MpFlcConfig.h"
#include "Cpl/Text/format.h"
#include "Cpl/Text/atob.h"
#include "Cpl/Text/FString.h"

///
using namespace Ajax::Dm;


///////////////////////////////////////////////////////////////////////////////
void MpFlcConfig::attach( Observer& observer, uint16_t initialSeqNumber ) noexcept
{
    attachSubscriber( observer, initialSeqNumber );
}

/// Type safe un-register observer
void MpFlcConfig::detach( Observer& observer ) noexcept
{
    detachSubscriber( observer );
}

const char* MpFlcConfig::getTypeAsText() const noexcept
{
    return "Ajax::Dm::MpFlcConfig";
}


///////////////////////////////////////////////////////////////////////////////
void MpFlcConfig::setJSONVal( JsonDocument& doc ) noexcept
{
    JsonObject valObj    = doc.createNestedObject( "val" );
    valObj["maxY"]       = m_data.maxY;
    valObj["outScalar"]  = m_data.outputScalar;
    valObj["dErrScalar"] = m_data.dErrScalar;
    valObj["errScalar"]  = m_data.errScalar;
    JsonArray array      = valObj.createNestedArray( "outK" );
    for ( unsigned i =0; i < AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS; i++ )
    {
        array.add( m_data.outK[i] );
    }
}

bool MpFlcConfig::fromJSON_( JsonVariant& src, LockRequest_T lockRequest, uint16_t& retSequenceNumber, Cpl::Text::String* errorMsg ) noexcept
{
    Ajax::Heating::Flc::Config_T updatedData = m_data;
    
    // Parse Numeric Fields
    if ( src["maxY"].is<int32_t>() )
    {
        updatedData.maxY = src["maxY"].as<int32_t>();
    }
    if ( src["outScalar"].is<int32_t>() )
    {
        updatedData.outputScalar = src["outScalar"].as<int32_t>();
    }
    if ( src["dErrScalar"].is<int32_t>() )
    {
        updatedData.dErrScalar = src["dErrScalar"].as<int32_t>();
    }
    if ( src["errScalar"].is<int32_t>() )
    {
        updatedData.errScalar = src["errScalar"].as<int32_t>();
    }

    // Parse Array
    if ( src["outK"].is<JsonArray>() )
    {
        for ( unsigned i=0; i < src["outK"].as<JsonArray>().size() && i < AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS; i++ )
        {
            updatedData.outK[i] = src["outK"].as<JsonArray>()[i];
        }
    }

    // Update the MP
    retSequenceNumber = write( updatedData, lockRequest );
    return true;
}


