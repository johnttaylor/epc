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

#include "State.h"
#include "mp/ModelPoints.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"
#include "Cpl/System/Trace.h"
#include <string.h>

#define SECT_   "Ajax::TShell"

///
using namespace Ajax::TShell;


///////////////////////////
State::State( Cpl::Container::SList<Cpl::TShell::Command>& commandList,
                      Cpl::TShell::Security::Permission_T        minPermLevel ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, verb, minPermLevel )
{
}


///////////////////////////
Cpl::TShell::Command::Result_T State::execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( cmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              outtext  = context.getOutputBuffer();
    Cpl::Text::String&              parm1    = context.getTokenBuffer();
    Cpl::Text::String&              parm2    = context.getTokenBuffer2();
    bool                            io       = true;

    // Read temperature inputs
    int32_t primtaryIdt = 0;
    int32_t idt;
    parm1 = "<invalid>";
    if ( mp::onBoardIdt.read( idt ) )
    {
        parm1.format( "%.2f", (double) (idt / 100.0) );
        primtaryIdt = idt;
    }
    parm2 = "<invalid>";
    if ( mp::remoteIdt.read( idt ) )
    {
        parm2.format( "%.2f", (double) (idt / 100.0) );
        if ( primtaryIdt == 0 )
        {
            primtaryIdt = idt;
        }
    }
    outtext.format( "Temperature: onbrd: %s   remote: %s", parm1.getString(), parm2.getString() );
    io &= context.writeFrame( outtext );

    // Setpoint
    int32_t setpt;
    parm1 = "<invalid>";
    if ( mp::heatSetpoint.read( setpt ) )
    {
        parm1.format( "%g             err=%g", (double) (setpt / 100.0), (double) ((setpt - primtaryIdt) / 100.0 ) );
    }
    outtext.format( "Setpoint:    %s", parm1.getString(), parm2.getString() );
    io &= context.writeFrame( outtext );

    // Read Outputs
    uint32_t pwm;
    parm1 = "<invalid>";
    if ( mp::cmdHeaterPWM.read( pwm ) )
    {
        parm1.format( "%3d", (pwm * 100) / 0xFFFF );
    }
    parm2 = "<invalid>";
    if ( mp::cmdFanPWM.read( pwm ) )
    {
        parm2.format( "%3d", (pwm * 100) / 0xFFFF );
    }
    outtext.format( "Outputs:     heatPWM: %s%%  fanPWM: %s%%", parm1.getString(), parm2.getString() );
    io &= context.writeFrame( outtext );

    // Modes
    Ajax::Type::FanMode fanMode;
    parm1 = "<invalid>";
    if ( mp::fanMode.read( fanMode ) )
    {
        parm1 = fanMode._to_string();
    }
    bool heatMode;
    parm2 = "<invalid>";
    if ( mp::heatingMode.read( heatMode ) )
    {
        parm2 = heatMode ? "ON " : "off";
    }
    outtext.format( "Modes:       Heating: %s   Fan: %s", parm2.getString(), parm1.getString() );
    io &= context.writeFrame( outtext );

    // Alerts
    Ajax::Dm::MpAlert::Data alert;
    parm1 = "---";
    if ( mp::sensorFailAlert.read( alert ) )
    {
        parm1.format( "RAISED (%u).", alert.priority );
    }
    parm2 = "---";
    if ( mp::failedSafeAlert.read( alert ) )
    {
        parm2.format( "RAISED (%u).", alert.priority );
    }
    outtext.format( "Alerts:      sensor: %s    safety: %s", parm1.getString(), parm2.getString() );
    io &= context.writeFrame( outtext );

    // Return the command result
    return io ? Command::eSUCCESS : Command::eERROR_IO;
}

