#ifndef Cpl_Io_File_Littlefs_BlockDriverApi_h_
#define Cpl_Io_File_Littlefs_BlockDriverApi_h_
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

#include "littlefs/lfs.h"


///
namespace Cpl {
///
namespace Io {
///
namespace File {
///
namespace Littlefs {


/** This partial abstract class defines the interface for block drivers. The 
    class is also responsible for the actual littlefs callbacks and redirecting
    the callbacks to a concrete instance of this class.  

    The class - and child classes - are only thread safe when LFS_THREADSAFE
    is defined.
 */
class BlockDriverApi
{
public:
    /** This method is used start/initialize the block driver at run time.  The
        method MUST be called before it used by littlefs (i.e. BEFORE
        Cpl::Io::File::Littlefs::initVolume() is called).

        The method returns true if successful; else false is returned when an
        error occurred.  If false is returned, future  read/prog/erse/sync calls
        will always return a failure.
     */
    virtual bool start() noexcept = 0;

    /** This method is used to stop/shutdown the block driver.  The method MUST be
        called when the block driver is no longer needed (i.e. AFTER
        Cpl::Io::File::Littlefs::shutdownVolume() is called).
     */
    virtual void stop() noexcept = 0;


public:
    /// littlefs block driver read function (instance based)
    virtual bool readfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size ) noexcept = 0;

    /// littlefs block driver prog function (instance based)
    virtual bool progfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size ) noexcept = 0;

    /// littlefs block driver erase function (instance based)
    virtual bool erasefn( const struct lfs_config* c, lfs_block_t block ) noexcept = 0;

    /// littlefs block driver sync function (instance based)
    virtual bool syncfn( const struct lfs_config* c ) noexcept = 0;


public:
    /// Use this function when "registering" the block driver's read function
    static int callback_readfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size ) noexcept;

    /// Use this function when "registering" the block driver's prog function
    static int callback_progfn( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size ) noexcept;

    /// Use this function when "registering" the block driver's erase function
    static int callback_erasefn( const struct lfs_config* c, lfs_block_t block ) noexcept;

    /// Use this function when "registering" the block driver's sync function
    static int callback_syncfn( const struct lfs_config* c ) noexcept;
};

}  // end namespace
}
}
}
#endif  // end header latch
