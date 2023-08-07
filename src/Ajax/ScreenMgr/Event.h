#ifndef Ajax_ScreenMgr_Event_h_
#define Ajax_ScreenMgr_Event_h_
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


#include "colony_config.h"
#include "colony_map.h"

/** Application specific type for Events. 
 */
#define AjaxScreenMgrEvent_T                AjaxScreenMgrEvent_T_MAP

/// Number of events that the event queue can hold
#ifndef OPTION_AJAX_SCREEN_MGR_EVENT_QUEUE_SIZE
#define OPTION_AJAX_SCREEN_MGR_EVENT_QUEUE_SIZE     5
#endif

#endif  // end header latch

