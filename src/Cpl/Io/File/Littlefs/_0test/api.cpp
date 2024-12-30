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

#include "colony_config.h"
#include "Cpl/Io/File/Api.h"
#include "Cpl/Io/File/Output.h"
#include "Cpl/Io/File/Input.h"
#include "Cpl/Text/FString.h"
#include "Cpl/System/Assert.h"
#include "Cpl/Io/File/Littlefs/Api.h"


#define SECT_ "_0test"

#define REQUIRE CPL_SYSTEM_ASSERT

///
using namespace Cpl::Io::File;


#define TESTINPUT_TXT_FILE_LENGTH   101
#define TESTINPUT_TEXT_HELLO_OFFEST 0x58

#define FILENAME "tinput.txt"

#define FILE_CONTENTS                                                     \
    "line 1\n"                                                            \
    "line 2\n"                                                            \
    "\n"                                                                  \
    "line 4 - extra stuff, really extra stuff, okay really extra stuff\n" \
    "line 5\n"                                                            \
    "AHello World."


////////////////////////////////////////////////////////////////////////////////
namespace {

class Walker : public Api::DirectoryWalker
{
public:
    int        m_depth;
    bool       m_files;
    bool       m_dirs;
    NameString m_workName;
    bool       m_contentCheck;

public:
    ///
    Walker( int depth, bool files = true, bool dirs = true )
        : m_depth( depth ), m_files( files ), m_dirs( dirs ), m_contentCheck( true )
    {
    }

public:
    Cpl::Type::Traverser::Status_T item( const char* currentParent, const char* fsEntryName, Api::Info& entryInfo )
    {
        // File check
        m_workName = fsEntryName;
        if ( entryInfo.m_isFile )
        {
            if ( !m_files )
            {
                m_contentCheck = false;
            }

            else if ( m_depth > 2 )
            {
                if ( m_workName != "d1.txt" && m_workName != "d2.txt" && m_workName != "d3.txt" )
                {
                    m_contentCheck = false;
                }
            }
            else if ( m_depth > 1 )
            {
                if ( m_workName != "d1.txt" && m_workName != "d2.txt" )
                {
                    m_contentCheck = false;
                }
            }
            else if ( m_workName != "d1.txt" )
            {
                m_contentCheck = false;
            }
        }

        // Dir check
        m_workName = fsEntryName;
        if ( entryInfo.m_isDir )
        {
            if ( !m_dirs )
            {
                m_contentCheck = false;
            }

            else if ( m_depth >= 2 )
            {
                if ( m_workName != "d2" && m_workName != "d3" && m_workName != "d22")
                {
                    m_contentCheck = false;
                }
            }
            else if ( m_depth >= 1 )
            {
                if ( m_workName != "d2" && m_workName != "d22" && m_workName != "d3"  ) 
                {
                    m_contentCheck = false;
                }
            }
            else
            {
                m_contentCheck = false;
            }
        }

        printf( "\n%s%s Parent=%s, item=%s.  (depth=%d, content=%d)", entryInfo.m_isFile ? "f" : "-", entryInfo.m_isDir ? "d" : "-", currentParent, fsEntryName, m_depth, m_contentCheck );
        return Cpl::Type::Traverser::eCONTINUE;
    }
};

};  // end anonymous namespace


