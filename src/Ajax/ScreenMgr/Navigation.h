#ifndef Ajax_ScreenMgr_Navigation_h_
#define Ajax_ScreenMgr_Navigation_h_
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

#include "Ajax/ScreenMgr/ScreenApi.h"

/// 
namespace Ajax {
/// 
namespace ScreenMgr {

 
/** This class defines the interface - used by ScreenApi instances - to navigate
    to other screen instances.
 */
class Navigation
{
public:
    /** This method pushes the current active screen onto the navigation stack
        and makes the specified screen the active screen.

        If the navigation stack is full, then Screen Manager 'logs an error' and
        the Home screen is made the active screen.

        If 'newScreen' is the current home screen - this is consider an error!
        However, when/if this happens, the call has the same behavior as 
        popToHome().  That said the application should NEVER rely on this
        behavior as it is subject to CHANGE without notice.
     */
    virtual void push( ScreenApi& newScreen ) noexcept = 0;

    /** This method pops N-instances off of the navigation stack and make the
        last popped instance the current active screen.  If N exceeds the
        number of items on the stack, the Screen Manager 'logs an error' and
        the Home screen is made the active screen.
     */
    virtual void pop( unsigned count=1 ) noexcept = 0;

    /** This method is similar to pop(), except is pops screen instances till
        there is match for 'returnToScreen'.  If the match is found, then
        'returnToScreen' is made the current active screen.  If no match is
        found, the Screen Manager 'logs an error' and the Home screen is made 
        the active screen.
     */
    virtual void popTo( ScreenApi& returnToScreen ) noexcept = 0;

    /** This method clears the navigation stack and make the Home screen the
        active screen.
     */
    virtual void popToHome() noexcept =0;

public:
    /** Returns a pointer to the current screen.  If there is no current
        screen (e.g. still displaying the splash screen), then nullptr
        is returned.
     */
    virtual ScreenApi* getCurrentScreen() noexcept = 0;

public:
    /// Virtual destructor
    virtual ~Navigation(){}
};

}       // end namespaces
}
#endif  // end header latch

