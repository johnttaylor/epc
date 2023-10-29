#ifndef Driver_I2C_Arduino_Master_h_
#define Driver_I2C_Arduino_Master_h_
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


#include "Driver/I2C/Master.h"
#include "Bsp/Api.h"    // Pull's in (the core) Arduino APIs
#include <Wire.h>


/// Default baud rate for the I2C Driver
#ifndef OPTION_DRIVER_I2C_ARDUINO_DEFAULT_BAUD_RATE
#define OPTION_DRIVER_I2C_ARDUINO_DEFAULT_BAUD_RATE     100000
#endif

///
namespace Driver {
///
namespace I2C {
///
namespace Arduino {


/** This class implements the I2C interface using the Arduino framework and/or
    APIs. 

    NOTE: Currently (8/2023) the 'noStop' semantics of the write/read methods
          is NOT supported.
 */
class Master : public Driver::I2C::Master
{
public:
    /** Constructor.
     */
    Master( TwoWire& i2cInstance = Wire,
            uint32_t timeoutMs   = 50 );    // Default timeout is 50ms


public:
    /// See Driver::I2C::Master
    bool start() noexcept;

    /// See Driver::I2C::Master
    void stop() noexcept;

    /// See Driver::I2C::Master
    Result_T  writeToDevice( uint8_t        device7BitAddress,
                             size_t         numBytesToTransmit,
                             const void*    srcData,
                             bool           noStop = false ) noexcept;

    /// See Driver::I2C::Master
    Result_T readFromDevice( uint8_t   device7BitAddress,
                             size_t    numBytesToRead,
                             void*     dstData,
                             bool      noStop = false );


    /// See Driver::I2C::Master
    size_t setBaudRate( size_t newBaudRateHz ) noexcept;
    
    /// See Driver::I2C::Master
    size_t setTransactionTimeout( size_t maxTimeMs ) noexcept;

protected:

    /// Handle an Ardunio driver instance
    TwoWire&  m_i2cDevice;

    /// Current Baud rate
    uint32_t  m_baudRate;

    /// Current timeout
    uint32_t  m_timeout_ms;

    /// Track my started state
    bool      m_started;
};




};      // end namespaces
};
};
#endif  // end header latch
