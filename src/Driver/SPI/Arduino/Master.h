#ifndef Driver_SPI_STM32_Master_h_
#define Driver_SPI_STM32_Master_h_
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


#include "Driver/SPI/Master.h"
#include "Bsp/Api.h"    // Pull's in the (core) Arduino APIs
#include "SPI.h"
#include <stdint.h>


///
namespace Driver {
///
namespace SPI {
///
namespace Arduino {


/** This class implements the SPI interface using the Arduino Framework.

    Note: The Pin assignments for SCK, MOSI, MISO are set/determined by the
          Arduino framework when it creates the SPI, SPI1, SPI2, etc. instances.
 */
class Master : public Driver::SPI::Master
{
public:
    /** SPI Settings (note: needed because the SPISettings class does not 
        allow changing the baudrate once instantiated)

        The Arduino documentation does not explicitly define what the 
        different SPI_MODEn are (the table is my best guess)
            SPI_MODE0   - Clock Polarity=0, rising SCLK
            SPI_MODE1   - Clock Polarity=0, falling SCLK
            SPI_MODE2   - Clock Polarity=1, rising SCLK
            SPI_MODE3   - Clock Polarity=1, falling SCLK
     */
    struct SPIConfig_T
    {
        uint32_t  baudrate;   // Baudrate in HZ, e.g. 20MHz is 20000000
        BitOrder  bitOrder;   // Bit order: MSBFIRST, LSBFIRST
        uint8_t   dataMode;   // Data mode: SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3

        /// Constructor
        SPIConfig_T( uint32_t bRate, BitOrder bOrder, uint8_t dMode )
            : baudrate( bRate )
            , bitOrder( bOrder )
            , dataMode( dMode )
        {
        }
    };
      

public:
    /** Constructor.
     */
    Master( SPIClass&   spiInstance,
            SPIConfig_T spiConfig,
            uint32_t    timeoutMs = 50 );    // Default timeout is 50ms


public:
    /// See Driver::SPI::Master
    bool start( size_t newBaudRateHz = 0 ) noexcept;

    /// See Driver::SPI::Master
    void stop() noexcept;

    /// See Driver::SPI::Master
    bool  transfer( size_t      numBytes,
                    const void* srcData,
                    void*       dstData = nullptr ) noexcept;

protected:

    /// Handle the Arduino driver instance
    SPIClass&   m_spiDevice;

    /// Handle the SPI configuration
    SPIConfig_T m_spiConfig;

    /// Timeout period for a SPI transaction
    uintptr_t   m_timeout;

    /// Track my started state
    bool        m_started;
};




};      // end namespaces
};
};
#endif  // end header latch
