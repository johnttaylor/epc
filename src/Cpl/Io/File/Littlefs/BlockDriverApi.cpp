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

#include "BlockDriverApi.h"


using namespace Cpl::Io::File::Littlefs;


int Cpl::Io::File::Littlefs::BlockDriverApi::callback_readfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size ) noexcept
{
    auto bd = (BlockDriverApi*)c->context;
    if ( bd )
    {
        return bd->readfn( c, block, off, buffer, size )? LFS_ERR_OK : LFS_ERR_IO;
    }
    return LFS_ERR_IO;
}

int Cpl::Io::File::Littlefs::BlockDriverApi::callback_progfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size ) noexcept
{
    auto bd = (BlockDriverApi*)c->context;
    if ( bd )
    {
        return bd->progfn( c, block, off, buffer, size )? LFS_ERR_OK : LFS_ERR_IO;
    }
    return LFS_ERR_IO;
}

int Cpl::Io::File::Littlefs::BlockDriverApi::callback_erasefn( const struct lfs_config* c, lfs_block_t block ) noexcept
{
    auto bd = (BlockDriverApi*)c->context;
    if ( bd )
    {
        return bd->erasefn( c, block )? LFS_ERR_OK : LFS_ERR_IO;
    }
    return LFS_ERR_IO;
}

int Cpl::Io::File::Littlefs::BlockDriverApi::callback_syncfn( const struct lfs_config* c ) noexcept
{
    auto bd = (BlockDriverApi*)c->context;
    if ( bd )
    {
        return bd->syncfn( c )? LFS_ERR_OK : LFS_ERR_IO;
    }
    return LFS_ERR_IO;
}
