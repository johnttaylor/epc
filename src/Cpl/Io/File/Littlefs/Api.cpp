/*-----------------------------------------------------------------------------
 * This file is part of the Shift-Right Open Repository.  The Repository is an
 * open source project with a BSD type of licensing agreement.  See the license
 * agreement (license.txt) in the root directory or on the Internet at
 * http://www.shift-right.com/openrepo/license.htm
 *
 * Copyright (c) 2001-2020  John T. Taylor
 *
 * Redistributions of the source code must retain the above copyright notice.
 *----------------------------------------------------------------------------*/

#include "Api.h"
#include "Private_.h"
#include "Cpl/System/Assert.h"
#include "Cpl/System/Mutex.h"
#include "Cpl/System/FatalError.h"
#include "Cpl/Text/strip.h"
///
using namespace Cpl::Io::File::Littlefs;


//////////////////////////////////////
FileMemoryPool Cpl::Io::File::Littlefs::g_fileMemoryPool;

//////////////////////////////////////
#ifdef LFS_THREADSAFE
 Cpl::System::Mutex Cpl::Io::File::Littlefs::g_fsmutex;

//
static int lock( const struct lfs_config* c )
{
    g_fsmutex.lock();
    return 0;
}

//
static int unlock( const struct lfs_config* c )
{
    g_fsmutex.unlock();
    return 0;
}
#endif


//////////////////////////////////////
Cpl::Io::File::Littlefs::Api::Volume_T::Volume_T( void*      blockDriver,
                                                  readfn     read,
                                                  progfn     prog,
                                                  erasefn    erase,
                                                  syncfn     sync,
                                                  lfs_size_t eraseSize,
                                                  lfs_size_t numEraseBlocks,
                                                  int32_t    block_cycles ) noexcept
{
    memset( &cfg, 0, sizeof( cfg ) );
    cfg.context          = blockDriver;
    cfg.read             = read;
    cfg.prog             = prog;
    cfg.erase            = erase;
    cfg.sync             = sync;
    cfg.read_size        = OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE;
    cfg.prog_size        = OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE;
    cfg.block_size       = eraseSize;
    cfg.block_count      = numEraseBlocks;
    cfg.block_cycles     = block_cycles;
    cfg.cache_size       = OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE;
    cfg.lookahead_size   = OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE;
    cfg.read_buffer      = readBuffer;
    cfg.prog_buffer      = progBuffer;
    cfg.lookahead_buffer = lookaheadBuffer;

#ifdef LFS_THREADSAFE
    cfg.lock   = lock;
    cfg.unlock = unlock;
#endif
}

//////////////////////////////////////
static unsigned                                numVolumes_ = 0;
static Cpl::Io::File::Littlefs::Api::Volume_T* volumes_[OPTION_CPL_IO_FILE_LITTLEFS_MAX_VOLUMES];


int Cpl::Io::File::Littlefs::Api::initVolume( Volume_T&   volumeToInit,
                                              const char* volumeName,
                                              bool        usingMultipleVolumes,
                                              bool        forceReformat ) noexcept
{
    CPL_SYSTEM_ASSERT( numVolumes_ < OPTION_CPL_IO_FILE_LITTLEFS_MAX_VOLUMES );

    // Cache the volume info and valid arguments
    volumes_[numVolumes_]   = &volumeToInit;
    volumeToInit.volumeName = volumeName;
    if ( usingMultipleVolumes && volumeName == nullptr )
    {
        Cpl::System::FatalError::logf( "Attempted to initialize multiple volumes without a volume name" );
        return LFS_ERR_INVAL;
    }
    numVolumes_++;

    // Mount the filesystem
    int err = 1;  // Assume failure
    if ( !forceReformat )
    {
        err = lfs_mount( &volumeToInit.fs, &volumeToInit.cfg );
    }

    // Reformat if we can't mount the filesystem. This should only happen on the first boot
    if ( err )
    {
        lfs_format( &volumeToInit.fs, &volumeToInit.cfg );
        err = lfs_mount( &volumeToInit.fs, &volumeToInit.cfg );
        if ( err )
        {
            return err;
        }
    }

    // Make the top level volume directory (but ONLY when using multiple volumes
    if ( usingMultipleVolumes )
    {
        err = lfs_mkdir( &volumeToInit.fs, volumeName );
        if ( err == LFS_ERR_EXIST )
        {
            err = 0;
        }
    }
    return err;
}

int Cpl::Io::File::Littlefs::Api::shutdownVolume( Volume_T& volumeToInit ) noexcept
{
    return lfs_unmount( &volumeToInit.fs );
}

lfs_t* Cpl::Io::File::Littlefs::Api::getInstance( const char* fsEntryName )
{
    // Simple case of a single volume
    if ( numVolumes_ == 1 )
    {
        return &volumes_[0]->fs;
    }

    // Get the volume name as a token
    fsEntryName = Cpl::Text::stripSpace( fsEntryName );
    if ( fsEntryName == nullptr )
    {
        return nullptr;
    }
    fsEntryName++;  // Advance past the leading '/'
    const char* endPtr = Cpl::Text::stripNotChars( fsEntryName, "/" );
    int         len    = endPtr - fsEntryName;
    if ( len == 0 )
    {
        return nullptr;
    }

    // find the volume by name
    for ( unsigned idx = 0; idx < numVolumes_; idx++ )
    {
        if ( strncmp( volumes_[idx]->volumeName, fsEntryName, len ) == 0 )
        {
            return &volumes_[idx]->fs;
        }
    }
    return nullptr;
}
