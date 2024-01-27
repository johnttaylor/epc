#ifndef Driver_AIO_Arduino_HALSingleIinput_h_
#define Driver_AIO_Arduino_HALSingleIinput_h_
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
#include "wiring_analog.h"

/// Target supports 12 bit ADC. Set 0 to disable 
#ifndef HAS_DRIVER_AIO_ARDUINO_HAL_SINGLE_INPUT_12ADC
#define HAS_DRIVER_AIO_ARDUINO_HAL_SINGLE_INPUT_12ADC       1 
#endif

/// Target supports 16 bit ADC. Set 0 to disable 
#ifndef HAS_DRIVER_AIO_ARDUINO_HAL_SINGLE_INPUT_16ADC
#define HAS_DRIVER_AIO_ARDUINO_HAL_SINGLE_INPUT_16ADC       0
#endif

/*-------------- PUBLIC API ------------------------------------------------*/
/** This method is used to initialize the GPIO for the pin/configuration specified
    by 'analogInputPin'
 */
void driverAIOHalSingleInputArduino_initialize( int analogInputPin, eAnalogReference avref );


/*--------------------------------------------------------------------------*/
#endif  