////////////////////////////////////////////////////////////////////////////////
void run_api_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut )
{
    printf( "\n\n*** run_api_tests() ***\n" );

    NameString name;
    NameString name2;

    // Create input file
    Output fdout( FILENAME, true, true );
    REQUIRE( fdout.isOpened() );
    fdout.write( FILE_CONTENTS );
    fdout.close();


    ///
    printf( "\nCopy, Appended..." );
    REQUIRE( Api::exists( "copy.txt" ) == false );
    REQUIRE( Api::exists( "copy2.txt" ) == false );
    REQUIRE( Api::copyFile( FILENAME, "copy.txt" ) );
    REQUIRE( Api::isFile( "copy.txt" ) );
    REQUIRE( Api::size( "copy.txt" ) == TESTINPUT_TXT_FILE_LENGTH );
    REQUIRE( Api::appendFile( FILENAME, "copy2.txt" ) );
    REQUIRE( Api::size( "copy2.txt" ) == TESTINPUT_TXT_FILE_LENGTH );
    REQUIRE( Api::appendFile( FILENAME, "copy.txt" ) );
    REQUIRE( Api::size( "copy.txt" ) == 2 * TESTINPUT_TXT_FILE_LENGTH );
    REQUIRE( Api::remove( "copy.txt" ) );
    REQUIRE( Api::isFile( "copy.txt" ) == false );
    REQUIRE( Api::remove( "copy2.txt" ) );
    REQUIRE( Api::exists( "copy2.txt" ) == false );

    // NOTE: canonicalPath() and getCwd() methods NOT SUPPORTED!!!!!
    printf( "\nCanonicalPath & getCwd..." );
    REQUIRE( Api::canonicalPath( ".", name ) == false );
    REQUIRE( Api::getCwd( name2 ) == false );

    ///
    printf( "\nCreate, move, remove, etc. ..." );
    REQUIRE( Api::exists( "bob" ) == false );
    REQUIRE( Api::createDirectory( "bob" ) );
    REQUIRE( Api::exists( "bob" ) );
    REQUIRE( Api::isDirectory( "bob" ) );
    REQUIRE( Api::exists( "uncle.txt" ) == false );
    REQUIRE( Api::createFile( "bob/uncle.txt" ) );
    REQUIRE( Api::exists( "bob/uncle.txt" ) );
    REQUIRE( Api::isFile( "bob/uncle.txt" ) );

    REQUIRE( Api::exists( "charlie" ) == false );
    REQUIRE( Api::renameInPlace( "bob", "charlie" ) );
    REQUIRE( Api::isDirectory( "charlie" ) );
    REQUIRE( Api::isFile( "charlie/uncle.txt" ) );

    REQUIRE( Api::exists( "uncle.txt" ) == false );
    REQUIRE( Api::moveFile( "charlie/uncle.txt", "uncle.txt" ) );
    REQUIRE( Api::isFile( "uncle.txt" ) );
    REQUIRE( Api::renameInPlace( "uncle.txt", "your.txt" ) );
    REQUIRE( Api::isFile( "your.txt" ) );

    REQUIRE( Api::createFile( "charlie/test.txt" ) );
    REQUIRE( Api::exists( "charlie/test.txt" ) );
    REQUIRE( Api::remove( "charlie" ) == false );
    REQUIRE( Api::remove( "charlie/test.txt" ) );
    REQUIRE( Api::remove( "charlie" ) );
    REQUIRE( Api::exists( "charlie" ) == false );
    REQUIRE( Api::remove( "your.txt" ) );
    REQUIRE( Api::exists( "your.txt" ) == false );


    ///
    printf( "\nInfo..." );
    REQUIRE( Api::exists( FILENAME ) );
    REQUIRE( Api::exists( "nothere.txt" ) == false );
    REQUIRE( Api::isFile( FILENAME ) );
    REQUIRE( Api::isDirectory( FILENAME ) == false );
    REQUIRE( Api::isReadable( FILENAME ) );
    REQUIRE( Api::isReadable( "tinput.xxx" ) == false );
    REQUIRE( Api::isWriteable( FILENAME ) );
    REQUIRE( Api::isWriteable( "tinput.tdd" ) == false );
    REQUIRE( Api::size( FILENAME ) == TESTINPUT_TXT_FILE_LENGTH );

    ///
    printf( "\nWalk directories..." );
    Api::remove( "output1.txt" );       // Clean up any previous test
    Api::remove( "output2.txt" );
    Api::remove( "output3.txt" );
    Api::remove( "output4.txt" );
    Api::remove( "tinput.txt" );
    Api::remove( "d1/d2/d3/d4/d5/d5a.txt" );
    Api::remove( "d1/d2/d3/d4/d5" );
    Api::remove( "d1/d2/d3/d4/d4a.txt" );
    Api::remove( "d1/d2/d3/d4" );       
    Api::remove( "d1/d2/d3/d3.txt" );
    Api::remove( "d1/d2/d3/d3a.txt" );
    Api::remove( "d1/d2/d3" );
    Api::remove( "d1/d2/d2.txt" );
    Api::remove( "d1/d22" );
    Api::remove( "d1/d2" );
    Api::remove( "d1/d1.txt" );
    Api::remove( "d1" );

    REQUIRE( Api::createDirectory( "d1" ) );
    REQUIRE( Api::createFile( "d1/d1.txt" ) );
    REQUIRE( Api::createDirectory( "d1/d2" ) );
    REQUIRE( Api::createDirectory( "d1/d22" ) );
    REQUIRE( Api::createFile( "d1/d2/d2.txt" ) );
    REQUIRE( Api::createDirectory( "d1/d2/d3" ) );
    REQUIRE( Api::createFile( "d1/d2/d3/d3.txt" ) );

    {
        Walker iterator( 3 );
        printf( "\nWalk 'd1', %d", OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH );
        REQUIRE( Api::walkDirectory( "d1", iterator, OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH ) );
        REQUIRE( iterator.m_contentCheck );
    }
    {
        Walker iterator( 3, true, false );
        printf( "\nWalk 'd1', %d", OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH );
        REQUIRE( Api::walkDirectory( "d1", iterator, OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH, true, false ) );
        REQUIRE( iterator.m_contentCheck );
    }
    {
        Walker iterator( 3, false, true );
        printf( "\nWalk 'd1', %d", OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH );
        REQUIRE( Api::walkDirectory( "d1", iterator, OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH, false, true ) );
        REQUIRE( iterator.m_contentCheck );
    }

    {
        Walker iterator( 2 );
        printf( "\nWalk 'd1', 2" );
        REQUIRE( Api::walkDirectory( "d1", iterator, 2 ) );
        REQUIRE( iterator.m_contentCheck );
    }
    {
        Walker iterator( 2, true, false );
        printf( "\nWalk 'd1', 2" );
        REQUIRE( Api::walkDirectory( "d1", iterator, 2, true, false ) );
        REQUIRE( iterator.m_contentCheck );
    }
    {
        Walker iterator( 2, false, true );
        printf( "\nWalk 'd1', 2" );
        REQUIRE( Api::walkDirectory( "d1", iterator, 2, false, true ) );
        REQUIRE( iterator.m_contentCheck );
    }

    {
        Walker iterator( 1 );
        printf( "\nWalk 'd1', 1" );
        REQUIRE( Api::walkDirectory( "d1", iterator, 1 ) );
        REQUIRE( iterator.m_contentCheck );
    }
    {
        Walker iterator( 1, true, false );
        printf( "\nWalk 'd1', 1" );
        REQUIRE( Api::walkDirectory( "d1", iterator, 1, true, false ) );
        REQUIRE( iterator.m_contentCheck );
    }
    {
        Walker iterator( 1, false, true );
        printf( "\nWalk 'd1', 1" );
        REQUIRE( Api::walkDirectory( "d1", iterator, 1, false, true ) );
        REQUIRE( iterator.m_contentCheck );
    }

    REQUIRE( Api::createDirectory( "d1/d2/d3/d4" ) );
    REQUIRE( Api::createFile( "d1/d2/d3/d3a.txt" ) );
    {
        Walker iterator( 3, true, false );
        printf( "\nWalk 'd1', %d", OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH );
        REQUIRE( Api::walkDirectory( "d1", iterator, OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH, true, false ) );
        REQUIRE( iterator.m_contentCheck == false );
    }
    {
        Walker iterator( 3, false, true );
        printf( "\nWalk 'd1', %d", OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH );
        REQUIRE( Api::walkDirectory( "d1", iterator, OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH, false, true ) );
        REQUIRE( iterator.m_contentCheck == false );
    }
    REQUIRE( Api::createFile( "d1/d2/d3/d4/d4a.txt" ) );
    REQUIRE( Api::createDirectory( "d1/d2/d3/d4/d5" ) );
    REQUIRE( Api::createFile( "d1/d2/d3/d4/d5/d5a.txt" ) );
    {
        Walker iterator( 4 );
        printf( "\nWalk '/', %d", 4 );
        REQUIRE( Api::walkDirectory( "/", iterator, 4 ) );
        // REQUIRE( iterator.m_contentCheck ); // Don't check 'content' because the Walker class doe not (yet?) support root directory
    }

    REQUIRE( Api::remove( "d1/d2/d3/d4/d5/d5a.txt" ) );
    REQUIRE( Api::remove( "d1/d2/d3/d4/d5" ) );
    REQUIRE( Api::remove( "d1/d2/d3/d4/d4a.txt" ) );
    REQUIRE( Api::remove( "d1/d2/d3/d4" ) );
    REQUIRE( Api::remove( "d1/d2/d3/d3.txt" ) );
    REQUIRE( Api::remove( "d1/d2/d3/d3a.txt" ) );
    REQUIRE( Api::remove( "d1/d2/d3" ) );
    REQUIRE( Api::remove( "d1/d2/d2.txt" ) );
    REQUIRE( Api::remove( "d1/d2" ) );
    REQUIRE( Api::remove( "d1/d22" ) );
    REQUIRE( Api::remove( "d1/d1.txt" ) );
    REQUIRE( Api::remove( "d1" ) );
}