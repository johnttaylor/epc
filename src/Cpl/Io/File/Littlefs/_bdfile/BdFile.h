#ifndef Cpl_Io_File_Littlefs__bdfile_BdFile_h_
#define Cpl_Io_File_Littlefs__bdfile_BdFile_h_
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



///
namespace Cpl {
///
namespace Io {
///
namespace File {
///
namespace Littlefs {

/** Concrete Block Driver that uses the Host's file system for the persistent
    storage media.  Linux/Posix and Windoze hosts are supported
 */
class BdFile : public BlockDriverApi
{
public:
    /** Constructor.  The 'fileName' is the name of the file that will be used
        as the block device.  The file will be created if it does not exist.
        If the file does exist, it will be opened for read/write access. The
        string must stay in scope for the life of the object.
     */
    BdFile( const char* fileName ) noexcept;

    /// Destructor
    ~BdFile() noexcept;


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

protected:
    /// SPI driver
    const char* m_fname;

    /// File descriptor
    int m_fd;
    
    /// Started state
    bool m_started;
};


}  // end namespace
}
}
}
#endif  // end header latch
