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
#include "Cpl/Io/File/Input.h"
#include "Cpl/Io/File/Output.h"
#include "Cpl/Io/LineReader.h"
#include "Cpl/Text/FString.h"
#include "Cpl/System/Assert.h"
#include "Cpl/Io/File/Littlefs/Api.h"


#define REQUIRE CPL_SYSTEM_ASSERT


#define TESTINPUT_TEXT_HELLO_OFFEST 0x58


///
using namespace Cpl::Io::File;
using namespace Cpl::Io::File::Littlefs;

#define FILENAME "tinput.txt"

#define FILE_CONTENTS                                                     \
    "line 1\n"                                                            \
    "line 2\n"                                                            \
    "\n"                                                                  \
    "line 4 - extra stuff, really extra stuff, okay really extra stuff\n" \
    "line 5\n"                                                            \
    "AHello World."

////////////////////////////////////////////////////////////////////////////////
void run_read_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut )
{
    printf( "\n\n*** run_read_tests() ***\n" );

    // Create input file
    Output fdout( FILENAME, true, true );
    REQUIRE( fdout.isOpened() );
    fdout.write( FILE_CONTENTS );
    fdout.close();

    printf( "\nReading Lines..." );
    Input               fd( FILENAME );
    Cpl::Io::LineReader reader( fd );
    REQUIRE( fd.isOpened() );

    Cpl::Text::FString<6> line;
    REQUIRE( reader.available() == true );
    reader.readln( line );
    printf( "\nline=[%s]", line.getString() );
    REQUIRE( line == "line 1" );
    REQUIRE( reader.readln( line ) );
    printf( "\nline=[%s]", line.getString() );
    REQUIRE( line == "line 2" );
    REQUIRE( reader.readln( line ) );
    unsigned long len;
    REQUIRE( fd.length( len ) == true );
    REQUIRE( len == sizeof( FILE_CONTENTS ) - 1 );
    printf( "\nline=[%s]", line.getString() );
    line.removeTrailingSpaces();
    REQUIRE( line.isEmpty() );
    REQUIRE( reader.readln( line ) );
    printf( "\nline=[%s]", line.getString() );
    REQUIRE( line == "line 4" );
    REQUIRE( reader.readln( line ) );
    printf( "\nline=[%s]", line.getString() );
    REQUIRE( line == "line 5" );
    reader.close();
    REQUIRE( fd.isOpened() == false );
    REQUIRE( reader.readln( line ) == false );
    REQUIRE( fd.read( line ) == false );

    //
    Input fd2( FILENAME );
    REQUIRE( fd2.isOpened() );
    char dummyChar = 29;
    REQUIRE( fd2.setAbsolutePos( TESTINPUT_TEXT_HELLO_OFFEST ) );
    REQUIRE( fd2.length( len ) == true );
    REQUIRE( len == sizeof( FILE_CONTENTS ) - 1 );
    REQUIRE( fd2.read( dummyChar ) == true );
    REQUIRE( dummyChar == 'A' );

    Cpl::Text::FString<10> buffer( "bob" );
    REQUIRE( fd2.read( buffer ) == true );
    REQUIRE( buffer == "Hello Worl" );

    char myBuffer[10] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29 };
    int  bytesRead    = 1;
    REQUIRE( fd2.available() == true );
    REQUIRE( fd2.read( myBuffer, sizeof( myBuffer ), bytesRead ) == true );
    REQUIRE( fd2.isEof() == false );
    REQUIRE( bytesRead == 2 );
    REQUIRE( myBuffer[0] == 'd' );
    REQUIRE( myBuffer[1] == '.' );
    REQUIRE( fd2.read( myBuffer, sizeof( myBuffer ), bytesRead ) == false );
    REQUIRE( fd2.isEof() == true );
    REQUIRE( bytesRead == 0 );

    fd2.close();
    REQUIRE( fd2.isOpened() == false );
    REQUIRE( fd2.read( dummyChar ) == false );


    Input fd3( FILENAME );
    REQUIRE( fd3.isOpened() );
    fd3.close();
    dummyChar = 29;
    REQUIRE( fd3.read( dummyChar ) == false );
    REQUIRE( dummyChar == 29 );

    REQUIRE( fd3.available() == false );

    REQUIRE( fd3.isEof() == true );
    unsigned long pos;
    REQUIRE( fd3.currentPos( pos ) == false );
    REQUIRE( fd3.setAbsolutePos( 1 ) == false );
    REQUIRE( fd3.setRelativePos( 1 ) == false );
    REQUIRE( fd3.setToEof() == false );
    len = 22;
    REQUIRE( fd3.length( len ) == false );
}
