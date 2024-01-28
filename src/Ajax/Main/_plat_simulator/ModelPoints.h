#ifndef Ajax_Main_Simulated_ModelPoints_h_
#define Ajax_Main_Simulated_ModelPoints_h_
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
 */

#include "Cpl/Dm/ModelDatabase.h"
#include "Cpl/Dm/Mp/Uint32.h"
#include "Cpl/Dm/Mp/Bool.h"

/** Encapsulate all Model Points in the 'mp' namespace to prevent polluting
    the global name space
 */
namespace mp {

/** This model point holds commanded PWM output signal for the Heater PWM signal

    \b Units: duty cycle 

    \b Range: 0 to OPTION_DRIVER_DIO_PWM_MAX_DUTY_CYCLE_VALUE

    \b Notes: 
        n/a
*/
extern Cpl::Dm::Mp::Uint32 mockedHeaterPwmOut;

/** This model point holds commanded PWM output signal for the Fan PWM signal

    \b Units: duty cycle

    \b Range: 0 to OPTION_DRIVER_DIO_PWM_MAX_DUTY_CYCLE_VALUE

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Uint32 mockedFanPwmOut;

/** This model point holds 'source' value for the HW Safety Limit Tripped
    input signal

    \b Units: boolean

    \b Range: 
        true:=   Safety Limit asserted
        false: = normal operation

    \b Notes:
        n/a
*/
extern Cpl::Dm::Mp::Bool mockedHwSafetyIn;


/*---------------------------------------------------------------------------*/
/// The Application's Model Point Database
extern Cpl::Dm::ModelDatabase   g_modelDatabase;


}       // end namespace
#endif  // end header latch
