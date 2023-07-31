#ifndef Ajax_ScreenMgr_StaticScreenApi_h_
#define Ajax_ScreenMgr_StaticScreenApi_h_
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

#include <stdint.h>

/// 
namespace Ajax {
/// 
namespace ScreenMgr {


 
/** This class defines interface for Screens that only contain static data. 
    The expected usage for this class is for screen like a 'Splash' screen.

    The current elapsed time (since power-up) is passed as argument to all
    of the calls.
 */
class StaticScreenApi
{
    /** This method requests the update/set/draw its screen contents
      */
    virtual void paint( uint32_t currentElapsedTimeMs ) noexcept = 0;

public:
    /// Virtual destructor
    virtual ~StaticScreenApi(){}
};

}       // end namespaces
}
#endif  // end header latch

