#ifndef Ajax_Constants_h_
#define Ajax_Constants_h_
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
/** @file 
    
    This file contains magic values that are specific to the Ajax application
    (but not to any one single component)

*/

#include "colony_config.h"


/// Maximum heating setpoint in hundreds of degrees Fahrenheit
#ifndef OPTION_JAX_MAX_HEATING_SETPOINT_F
#define OPTION_JAX_MAX_HEATING_SETPOINT_F       (85*100)
#endif

/// Minimum heating setpoint in hundreds of degrees Fahrenheit
#ifndef OPTION_JAX_MIN_HEATING_SETPOINT_F
#define OPTION_JAX_MIN_HEATING_SETPOINT_F       (55*100)
#endif

/** Maximum size, in bytes, a version string can be.  Does NOT include the
    null terminator
 */
#ifndef OPTION_AJAX_MAX_VERSION_LENGTH
#define OPTION_AJAX_MAX_VERSION_LENGTH      16  // Assumed format is: mm.nn.pp-#######
#endif

/** Maximum size, in bytes, a model string can be.  Does NOT include the
    null terminator
 */
#ifndef OPTION_AJAX_MAX_MODEL_LENGTH
#define OPTION_AJAX_MAX_MODEL_LENGTH        16
#endif

 /** Maximum size, in bytes, a serial number string can be.  Does NOT include the
     null terminator
  */
#ifndef OPTION_AJAX_MAX_SERIAL_NUM_LENGTH
#define OPTION_AJAX_MAX_SERIAL_NUM_LENGTH   16
#endif

#endif  // end header latch
