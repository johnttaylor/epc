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

#include "BdFile.h"
#include "Cpl/System/Trace.h"

using namespace Cpl::Io::File::Littlefs;

#define SECT_ "Cpl::Io::File::Littlefs::BdFile"


#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#define OPENFLAGS     ( O_RDWR | O_CREAT | O_BINARY )
#define OPENFN        _open
#define CLOSEFN       _close
#define WRITEFN       _write
#define READFN        _read
#define LSEEKFN       _lseek
#define FLUSHFN( fd ) FlushFileBuffers( (HANDLE)_get_osfhandle( fd ) ) ? 0 : -1
#define ssize_t       SSIZE_T

// Posix
#else
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define OPENFLAGS     ( O_RDWR | O_CREAT )
#define OPENFN        open
#define CLOSEFN       close
#define WRITEFN       write
#define READFN        read
#define LSEEKFN       lseek
#define FLUSHFN( fd ) fsync( fd )
#endif

/////////////////////////////////////
BdFile::BdFile( const char* fileName ) noexcept
    : m_fname( fileName )
    , m_fd( -1 )
    , m_started( false )
{
}

BdFile::~BdFile() noexcept
{
    stop();
}

////////////////////////////////////////
bool BdFile::start() noexcept
{
    if ( !m_started )
    {
        // open file
        m_fd = OPENFN( m_fname, OPENFLAGS, 0666 );
        if ( m_fd < 0 )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to open file: %s, err=", m_fname, errno ) );
            return false;
        }

        m_started = true;
    }

    return true;
}

void BdFile::stop() noexcept
{
    if ( m_started )
    {
        m_started = false;
        if ( CLOSEFN( m_fd ) < 0 )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Failed to close file: %s, err=", m_fname, errno ) );
        }
    }
}


///////////////////////////////////////////////
bool BdFile::readfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size ) noexcept
{
    // validate that the read is valid
    if ( block >= c->block_count ||
         ( off % c->read_size ) != 0 ||
         ( size % c->read_size ) != 0 ||
         ( off + size ) > c->block_size )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Read: Invalid request: block=%d, off=%d, size=%d", block, off, size ) );
        return false;
    }

    // zero for reproducibility (in case file is truncated)
    memset( buffer, 0, size );

    // Starting address/offset
    lfs_off_t address = block * c->block_size + off;

    // read
    if ( LSEEKFN( m_fd, address, SEEK_SET ) < 0 )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Read: Failed to seek to address: %ld (%d)", address, -errno ) );
        return false;
    }

    if ( READFN( m_fd, buffer, size ) < 0 )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Read: Failed to read from file: %d (len=%u)", -errno, size ) );
        return false;
    }

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
    return true;
}

bool BdFile::progfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size ) noexcept
{
    // check if the write is valid
    if ( block >= c->block_count ||
         ( off % c->prog_size ) != 0 ||
         ( size % c->prog_size ) != 0 ||
         ( off + size ) > c->block_size )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Write: invalid request: block=%d, off=%d, size=%d", block, off, size ) );
        return false;
    }


    // Starting address/offset
    lfs_off_t address = block * c->block_size + off;
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "Write: block=%d, off=%d, size=%d, addr=%d", block, off, size, address ) );

    // program data
    if ( LSEEKFN( m_fd, address, SEEK_SET ) < 0 )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Write: Failed to seek to address: %ld (%d)", address, -errno ) );
        return false;
    }
    if ( WRITEFN( m_fd, buffer, size ) < 0 )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Write: Failed to write to file: %d (len=%u)", -errno, size ) );
        return false;
    }

    CPL_SYSTEM_TRACE_MSG( SECT_,
                          ( "  data: %02X %02X %02X %02X %02X %02X %02X %02X",
                            ( (uint8_t*)buffer )[0],
                            ( (uint8_t*)buffer )[1],
                            ( (uint8_t*)buffer )[2],
                            ( (uint8_t*)buffer )[3],
                            ( (uint8_t*)buffer )[4],
                            ( (uint8_t*)buffer )[5],
                            ( (uint8_t*)buffer )[6],
                            ( (uint8_t*)buffer )[7] ) );
    return true;
}

bool BdFile::erasefn( const struct lfs_config* c, lfs_block_t block ) noexcept
{
    // Validate the block number
    if ( block >= c->block_count )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Erase: Invalid request: block=%d", block ) );
        return false;
    }
    uint32_t sectorAddress = block * c->block_size;

    CPL_SYSTEM_TRACE_MSG( SECT_, ( "Erase: block=%d (address=%lu)", block, (unsigned long)sectorAddress ) );

    // Move to start of the sector
    if ( LSEEKFN( m_fd, sectorAddress, SEEK_SET ) < 0 )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Erase: seek failed -> %d (seek=%lu)", -errno, sectorAddress ) );
        return false;
    }

    // Simulate flash and set all bytes to 0xff
    off_t sectorSize = c->block_size;
    while ( sectorSize )
    {
        uint8_t eraseValue = 0xff;
        ssize_t res2       = WRITEFN( m_fd, &eraseValue, 1 );
        if ( res2 < 0 )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "Erase: write failed -> %d (remain=%lu)", -errno, sectorSize ) );
            return false;
        }
        sectorSize--;
    }

    return true;
}

bool BdFile::syncfn( const struct lfs_config* c ) noexcept
{
    // file sync
    if ( FLUSHFN( m_fd ) )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Sync: Failed -> %d", -errno ));
        return false;
    }
    return true;
}

