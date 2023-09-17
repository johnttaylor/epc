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

#include "Cmd.h"
#include "Cpl/Text/atob.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/ElapsedTime.h"
#include <string.h>         

///
using namespace Driver::NV;


///////////////////////////
Cmd::Cmd( Cpl::Container::Map<Cpl::TShell::Command>& commandList,
          Driver::NV::Api&                           nvDriver,
          Cpl::TShell::Security::Permission_T        minPermLevel ) noexcept
    : Command( commandList, verb, minPermLevel )
    , m_nvDriver( nvDriver )
{
}

///////////////////////////
Cpl::TShell::Command::Result_T Cmd::execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( cmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              outtext  = context.getOutputBuffer();
    bool                            io       = true;

    // ERASE
    if ( tokens.numParameters() == 2 && strcmp( tokens.getParameter( 1 ), "ERASE" ) == 0 )
    {
        // Erase the entire NV storage
        uint8_t buf[128];
        memset( buf, 0xFF, sizeof(buf) );
        size_t        bytesRemaining = m_nvDriver.getTotalSize();
        size_t        offset         = 0;
        unsigned long startTime      = Cpl::System::ElapsedTime::milliseconds();
        while ( bytesRemaining )
        {
            size_t bytesToWrite = bytesRemaining > sizeof( buf ) ? sizeof( buf ) : bytesRemaining;
            if ( !m_nvDriver.write( offset, buf, bytesToWrite ) )
            {
                outtext.format( "Erase failed at offset: %lu", offset );
                context.writeFrame( outtext );
                return Command::eERROR_FAILED;
            }
            bytesRemaining -= bytesToWrite;
            offset         += bytesToWrite;
        }
        unsigned long endTime = Cpl::System::ElapsedTime::milliseconds();
        outtext.format( "Erase completed. Duration = %lu ms", endTime-startTime );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    // If I get here -->the argument(s) where bad
    return Cpl::TShell::Command::eERROR_INVALID_ARGS;
}
