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
/* This file is generated from Fsm.cdd - do not edit manually  */
/* Generated on:  Version 6.0.2.3601 */


#ifndef FSM_AJAX_HEATING_SUPERVISOR_EXT_H
#define FSM_AJAX_HEATING_SUPERVISOR_EXT_H

// Events which can be sent to the state-machine

#ifndef DOXYGEN_WILL_SKIP_THIS

namespace Ajax { namespace Heating { namespace Supervisor  {

    enum FSM_EVENT_T {
            Fsm_evEnabled=0U,
            Fsm_evHiTemp,
            Fsm_evNoTempSensor,
            Fsm_evDisabled,
            Fsm_evSafeTemp,
            Fsm_evSensorAvailable,
            FSM_NO_MSG
    };






}}} //namespace Ajax { namespace Heating { namespace Supervisor 


#endif // !DOXYGEN_WILL_SKIP_THIS

#endif
