/*
 * Block device emulated in a file
 *
 * Copyright (c) 2022, The littlefs authors.
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "driver.h"

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


int lfs_filebd_create( lfs_filebd_t* bdInstance, const char* path, const struct lfs_filebd_config* bdcfg )
{
    bdInstance->cfg = bdcfg;

    // open file
    bdInstance->fd = OPENFN( path, OPENFLAGS, 0666 );

    if ( bdInstance->fd < 0 )
    {
        int err = -errno;
        LFS_FILEBD_TRACE( "lfs_filebd_create -> %d", err );
        return err;
    }

    LFS_FILEBD_TRACE( "lfs_filebd_create -> %d", 0 );
    return 0;
}

int lfs_filebd_destroy( lfs_filebd_t* bdInstance )
{
    LFS_FILEBD_TRACE( "lfs_filebd_destroy(%p)", (void*)bdInstance );
    int           err = CLOSEFN( bdInstance->fd );
    if ( err < 0 )
    {
        err = -errno;
        LFS_FILEBD_TRACE( "lfs_filebd_destroy -> %d", err );
        return err;
    }
    LFS_FILEBD_TRACE( "lfs_filebd_destroy -> %d", 0 );
    return 0;
}

int lfs_filebd_read( const struct lfs_config* cfg, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size )
{
    LFS_FILEBD_TRACE(
        "lfs_filebd_read(%p, "
        "0x%" PRIx32 ", %" PRIu32 ", %p, %" PRIu32 ")",
        (void*)cfg,
        block,
        off,
        buffer,
        size );
    lfs_filebd_t* bd = cfg->context;

    // check if read is valid
    LFS_ASSERT( block < bd->cfg->erase_count );
    LFS_ASSERT( off % bd->cfg->read_size == 0 );
    LFS_ASSERT( size % bd->cfg->read_size == 0 );
    LFS_ASSERT( off + size <= bd->cfg->erase_size );

    // zero for reproducibility (in case file is truncated)
    memset( buffer, 0, size );

    // read
    off_t res1 = LSEEKFN( bd->fd,
                          (off_t)block * bd->cfg->erase_size + (off_t)off,
                          SEEK_SET );
    if ( res1 < 0 )
    {
        int err = -errno;
        LFS_FILEBD_TRACE( "lfs_filebd_read -> %d", err );
        return err;
    }

    ssize_t res2 = READFN( bd->fd, buffer, size );
    if ( res2 < 0 )
    {
        int err = -errno;
        LFS_FILEBD_TRACE( "lfs_filebd_read -> %d", err );
        return err;
    }

    LFS_FILEBD_TRACE( "lfs_filebd_read -> %d", 0 );
    return 0;
}

int lfs_filebd_prog( const struct lfs_config* cfg, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size )
{
    LFS_FILEBD_TRACE(
        "lfs_filebd_prog(%p, "
        "0x%" PRIx32 ", %" PRIu32 ", %p, %" PRIu32 ")",
        (void*)cfg,
        block,
        off,
        buffer,
        size );
    lfs_filebd_t* bd = cfg->context;

    // check if write is valid
    LFS_ASSERT( block < bd->cfg->erase_count );
    LFS_ASSERT( off % bd->cfg->prog_size == 0 );
    LFS_ASSERT( size % bd->cfg->prog_size == 0 );
    LFS_ASSERT( off + size <= bd->cfg->erase_size );

    // program data
    off_t res1 = LSEEKFN( bd->fd,
                          (off_t)block * bd->cfg->erase_size + (off_t)off,
                          SEEK_SET );
    if ( res1 < 0 )
    {
        int err = -errno;
        LFS_FILEBD_TRACE( "lfs_filebd_prog -> %d", err );
        return err;
    }

    ssize_t res2 = WRITEFN( bd->fd, buffer, size );
    if ( res2 < 0 )
    {
        int err = -errno;
        LFS_FILEBD_TRACE( "lfs_filebd_prog -> %d", err );
        return err;
    }

    LFS_FILEBD_TRACE( "lfs_filebd_prog -> %d", 0 );
    return 0;
}

int lfs_filebd_erase( const struct lfs_config* cfg, lfs_block_t block )
{
    LFS_FILEBD_TRACE( "lfs_filebd_erase(%p, 0x%" PRIx32 " (%" PRIu32 "))",
                      (void*)cfg,
                      block,
                      ( (lfs_filebd_t*)cfg->context )->cfg->erase_size );
    lfs_filebd_t* bd = cfg->context;

    // check if erase is valid
    LFS_ASSERT( block < bd->cfg->erase_count );

    // erase is a noop

    LFS_FILEBD_TRACE( "lfs_filebd_erase -> %d", 0 );
    return 0;
}

int lfs_filebd_sync( const struct lfs_config* cfg )
{
    LFS_FILEBD_TRACE( "lfs_filebd_sync(%p)", (void*)cfg );

    // file sync
    lfs_filebd_t* bd  = cfg->context;
    int           err = FLUSHFN( bd->fd );
    if ( err )
    {
        err = -errno;
        LFS_FILEBD_TRACE( "lfs_filebd_sync -> %d", 0 );
        return err;
    }

    LFS_FILEBD_TRACE( "lfs_filebd_sync -> %d", 0 );
    return 0;
}
