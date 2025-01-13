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

#include "BdSpi.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Api.h"
#include <algorithm>

using namespace Cpl::Io::File::Littlefs;

#define SECT_            "Cpl::Io::File::Littlefs::BdSpi"

#define WAIT_ON_RESET_MS 2

// FLASH Commands
enum
{
    SFLASH_CMD_READ          = 0x03,
    SFLASH_CMD_READ_JEDEC_ID = 0x9f,
    SFLASH_CMD_PAGE_PROGRAM  = 0x02,
    SFLASH_CMD_READ_STATUS1  = 0x05,
    SFLASH_CMD_READ_STATUS2  = 0x35,
    SFLASH_CMD_ENABLE_RESET  = 0x66,
    SFLASH_CMD_RESET         = 0x99,
    SFLASH_CMD_WRITE_ENABLE  = 0x06,
    SFLASH_CMD_WRITE_DISABLE = 0x04,
    SFLASH_CMD_ERASE_SECTOR  = 0x20,
};


/////////////////////////////////////
BdSpi::BdSpi( Driver::SPI::MasterHalfDuplex& spi,
              Driver::DIO::Out&              cs,
              lfs_size_t                     flashPageSize ) noexcept
    : m_spi( spi )
    , m_cs( cs )
    , m_flashPageSize( flashPageSize )
    , m_started( false )
{
}

BdSpi::~BdSpi() noexcept
{
}

////////////////////////////////////////
bool BdSpi::start() noexcept
{
    if ( !m_started )
    {
        // Start my supporting drivers
        if ( !m_spi.start() )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to start the SPI driver" ) );
            return false;
        }
        if ( !m_cs.start( false ) )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to start the CS driver" ) );
            return false;
        }

        // Wait till the chip is in a 'ready' state
        if ( !waitUntilReady() )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Timed out while waiting for the chip to be in the ready state (before soft reset)." ) );
            return false;
        }
        if ( !waitUntilSUS() )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Timed out while waiting for the chip to NOT be in the suspended state." ) );
            return false;
        }

        // uint8_t statusReg2 = readStatus();
        // CPL_SYSTEM_TRACE_MSG( SECT_, ( "Status Register 2: %02X", statusReg2 ) );

        // Read the JEDEC ID to verify the device is present
        JedecID_T jedecID;
        if ( readJedecID( jedecID ) && jedecID.manufacturerID != 0 && jedecID.memoryType != 0 && jedecID.capacity != 0 )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "JEDEC ID: %02X %02X %02X", jedecID.manufacturerID, jedecID.memoryType, jedecID.capacity ) );
        }
        else
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to read (OR bad values) the JEDEC ID [%02X %02X %02X]", jedecID.manufacturerID, jedecID.memoryType, jedecID.capacity ) );
            return false;
        }


        // Reset the device
        if ( !sendCommand( SFLASH_CMD_ENABLE_RESET ) )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to enable the reset" ) );
            return false;
        }
        if ( !sendCommand( SFLASH_CMD_RESET ) )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to reset the device" ) );
            return false;
        }

        // Wait at least 30us for the reset
        Cpl::System::Api::sleep( WAIT_ON_RESET_MS );

        // Wait for the device to be ready
        if ( !waitUntilReady() )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Timed out while waiting for the chip to be in the ready state (AFTER soft reset)." ) );
            return false;
        }

        m_started = true;
    }

    return true;
}

void BdSpi::stop() noexcept
{
    if ( m_started )
    {
        m_started = false;
        m_spi.stop();
        m_cs.stop();
    }
}

///////////////////////////////////////////////
bool BdSpi::readfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size ) noexcept
{
    // validate that the read is valid
    if ( block >= c->block_count ||
         ( off % c->read_size ) != 0 ||
         ( size % c->read_size ) != 0 ||
         ( off + size ) > c->block_size )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Invalid read request: block=%d, off=%d, size=%d", block, off, size ) );
        return false;
    }


    // Starting address/offset
    lfs_off_t address = block * c->block_size + off;

    // Before we erase the sector we need to wait for any writes to finish
    waitUntilReady();

    // Read command (24-bit address MSB first) PLUS rx clock cycles
    uint8_t cmdAndAddr[4] = { SFLASH_CMD_READ,
                              (uint8_t)( ( address & 0xFF0000 ) >> 16 ),
                              (uint8_t)( ( address & 0x00FF00 ) >> 8 ),
                              (uint8_t)( ( address & 0x0000FF ) ) };
    m_cs.assertOutput();
    m_spi.transmit( 4, cmdAndAddr );
    bool status = m_spi.receive( size, buffer );
    m_cs.deassertOutput();

    if ( !status )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to read from the device (block=%d, off=%d, size=%d)", block, off, size ) );
    }
    else
    {
        CPL_SYSTEM_TRACE_MSG( SECT_,
                              ( "Read: block=%d, off=%d, size=%d, addr=%d: %02X %02X %02X %02X %02X %02X %02X %02X",
                                block,
                                off,
                                size,
                                address,
                                ( (uint8_t*)buffer )[0],
                                ( (uint8_t*)buffer )[1],
                                ( (uint8_t*)buffer )[2],
                                ( (uint8_t*)buffer )[3],
                                ( (uint8_t*)buffer )[4],
                                ( (uint8_t*)buffer )[5],
                                ( (uint8_t*)buffer )[6],
                                ( (uint8_t*)buffer )[7] ) );
    }
    return status;
}

