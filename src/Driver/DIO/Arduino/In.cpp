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

#include "Driver/DIO/In.h"
#include <stdint.h>

using namespace Driver::DIO;


In::In( int pinConfig, bool assertedHigh )
    : m_pin( pinConfig )
    , m_assertedHigh( assertedHigh )
    , m_started( false )
{
}


bool In::start()
{
    if ( !m_started )
    {
        m_started = true;
        return true;
    }

    return false;
}

void In::stop()
{
    if ( m_started )
    {
        m_started = false;
    }
}

bool In::sample() const
{
    if ( m_started )
    {
        uint32_t phy = digitalRead( m_pin );
        bool     log = phy == HIGH ? true : false;
        return m_assertedHigh ? log : !log;
    }

    return false;
}

