#ifndef Ajax_ScreenMgr_EventQueue_h_
#define Ajax_ScreenMgr_EventQueue_h_
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

#include "colony_map.h"

#include <stdint.h>
#include <string.h>

/** Application specific type for Events. 
 */
#define AjaxScreenMgrEvent_T                AjaxScreenMgrEvent_T_MAP

/// 
namespace Ajax {
/// 
namespace ScreenMgr {

/** This class an interface for Event Queue.  The application is expected to
    provide an 'Event Monitor' that is responsible for generating the events.
    The Screen Manager is the consumer of the Events.

    The Queue has FIFO semantics.  
 */
class EventQueue
{
public:
    /** Inserts event into the Queue. If the queue is full, the method returns
        false and the event will be discarded and the Screen Manager will 
        'log an error'.  True is return when the event was successfully added to
        the Queue.
     */
    virtual bool add( AjaxScreenMgrEvent_T event ) noexcept = 0;

public:
    /// Virtual destructor
    virtual ~EventQueue(){}
};

}      // end namespaces
}
#endif  // end header latch

