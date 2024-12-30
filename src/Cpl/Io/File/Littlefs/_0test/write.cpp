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
#include "Cpl/Io/File/Output.h"
#include "Cpl/Io/File/Input.h"
#include "Cpl/Io/LineReader.h"
#include "Cpl/Io/LineWriter.h"
#include "Cpl/Text/FString.h"
#include "Cpl/System/Assert.h"
#include "Cpl/Io/File/Littlefs/Api.h"


#define SECT_     "_0test"

#define REQUIRE   CPL_SYSTEM_ASSERT

/// 
using namespace Cpl::Io::File;
using namespace Cpl::Io::File::Littlefs;



////////////////////////////////////////////////////////////////////////////////
void run_write_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut )
{
    printf( "\n\n*** run_write_tests() ***\n" );

    Cpl::Text::FString<256> sum;
    Cpl::Text::FString<10>  buffer( "bob" );
    char                    myBuffer[10] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29 };
    int                     bytesWritten;
    Output                  fd( "output1.txt", true, true );
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
    for ( size_t i=0; i < sizeof( myBuffer ); i++ ) { sum += myBuffer[i]; }
    REQUIRE( fd.write( myBuffer, sizeof( myBuffer ), bytesWritten ) );
    REQUIRE( (size_t) bytesWritten == sizeof( myBuffer ) );
    for ( int i=0; i < bytesWritten; i++ ) { sum += myBuffer[i]; }

    fd.flush();
    fd.close();
    REQUIRE( fd.write( 'a' ) == false );
    REQUIRE( fd.isOpened() == false );

    Input infd( "output1.txt" );
    REQUIRE( infd.isOpened() );
    Cpl::Text::FString<256> inbuffer;
    infd.read( inbuffer );
    REQUIRE( inbuffer == sum );
    REQUIRE( infd.read( inbuffer ) == false ); // Ensure EOF is hit
    REQUIRE( infd.isEof() );
    infd.close();
    REQUIRE( infd.isOpened() == false );


    //
    Output fd2( "output2.txt", true, true );
    Cpl::Io::LineWriter writer( fd2 );
    REQUIRE( writer.println() );
    REQUIRE( writer.println( "Hello World" ) );
    REQUIRE( writer.print( "Hello" ) );
    REQUIRE( writer.print( "World" ) );
    REQUIRE( writer.print( " again!" ) );
    REQUIRE( writer.println() );
    writer.close();
    REQUIRE( writer.println() == false );
    REQUIRE( fd.write( 'a' ) == false );

    Cpl::Text::FString<256> buffer2;
    Input infd2( "output2.txt" );
    REQUIRE( infd2.isOpened() );
    Cpl::Io::LineReader reader( infd2 );
    REQUIRE( reader.readln( buffer2 ) );
    REQUIRE( buffer2.isEmpty() );
    REQUIRE( reader.readln( buffer2 ) );
    REQUIRE( buffer2 == "Hello World" );
    REQUIRE( reader.readln( buffer2 ) );
    REQUIRE( buffer2 == "HelloWorld again!" );
	REQUIRE( reader.readln( buffer2 ) == false );
	REQUIRE( infd2.isEof() );
    reader.close();
    REQUIRE( infd2.isOpened() == false );


    Output fd3( "output2.txt", true, true );
    REQUIRE( fd3.isOpened() );
    fd3.close();
    char dummyChar = 29;
    REQUIRE( fd3.write( dummyChar ) == false );
    REQUIRE( dummyChar == 29 );
    fd3.flush();
    
    REQUIRE( fd3.isEof() == true );
    unsigned long pos;
    REQUIRE( fd3.currentPos( pos ) == false );
    REQUIRE( fd3.setAbsolutePos( 1 ) == false );
    REQUIRE( fd3.setRelativePos( 1 ) == false );
    REQUIRE( fd3.setToEof() == false );
    unsigned long len = 22;
    REQUIRE( fd3.length( len ) == false );
}