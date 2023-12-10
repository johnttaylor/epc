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
#include "Cpl/System/Trace.h"
#include <string.h>

#define SECT_ "Driver::SPI::Arduino"

using namespace Driver::SPI::Arduino;

//////////////////////////////////////////////////////////////////////////////
Master::Master( SPIClass&   spiInstance,
                SPIConfig_T spiConfig )
    : m_spiDevice( spiInstance )
    , m_spiConfig( spiConfig )
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
        
        // Only need to call this once (the exception is when using SPI interrupts - which this driver not)
        SPISettings settings( m_spiConfig.baudrate, m_spiConfig.bitOrder, m_spiConfig.dataMode );
        m_spiDevice.beginTransaction( settings );    
        return true;
    }

    return false;
}

void Master::stop() noexcept
{
    if ( m_started )
    {
        m_started = false;
        m_spiDevice.endTransaction();
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
        // Bi-directional transfer
        if ( dstData != nullptr )
        {
            // Arduino uses a single 'transfer' buffer -->so copy src to the dst buffer
            memcpy( dstData, srcData, numBytes );

            // Perform the transfer
            m_spiDevice.transfer( dstData, numBytes );
        }

        // Output only transfer
        else
        {
            // NOTE: Since it is possible that 'srcData' points to read-only 
            //       data (e.g. a buffer in flash memory), but the Arduino SPI 
            //       API requires a writable buffer - we create a temporary 
            //       writable buffer on the stack.
            
            // Fail transaction if there is not sufficient buffer space
            if ( numBytes > OPTION_DRIVER_SPI_ARDUINO_OUTPUT_ONLY_BUF_SIZE )
            {
                CPL_SYSTEM_TRACE_MSG( SECT_, ("Transfer failed because local 'srcBuffer' is too small. size=%u, needed size=%u", OPTION_DRIVER_SPI_ARDUINO_OUTPUT_ONLY_BUF_SIZE, numBytes) );
                return false;
            }

            uint8_t buf[OPTION_DRIVER_SPI_ARDUINO_OUTPUT_ONLY_BUF_SIZE];
            memcpy( buf, srcData, numBytes );
            m_spiDevice.transfer( buf, numBytes );
        }
        return true;
    }

    return false;
}
