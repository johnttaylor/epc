/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
 * open source project with a BSD type of licensing agreement.  See the license
 * agreement (license.txt) in the top/ directory or on the Internet at
 * http://integerfox.com/colony.core/license.txt
 *
 * Copyright (c) 2014-2024  John T. Taylor
 *
 * Redistributions of the source code must retain the above copyright notice.
**----------------------------------------------------------------------------*/

#include "Output.h"
#include <stdarg.h>
#include <string.h>


//
using namespace Cpl::Io;


//////////////////////
bool Output::write( char c )
{
    return write( (const void*) &c, sizeof( c ) );
}


bool Output::write( const char* string )
{
    return write( string, strlen( string ) );
}


bool Output::write( const Cpl::Text::String& string )
{
    return write( string(), string.length() );
}


bool Output::write( Cpl::Text::String& formatBuffer, const char* format, ... )
{
    va_list ap;
    va_start( ap, format );
    bool result = vwrite( formatBuffer, format, ap );
    va_end( ap );
    return result;
}


bool Output::vwrite( Cpl::Text::String& formatBuffer, const char* format, va_list ap )
{
    formatBuffer.vformat( format, ap );
    return write( formatBuffer(), formatBuffer.length() );
}


bool Output::write( const void* buffer, int numBytes )
{
    int  written       = 0;
    const uint8_t* ptr = (const uint8_t*) buffer;

    // Loop until all data is written
    while ( numBytes )
    {
        if ( !write( ptr, numBytes, written ) )
        {
            return false;
        }
        numBytes -= written;
        ptr      += written;
    }

    // If I get here, all data has been written
    return true;
}

