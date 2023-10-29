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
#include <stdint.h>

using namespace Driver::I2C::Arduino;

// Driver has conditional functionality
#ifdef WIRE_HAS_TIMEOUT
#define SET_TIMEOUT(d_ms)      m_i2cDevice.setWireTimeout( d_ms * 1000, true )
#else
#define SET_TIMEOUT(d)
#endif

// Driver has conditional functionality
#ifdef WIRE_HAS_END
#define END()               m_i2cDevice.end()
#else
#define END()
#endif

//////////////////////////////////////////////////////////////////////////////
Master::Master( TwoWire& i2cInstance,
                uint32_t timeoutMs )
    : m_i2cDevice( i2cInstance )
    , m_timeout_ms( timeoutMs )
{
    SET_TIMEOUT( timeoutMs );
}

//////////////////////////////////////////////////////////////////////////////
bool Master::start() noexcept
{
    if ( !m_started )
    {
        m_started  = true;
        m_baudRate = OPTION_DRIVER_I2C_ARDUINO_DEFAULT_BAUD_RATE;
        m_i2cDevice.begin();
        SET_TIMEOUT( m_timeout_ms );
        return true;
    }

    return false;
}

void Master::stop() noexcept
{
    if ( m_started )
    {
        m_started = false;
        END();
    }
}

/////////////////////////////////////////////////////
Master::Result_T  Master::writeToDevice( uint8_t        device7BitAddress,
                                         size_t         numBytesToTransmit,
                                         const void*    srcData,
                                         bool           noStop ) noexcept
{
    if ( m_started )
    {
        m_i2cDevice.beginTransmission( device7BitAddress );
        m_i2cDevice.write( (const uint8_t*) srcData, numBytesToTransmit );
        uint8_t result = m_i2cDevice.endTransmission( !noStop );
        switch ( result )
        {
        case 0: 
            return Master::Result_T::eSUCCESS;
        case 2:
        case 3:
            return Master::Result_T::eNO_ACK;
        case 5:
            return Master::Result_T::eTIMEOUT;
        default:
            return Master::Result_T::eERROR;
        }
    }
    
    return Master::Result_T::eNOT_STARTED;
}

Master::Result_T Master::readFromDevice( uint8_t   device7BitAddress,
                                         size_t    numBytesToRead,
                                         void*     dstData,
                                         bool      noStop )
{
    if ( m_started )
    {
        uint8_t numRead = m_i2cDevice.requestFrom( device7BitAddress, numBytesToRead, !noStop );
        if ( numRead == 0 && numBytesToRead != 0 )
        {
            return Master::Result_T::eERROR;
        }

        uint8_t* dstPtr = (uint8_t*) dstData;
        while ( numRead )
        {
            *dstPtr = m_i2cDevice.read();
            dstPtr++;
            numRead--;
        }
        return Master::Result_T::eSUCCESS;
    }

    return Master::Result_T::eNOT_STARTED;
}

size_t Master::setBaudRate( size_t newBaudRateHz ) noexcept
{
    size_t oldBaudRate = m_baudRate;
    m_baudRate         = newBaudRateHz;
    m_i2cDevice.setClock( m_baudRate );
    return oldBaudRate;
}

size_t Master::setTransactionTimeout( size_t maxTimeMs ) noexcept
{
    size_t oldTimeout = m_timeout_ms;
    m_timeout_ms      = maxTimeMs;
    SET_TIMEOUT( m_timeout_ms );
    return oldTimeout;
}

