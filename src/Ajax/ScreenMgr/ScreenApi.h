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

#include "Cpl/Container/Item.h"
#include <stdint.h>

/// 
namespace Ajax {
/// 
namespace ScreenMgr {


 
/** This class defines the interface for a 'Screen'.  

    The current elapsed time (since power-up) is passed as argument to all 
    of the calls.

    The screen supports being in containers - HOWEVER - the container use
    is reserved to the ScreenMgr namespace.
 */
class ScreenApi: Cpl::Container::ExtendedItem
{
public:
    /** This method is called as part of making the screen the 'active' screen
     */
    virtual void enter( uint32_t currentElapsedTimeMs ) noexcept = 0;

    /** This method is called as part of the sequence where the screen instance
        is no longer the active screen.
     */
    virtual void exit( uint32_t currentElapsedTimeMs ) noexcept = 0;

public:
    /** This method is used to inform the active screen that the display has
        been 'turned off'.  When the screen is over - no content is visible to
        user.  The actual state of the display RAM is indeterminate
     */
    virtual void sleep( uint32_t currentElapsedTimeMs ) noexcept = 0;

    /** This method is used to inform the active screen that the display has 
        been turned back on.  The Screen Manager is responsible for triggering
        a refresh() call if the display RAM needs to be updated.
     */
    virtual void wake( uint32_t currentElapsedTimeMs ) noexcept = 0;

public:
    /** This method is used to dispatch/forward a UI event to active screen.
     */
    virtual void dispatch( AjaxScreenMgrEvent_T event, uint32_t currentElapsedTimeMs ) noexcept = 0;

    /** This method is used to notifiy the active screen that the Screen Managers
        20Hz timer has expired (i.e. called every 50ms)
     */
    virtual void tick( uint32_t currentElapsedTimeMs );

    /** This method requests the active screen to ensure the its screen contents
        are up to date.  If the screen contents have not changed since the 
        last time the method was - the method returns false.  Returning true
        forces a physical display update.
      */
    virtual bool refresh( uint32_t currentElapsedTimeMs );

public:
    /// Virtual destructor
    virtual ~ScreenApi(){}
};

}       // end namespaces
}
#endif  // end header latch

