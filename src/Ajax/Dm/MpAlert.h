#ifndef Ajax_Dm_MpAlert_h_
#define Ajax_Dm_MpAlert_h_
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
#include "Ajax/Type/Alert.h"



///
namespace Ajax {
///
namespace Dm {


/** This class provides a concrete implementation for a basic Alert that has
    following attributes:

        - Tracks the 'user acknowledgeable' state
        - Associates a priority level (relative to other instances) to the Alert
    
    The MP's valid/invalid state is used to indicate that the Alert is 
    raised/lowered respectively

    The toJSON() method is a read/modify operation, i.e. omitted key/value 
    fields in the 'val' object are NOT updated.

    The toJSON()/fromJSON format is:
    \code

    { name:"<mpname>", type:"<mptypestring>", valid:true|false, seqnum:nnnn, locked:true|false, 
      val:{"name":"<AlertEnum>, "priority":nn} }

    \endcode

    The Alert MP does NOT really have any run-time modifiable fields - it uses
    valid/invalid state for active/inactive state of the Alert.  To set an Alert MP
    to the valid/active-alert -->set the "val" field to true, e.g.:
        { name:"alertApple", val:true }

    NOTE: All methods in this class ARE thread Safe unless explicitly
          documented otherwise.
 */
class MpAlert : public Cpl::Dm::ModelPointCommon_
{
public:
    /** The MP's Data container.
     */
    struct Data
    {
        Ajax::Type::Alert   name;           //!< Identifies the Alert instance
        uint8_t             priority;       //!< Priority of the alarm.  Zero is highest, 255 is the lowest

        /// Constructor (to ensure any pad bytes get zero'd)
        Data()
        {
            memset( (void*) this, 0, sizeof( Data ) );
        }

        /// Constructor (to ensure any pad bytes get zero'd)
        Data( Ajax::Type::Alert alertName, uint8_t priority )
        {
            memset( (void*) this, 0, sizeof( Data ) );
            this->name     = alertName;
            this->priority = priority;
        }

        /// Constructor (to ensure any pad bytes get zero'd)
        Data( Data& other )
        {
            memcpy( (void*) this, &other, sizeof( Data ) );
        }
    };

protected:
    /// Storage for the MP's data
    Data                m_data;

public:
    /** Constructor (MP in the invalid state)
     */
    MpAlert( Cpl::Dm::ModelDatabase& myModelBase, 
             const char*             symbolicName, 
             Ajax::Type::Alert       alertIdentifer,
             uint8_t                 priority );

public:
    /// Type safe read. See Cpl::Dm::ModelPoint
    inline bool read( Data& dstData, uint16_t* seqNumPtr=0 ) const noexcept
    {
        return readData( &dstData, sizeof( Data ), seqNumPtr );
    }

    /// Type safe write. See Cpl::Dm::ModelPoint
    inline uint16_t write( const Data& srcData, LockRequest_T lockRequest = eNO_REQUEST ) noexcept
    {
        return writeData( &srcData, sizeof( Data ), lockRequest );
    }

    /// Sets the Alert state
    uint16_t raiseAlert( LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /// Clears/lowers the Alert.  
    uint16_t lowerAlert( LockRequest_T lockRequest = eNO_REQUEST ) noexcept
    {
        return setInvalid();
    }

public:
    /// Updates the MP with the valid-state/data from 'src'. Note: the src.lock state is NOT copied
    inline uint16_t copyFrom( const MpAlert& src, LockRequest_T lockRequest = eNO_REQUEST ) noexcept
    {
        return copyDataAndStateFrom( src, lockRequest );
    }

public:
    /// Type safe subscriber
    typedef Cpl::Dm::Subscriber<MpAlert> Observer;

    /// Type safe register observer
    void attach( Observer& observer, uint16_t initialSeqNumber=SEQUENCE_NUMBER_UNKNOWN ) noexcept;

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

    /// Set valid default values when the MP is invalidated
    void hookSetInvalid() noexcept;
};



};      // end namespaces
};
#endif  // end header latch
