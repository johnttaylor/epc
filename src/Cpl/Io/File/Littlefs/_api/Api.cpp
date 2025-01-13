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

#include "Cpl/Io/File/Api.h"
#include "Cpl/Io/File/Output.h"
#include "DirList_.h"
#include "Cpl/Io/File/Littlefs/Private_.h"
#include <new>

///
using namespace Cpl::Io::File;

static int getStats( const char* fileEntryName, struct lfs_info& info )
{
    auto lfs = Cpl::Io::File::Littlefs::Api::getInstance( fileEntryName );
    if ( lfs == nullptr )
    {
        return LFS_ERR_NOENT;
    }
    return lfs_stat( lfs, fileEntryName, &info );
}


/////////////////////////////////////////////////////
bool Api::getInfo( const char* fsEntryName, Info& infoOut )
{
    struct lfs_info info;
    if ( getStats( fsEntryName, info ) == LFS_ERR_OK )
    {
        infoOut.m_isDir  = info.type == LFS_TYPE_DIR;
        infoOut.m_isFile = info.type == LFS_TYPE_REG;
        infoOut.m_size   = info.size;

        // Unsupported attributes
        infoOut.m_readable  = true;
        infoOut.m_writeable = true;
        infoOut.m_mtime     = 0;
        return true;
    }

    return false;
}

/////////////////////////////////////////////////////
bool Api::canonicalPath( const char* relPath, Cpl::Text::String& absPath )
{
    // NOT SUPPORTED!
    return false;
}

bool Api::getCwd( Cpl::Text::String& cwd )
{
    // NOT SUPPORTED!
    return false;
}

/////////////////////////////////////////////////////
bool Api::exists( const char* fsEntryName )
{
    struct lfs_info info;
    return getStats( fsEntryName, info ) == LFS_ERR_OK;
}

bool Api::createFile( const char* fileName )
{
    Output fd( fileName, true, true );
    bool   result = fd.isOpened();
    fd.close();

    return result;
}

bool Api::createDirectory( const char* dirName )
{
    auto lfs = Cpl::Io::File::Littlefs::Api::getInstance( dirName );
    if ( lfs == nullptr )
    {
        return false;
    }

    int err = lfs_mkdir( lfs, dirName );
    return err == LFS_ERR_OK;
}


bool Api::renameInPlace( const char* oldName, const char* newName )
{
    auto lfsSrc = Cpl::Io::File::Littlefs::Api::getInstance( oldName );
    auto lfsDst = Cpl::Io::File::Littlefs::Api::getInstance( newName );
    if ( lfsSrc != nullptr && lfsSrc == lfsDst )
    {
        int err = lfs_rename( lfsSrc, oldName, newName );
        return err == LFS_ERR_OK;
    }
    return false;
}


bool Api::moveFile( const char* oldFileName, const char* newFileName )
{
    if ( copyFile( oldFileName, newFileName ) )
    {
        return remove( oldFileName );
    }
    return false;
}


bool Api::remove( const char* fsEntryName )
{
    auto* lfs = Cpl::Io::File::Littlefs::Api::getInstance( fsEntryName );
    if ( lfs != nullptr )
    {
        return lfs_remove( lfs, fsEntryName ) == LFS_ERR_OK;
    }
    return false;
}


/////////////////////////////////////////////////////
#ifdef LFS_THREADSAFE
#define CRITICAL_SECTION()  Cpl::System::Mutex::ScopeBlock guard( Cpl::Io::File::Littlefs::g_fsmutex )
#else
#define CRITICAL_SECTION()
#endif

// The DirList_ object is TOO LARGE to put on the stack
static uint8_t memDirList_[sizeof( DirList_ )];


bool Api::walkDirectory( const char*      dirToList,
                         DirectoryWalker& callback,
                         int              depth,
                         bool             filesOnly,
                         bool             dirsOnly )
{
    // Fail if the depth is deeper than the DirList_ object can handle
    if ( depth > OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH )
    {
        return false;
    }   

    // Get the Littlefs instance for the directory
    auto lfs = Cpl::Io::File::Littlefs::Api::getInstance( dirToList );
    if ( lfs != nullptr )
    {
        CRITICAL_SECTION(); // Need a critical (when LFS_THREADSAFE is defined) because I ONLY have memory for ONE DirList_ instance
        
        // Create the DirList_ object from static memory since it is too large to put on the stack
        DirList_* lister = new(memDirList_) DirList_( lfs, dirToList, depth, filesOnly, dirsOnly );
        bool result = lister->traverse( callback );
        lister->~DirList_();
        return result;
    }

    return false;
}
