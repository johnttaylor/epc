#ifndef Driver_SPI_STM32_MasterHalfDuplex_h_
#define Driver_SPI_STM32_MasterHalfDuplex_h_
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


#include "Driver/SPI/MasterHalfDuplex.h"
#include "Bsp/Api.h"  // Pull's in the ST HAL APIs
#include <stdint.h>

///
namespace Driver {
///
namespace SPI {
///
namespace STM32 {


/** This class implements the SPI Half-duplex interface for the STM32 family of
    micro-controller using the ST's MX Cube/IDE to configure the SPI peripherals
    and IO pins
 */
class MasterHalfDuplex : public Driver::SPI::MasterHalfDuplex
{
public:
    /** Constructor.

        The 'spiInstance' MUST have already been initialize, i.e. the low
        level MX_SPIx_Init() from the ST HAL APIs has been called

        TODO: Add support for configuring the SPI interface WITHOUT using
              ST's MX tools.
     */
    MasterHalfDuplex( SPI_HandleTypeDef* spiInstance,
                      uint32_t           timeoutMs = 50 );  // Default timeout is 50ms


public:
    /// See Driver::SPI::MasterHalfDuplex
    bool start( size_t newBaudRateHz = 0 ) noexcept;

    /// See Driver::SPI::MasterHalfDuplex
    void stop() noexcept;

    /// See Driver::SPI::MasterHalfDuplex
    bool transmit( size_t      numBytes,
                   const void* srcData ) noexcept;

    /// See Driver::SPI::MasterHalfDuplex
    bool receive( size_t numBytes,
                  void*  dstData ) noexcept;

protected:
    /// Handle the low-level ST HAL driver instance
    SPI_HandleTypeDef* m_spiDevice;

    /// Timeout period for a SPI transaction
    uint32_t m_timeout;

    /// Track my started state
    bool m_started;
};


};  // end namespaces
};
};
#endif  // end header latch
