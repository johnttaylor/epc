/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2023  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/*
    Implementation of the System::FatalError interface using the standard
    C library.

    Notes:
        o The log messages are sent to stderr
        o The application is exited with an errorcode of '1'
        o 'Extra Info' is limited to a '@@ Fatal Error:' prefix
        o The implementation is NOT thread safe.

*/

#include "Cpl/System/FatalError.h"
#include "Cpl/System/Shutdown.h"
#include "Cpl/System/Api.h"
#include "Ajax/Logging/Api.h"
#include "Cpl/Logging/Api.h"
#include <stdarg.h> 
#include <stdio.h>  


using namespace Cpl::System;

#define EXTRA_INFO  "@@ Fatal Error: "


////////////////////////////////////////////////////////////////////////////////
void FatalError::log( const char* message )
{
    Ajax::Logging::logf( Ajax::Logging::CriticalMsg::FATAL_ERROR, message );
    Cpl::System::Api::sleep( 200 ); // Delay in attempt to allow the fatal error to be logged

    fprintf( stderr, "\n%s%s\n", EXTRA_INFO, message );
    Shutdown::failure( OPTION_CPL_SYSTEM_FATAL_ERROR_EXIT_CODE );
}

void FatalError::log( const char* message, size_t value )
{
    Ajax::Logging::logf( Ajax::Logging::CriticalMsg::FATAL_ERROR, "%s [%p]", message, (void*) value );
    Cpl::System::Api::sleep( 200 ); // Delay in attempt to allow the fatal error to be logged

    fprintf( stderr, "\n%s%s [%p]\n", EXTRA_INFO, message, (void*) value );
    Shutdown::failure( OPTION_CPL_SYSTEM_FATAL_ERROR_EXIT_CODE );
}


void FatalError::logf( const char* format, ... )
{
    va_list ap;
    va_start( ap, format );

    Cpl::Logging::vlogf<Ajax::Logging::CategoryId, Ajax::Logging::CriticalMsg>( Ajax::Logging::CategoryId::CRITICAL, Ajax::Logging::CriticalMsg::FATAL_ERROR, format, ap );
    Cpl::System::Api::sleep( 200 ); // Delay in attempt to allow the fatal error to be logged

    fprintf( stderr, "\n%s", EXTRA_INFO );
    vfprintf( stderr, format, ap );
    fprintf( stderr, "\n" );
    Shutdown::failure( OPTION_CPL_SYSTEM_FATAL_ERROR_EXIT_CODE );
}


////////////////////////////////////////////////////////////////////////////////
void FatalError::logRaw( const char* message )
{
    log( message );
}

void FatalError::logRaw( const char* message, size_t value )
{
    log( message, value );
}



