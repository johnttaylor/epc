/*-----------------------------------------------------------------------------
 * This file is part of the Colony.Core Project.  The Colony.Core Project is an
 * open source project with a BSD type of licensing agreement.  See the license
 * agreement (license.txt) in the top/ directory or on the Internet at
 * http://integerfox.com/colony.core/license.txt
 *
 * Copyright (c) 2014-2022  John T. Taylor
 *
 * Redistributions of the source code must retain the above copyright notice.
 *----------------------------------------------------------------------------*/

#include "InputOutput.h"
#include <stdio.h>

///
using namespace Cpl::Io::Serial::PhonyStdio;


////////////////////////////////////
InputOutput::InputOutput()
    : m_eos( false )
{
}


InputOutput::~InputOutput()
{
    close();
}


////////////////////////////////////
bool InputOutput::read( void* buffer, int numBytes, int& bytesRead )
{
    // Ignore read requests of ZERO bytes
    if ( numBytes == 0 )
    {
        bytesRead = 0;
        return true;
    }

    // perform the read. ONLY read one byte at a time!
    bytesRead   = (int)fread( buffer, sizeof( char ), 1, stdin );
    bool result = true;
    m_eos       = false;
    if ( bytesRead <= 0 )
    {
        result = false;
        m_eos  = feof( stdin ) ? true : false;
    }
    // printf( "eos=%d, ferr=%d, bytesRead=%d, result=%d\n", m_eos, ferror( stdin ), bytesRead, result );
    clearerr( stdin );
    return result;
}

bool InputOutput::available()
{
    return true;  // Not supported
}


////////////////////////////////////
bool InputOutput::write( const void* buffer, int maxBytes, int& bytesWritten )
{
    // Ignore write requests of ZERO bytes
    if ( maxBytes == 0 )
    {
        bytesWritten = 0;
        return true;
    }

    // perform the write
    bytesWritten = (int)fwrite( buffer, sizeof( char ), maxBytes, stdout );
    bool result  = true;
    m_eos        = false;
    if ( bytesWritten <= 0 )
    {
        m_eos  = feof( stdout ) ? true : false;
        result = false;
    }
    // printf( "eos=%d, ferr=%d, bytesWritten=%d, result=%d\n", m_eos, ferror( stdout ), bytesWritten, result );
    clearerr( stdout );
    return result;
}


void InputOutput::flush()
{
    fflush( stdout );
}

bool InputOutput::isEos()
{
    return m_eos;
}

void InputOutput::close()
{
    // Do nothing
}