bool BdSpi::progfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size ) noexcept
{
    // check if the write is valid
    if ( block >= c->block_count ||
         ( off % c->prog_size ) != 0 ||
         ( size % c->prog_size ) != 0 ||
         ( off + size ) > c->block_size )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Invalid write request: block=%d, off=%d, size=%d", block, off, size ) );
        return false;
    }


    // Starting address/offset
    lfs_off_t address = block * c->block_size + off;
    auto      srcPtr  = (const uint8_t*)buffer;

    CPL_SYSTEM_TRACE_MSG( SECT_, ( "Write: block=%d, off=%d, size=%d, addr=%d", block, off, size, address ) );

    // write one page at a time
    lfs_size_t remaining = size;
    while ( remaining )
    {
        lfs_size_t const leftOnPage = m_flashPageSize - ( address & ( m_flashPageSize - 1 ) );
        lfs_size_t const toWrite    = std::min( remaining, leftOnPage );
        if ( !writeToPage( address, srcPtr, toWrite ) )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to write to page" ) );
            return false;
        }

        remaining -= toWrite;
        srcPtr    += toWrite;
        address   += toWrite;
    }
    return true;
}

bool BdSpi::erasefn( const struct lfs_config* c, lfs_block_t block ) noexcept
{
    // Validate the block number
    if ( block >= c->block_count )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Invalid erase request: block=%d", block ) );
        return false;
    }
    uint32_t sectorAddress = block * c->block_size;

    CPL_SYSTEM_TRACE_MSG( SECT_, ( "Erase: block=%d (address=%lu)", block, (unsigned long)sectorAddress ) );

    // Before we erase the sector we need to wait for any writes to finish
    waitUntilReady();

    // Enable writing
    sendCommand( SFLASH_CMD_WRITE_ENABLE );

    // Erase command (24-bit address MSB first)
    uint8_t cmdAndAddr[4] = { SFLASH_CMD_ERASE_SECTOR,
                              (uint8_t)( ( sectorAddress & 0xFF0000 ) >> 16 ),
                              (uint8_t)( ( sectorAddress & 0x00FF00 ) >> 8 ),
                              (uint8_t)( ( sectorAddress & 0x0000FF ) ) };
    m_cs.assertOutput();
    bool status = m_spi.transmit( 4, cmdAndAddr );
    m_cs.deassertOutput();

    if ( !status )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to erase sector" ) );
    }
    return status;
}

bool BdSpi::syncfn( const struct lfs_config* c ) noexcept
{
    // Ensure all writes are complete
    return waitUntilReady();
}

///////////////////////////////////////////////
bool BdSpi::waitUntilReady() noexcept
{
    unsigned retryCount = OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_MAX_RETRIES;
    while ( retryCount-- && ( readStatus() & 0x01 ) )
    {
        Cpl::System::Api::sleep( OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_RETRY_DELAY_MS );
    }

    return retryCount != 0;
}

bool BdSpi::waitUntilSUS() noexcept
{
    unsigned retryCount = OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_MAX_RETRIES;
    while ( retryCount-- && ( readStatus2() & 0x80 ) )
    {
        Cpl::System::Api::sleep( OPTION_CPL_IO_FILE_LITTLEFS_BDSPI_READ_RETRY_DELAY_MS );
    }

    return retryCount != 0;
}

uint8_t BdSpi::readStatus() noexcept
{
    uint8_t command = SFLASH_CMD_READ_STATUS1;
    m_cs.assertOutput();
    m_spi.transmit( 1, &command );
    uint8_t status;
    m_spi.receive( 1, &status );
    m_cs.deassertOutput();
    //CPL_SYSTEM_TRACE_MSG( SECT_, ( "Status Register 1: %02X", status ) );
    return status;
}

uint8_t BdSpi::readStatus2() noexcept
{
    uint8_t command = SFLASH_CMD_READ_STATUS2;
    m_cs.assertOutput();
    m_spi.transmit( 1, &command );
    uint8_t status;
    m_spi.receive( 1, &status );
    m_cs.deassertOutput();

    return status;
}


bool BdSpi::sendCommand( uint8_t command ) noexcept
{
    m_cs.assertOutput();
    bool success = m_spi.transmit( 1, &command );
    m_cs.deassertOutput();

    return success;
}

bool BdSpi::readJedecID( JedecID_T& dstID ) noexcept
{
    uint8_t command = SFLASH_CMD_READ_JEDEC_ID;
    m_cs.assertOutput();
    m_spi.transmit( 1, &command );
    bool success = m_spi.receive( 3, &dstID );
    m_cs.deassertOutput();

    return success;
}

bool BdSpi::writeToPage( lfs_off_t offset, const void* buffer, lfs_size_t numBytes ) noexcept
{
    // Wait for any previous writes to complete
    waitUntilReady();

    // Enable writing
    sendCommand( SFLASH_CMD_WRITE_ENABLE );

    // Page write command (24-bit address MSB first) PLUS data
    uint8_t cmdAndAddr[4] = { SFLASH_CMD_PAGE_PROGRAM,
                              (uint8_t)( ( offset & 0xFF0000 ) >> 16 ),
                              (uint8_t)( ( offset & 0x00FF00 ) >> 8 ),
                              (uint8_t)( ( offset & 0x0000FF ) ) };
    m_cs.assertOutput();
    m_spi.transmit( 4, cmdAndAddr );
    bool status = m_spi.transmit( numBytes, buffer );
    m_cs.deassertOutput();
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "WriteToPage: %d - %02X %02X %02X %02X %02X %02X %02X %02X", offset, ( (uint8_t*)buffer )[0], ( (uint8_t*)buffer )[1], ( (uint8_t*)buffer )[2], ( (uint8_t*)buffer )[3], ( (uint8_t*)buffer )[4], ( (uint8_t*)buffer )[5], ( (uint8_t*)buffer )[6], ( (uint8_t*)buffer )[7] ) );
    return status;
}
