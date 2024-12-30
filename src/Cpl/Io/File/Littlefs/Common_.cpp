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

#include "Cpl/Io/File/Common_.h"
#include "Private_.h"
#include <new>

///
using namespace Cpl::Io::File;
using namespace Cpl::Io::File::Littlefs;

/////////////////////////////////////////////////////
Cpl::Io::Descriptor Common_::open( const char* fileEntryName, bool readOnly, bool forceCreate, bool forceEmptyFile )
{
    // Set open flags as requested
    int flags = readOnly ? LFS_O_RDONLY : LFS_O_RDWR;
    if ( forceCreate )
    {
        flags |= LFS_O_CREAT;
    }
    if ( forceEmptyFile )
    {
        flags |= LFS_O_TRUNC;
    }

    // Allocate memory for a file descriptor instance
    lfs_t*      lfs   = Api::getInstance( fileEntryName );
    FileDesc_T* cplFd = nullptr;
    if ( lfs )
    {
        void* memCplFd = g_fileMemoryPool.allocate();
        if ( memCplFd != nullptr )
        {
            // Attempt to Open the file
            cplFd = new( memCplFd ) FileDesc_T();
            cplFd->lfs = lfs;  // Save the file system pointer
            int err    = lfs_file_opencfg( lfs, &cplFd->fileHdl, fileEntryName, flags, &cplFd->fileCacheAndAttrs );
            if ( err )
            {
                cplFd = nullptr;
                g_fileMemoryPool.free( *cplFd );
            }
        }
    }
    // Return the file handle
    Cpl::Io::Descriptor fd( (void*)cplFd );
    return fd;
}

/////////////////////////////////////////////////////
bool Common_::currentPos( Cpl::Io::Descriptor fd, unsigned long& curpos )
{
    if ( fd.m_handlePtr == nullptr )
    {
        return 0;
    }

    auto       lfsFd = (FileDesc_T*)fd.m_handlePtr;
    lfs_soff_t pos   = lfs_file_tell( lfsFd->lfs, &lfsFd->fileHdl );
    if ( pos < 0 )
    {
        return false;
    }
    curpos = (unsigned long)pos;
    return true;
}


bool Common_::setRelativePos( Cpl::Io::Descriptor fd, long deltaOffset )
{
    if ( fd.m_handlePtr == 0 )
    {
        return false;
    }

    auto       lfsFd = (FileDesc_T*)fd.m_handlePtr;
    lfs_soff_t pos   = lfs_file_seek( lfsFd->lfs, &lfsFd->fileHdl, deltaOffset, LFS_SEEK_CUR );
    if ( pos < 0 )
    {
        return false;
    }
    return true;
}


bool Common_::setAbsolutePos( Cpl::Io::Descriptor fd, unsigned long newoffset )
{
    if ( fd.m_handlePtr == 0 )
    {
        return false;
    }

    auto       lfsFd = (FileDesc_T*)fd.m_handlePtr;
    lfs_soff_t pos   = lfs_file_seek( lfsFd->lfs, &lfsFd->fileHdl, newoffset, LFS_SEEK_SET );
    if ( pos < 0 )
    {
        return false;
    }
    return true;
}


bool Common_::setToEof( Cpl::Io::Descriptor fd )
{
    if ( fd.m_handlePtr == 0 )
    {
        return false;
    }

    auto       lfsFd = (FileDesc_T*)fd.m_handlePtr;
    lfs_soff_t pos   = lfs_file_seek( lfsFd->lfs, &lfsFd->fileHdl, 0, LFS_SEEK_END );
    if ( pos < 0 )
    {
        return false;
    }
    return true;
}


bool Common_::length( Cpl::Io::Descriptor fd, unsigned long& length )
{
    if ( fd.m_handlePtr == 0 )
    {
        return 0;  // Error -->return zero
    }

    auto       lfsFd = (FileDesc_T*)fd.m_handlePtr;
    lfs_soff_t len   = lfs_file_size( lfsFd->lfs, &lfsFd->fileHdl );
    if ( len < 0 )
    {
        return false;
    }
    
    length = (unsigned long)len;
    return true;
}

