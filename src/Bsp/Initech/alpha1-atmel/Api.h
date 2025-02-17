#ifndef Bsp_grand_central_m4_Api_h_
#define Bsp_grand_central_m4_Api_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2017  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file


    This BSP is developed/build with a 'just-in-time' approach.  This means
    as functional is added to the BSP incrementally as there is 'client'
    need for.  This BSP does not claim to provide full/complete functional
    and/or APIs for everything the board supports.


    DO NOT include this file directly! Instead include the generic BSP
    interface - src/Bsp/Api.h - and then configure your project's
    'colony_map.h' to include THIS file.

*----------------------------------------------------------------------------*/


#include "colony_config.h"
#include "FreeRTOS.h"
#include "USBAPI.h"
#include "HardwareSerial.h"
#include "variant.h"

//////////////////////////////////////////////////////////
/// Board Specific APIs
//////////////////////////////////////////////////////////
/** Initialize the primary serial port AND it waits for
    a Host to connect to the serial port.
*/
void Bsp_beginArduinoSerialObject( unsigned long baudrate, uint16_t frameConfig );

/** This method initializes and start the SdFat file system on the onboard 8MB
    data flash
 */
void Bsp_beginFileSystem();

/** Returns a reference to the Primary Serial Port Object. This is implicitly
    exposed (can't directly expose it due to racing conditions on how the colony_map.h
    header files work).  This means that the application must cut-n-paste the following
    extern statement to get access to this method.

    extern Cpl::Io::InputOutput& Bsp_Serial(void);
*/

/* Serial Object's Frame Configuration Options.  Note: These
   values are copied from HardwareSerial.h header file because
   including the HardwareSerial.h file directly breaks all sorts
   of things (i.e. the Arduino stuff is not namespace protected)
*/
#define HARDSER_PARITY_EVEN         (0x1ul)
#define HARDSER_PARITY_ODD	        (0x2ul)
#define HARDSER_PARITY_NONE         (0x3ul)
#define HARDSER_PARITY_MASK	        (0xFul)

#define HARDSER_STOP_BIT_1		    (0x10ul)
#define HARDSER_STOP_BIT_1_5	    (0x20ul)
#define HARDSER_STOP_BIT_2	 	    (0x30ul)
#define HARDSER_STOP_BIT_MASK	    (0xF0ul)

#define HARDSER_DATA_5	 	        (0x100ul)
#define HARDSER_DATA_6	 	        (0x200ul)
#define HARDSER_DATA_7	 	        (0x300ul)
#define HARDSER_DATA_8	 	        (0x400ul)
#define HARDSER_DATA_MASK	        (0xF00ul)

#define SERIAL_5N1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_NONE | HARDSER_DATA_5)
#define SERIAL_6N1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_NONE | HARDSER_DATA_6)
#define SERIAL_7N1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_NONE | HARDSER_DATA_7)
#define SERIAL_8N1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_NONE | HARDSER_DATA_8)
#define SERIAL_5N2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_NONE | HARDSER_DATA_5)
#define SERIAL_6N2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_NONE | HARDSER_DATA_6)
#define SERIAL_7N2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_NONE | HARDSER_DATA_7)
#define SERIAL_8N2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_NONE | HARDSER_DATA_8)
#define SERIAL_5E1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_EVEN | HARDSER_DATA_5)
#define SERIAL_6E1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_EVEN | HARDSER_DATA_6)
#define SERIAL_7E1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_EVEN | HARDSER_DATA_7)
#define SERIAL_8E1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_EVEN | HARDSER_DATA_8)
#define SERIAL_5E2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_EVEN | HARDSER_DATA_5)
#define SERIAL_6E2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_EVEN | HARDSER_DATA_6)
#define SERIAL_7E2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_EVEN | HARDSER_DATA_7)
#define SERIAL_8E2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_EVEN | HARDSER_DATA_8)
#define SERIAL_5O1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_ODD  | HARDSER_DATA_5)
#define SERIAL_6O1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_ODD  | HARDSER_DATA_6)
#define SERIAL_7O1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_ODD  | HARDSER_DATA_7)
#define SERIAL_8O1	                (HARDSER_STOP_BIT_1 | HARDSER_PARITY_ODD  | HARDSER_DATA_8)
#define SERIAL_5O2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_ODD  | HARDSER_DATA_5)
#define SERIAL_6O2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_ODD  | HARDSER_DATA_6)
#define SERIAL_7O2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_ODD  | HARDSER_DATA_7)
#define SERIAL_8O2	                (HARDSER_STOP_BIT_2 | HARDSER_PARITY_ODD  | HARDSER_DATA_8)

//////////////////////////////////////////////////////////
/// Board Specific APIs
//////////////////////////////////////////////////////////

/// Arduino Pin Number
#define PIN_ONBOARD_IDT         PIN_A0

/// Arduino Pin Number
#define PIN_BUTTON_A            12

