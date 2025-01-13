#ifndef Cpl_Io_File_Littlefs__bdspi_BdSpi_h_
#define Cpl_Io_File_Littlefs__bdspi_BdSpi_h_
/*-----------------------------------------------------------------------------
 * This file is part of the Colony.Core Project.  The Colony.Core Project is an
 * open source project with a BSD type of licensing agreement.  See the license
 * agreement (license.txt) in the top/ directory or on the Internet at
 * http://integerfox.com/colony.core/license.txt
 *
 * Copyright (c) 2014-2020  John T. Taylor
 *
 * Redistributions of the source code must retain the above copyright notice.
 *----------------------------------------------------------------------------*/
/** @file */

#include "colony_config.h"
#include "Cpl/Io/File/Littlefs/BlockDriverApi.h"
#include "Driver/SPI/MasterHalfDuplex.h"
#include "Driver/DIO/Out.h"


/** Number of times to retry a read operation when an error occurs

    Note: OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_MAX_RETRIES * OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_RETRY_DELAY_MS
          should be greater than the max block erase time of the flash chip
*/
#ifndef OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_MAX_RETRIES
#define OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_MAX_RETRIES 2100
#endif

/// Delay, in milliseconds, between read retries 
#ifndef OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_RETRY_DELAY_MS
#define OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_RETRY_DELAY_MS 1
#endif

/// Default Page size in bytes 
#ifndef OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_DEFAULT_FLASH_PAGE_SIZE
#define OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_DEFAULT_FLASH_PAGE_SIZE 256
#endif


///
namespace Cpl {
///
namespace Io {
///
namespace File {
///
namespace Littlefs {


/** Concrete Block Driver for a 'traditional' SPI based NOR flash.  The driver
    is intended to be generic driver with a 24bit address space.  That said,
    the driver was verified using a Winbond W25Q128JV SPI NOR flash device.
 */
class BdSpi : public BlockDriverApi
{
public:
    /// JEDEC ID
    struct JedecID_T
    {
        uint8_t manufacturerID;  //!< Manufacturer ID
        uint8_t memoryType;      //!< Memory Type
        uint8_t capacity;        //!< Capacity
    };

public:
    /** Constructor.  Note: The class ASSUMES that is responsible for starting
        and stopping the SPI and CS drivers.
     */
    BdSpi( Driver::SPI::MasterHalfDuplex& spi,
           Driver::DIO::Out&              cs,
           lfs_size_t                     flashPageSize = OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_DEFAULT_FLASH_PAGE_SIZE ) noexcept;

    /// Destructor
    ~BdSpi() noexcept;


public:
    /// See Cpl::Io::File::BlockDriverApi
    bool start() noexcept;

    /// See Cpl::Io::File::BlockDriverApi
    void stop() noexcept;


public:
    /// See Cpl::Io::Littlefs::BlockDriverApi
    bool readfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size ) noexcept;

    /// See Cpl::Io::Littlefs::BlockDriverApi
    bool progfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size ) noexcept;

    /// See Cpl::Io::Littlefs::BlockDriverApi
    bool erasefn( const struct lfs_config* c, lfs_block_t block ) noexcept;

    /// See Cpl::Io::Littlefs::BlockDriverApi
    bool syncfn( const struct lfs_config* c ) noexcept;

protected:
    /// Helper function
    uint8_t readStatus() noexcept;

    /// Helper function
    uint8_t readStatus2() noexcept;

    /// Helper function
    bool waitUntilReady() noexcept;

    /// Helper function
    bool waitUntilSUS() noexcept;

    /// Helper function
    bool sendCommand( uint8_t command ) noexcept;

    /// Helper function
    bool readJedecID( JedecID_T& dstID ) noexcept;

    /// Helper function (assume the requested write offset/size fits into a single page)
    bool writeToPage( lfs_off_t offset, const void* buffer, lfs_size_t numBytes ) noexcept;

protected:
    /// SPI driver
    Driver::SPI::MasterHalfDuplex& m_spi;

    /// Chip Select driver
    Driver::DIO::Out& m_cs;

    /// Page size
    lfs_size_t m_flashPageSize;

    /// Started state
    bool m_started;
};

}  // end namespace
}
}
}
#endif  // end header latch
