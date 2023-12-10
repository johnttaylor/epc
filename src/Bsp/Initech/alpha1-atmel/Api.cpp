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

#include "Bsp/Api.h"
#include "Cpl/Io/Serial/Arduino/InputOutput.h"

#define SECT_   "bsp"


static Cpl::Io::Serial::Arduino::InputOutput fd_;
extern Cpl::Io::InputOutput& Bsp_Serial( void );



///////////////////////////////////////////
void Bsp_Api_initialize( void )
{
    // Configure Board specific IO
    pinMode( PIN_RESET_LCD, OUTPUT );
    pinMode( PIN_LCD_DC, OUTPUT );
    pinMode( PIN_PWM_LED_RED, OUTPUT );
    pinMode( PIN_PWM_LED_GREEN, OUTPUT );
    pinMode( PIN_PWM_LED_BLUE, OUTPUT );
    pinMode( PIN_PWM_LCD_BACKLIGHT, OUTPUT );
    pinMode( PIN_LCD_CS, OUTPUT );

    pinMode( PIN_BUTTON_A, INPUT_PULLUP );
    pinMode( PIN_BUTTON_B, INPUT_PULLUP );
    pinMode( PIN_BUTTON_X, INPUT_PULLUP );
    pinMode( PIN_BUTTON_Y, INPUT_PULLUP );

    // Configure the LEDs as output pins 
    pinMode( OPTION_BSP_DEBUG_LED1_INDEX, OUTPUT );

    // Initialize the MCU's TRNG
    MCLK->APBCMASK.bit.TRNG_   = 1; // Enable Main Clock (MCLK) for TRNG
    TRNG->CTRLA.bit.ENABLE     = 1; // Enable TRNG
    TRNG->INTENSET.bit.DATARDY = 1; // Enable TRNG interrupt when data ready
}

void Bsp_beginArduinoSerialObject( unsigned long baudrate, uint16_t frameConfig )
{
    fd_.start( baudrate, frameConfig );
    //while ( fd_.isReady() == false )
    //    ;
}

Cpl::Io::InputOutput& Bsp_Serial( void )
{
    return fd_;
}




