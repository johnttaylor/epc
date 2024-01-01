#ifndef Eros_TShell_Rgb_h
#define Eros_TShell_Rgb_h
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
#include "Driver/LED/RedGreenBlue.h"

///
namespace Eros {
///
namespace TShell {

/** This class implements a TShell command to verify the operation of the RGB
    LED on the display board.
 */
class Rgb : public Cpl::TShell::Cmd::Command
{
public:
    /// The command verb/identifier
    static constexpr const char* verb = "rgb";

    /// The command usage string
    static constexpr const char* usage = "rgb off\n"
                                         "rgb <red> <green> <blue>\n"
                                         "rgb <brightness>";

    /** The command detailed help string (recommended that lines do not exceed 80 chars)
                                                          1         2         3         4         5         6         7         8
                                                 12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* detailedHelp = "  Controls the RGB LED color and brightness. All numeric values are 0 to 255";


public:
    /// See Cpl::TShell::Command                                                               `
    const char* getUsage() const noexcept { return usage; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return detailedHelp; }


public:
    /// Constructor
    Rgb( Cpl::Container::Map<Cpl::TShell::Command>& commandList,
         Driver::LED::RedGreenBlue&                 statusLED,
         Cpl::TShell::Security::Permission_T        minPermLevel=OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL ) noexcept;


public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept;

protected:
    /// LED Driver
    Driver::LED::RedGreenBlue&  m_ledDriver;
};

}       // end namespaces
}
#endif  // end header latch
