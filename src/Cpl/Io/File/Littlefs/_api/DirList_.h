#ifndef Cpl_Io_File_DirList_x_h_
#define Cpl_Io_File_DirList_x_h_
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
#include "Cpl/Io/File/Api.h"
#include "Cpl/Io/File/Littlefs/Api.h"
#include "Cpl/Container/Stack.h"

///
namespace Cpl {
///
namespace Io {
///
namespace File {


/** This private concrete does the work for Api::walkDirectory() method
 */
class DirList_
{
protected:
    /// Structure to hold the directory name and depth
    struct DirEntry_T
    {
        /// The directory name
        NameString m_name;

        /// The depth of the directory
        unsigned m_depth;

        /// Constructor
        DirEntry_T()
            : m_name( ""  )
            , m_depth( 0 )
        {
        }
        
        /// Constructor
        DirEntry_T( const char* name, unsigned depth=1 )
            : m_name( name  )
            , m_depth( depth )
        {
        }
    };

protected:
    /// Maximum depth of directory traversal
    unsigned m_depth;

    /// Filter flag for files only
    bool m_filesOnly;

    /// Filter flag for directories only
    bool m_dirsOnly;

    /// The current directory 
    DirEntry_T m_curDir;

    /// Pointer to the lfs instance
    lfs* m_lfs;

    /// Stack of directory names (so we don't use recursion)
    Cpl::Container::Stack<DirEntry_T> m_stack;

    /// Memory for the stack
    DirEntry_T m_stackMemory[OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH];

public:
    /// Constructor
    DirList_( lfs* lfs, const char* rootDir, unsigned depth = 1, bool filesOnly = false, bool dirsOnly = false );


    /// Destructor
    ~DirList_();


public:
    /// Traverses a directory
    bool traverse( Api::DirectoryWalker& callback );
};


};  // end namespaces
};
};
#endif  // end header latch
