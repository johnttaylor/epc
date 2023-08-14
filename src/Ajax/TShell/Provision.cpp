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

#include "Provision.h"
#include "mp/ModelPoints.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"
#include <string.h>

///
using namespace Ajax::TShell;


///////////////////////////
Provision::Provision( Cpl::Container::Map<Cpl::TShell::Command>& commandList,
                      Ajax::Main::PersonalityRecord&             personalityRecord,
                      Cpl::Persistent::RecordServer&             recordServer,
                      Cpl::TShell::Security::Permission_T        minPermLevel ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, verb, minPermLevel )
    , m_recordServer( recordServer )
    , m_personalityRec( personalityRecord )
{
}


///////////////////////////
Cpl::TShell::Command::Result_T Provision::execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( cmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              outtext  = context.getOutputBuffer();

    // Initial check for not-enough-tokens
    if ( tokens.numParameters() != 4 )
    {
        return Command::eERROR_MISSING_ARGS;
    }

    mp::modelNumber.write( tokens.getParameter( 1 ) );
    mp::serialNumber.write( tokens.getParameter( 2 ) );
    // TODO: mp::consolePwdHash.write( tokens.getParameter( 3 ) );
    // TODO: mp::AlgorithmMPxyz.write( .. );

    // Update NVRAM
    m_personalityRec.flush( m_recordServer );

    // Feedback
    bool io = true;
    outtext.format( "Model Number:     %s", tokens.getParameter( 1 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "Serial Number:    %s", tokens.getParameter( 2 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "Console Pwd:      %s", tokens.getParameter( 3 ) );
    io &= context.writeFrame( outtext );

    // Return the command result
    return io? Command::eSUCCESS: Command::eERROR_IO;
}
