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

/// Alert priority: Hi-Temp Alert (lower number == higher priority)
#define AJAX_ALERT_PRIORITY_HI_TEMP         1

/// Alert priority: No Sensor Available Alert (lower number == higher priority)
#define AJAX_ALERT_PRIORITY_SENSOR_FAILED   0     


/// Maximum heating setpoint in hundreds of degrees Fahrenheit
#ifndef OPTION_AJAX_MAX_HEATING_SETPOINT_F
#define OPTION_AJAX_MAX_HEATING_SETPOINT_F       (85*100)
#endif

/// Minimum heating setpoint in hundreds of degrees Fahrenheit
#ifndef OPTION_AJAX_MIN_HEATING_SETPOINT_F
#define OPTION_AJAX_MIN_HEATING_SETPOINT_F       (55*100)
#endif

/// Maximum supported indoor temperature value in hundreds of degrees Fahrenheit (outside of the range is considered an error)
#ifndef OPTION_AJAX_MAX_INDOOR_TEMPERATURE_F
#define OPTION_AJAX_MAX_INDOOR_TEMPERATURE_F     (100*100)
#endif

/// Minimum supported indoor temperature value in hundreds of degrees Fahrenheit (outside of the range is considered an error)
#ifndef OPTION_AJAX_MIN_INDOOR_TEMPERATURE_F
#define OPTION_AJAX_MIN_INDOOR_TEMPERATURE_F     (40*100)
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

/// Size, in bytes, of the Hashed password
#ifndef OPTION_AJAX_HASHED_PASSWORD_SIZE
#define OPTION_AJAX_HASHED_PASSWORD_SIZE            64
#endif

/// Size, in bytes, of the salt used when hashing the password
#ifndef OPTION_AJAX_HASHED_PASSWORD_SALT_SIZE
#define OPTION_AJAX_HASHED_PASSWORD_SALT_SIZE       16
#endif

/// Number of iteration to use when hashing a password
#ifndef OPTION_AJAX_HASHED_PASSWORD_ITERATIONS
#define OPTION_AJAX_HASHED_PASSWORD_ITERATIONS      2000    // ~2.3 seconds on the STM32 target
#endif

/// Maximum,size in bytes, of a password
#ifndef OPTION_AJAX_PASSWORD_MAX_LENGTH
#define OPTION_AJAX_PASSWORD_MAX_LENGTH             32
#endif

/// Minimum,size in bytes, of a password
#ifndef OPTION_AJAX_PASSWORD_MIN_LENGTH
#define OPTION_AJAX_PASSWORD_MIN_LENGTH             12
#endif

#endif  // end header latch

