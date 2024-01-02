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

#include "Rgb.h"
#include "mp/ModelPoints.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Text/atob.h"
#include <string.h>

#define SECT_   "Eros::TShell"

///
using namespace Eros::TShell;


///////////////////////////
Rgb::Rgb( Cpl::Container::Map<Cpl::TShell::Command>& commandList,
          Driver::LED::RedGreenBlue&                 statusLED,
          Cpl::TShell::Security::Permission_T        minPermLevel ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, verb, minPermLevel )
    , m_ledDriver( statusLED )
{
}


///////////////////////////
Cpl::TShell::Command::Result_T Rgb::execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( cmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              outtext  = context.getOutputBuffer();
    bool                            io       = true;

    // Turn Off
    if ( tokens.numParameters() == 2 && tokens.getParameter( 1 )[0] == 'o' )
    {
        m_ledDriver.setOff();
        io &= context.writeFrame( "RGB LED turned off." );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    // Set brightness
    if ( tokens.numParameters() == 2 )
    {
        unsigned val;
        if ( !Cpl::Text::a2ui( val, tokens.getParameter( 1 ) ) || val >= 256 )
        {
            outtext.format( "Error: Brightness argument invalid (%s)", tokens.getParameter( 1 ) );
            context.writeFrame( outtext );
            return Command::eERROR_INVALID_ARGS;
        }

        m_ledDriver.setBrightness( val );
        outtext.format( "Brightness set to: %s", tokens.getParameter( 1 ) );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    // Set color
    if ( tokens.numParameters() == 4 )
    {
        unsigned red;
        unsigned green;
        unsigned blue;
        if ( !Cpl::Text::a2ui( red, tokens.getParameter( 1 ) ) || red >= 256 ||
             !Cpl::Text::a2ui( green, tokens.getParameter( 2 ) ) || green >= 256 ||
             !Cpl::Text::a2ui( blue, tokens.getParameter( 3 ) ) || blue >= 256 )
        {
            outtext.format( "Error: one or more color argument invalid (%s %s %s)", tokens.getParameter( 1 ), tokens.getParameter( 2 ), tokens.getParameter( 3 ) );
            context.writeFrame( outtext );
            return Command::eERROR_INVALID_ARGS;
        }

        m_ledDriver.setRgb( red, green, blue );
        outtext.format( "RGB Color set to: %s %s %s", tokens.getParameter( 1 ), tokens.getParameter( 2 ), tokens.getParameter( 3 ) );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    // If I get here, the command syntax was not valid
    return Command::eERROR_INVALID_ARGS;
}

