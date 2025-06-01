/*-----------------------------------------------------------------------------
 * This file is part of the Colony.Core Project.  The Colony.Core Project is an
 * open source project with a BSD type of licensing agreement.  See the license
 * agreement (license.txt) in the top/ directory or on the Internet at
 * http://integerfox.com/colony.core/license.txt
 *
 * Copyright (c) 2014-2025  John T. Taylor
 *
 * Redistributions of the source code must retain the above copyright notice.
 *----------------------------------------------------------------------------*/
/** @file */

#include "MasterHalfDuplex.h"
#include "Cpl/System/Assert.h"
#include <stdint.h>

using namespace Driver::SPI::STM32;

//////////////////////////////////////////////////////////////////////////////
MasterHalfDuplex::MasterHalfDuplex( SPI_HandleTypeDef* spiInstance,
                                    uint32_t           timeoutMs )
    : m_spiDevice( spiInstance )
    , m_timeout( timeoutMs )
{
}

//////////////////////////////////////////////////////////////////////////////
bool MasterHalfDuplex::start( size_t newBaudRateHz ) noexcept
{
    if ( !m_started )
    {
        m_started = true;

        // TODO: Support setting/changing the baud rate

        return true;
    }

    return false;
}

void MasterHalfDuplex::stop() noexcept
{
    if ( m_started )
    {
        m_started = false;

        /* TODO: Support freeing up the GPIO pins.  This require being able to
                 config the SPI peripheral/pins independently of the ST MX tools
        */
    }
}

bool MasterHalfDuplex::transmit( size_t      numBytes,
                                 const void* srcData ) noexcept
{
    CPL_SYSTEM_ASSERT( numBytes <= UINT16_MAX )

    if ( m_started && srcData )
    {
        HAL_StatusTypeDef r = HAL_SPI_Transmit( m_spiDevice, (uint8_t*)srcData, (uint16_t)numBytes, m_timeout );
        return r == HAL_OK;
    }

    return false;
}

bool MasterHalfDuplex::receive( size_t numBytes,
                                void*  dstData ) noexcept
{
    CPL_SYSTEM_ASSERT( numBytes <= UINT16_MAX )

    if ( m_started && dstData )
    {
        HAL_StatusTypeDef r = HAL_SPI_Receive( m_spiDevice, (uint8_t*)dstData, (uint16_t)numBytes, m_timeout );
        return r == HAL_OK;
    }

    return false;
}
