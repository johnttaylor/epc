#ifndef mp_ModelPoints_h_
#define mp_ModelPoints_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015-2023  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file

    This file defines all of the Model Points for Ajax application.

    All of the model points are placed into the 'mp' namespace.  The model
    point names map 1-to-1 with the instance names.

    The Application/Client is RESPONSIBLE for ensuring input values are with
    the defined range for the model point instance.
 */

#include "Cpl/Dm/ModelDatabase.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/Mp/Uint32.h"
#include "Ajax/ScreenMgr/MpScreenApiPtr.h"
#include "Ajax/ScreenMgr/MpStaticScreenApiPtr.h"

/** Encapsulate all Model Points in the 'mp' namespace to prevent polluting
    the global name space
 */
namespace mp {

/** This method point is used to set the shutdown screen. A write to this MP
    triggers a transition to the shutdown screen

    \b Units: n/a

    \b Range: n/a

    \b Notes: 
        n/a
*/
extern Ajax::ScreenMgr::MpStaticScreenApiPtr shutdownScrPtr;

/** This method point is used to set the error/halt screen. A write to this MP
    triggers a transition to the error/halt screen.  The UI effectively becomes
    inoperable once it transitions to the error/halt screen.

    \b Units: n/a

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Ajax::ScreenMgr::MpStaticScreenApiPtr errorScrPtr;

/** This model point is used to set the Home Screen for the UI.  The first
    time the MP is written to, the UI transition from the splash screen to 
    the Home screen.  The Home Screen can be changed at as needed 

    \b Units: n/a

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Ajax::ScreenMgr::MpScreenApiPtr homeScrPtr;

/** This model point is used to turn the display on/off at run time

    \b Units: bool

    \b Range: true:  -->triggers turning OFF the display
              false: -->triggers turning back ON the display

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Bool    displaySleepTrigger;

/** This model point is used to by the UI's Event Queue.  It contains the
    current element count for the Event Queue.  Change notification from this
    MP are used to trigger event processing.

    \b Units: counter

    \b Range: 0 - N, where N is the size of the Event Queue

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint32  uiEventQueueCount;


/*---------------------------------------------------------------------------*/
/// The Application's Model Point Database
extern Cpl::Dm::ModelDatabase   g_modelDatabase;


}       // end namespace
#endif  // end header latch
