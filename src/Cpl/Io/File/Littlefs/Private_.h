#ifndef Cpl_Io_File_Littlefs_Private_h_
#define Cpl_Io_File_Littlefs_Private_h_
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
#include "Cpl/Memory/SPool.h"
#include "Cpl/System/Mutex.h"
#include "Cpl/Io/File/Littlefs/Api.h"


///
namespace Cpl {
///
namespace Io {
///
namespace File {
///
namespace Littlefs {

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/// This struct defines the memory need per opened file
struct FileDesc_T
{
    lfs_file_config fileCacheAndAttrs;  //!< The file buffer cache and file attributes
    lfs_file_t      fileHdl;            //!< The file handle
    lfs_t*          lfs;                //!< Pointer to the file system that the file descriptor was opened on/for
    uint8_t         memFileCacheBuffer[OPTION_CPL_IO_FILE_LITTLEFS_CACHE_SIZE];  //!< The file cache buffer

    /// Constructor
    FileDesc_T()
    {
        memset( this, 0, sizeof( *this ) );
        this->fileCacheAndAttrs.buffer = memFileCacheBuffer;
    }
};


// SINGLE-THREADED MEMORY POOL
#ifndef LFS_THREADSAFE

/** Memory pool for opened files.  The class is NOT thread safe
 */
class FileMemoryPool
{
public:
    /// Constructor
    FileMemoryPool(){};

public:
    /// Allocates memory for a file
    void* allocate() { return m_pool.allocate( sizeof( FileDesc_T )); }

    /// Frees memory for a file
    void free( FileDesc_T& p ) { m_pool.release( &p ); }

protected:
    /// Memory pool
    Cpl::Memory::SPool<FileDesc_T, OPTION_CPL_IO_FILE_LITTLEFS_MAX_CONCURRENT_FILES> m_pool;
};


// CONCURRENT MEMORY POOL
#else
/// Expose the underlying mutex
extern Cpl::System::Mutex g_fsmutex;

/** This class is the same as 'FileMemoryPool' except that is thread safe
 */
class FileMemoryPool
{
public:
    /// Constructor
    FileMemoryPool(){};

public:
    /// Allocates memory for a file
    void* allocate()
    {
        Cpl::System::Mutex::ScopeBlock lock( g_fsmutex );
        return m_pool.allocate( sizeof( FileDesc_T ) );
    }

    /// Frees memory for a file
    void free( FileDesc_T& p )
    {
        Cpl::System::Mutex::ScopeBlock lock( g_fsmutex );
        m_pool.release( &p );
    }

protected:
    /// Memory pool
    Cpl::Memory::SPool<FileDesc_T, OPTION_CPL_IO_FILE_LITTLEFS_MAX_CONCURRENT_FILES> m_pool;
};
#endif  // end !LFS_THREADSAFE

/*----------------------------------------------------------------------------*/
/// Expose the memory pool instance
extern FileMemoryPool g_fileMemoryPool;

}  // end namespace
}
}
}
#endif  // end header latch
