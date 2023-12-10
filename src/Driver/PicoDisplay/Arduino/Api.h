#ifndef Driver_PicoDisplay_Arduino_API_h
#define Driver_PicoDisplay_Arduino_API_h
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2022  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file

    This file defines the interface for initializing the concrete implementation
    of the PicoDisplay driver using the actual hardware

 */

#include "colony_config.h"
#include "pimoroni_common.hpp"
#include "pico_display.hpp"


/// Pin for Button A
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_A_PIN
#error Must define: OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_A_PIN      
#endif

/// Pin for Button B
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_B_PIN
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_B_PIN      
#endif

/// Pin for Button X
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_X_PIN
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_X_PIN      
#endif

/// Pin for Button Y
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_Y_PIN
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_Y_PIN      
#endif

/// RGB LED: The Timer for the RED PWM signal 
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_RGB_RED_PWM
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_RGB_RED_PWM       
#endif

/// RGB LED: The Timer for the GREEN PWM signal 
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_RGB_GREEN_PWM  
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_RGB_GREEN_PWM     
#endif

/// RGB LED: The Timer for the BLUE PWM signal 
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_RGB_BLUE_PWM 
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_RGB_BLUE_PWM      
#endif

/// The default screen orientation
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_LCD_ROTATION         
#define OPTION_DRIVER_PICO_DISPLAY_ARDUINO_LCD_ROTATION      pimoroni::ROTATE_0
#endif

/// The default SPI bus
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_SPI_BUS
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_SPI_BUS           
#endif

/// The default SPI pins - Pin
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_SPI_CS_PIN
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_SPI_CS_PIN
#endif

/// The default SPI pins (Data/Command pin for the LCD controller) - Pin
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_SPI_DC_PIN
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_SPI_DC_PIN            
#endif

/// The default PWM signal to the backlight - Timer
#ifndef OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BACKLIGHT_PWM
#error Must define:  OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BACKLIGHT_PWM     
#endif


///
namespace Driver {
///
namespace PicoDisplay {
///
namespace Arduino {

/** This method is used to initialize the platform specific drivers
 */
void initialize();


} // End namespace(s)
}
}

#endif // end header latch
