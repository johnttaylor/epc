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

#include "Driver/DIO/Pwm.h"
#include <stdint.h>


#define ARDUINO_MAX_DUTY_CYCLE      255

using namespace Driver::DIO;


Pwm::Pwm( int pwmConfig )
    : m_pwm( pwmConfig )
    , m_started( false )

{
}

bool Pwm::start( size_t initialLogicalDutyCycle )
{
    if ( !m_started )
    {
        m_started = true;
        setDutyCycle( initialLogicalDutyCycle );
        return true;
    }

    return true;
}

void Pwm::stop()
{
    if ( m_started )
    {
        // TODO: Research Arduino framework to see how to stop/'free' the PIN
        setDutyCycle( 0 );
        m_started = false;
    }
}


void Pwm::setDutyCycle( size_t logicalDutyCycle )
{
    if ( m_started )
    {
        // Scale the logical duty-cycle to the hardware range
        size_t dutyCycle = logicalDutyCycle / (ARDUINO_MAX_DUTY_CYCLE + 1);
        if ( dutyCycle > ARDUINO_MAX_DUTY_CYCLE )
        {
            dutyCycle = ARDUINO_MAX_DUTY_CYCLE;
        }

        // Set the output
        analogWrite( m_pwm, dutyCycle );
    }
}
