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
/** @file */

#include "Master.h"
#include "Cpl/System/Assert.h"
#include <string.h>

using namespace Driver::SPI::Arduino;

//////////////////////////////////////////////////////////////////////////////
Master::Master( SPIClass&   spiInstance,
                SPIConfig_T spiConfig,
                uint32_t    timeoutMs )
    : m_spiDevice( spiInstance )
    , m_spiConfig( spiConfig )
    , m_timeout( timeoutMs )
{
}

//////////////////////////////////////////////////////////////////////////////
bool Master::start( size_t newBaudRateHz ) noexcept
{
    if ( !m_started )
    {
        m_started = true;
        m_spiConfig.baudrate = newBaudRateHz;
        m_spiDevice.begin();
        return true;
    }

    return false;
}

void Master::stop() noexcept
{
    if ( m_started )
    {
        m_started = false;
        m_spiDevice.end();
    }
}


bool Master::transfer( size_t      numBytes,
                       const void* srcData,
                       void*       dstData ) noexcept
{
    CPL_SYSTEM_ASSERT( numBytes <= UINT16_MAX )

    if ( m_started )
    {
        // Arduino uses a single 'transfer' buffer -->so copy src to the dst buffer
        memcpy( dstData, srcData, numBytes );

        // Perform the transfer
        SPISettings settings( m_spiConfig.baudrate, m_spiConfig.bitOrder, m_spiConfig.dataMode );
        m_spiDevice.beginTransaction( settings );
        m_spiDevice.transfer( dstData, numBytes );
        m_spiDevice.endTransaction();
        return true;
    }

    return false;
}