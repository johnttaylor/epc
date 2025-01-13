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
#include "Cpl/Io/File/InputOutput.h"
#include "Cpl/Io/LineReader.h"
#include "Cpl/Io/LineWriter.h"
#include "Cpl/Text/FString.h"
#include "Cpl/System/Assert.h"
#include "Cpl/Io/File/Littlefs/Api.h"
#include "Cpl/Io/File/Api.h"


#define REQUIRE CPL_SYSTEM_ASSERT

#define OUTPUT3_TXT "/vola/output3.txt"
#define OUTPUT4_TXT "/volb/output4.txt"

///
using namespace Cpl::Io::File;

////////////////////////////////////////////////////////////////////////////////
namespace {

class Walker : public Api::DirectoryWalker
{
public:
    ///
    Walker()
    {
    }

public:
    Cpl::Type::Traverser::Status_T item( const char* currentParent, const char* fsEntryName, Api::Info& entryInfo )
    {
        // File check
        printf( "\n%s%s Parent=%s, item=%s.", entryInfo.m_isFile ? "f" : "-", entryInfo.m_isDir ? "d" : "-", currentParent, fsEntryName);
        return Cpl::Type::Traverser::eCONTINUE;
    }
};

} // end anonymous namespace

////////////////////////////////////////////////////////////////////////////////
void run_readwrite_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut1, Cpl::Io::File::Littlefs::Api::Volume_T& uut2 )
{
    printf( "\n\n*** run_readwrite_tests() ***\n" );

    //
    Cpl::Text::FString<256> sum;
    Cpl::Text::FString<10>  buffer( "bob" );
    char                    myBuffer[10] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29 };
    int                     bytesWritten;
    InputOutput             fd( OUTPUT3_TXT, true, true );
    REQUIRE( fd.isOpened() );
    REQUIRE( fd.write( 'a' ) );
    sum = 'a';
    REQUIRE( fd.write( "bob's your uncle" ) );
    sum += "bob's your uncle";
    REQUIRE( fd.write( buffer ) );
    sum += buffer;
    REQUIRE( fd.write( buffer, "Hello %s", "World" ) );
    REQUIRE( buffer == "Hello Worl" );
    sum += buffer;
    REQUIRE( fd.write( myBuffer, sizeof( myBuffer ) ) );
    for ( size_t i = 0; i < sizeof( myBuffer ); i++ )
    {
        sum += myBuffer[i];
    }
    REQUIRE( fd.write( myBuffer, sizeof( myBuffer ), bytesWritten ) );
    REQUIRE( (size_t)bytesWritten == sizeof( myBuffer ) );
    for ( int i = 0; i < bytesWritten; i++ )
    {
        sum += myBuffer[i];
    }
    fd.flush();

    REQUIRE( fd.isOpened() );
    Cpl::Text::FString<256> inbuffer;
    REQUIRE( fd.setAbsolutePos( 0 ) );
    fd.read( inbuffer );
    REQUIRE( inbuffer == sum );
    REQUIRE( fd.read( inbuffer ) == false );  // Ensure EOF is hit
    REQUIRE( fd.isEof() );
    fd.close();
    REQUIRE( fd.isOpened() == false );
    REQUIRE( fd.write( 'a' ) == false );
    char dummy;
    REQUIRE( fd.read( dummy ) == false );
    REQUIRE( fd.isEof() );
    REQUIRE( fd.isOpened() == false );

    //
    InputOutput         fd2( OUTPUT4_TXT, true, true );
    Cpl::Io::LineWriter writer( fd2 );
    REQUIRE( writer.println() );
    REQUIRE( writer.println( "Hello World" ) );
    REQUIRE( writer.print( "Hello" ) );
    REQUIRE( writer.print( "World" ) );
    REQUIRE( writer.print( " again!" ) );
    REQUIRE( writer.println() );

    Cpl::Text::FString<256> buffer2;
    REQUIRE( fd2.isOpened() );
    REQUIRE( fd2.setAbsolutePos( 0 ) );
    Cpl::Io::LineReader reader( fd2 );
    REQUIRE( reader.readln( buffer2 ) );
    REQUIRE( buffer2.isEmpty() );
    REQUIRE( reader.readln( buffer2 ) );
    REQUIRE( buffer2 == "Hello World" );
    REQUIRE( reader.readln( buffer2 ) );
    REQUIRE( buffer2 == "HelloWorld again!" );
    REQUIRE( reader.readln( buffer2 ) == false );
    REQUIRE( fd2.isEof() );
    reader.close();
    REQUIRE( fd2.isOpened() == false );
    writer.close();
    REQUIRE( writer.println() == false );
    REQUIRE( fd2.write( 'a' ) == false );

    fd.close();
    fd2.close();

    {
        Walker iterator;;
        REQUIRE( Api::walkDirectory( "/vola", iterator, OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH ) );
    }
    {
        Walker iterator;;
        REQUIRE( Api::walkDirectory( "/volb", iterator, OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH ) );
    }
}