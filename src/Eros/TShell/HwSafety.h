#ifndef Eros_TShell_HwSafety_h
#define Eros_TShell_HwSafety_h
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
/** @file */

#include "colony_config.h"
#include "Cpl/TShell/Cmd/Command.h"
#include "Driver/DIO/In.h"

///
namespace Eros
{
///
namespace TShell
{

/** This class implements a TShell command to verify the PWM signals on the board
 */
class HwSafety : public Cpl::TShell::Cmd::Command
{
public:
    /// The command verb/identifier
    static constexpr const char* verb = "hws";

    /// The command usage string
    static constexpr const char* usage = "hws";

    /** The command detailed help string (recommended that lines do not exceed 80 chars)
                                                          1         2         3         4         5         6         7         8
                                                 12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* detailedHelp = "  Samples the Hardware Safety Limit tripped signal.";

public:
    /// See Cpl::TShell::Command                                                               `
    const char* getUsage() const noexcept { return usage; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return detailedHelp; }


public:
    /// Constructor
    HwSafety( Cpl::Container::SList<Cpl::TShell::Command>& commandList,
              Driver::DIO::In&                           hwSafetyInDriver,
              Cpl::TShell::Security::Permission_T        minPermLevel=OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL ) noexcept;


public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept;

protected:
    /// Driver
    Driver::DIO::In&  m_hwSafetyInDriver;
};

}       // end namespaces
}
#endif  // end header latch
