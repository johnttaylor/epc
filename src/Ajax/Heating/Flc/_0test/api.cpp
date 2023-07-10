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

#include "Catch/catch.hpp"
#include "Ajax/Heating/Flc/Api.h"    
#include "Cpl/System/_testsupport/Shutdown_TS.h"


using namespace Ajax::Heating::Flc;

////////////////////////////////////////////////////////////////////////////////

static int32_t k1_[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ={ -16, -8, 0, 8, 16 };
static int32_t k2_[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ={ -20, -10, 0, 10, 20 };

////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "api" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    SECTION( "baseline" )
    {
        Config_T cfg;
        cfg.maxY         = 1000;
        cfg.outputScalar = 1000;
        cfg.errScalar    = 4;
        cfg.dErrScalar   = 16;
        memcpy( cfg.outK, k2_, sizeof( cfg.outK ) );

        Api uut( cfg );
        // set an initial previous error
        int32_t r = uut.calcChange( 0, 76 );
        //printf( "r0= %d\n", r );
        REQUIRE( r == 3040 );

        // Expecting a err value of 0.8' and dErr of 0.04, with a final output of 3200
        r = uut.calcChange( 0, 80 );
        //printf( "r1= %d\n", r );
        REQUIRE( r == 3200 );

        r = uut.calcChange( 0, 80 );
        //printf( "r2= %d\n", r );
        REQUIRE( r == 3200 );
    }

#define MIN_PWM     200
#define MAX_PWM     1000
#define CLAMP_PWM(pwm, adj) {pwm+=adj; if ( pwm < MIN_PWM ) {pwm = MIN_PWM;}if ( pwm > MAX_PWM ){pwm = MAX_PWM;}}
#define DISPLAY( t, newPwm, curTemp, setpt, adjust ) \
    printf( "T=%03d, pwm=%03d, temp=%4d, err=%4d, adjust=%4d\n", \
             t, newPwm, curTemp, setpt-curTemp, adjust )

    SECTION( "ramp-up" )
    {
        // NOT REALLY A TEST -->just a manual "sanity check"
        Config_T cfg;
        cfg.maxY         = 1000;
        cfg.outputScalar = 10;
        cfg.errScalar    = 4;
        cfg.dErrScalar   = 16;
        memcpy( cfg.outK, k2_, sizeof( cfg.outK ) );

        Api uut( cfg );
        int32_t curTemp   = 70 * 100;
        int32_t setpoint  = 72 * 100;
        int32_t pwm       = 0;
        int32_t pwmAdjust = 0;
        unsigned t        = 0;

        pwmAdjust = uut.calcChange( curTemp, setpoint );
        CLAMP_PWM( pwm, pwmAdjust );
        DISPLAY( t, pwm, curTemp, setpoint, pwmAdjust );

        t++;
        pwmAdjust = uut.calcChange( curTemp, setpoint );
        CLAMP_PWM( pwm, pwmAdjust );
        DISPLAY( t, pwm, curTemp, setpoint, pwmAdjust );

        t++;
        curTemp += 20;  // increase 0.2'
        pwmAdjust = uut.calcChange( curTemp, setpoint );
        CLAMP_PWM( pwm, pwmAdjust );
        DISPLAY( t, pwm, curTemp, setpoint, pwmAdjust );

        t++;
        curTemp += 40;  // increase 0.4'
        pwmAdjust = uut.calcChange( curTemp, setpoint );
        CLAMP_PWM( pwm, pwmAdjust );
        DISPLAY( t, pwm, curTemp, setpoint, pwmAdjust );

        t++;
        curTemp += 20;  // increase 0.2'
        pwmAdjust = uut.calcChange( curTemp, setpoint );
        CLAMP_PWM( pwm, pwmAdjust );
        DISPLAY( t, pwm, curTemp, setpoint, pwmAdjust );
    }

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}