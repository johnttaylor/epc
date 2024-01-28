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

#include "HwSafety.h"

#define SECT_   "Eros::TShell"

///
using namespace Eros::TShell;


///////////////////////////
HwSafety::HwSafety( Cpl::Container::Map<Cpl::TShell::Command>& commandList,
          Driver::DIO::In&                           hwSafetyInDriver,
          Cpl::TShell::Security::Permission_T        minPermLevel ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, verb, minPermLevel )
    , m_hwSafetyInDriver( hwSafetyInDriver )
{
}


///////////////////////////
Cpl::TShell::Command::Result_T HwSafety::execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::String& outtext = context.getOutputBuffer();

    // Read the Input signal on demand
    bool val = m_hwSafetyInDriver.sample();
    outtext.format( "HW Safety Limited Tripped: %s", val ? "ASSERTED" : "deasserted" );
    return context.writeFrame( outtext )? Command::eSUCCESS : Command::eERROR_IO;
}

