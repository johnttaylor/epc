#ifndef Ajax_ScreenMgr_MpScreenApiPtr_h_
#define Ajax_ScreenMgr_MpScreenApiPtr_h_
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


#include "Cpl/Dm/Mp/Numeric.h"

///
namespace Ajax {
///
namespace ScreenMgr {

/** Forward reference for a ScreenApi pointer (breaks undesired dependencies)
 */
class ScreenApi;

/** This class provides a concrete implementation for a Point who's data is a
	ScreenApi pointer.  
	
	The toJSON()/fromJSON format is:
		\code

		{ name:"<mpname>", type:"<mptypestring>", valid:true|false, seqnum:nnnn, locked:true|false, val:"<hexvalue>" }

		\endcode

 NOTE: All methods in this class ARE thread Safe unless explicitly
		  documented otherwise.
 */
class MpScreenApiPtr : public Cpl::Dm::Mp::Pointer_<MpScreenApiPtr>
{
public:
    /// Constructor. Invalid MP. 
    MpScreenApiPtr( Cpl::Dm::ModelDatabase& myModelBase, const char* symbolicName )
        :Pointer_<MpScreenApiPtr>( myModelBase, symbolicName )
    {
    }

    /// Constructor. Valid MP.  Requires an initial value
    MpScreenApiPtr( Cpl::Dm::ModelDatabase& myModelBase, const char* symbolicName, void* initialValue )
        : Pointer_<MpScreenApiPtr>( myModelBase, symbolicName, initialValue )
    {
    }


public:
    /// Type safe read. See Cpl::Dm::ModelPoint
    inline bool read( ScreenApi*& dstData, uint16_t* seqNumPtr = 0 ) const noexcept
    {
        return readData( &dstData, sizeof( void* ), seqNumPtr );
    }

    /// Type safe write. See Cpl::Dm::ModelPoint
    inline uint16_t write( ScreenApi* newValue, Cpl::Dm::ModelPoint::LockRequest_T lockRequest = Cpl::Dm::ModelPoint::eNO_REQUEST ) noexcept
    {
        return writeData( &newValue, sizeof( void* ), lockRequest );
    }

public:
    /// Type safe subscriber
    typedef Cpl::Dm::Subscriber<MpScreenApiPtr> Observer;

    /// See Cpl::Dm::ModelPointCommon
    inline bool readAndSync( ScreenApi*& dstData, Cpl::Dm::SubscriberApi& observerToSync )
    {
        return ModelPointCommon_::readAndSync( &dstData, sizeof( void* ), observerToSync );
    }

    ///  See Cpl::Dm::ModelPoint.
    const char* getTypeAsText() const noexcept
    {
        return "Ajax::ScreenMgr::MpScreenApiPtr";
    }
};



}       // end namespaces
}
#endif  // end header latch
