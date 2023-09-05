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
#include "Cpl/Dm/Mp/Uint64.h"
#include "Cpl/Dm/Mp/Int32.h"
#include "Cpl/Dm/Mp/String.h"
#include "Ajax/ScreenMgr/MpScreenApiPtr.h"
#include "Ajax/ScreenMgr/MpStaticScreenApiPtr.h"
#include "Ajax/Dm/MpFanMode.h"
#include "Ajax/Constants.h"


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

/** This model point holds the firmware's version string

    \b Units: n/a

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::String<OPTION_AJAX_MAX_VERSION_LENGTH>  fwVersion;

/** This model point holds the product's Model Number string

    \b Units: n/a

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::String<OPTION_AJAX_MAX_MODEL_LENGTH>    modelNumber;


/** This model point holds the product's Serial Number string

    \b Units: n/a

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::String<OPTION_AJAX_MAX_SERIAL_NUM_LENGTH> serialNumber;

/** This model point holds the current FanSpeed setting

    \b Units: n/a

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Ajax::Dm::MpFanMode  fanMode;

/** This model point holds the current temperature setpoint

    \b Units: hundreds of degrees Fahrenheit (75.3'F -->7530)

    \b Range: OPTION_JAX_MIN_HEATING_SETPOINT_F - OPTION_JAX_MAX_HEATING_SETPOINT_F

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Int32   heatSetpoint;


/** This model point holds the current Mode - On/Off

    \b Units: bool

    \b Range: true:     Active heating allowed
              false:    No active heating

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Bool    heatingMode;


/** This model point holds metrics counter: Boot counter.
    Is incremented every time the MCU is reset

    \b Units: free running counter

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint32  metricBootCounter;

/** This model point holds metrics counter: Running time.
    Is updated every N seconds of elapsed time.

    \b Units: seconds

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint64  metricRunningTime;

/** This model point holds metrics counter: Heater on time.
    Is updated every M seconds of when the heater is commanded to be on

    \b Units: seconds

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint64  metricHeaterOnTime;

/** This model point holds metrics counter: Fan on time.
    Is updated every M seconds of when the Fan is commanded to be on

    \b Units: seconds

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint64  metricFanOnTime;

/** This model point holds metrics counter: Temperature Sensor Fault counter.
    Is incremented every time a temperature sensor fault is raised.

    \b Units: free running counter

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint32  metricFaultTempSensor;

/** This model point holds metrics counter: Heater Safety Fault counter.
    Is incremented every time the HW based heater safety circuit 'tripped'
    input signal to the MCU is asserted.

    \b Units: free running counter

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint32  metricFaultHeaterSafety;

/** This model point is contains the current count of number of elements
    in the Logging queue.  It is used to for generating change notifications
    to the Persistent storage framework to write record(s) to the physical
    media.

    \b Units: counter

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint32  loggingQueCount;

/** This model point holds the latest 'log key' for the last log entry
    written to persistent storage.

    \b Units: free running counter

    \b Range: n/a

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint64  latestLoggingEntryKey;

/*---------------------------------------------------------------------------*/
/// The Application's Model Point Database
extern Cpl::Dm::ModelDatabase   g_modelDatabase;


}       // end namespace
#endif  // end header latch
