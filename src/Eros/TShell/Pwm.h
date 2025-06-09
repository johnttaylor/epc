#ifndef Eros_TShell_Pwm_h
#define Eros_TShell_Pwm_h
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
#include "Driver/DIO/Pwm.h"

///
namespace Eros {
///
namespace TShell {

/** This class implements a TShell command to verify the PWM signals on the board
 */
class Pwm : public Cpl::TShell::Cmd::Command
{
public:
    /// The command verb/identifier
    static constexpr const char* verb = "pwm";

    /// The command usage string
    static constexpr const char* usage = "pwm (heater|fan) <dutycycle%>\n"
                                         "pwm backlight    <brightnes>";

    /** The command detailed help string (recommended that lines do not exceed 80 chars)
                                                          1         2         3         4         5         6         7         8
                                                 12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* detailedHelp = "  Sets the PWM duty cycle for the heater and fan signals AND the LCD back-light\n"
                                                "  brightness (0-255).";

public:
    /// See Cpl::TShell::Command                                                               `
    const char* getUsage() const noexcept { return usage; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return detailedHelp; }


public:
    /// Constructor
    Pwm( Cpl::Container::SList<Cpl::TShell::Command>& commandList,
         Driver::DIO::Pwm&                          heaterPWMDriver,
         Driver::DIO::Pwm&                          fanPWMDriver,
         Cpl::TShell::Security::Permission_T        minPermLevel=OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL ) noexcept;


public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept;

protected:
    /// Driver
    Driver::DIO::Pwm&  m_heaterDriver;

    /// Driver
    Driver::DIO::Pwm&  m_fanDriver;
};

}       // end namespaces
}
#endif  // end header latch
