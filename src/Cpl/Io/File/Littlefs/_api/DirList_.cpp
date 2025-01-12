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

#include "DirList_.h"
#include "Cpl/System/Assert.h"

//
using namespace Cpl::Io::File;


///////////////
DirList_::DirList_( lfs* lfs, const char* rootDir, unsigned depth, bool filesOnly, bool dirsOnly )
    : m_depth( depth )
    , m_filesOnly( filesOnly )
    , m_dirsOnly( dirsOnly )
    , m_curDir( rootDir )
    , m_lfs( lfs )
    , m_stack( OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH, m_stackMemory )
{
    CPL_SYSTEM_ASSERT( lfs );
}

DirList_::~DirList_()
{
}


///////////////
bool DirList_::traverse( Api::DirectoryWalker& callback )
{
    // NOTE: Purposely not using recursion to avoid stack overflow
    m_stack.push( m_curDir );
    while ( !m_stack.isEmpty() )
    {
        bool newDirLevel = false;
        m_stack.pop( m_curDir );
        unsigned curDepth = m_curDir.m_depth;
        //printf( "\nPOP curDir=%s, curDepth=%d (%d)", m_curDir.m_name.getString(), curDepth, m_curDir.m_depth );

        // Open the current directory
        lfs_dir_t dir;
        if ( lfs_dir_open( m_lfs, &dir, m_curDir.m_name() ) != LFS_ERR_OK )
        {
            return false;
        }

        // Read the content's of the directory
        struct lfs_info info;
        while ( lfs_dir_read( m_lfs, &dir, &info ) > 0 )
        {
            Api::Info cbInfo;
            cbInfo.m_isDir     = info.type == LFS_TYPE_DIR;
            cbInfo.m_isFile    = info.type == LFS_TYPE_REG;
            cbInfo.m_size      = info.size;
            cbInfo.m_readable  = true;
            cbInfo.m_writeable = true;
            cbInfo.m_mtime     = 0;

            // The current entry is a directory
            if ( info.type == LFS_TYPE_DIR )
            {
                // Filter out the "." and ".." directories
                if ( strcmp( info.name, "." ) != 0 && strcmp( info.name, ".." ) != 0 )
                {
                    // Callback the client
                    if ( ( !m_filesOnly && !m_dirsOnly ) || m_dirsOnly )
                    {
                        if ( callback.item( m_curDir.m_name, info.name, cbInfo ) == Cpl::Type::Traverser::eABORT )
                        {
                            lfs_dir_close( m_lfs, &dir );
                            return false;
                        }
                    }

                    // Track my depth
                    if ( !newDirLevel && m_curDir.m_name != "/" && m_curDir.m_name != "." )
                    {
                        curDepth++;
                    }

                    // Limit the depth of the traversal
                    if ( curDepth < m_depth )
                    {
                        newDirLevel = true;

                        // Push the found directory name onto the stack
                        m_curDir.m_depth = curDepth;
                        m_stack.push( m_curDir );
                        DirEntry_T& pushedItem = *( m_stack.peekTop() );  // Cheat here and use the memory on the stack instead of allocating a local variable
                        if ( pushedItem.m_name != "/" )
                        {
                            pushedItem.m_name += "/";
                        }
                        pushedItem.m_name += info.name;
                        //printf( "\nPUSHED dir=%s (parent=%s), curDepth=%d (%d)", pushedItem.m_name.getString(), m_curDir.m_name.getString(), curDepth, m_curDir.m_depth );
                    }
                }
            }

            // The current entry is a file
            else if ( info.type == LFS_TYPE_REG )
            {
                if ( ( !m_filesOnly && !m_dirsOnly ) || m_filesOnly )
                {
                    if ( callback.item( m_curDir.m_name, info.name, cbInfo ) == Cpl::Type::Traverser::eABORT )
                    {
                        lfs_dir_close( m_lfs, &dir );
                        return false;
                    }
                }
            }
        }

        // Housekeeping
        if ( !newDirLevel )
        {
            curDepth--;
        }
        if ( lfs_dir_close( m_lfs, &dir ) != LFS_ERR_OK )
        {
            return false;
        }
    }

    return true;
}