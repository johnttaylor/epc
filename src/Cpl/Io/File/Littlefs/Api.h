#ifndef Cpl_Io_File_Littlefs_Api_h_
#define Cpl_Io_File_Littlefs_Api_h_
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
#include "littlefs/lfs.h"
#include <string.h>

/** The maximum number of files that can be opened at one time */
#ifndef OPTION_CPL_IO_FILE_LITTLEFS_MAX_CONCURRENT_FILES
#define OPTION_CPL_IO_FILE_LITTLEFS_MAX_CONCURRENT_FILES 4
#endif

/** The size, in bytes, of the lfs 'cache buffers'.  This size is used
    for read, prog, lookahead, etc. buffers. The total amount of static allocated
    memory will be:
        (3 + OPTION_CPL_IO_FILE_LITTLEFS_MAX_CONCURRENT_FILES) * OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE

 */
#ifndef OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE
#define OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE 32
#endif

/** The maximum number of volumes that can be mounted */
#ifndef OPTION_CPL_IO_FILE_LITTLEFS_MAX_VOLUMES
#define OPTION_CPL_IO_FILE_LITTLEFS_MAX_VOLUMES 1
#endif

/** Maximum support depth for directory traversal. Each level adds CPL_IO_FILE_MAX_NAME
    bytes of memory to the size of the DirList_ class.

    Note: CPL_IO_FILE_MAX_NAME is the max PATH+FILENAME length, not the max file name length
 */
#ifndef OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH
#define OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH 4
#endif


///
namespace Cpl {
///
namespace Io {
///
namespace File {
///
namespace Littlefs {

/** This 'singleton' class is used to create and manage the littlefs file system.

    The littlefs file system defaults to a single-threaded model, i.e. NOT
    thread safe.  If the LFS_THREAD_SAFE macro is defined, then the file system
    will be thread safe.

    See the README.txt file for additional information and limitations of the
    littlefs file system.
 */
class Api
{
public:
    /// Typedef for littlefs block driver read function
    typedef int ( *readfn )( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size );

    /// Typedef for littlefs block driver prog function
    typedef int ( *progfn )( const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size );

    /// Typedef for littlefs block driver erase function
    typedef int ( *erasefn )( const struct lfs_config* c, lfs_block_t block );

    /// Typedef for littlefs block driver sync function
    typedef int ( *syncfn )( const struct lfs_config* c );

public:
    /** This structure defines the configuration and the underlying block driver
        used for each volume instance.  A volume maps one-to-one with physical
        storage media, e.g. a SPI Data flash, EEPROM IC, etc.

        Note: The filesystem supports 'mounting' multiple volumes.  See the
              README.txt file for additional information when using multiple
              volumes
     */
    struct Volume_T
    {
        lfs_t       fs;                                                       //!< The littlefs file system
        lfs_config  cfg;                                                      //!< The littlefs configuration
        int32_t     block_cycles;                                             //!< Number of erase cycles before littlefs evicts metadata logs and moves. Suggested values are in the range 100-1000
        uint8_t     readBuffer[OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE];       //!< Read buffer. MUST be 'cacheSize' in size
        uint8_t     progBuffer[OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE];       //!< Program buffer. MUST be 'cacheSize' in size
        uint8_t     lookaheadBuffer[OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE];  //!< Lookahead buffer. MUST be 'cacheSize' in size
        const char* volumeName;                                               //!< The name of the volume.  If multiple volumes are used, then each volume MUST have a unique name

        /** Constructor.  The argument list is the MINIMUM info required to create a volume
            @param blockDriver      Opaque context for the block driver. MUST be a unique instance each per volume
            @param read             Read a region in a block
            @param prog             Program a region in a block
            @param erase            Erase a block
            @param sync             Flush the block device (to its physical media)
            @param eraseSize        Size of an erasable block in bytes
            @param numEraseBlocks   Number of erasable blocks
            @param blockCycles      Number of erase cycles before littlefs evicts metadata logs and moves. Suggested range 100-1000
         */
        Volume_T( void*      blockDriver,
                  readfn     read,
                  progfn     prog,
                  erasefn    erase,
                  syncfn     sync,
                  lfs_size_t eraseSize,
                  lfs_size_t numEraseBlocks,
                  int32_t    blockCycles = 500 ) noexcept;

        /// Default constructor.  USE Wisely (see above constructor)
        Volume_T() noexcept { memset( this, 0, sizeof( *this ) ); }
    };

public:
    /** This method is used to initialize the file system.  The method returns
        zero if the file system is successfully initialized; else a littlefs
        error code is returned (see xsrc\littlefs\lfs.h).

        The method MUST be called before any CPL File IO operations are called

        Multiple volumes can be initialized, but each volume must have its own
        block driver instance.  When initializing multiple volumes, the 'volumeName'
        parameter is used (and MUST be non-null) to identify the volume.  See
        the README.txt file for additional information when using multiple
        volumes.
     */
    static int initVolume( Volume_T&   volumeToInit,
                           const char* volumeName           = nullptr,
                           bool        usingMultipleVolumes = false,
                           bool        forceReformat        = false ) noexcept;

    /** This method is used to shutdown the file system.  The method returns
        zero if the file system is successfully shutdown; else a littlefs
        error code is returned (see xsrc\littlefs\lfs.h).

        The method should be called during an orderly shutdown of the
        application.  The method should be called for each volume that was
        initialized.
     */
    static int shutdownVolume( Volume_T& volumeToShutdown ) noexcept;

public:
    /** This method returns the littlefs filesystem instance based on the
        file/dir path name.

        If the volume was not found, or the desired volume is not mounted, then
        the method will return nullptr.

        NOTE: If only one volume is used, then the method will always return the
        the littlefs instance for that volume - regardless of the 'fsEntryName'
    */
    static lfs_t* getInstance( const char* fsEntryName );
};

}  // end namespace
}
}
}
#endif  // end header latch