/// Arduino Pin Number
#define PIN_BUTTON_B            11

/// Arduino Pin Number
#define PIN_BUTTON_X            10 

/// Arduino Pin Number
#define PIN_BUTTON_Y            9

/// Arduino Pin Number
#define PIN_HW_SAFETY           8

/// Arduino Pin Number
#define PIN_RESET_LCD           7

/// Arduino Pin Number
#define PIN_LCD_DC              6

/// Arduino Pin Number
#define PIN_PWM_LED_RED         5 

/// Arduino Pin Number
#define PIN_PWM_LED_GREEN       4

/// Arduino Pin Number
#define PIN_PWM_LED_BLUE        3

/// Arduino Pin Number
#define PIN_PWM_LCD_BACKLIGHT   2

/// Arduino Pin Number
#define PIN_LCD_CS              53

/// Arduino Pin Number
#define PIN_PWM_HEATER          18

/// Arduino Pin Number
#define PIN_PWM_FAN             19

/// Arduino Pin Number (alias)
#define PIN_LCD_SPI_CLK         PIN_SPI_SCK

/// Arduino Pin Number (alias)
#define PIN_LCD_SPI_MOSI        PIN_SPI_MOSI

/// Arduino Pin Number (alias)
#define PIN_LCD_SPI_MISO        PIN_SPI_MISO

/// Arduino SPI driver instance (alias)
#define LCD_SPI_DRIVER          SPI


/// Arduino Pin Number (alias)
#define PIN_I2C_BUS_SCK         PIN_WIRE_SCL

/// Arduino Pin Number (alias)
#define PIN_I2C_BUS_SDA         PIN_WIRE_SDA

/// Arduino I2C driver instance (alias)
#define I2C_BUS_DRIVER          Wire

/// 7bit address for the offboard EEPROM
#define BSP_I2C_ADDRESS_EEPROM      0x50


//////////////////////////////////////////////////////////
/// ARM Specific APIs
//////////////////////////////////////////////////////////

/// Disable a specific interrupt (with memory barrier protection)
#define Bsp_NVIC_disableIRQ( irqNum )   do { NVIC_DisableIRQ(irqNum); __DSB(); __ISB(); } while(0)

/// Enable a specific interrupt (with memory barrier protection)
#define Bsp_NVIC_enableIRQ( irqNum )    do { NVIC_EnableIRQ(irqNum); __DSB(); __ISB(); } while(0)

/// Resets the MCU
#define Bsp_Api_reset_MCU()             NVIC_SystemReset()

//////////////////////////////////////////////////////////
/// Generic APIs
//////////////////////////////////////////////////////////

/// Generic API
#define Bsp_Api_nop_MAP()                       __asm("nop")                

/// Generic API
#define Bsp_Api_disableIrqs_MAP()               taskENTER_CRITICAL()

/// Generic API
#define Bsp_Api_enableIrqs_MAP()                taskEXIT_CRITICAL()

/// Generic API
#define Bsp_Api_pushAndDisableIrqs_MAP()        taskENTER_CRITICAL()    // FIXME: This really needs to PUSH the IRQ state!!!

/// Generic API
#define Bsp_Api_popIrqs_MAP()                   taskEXIT_CRITICAL()     // FIXME: This really needs to POP the IRQ state!!!!




/// Default Mapping for the "debug LED 1"
#ifndef OPTION_BSP_DEBUG_LED1_INDEX
#define OPTION_BSP_DEBUG_LED1_INDEX             PIN_LED
#endif

/// Default Mapping for the "debug LED 2"
#ifndef OPTION_BSP_DEBUG_LED2_INDEX
#define OPTION_BSP_DEBUG_LED2_INDEX             DEBUG_LED_IS_NOT_SUPPORTED
#endif


/// Generic API
#define Bsp_Api_turnOn_debug1_MAP()             digitalWrite( OPTION_BSP_DEBUG_LED1_INDEX, HIGH )

/// Generic API
#define Bsp_Api_turnOff_debug1_MAP()            digitalWrite( OPTION_BSP_DEBUG_LED1_INDEX, LOW )

/// Generic API
#define Bsp_Api_toggle_debug1_MAP()             digitalWrite( OPTION_BSP_DEBUG_LED1_INDEX, !digitalRead(OPTION_BSP_DEBUG_LED1_INDEX) )


/// Generic API
#define Bsp_Api_turnOn_debug2_MAP()             digitalWrite( OPTION_BSP_DEBUG_LED2_INDEX, HIGH )

/// Generic API
#define Bsp_Api_turnOff_debug2_MAP()            digitalWrite( OPTION_BSP_DEBUG_LED2_INDEX, LOW )

/// Generic API
#define Bsp_Api_toggle_debug2_MAP()             digitalWrite( OPTION_BSP_DEBUG_LED2_INDEX, !digitalRead(OPTION_BSP_DEBUG_LED2_INDEX) )



#endif  // end header latch
