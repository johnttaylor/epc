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

#include "Ui.h"
#include "mp/ModelPoints.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"
#include "Cpl/System/Trace.h"
#include <string.h>

#define SECT_   "Ajax::TShell"

///
using namespace Ajax::TShell;


///////////////////////////
Ui::Ui( Cpl::Container::SList<Cpl::TShell::Command>&          commandList,
        Cpl::Container::RingBufferMP<AjaxScreenMgrEvent_T>& uiEventRingBuffer,
        Cpl::TShell::Security::Permission_T                 minPermLevel ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, verb, minPermLevel )
    , m_uiEventFIFO( uiEventRingBuffer )
{
}


///////////////////////////
Cpl::TShell::Command::Result_T Ui::execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( cmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              outtext  = context.getOutputBuffer();
    bool                            io       = true;

    // Quick error check
    if ( tokens.numParameters() != 2 )
    {
        context.writeFrame( "Incorrect number of command arguments" );
        return Command::eERROR_INVALID_ARGS;
    }

    // Parse event name
    const char* evname = tokens.getParameter( 1 );
    if ( strcmp( evname, "btn-a" ) == 0 )
    {
        m_uiEventFIFO.add( AJAX_UI_EVENT_BUTTON_A );
    }
    else if ( strcmp( evname, "btn-b" ) == 0 )
    {
        m_uiEventFIFO.add( AJAX_UI_EVENT_BUTTON_B );
    }
    else if ( strcmp( evname, "btn-x" ) == 0 )
    {
        m_uiEventFIFO.add( AJAX_UI_EVENT_BUTTON_X );
    }
    else if ( strcmp( evname, "btn-y" ) == 0 )
    {
        m_uiEventFIFO.add( AJAX_UI_EVENT_BUTTON_Y );
    }
    else if ( strcmp( evname, "btn-esc" ) == 0 )
    {
        m_uiEventFIFO.add( AJAX_UI_EVENT_BUTTON_ESC );
    }
    else
    {
        outtext.format( "invalid event name (%s). Type 'help ui' for a list of supported events", evname );
        context.writeFrame( outtext );
        return Command::eERROR_INVALID_ARGS;
    }

    outtext.format( "Generated event: %s", evname );
    io &= context.writeFrame( outtext );
    return io ? Command::eSUCCESS : Command::eERROR_IO;
}

