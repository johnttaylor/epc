#ifndef Ajax_Dm_MpAlertSummary_h_
#define Ajax_Dm_MpAlertSummary_h_
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

#include "Cpl/Dm/ModelPointCommon_.h"
#include "Ajax/Dm/MpAlert.h"


///
namespace Ajax {
///
namespace Dm {


/** This class provides a concrete implementation for an Alert Summary

    The toJSON() method is a read/modify operation, i.e. omitted key/value 
    fields in the 'val' object are NOT updated.

    The toJSON()/fromJSON format is:
    \code

    { name:"<mpname>", type:"<mptypestring>", valid:true|false, seqnum:nnnn, locked:true|false, 
      val:{"count":1, active:["<alertEnum>",...]} }


    NOTE: When writing the 'val' object to the MP, ONLY the 'active' field is 
          written and the 'count' field is derived from the list count. For 
          example:
            {name:"<mpname>,val:["<alertEnum>", ...]}

    \endcode

    NOTE: All methods in this class ARE thread Safe unless explicitly
          documented otherwise.
 */
class MpAlertSummary : public Cpl::Dm::ModelPointCommon_
{
public:
    /** The MP's Data container.
     */
    struct Data
    {
        unsigned          count;                                        //!< Number of currently active Alerts
        Ajax::Type::Alert activeAlerts[Ajax::Type::Alert::NUM_ALERTS];  //!< Sorted (by priority) list of active Alerts

        /// Constructor (to ensure any pad bytes get zero'd)
        Data()
        {
            memset( (void*)this, 0, sizeof( Data ) );
        }
    };

protected:
    /// Storage for the MP's data
    Data m_data;

public:
    /** Constructor (MP in the invalid state)
     */
    MpAlertSummary( Cpl::Dm::ModelDatabase& myModelBase, const char* symbolicName );

public:
    /// Type safe read. See Cpl::Dm::ModelPoint
    inline bool read( Data& dstData, uint16_t* seqNumPtr = 0 ) const noexcept
    {
        return readData( &dstData, sizeof( Data ), seqNumPtr );
    }

    /// Type safe write. See Cpl::Dm::ModelPoint
    inline uint16_t write( const Data& srcData, LockRequest_T lockRequest = eNO_REQUEST ) noexcept
    {
        return writeData( &srcData, sizeof( Data ), lockRequest );
    }

public:
    /// Updates the MP with the valid-state/data from 'src'. Note: the src.lock state is NOT copied
    inline uint16_t copyFrom( const MpAlertSummary& src, LockRequest_T lockRequest = eNO_REQUEST ) noexcept
    {
        return copyDataAndStateFrom( src, lockRequest );
    }

public:
    /// Type safe subscriber
    typedef Cpl::Dm::Subscriber<MpAlertSummary> Observer;

    /// Type safe register observer
    void attach( Observer& observer, uint16_t initialSeqNumber = SEQUENCE_NUMBER_UNKNOWN ) noexcept;

    /// Type safe un-register observer
    void detach( Observer& observer ) noexcept;

    /// See Cpl::Dm::ModelPointCommon
    inline bool readAndSync( Data& dstData, Cpl::Dm::SubscriberApi& observerToSync )
    {
        uint16_t seqNum;
        return ModelPointCommon_::readAndSync( &dstData, sizeof( Data ), seqNum, observerToSync );
    }

    /// See Cpl::Dm::ModelPointCommon
    inline bool readAndSync( Data& dstData, uint16_t& seqNum, Cpl::Dm::SubscriberApi& observerToSync )
    {
        return ModelPointCommon_::readAndSync( &dstData, sizeof( Data ), seqNum, observerToSync );
    }

public:
    /// See Cpl::Dm::ModelPoint.
    const char* getTypeAsText() const noexcept;

    /// See Cpl::Dm::Point.
    bool fromJSON_( JsonVariant& src, LockRequest_T lockRequest, uint16_t& retSequenceNumber, Cpl::Text::String* errorMsg ) noexcept;

protected:
    /// See Cpl::Dm::Point.
    void setJSONVal( JsonDocument& doc ) noexcept;
};


};  // end namespaces
};
#endif  // end header latch
