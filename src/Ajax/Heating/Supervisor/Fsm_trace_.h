/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2020  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

/* Command line options: -p CADIFRA -doxygen -o Fsm -l cppx -Trace Fsm.cdd   */
/* This file is generated from Fsm_trace - do not edit manually  */
/* Generated on: Version 6.0.2.3601 */ 


#ifndef FSM_TRACE_AJAX_HEATING_SUPERVISOR_H
#define FSM_TRACE_AJAX_HEATING_SUPERVISOR_H

#include "Cpl/System/Trace.h"

/// Trace Section
#define SECT_ "Ajax::Heating::Supervisor::Fsm"


#include <stdint.h> 

#ifndef DOXYGEN_WILL_SKIP_THIS

namespace Ajax { namespace Heating { namespace Supervisor  {


    /*Simulation strings which can be sent to the state-machine statediagram.simulator via UDP */
    /*or to print which event triggered a state change in trace code.  */
    #define FSM_TRACE_EVENT_LEN (7U)

#define FsmTraceEvent(a) CPL_SYSTEM_TRACE_MSG( SECT_, ( "  Old State=%s, Event=%s", getNameByState(getInnermostActiveState()), FsmTraceEvents[a] ));

    const char* FsmTraceEvents[] = {
    "evHiTemp",
    "evSafeTemp",
    "evEnabled",
    "evSensorAvailable",
    "evNoTempSensor",
    "evDisabled",
    "__INIT__"
    };


}}} //namespace Ajax { namespace Heating { namespace Supervisor 


#endif // !DOXYGEN_WILL_SKIP_THIS

#endif
