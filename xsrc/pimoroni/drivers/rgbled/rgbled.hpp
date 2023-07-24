#pragma once

#include <stdint.h>
#include <math.h>

#include "common/pimoroni_common.hpp"
#include "Driver/DIO/Pwm.h"
#include "Cpl/System/Assert.h"

namespace pimoroni {

class RGBLED {
public:
    RGBLED( Driver::DIO::Pwm& pin_r, Driver::DIO::Pwm& pin_g, Driver::DIO::Pwm& pin_b, Polarity polarity=Polarity::ACTIVE_LOW, uint8_t brightness=255 )
        : pin_r( pin_r )
        , pin_g( pin_g )
        , pin_b( pin_b )
        , polarity( polarity )
        , led_r( 0 )
        , led_g( 0 )
        , led_b( 0 )
        , led_brightness( brightness ) {

        CPL_SYSTEM_ASSERT( pin_r.getMaxDutyCycle() == 0xFFFF );
        CPL_SYSTEM_ASSERT( pin_g.getMaxDutyCycle() == 0xFFFF );
        CPL_SYSTEM_ASSERT( pin_b.getMaxDutyCycle() == 0xFFFF );

        uint16_t p = 0;
        if ( polarity == Polarity::ACTIVE_LOW ) {
            p = UINT16_MAX - p;
        }
        pin_r.start( p );
        pin_g.start( p );
        pin_b.start( p );
    };

    ~RGBLED() {
        pin_r.stop();
        pin_g.stop();
        pin_b.stop();
    }
    void set_rgb( uint8_t r, uint8_t g, uint8_t b );
    void set_hsv( float h, float s, float v );
    void set_brightness( uint8_t brightness );
private:
    Driver::DIO::Pwm& pin_r;
    Driver::DIO::Pwm& pin_g;
    Driver::DIO::Pwm& pin_b;
    Polarity polarity;

    uint8_t led_r;
    uint8_t led_g;
    uint8_t led_b;
    uint8_t led_brightness;

    void update_pwm();
};

}