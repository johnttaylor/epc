#ifndef Ajax_Dm_MpFlcConfig_h_
#define Ajax_Dm_MpFlcConfig_h_
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


#include "Cpl/Dm/ModelPointCommon_.h"
#include "Ajax/Heating/Flc/Config.h"

///
namespace Ajax {
///
namespace Dm {


/** This class provides a concrete implementation for a Point who's data a
    Ajax::Heating::Flc::Config_t data structure

    The toJSON/fromJSON() format is:
    \code

    { name:"<mpname>", type=:<mptypestring>", valid:true|false, seqnum:nnnn, locked:true|false, 
      val:{ "outScaler":nnn, "maxY": nnn, "inMaxX":nnn, "inMinX":nnn, "inPoints":[n,n,n,n,n], "outPoints":[n,n,n,n,n] }
    }

    \endcode


    NOTE: All methods in this class ARE thread Safe unless explicitly
          documented otherwise.
 */
class MpFlcConfig : public Cpl::Dm::ModelPointCommon_
{
public:
    /// Constructor. Invalid MP. 
    MpFlcConfig( Cpl::Dm::ModelDatabase& myModelBase, const char* symbolicName )
        :Cpl::Dm::ModelPointCommon_( myModelBase, symbolicName, &m_data, sizeof( m_data ), false )
    {
    }

    /// Constructor. Valid MP.  Requires an initial value
    MpFlcConfig( Cpl::Dm::ModelDatabase& myModelBase, const char* symbolicName, Cpl::System::ElapsedTime::Precision_T initialValue )
        :Cpl::Dm::ModelPointCommon_( myModelBase, symbolicName, &m_data, sizeof( m_data ), true )
    {
        m_data = initialValue;
    }

public:
    /// Type safe read. See Cpl::Dm::ModelPoint
    inline bool read( Cpl::System::ElapsedTime::Precision_T& dstData, uint16_t* seqNumPtr=0 ) const noexcept
    {
        return readData( &dstData, sizeof( m_data ), seqNumPtr );
    }

    /// Type safe write. See Cpl::Dm::ModelPoint
    inline uint16_t write( Cpl::System::ElapsedTime::Precision_T newValue, LockRequest_T lockRequest = eNO_REQUEST ) noexcept
    {
        return writeData( &newValue, sizeof( m_data ), lockRequest );
    }

    /// Updates the MP with the valid-state/data from 'src'. Note: the src.lock state is NOT copied
    inline uint16_t copyFrom( const MpFlcConfig& src, LockRequest_T lockRequest = eNO_REQUEST ) noexcept
    {
        return copyDataAndStateFrom( src, lockRequest );
    }

public:
    /// Type safe subscriber
    typedef Cpl::Dm::Subscriber<MpFlcConfig> Observer;

    /// Type safe register observer
    void attach( Observer& observer, uint16_t initialSeqNumber=SEQUENCE_NUMBER_UNKNOWN ) noexcept;

    /// Type safe un-register observer
    void detach( Observer& observer ) noexcept;

    /// See Cpl::Dm::ModelPointCommon
    inline bool readAndSync( Cpl::System::ElapsedTime::Precision_T& dstData, SubscriberApi& observerToSync )
    {
        return ModelPointCommon_::readAndSync( &dstData, sizeof( m_data ), observerToSync );
    }

public:
    ///  See Cpl::Dm::ModelPoint.
    const char* getTypeAsText() const noexcept;

    /// See Cpl::Dm::Point.  
    bool fromJSON_( JsonVariant& src, LockRequest_T lockRequest, uint16_t& retSequenceNumber, Cpl::Text::String* errorMsg ) noexcept;

protected:
    /// See Cpl::Dm::Point.  
    void setJSONVal( JsonDocument& doc ) noexcept;

protected:
    /// My data
    Cpl::System::ElapsedTime::Precision_T m_data;
};



};      // end namespaces
};
#endif  // end header latch
