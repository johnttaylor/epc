#ifndef Ajax_ScreenMgr_ScreenApi_h_
#define Ajax_ScreenMgr_ScreenApi_h_
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

#include "Ajax/ScreenMgr/Event.h"
#include "Cpl/System/ElapsedTime.h"

/// 
namespace Ajax {
/// 
namespace ScreenMgr {


 
/** This class defines the interface for a 'Screen'.  

    The current elapsed time (since power-up) is passed as argument to all 
    of the calls.
 */
class ScreenApi
{
public:
    /** This method is called as part of making the screen the 'active' screen
     */
    virtual void enter( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept = 0;

    /** This method is called as part of the sequence where the screen instance
        is no longer the active screen.
     */
    virtual void exit( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept = 0;

public:
    /** This method is used to inform the active screen that the display has
        been 'turned off'.  When the screen is over - no content is visible to
        user.  The actual state of the display RAM is indeterminate
     */
    virtual void sleep( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept = 0;

    /** This method is used to inform the active screen that the display has 
        been turned back on.  The Screen Manager is responsible for triggering
        a refresh() call if the display RAM needs to be updated.
     */
    virtual void wake( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept = 0;

public:
    /** This method is used to dispatch/forward a UI event to active screen.
     */
    virtual void dispatch( AjaxScreenMgrEvent_T event, Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept = 0;

    /** This method is used to notify the active screen that the Screen Managers
        20Hz timer has expired (i.e. called every 50ms)

        NOTE: A software timer is used, so the accuracy of the callback frequency
              is NOT guaranteed
     */
    virtual void tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept = 0;

    /** This method requests the active screen to ensure the its screen contents
        are up to date.  If the screen contents have not changed since the 
        last time the method was - the method returns false.  Returning true
        forces a physical display update.
      */
    virtual bool refresh( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept = 0;

public:
    /// Virtual destructor
    virtual ~ScreenApi(){}
};

}       // end namespaces
}
#endif  // end header latch

