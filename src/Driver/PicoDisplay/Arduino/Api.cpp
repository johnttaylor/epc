/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2022  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "Api.h"
#include "drivers/st7789/st7789.hpp"    
#include "Driver/PicoDisplay/Api.h"
#include "Driver/LED/Pimoroni/RedGreenBlue.h"
#include "Driver/Button/Arduino/Hal.h"
#include "Driver/DIO/Out.h"
#include "Driver/DIO/Pwm.h"
#include "Driver/SPI/Arduino/Master.h"
#include "Cpl/System/Api.h"



static DriverButtonPinHalArduino_T     pinButtonA_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_A_PIN, INPUT_PULLUP, true );
static DriverButtonPinHalArduino_T     pinButtonB_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_B_PIN, INPUT_PULLUP, true );
static DriverButtonPinHalArduino_T     pinButtonX_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_X_PIN, INPUT_PULLUP, true );
static DriverButtonPinHalArduino_T     pinButtonY_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BUTTON_Y_PIN, INPUT_PULLUP, true );
static Driver::Button::PolledDebounced buttonA_( pinButtonA_ );
static Driver::Button::PolledDebounced buttonB_( pinButtonB_ );
static Driver::Button::PolledDebounced buttonX_( pinButtonX_ );
static Driver::Button::PolledDebounced buttonY_( pinButtonY_ );

static Driver::DIO::Pwm                pwmLedRDriver_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_RGB_RED_PWM  );
static Driver::DIO::Pwm                pwmLedGDriver_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_RGB_GREEN_PWM );
static Driver::DIO::Pwm                pwmLedBDriver_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_RGB_BLUE_PWM );

static Driver::LED::Pimoroni::RedGreenBlue rgbLEDDriver_( pwmLedRDriver_,
                                                          pwmLedGDriver_,
                                                          pwmLedBDriver_ );

static Driver::DIO::Out              csDriver_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_SPI_CS_PIN, false );
static Driver::DIO::Out              dcDriver_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_SPI_DC_PIN, false );
static Driver::DIO::Pwm              pwmDriver_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_BACKLIGHT_PWM );
static Driver::SPI::Arduino::Master::SPIConfig_T cfg_( 8000000, MSBFIRST, SPI_MODE3 );
static Driver::SPI::Arduino::Master              spiDriver_( OPTION_DRIVER_PICO_DISPLAY_ARDUINO_SPI_BUS, cfg_ );


pimoroni::ST7789 st7789_( pimoroni::PicoDisplay::WIDTH, pimoroni::PicoDisplay::HEIGHT, pimoroni::ROTATE_0, false, spiDriver_, csDriver_, dcDriver_, pwmDriver_ );



/////////////////////////////////////////////
void Driver::PicoDisplay::Arduino::initialize()
{
    st7789_.start(); 
    rgbLEDDriver_.start();

    driverButtonHalArduino_initialize( buttonA_.getHandle() );
    driverButtonHalArduino_initialize( buttonB_.getHandle() );
    driverButtonHalArduino_initialize( buttonX_.getHandle() );
    driverButtonHalArduino_initialize( buttonY_.getHandle() );
}

Driver::Button::PolledDebounced& Driver::PicoDisplay::Api::buttonA() { return buttonA_; }
Driver::Button::PolledDebounced& Driver::PicoDisplay::Api::buttonB() { return buttonB_; }
Driver::Button::PolledDebounced& Driver::PicoDisplay::Api::buttonX() { return buttonX_; }
Driver::Button::PolledDebounced& Driver::PicoDisplay::Api::buttonY() { return buttonY_; }

Driver::LED::RedGreenBlue& Driver::PicoDisplay::Api::rgbLED() { return rgbLEDDriver_; }

void Driver::PicoDisplay::Api::updateLCD( pimoroni::PicoGraphics& graphics )
{
    st7789_.update( &graphics );
}
void Driver::PicoDisplay::Api::setLCDBrightness( uint8_t brightnessLevel )
{
    st7789_.set_backlight( brightnessLevel );
}

void Driver::PicoDisplay::Api::nop()
{
    // Do NOTHING!
}
