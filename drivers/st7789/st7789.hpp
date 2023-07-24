#pragma once

#include "common/pimoroni_common.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "Driver/SPI/Master.h"
#include "Driver/DIO/Out.h"
#include "Driver/DIO/Pwm.h"

// Platform independent, SPI only driver

/** Back-light AP2502 controller:
    PWM EN signal: PWM frequency 100Hz to 150KHz, nominal range is: 100Hz to 10KHz
    Duty cycle controls brightness
 */

namespace pimoroni {

class ST7789 : public DisplayDriver {

public:
    bool round;

    //--------------------------------------------------
    // Variables
    //--------------------------------------------------
private:
    Driver::SPI::Master& spi;
    Driver::DIO::Out&    cs;
    Driver::DIO::Out&    dc;
    Driver::DIO::Pwm&    bl;


    // The ST7789 requires 16 ns between SPI rising edges.
    // 16 ns = 62,500,000 Hz
    static const uint32_t SPI_BAUD = 62'500'000;


public:

    /** Serial init
        csDriver -->must be configured for Active Low
        dcDriver -->must be confifured for Active Low
        blDirver -->must be configured for PWM frequency range of: 100Hz to 150KHz
     */
    ST7789( uint16_t             width,
            uint16_t             height,
            Rotation             rotation,
            bool                 round,
            Driver::SPI::Master& spiDriver,
            Driver::DIO::Out&    csDriver,
            Driver::DIO::Out&    dcDriver,
            Driver::DIO::Pwm&    blDriver )
        : DisplayDriver( width, height, rotation )
        , round( round )
        , spi( spiDriver )
        , cs( csDriver )
        , dc( dcDriver )
        , bl( blDriver )
    {
    }

    void start();
    void cleanup() override;
    void update( PicoGraphics *graphics ) override;
    void set_backlight( uint8_t brightness ) override;

private:
    void common_init();
    void configure_display( Rotation rotate );
    void command( uint8_t command, size_t len = 0, const char *data = NULL );
};

}
