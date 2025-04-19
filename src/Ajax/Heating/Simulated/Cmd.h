#ifndef Ajax_Heating_Simulated_Cmd_h
#define Ajax_Heating_Simulated_Cmd_h
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
/** @file */

#include "colony_config.h"
#include "Cpl/TShell/Cmd/Command.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/Mp/Double.h"
#include "Cpl/Dm/Mp/Int32.h"

///
namespace Ajax {
///
namespace Heating {
///
namespace Simulated {


/** This class implements a  Shell command.
 */
class Cmd : public Cpl::TShell::Cmd::Command
{
public:
    /// The command verb/identifier
    static constexpr const char* verb = "house";

    /// The command usage string
    static constexpr const char* usage = "house [<odt>]\n" 
                                         "house enable <odt>\n" 
                                         "house disable";

    /** The command detailed help string (recommended that lines do not exceed 80 chars)
                                                          1         2         3         4         5         6         7         8
                                                 12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* detailedHelp = "  Enables/disables the House simulator.  The house simulator uses the specified\n" \
                                                "  outdoor temperature <odt> and the PWM outputs signals to generate a indoor\n" \
                                                "  temperature.";


public:
    /// See Cpl::TShell::Command                                                               `
    const char* getUsage() const noexcept { return usage; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return detailedHelp; }


public:
    /// Constructor
    Cmd( Cpl::Container::SList<Cpl::TShell::Command>& commandList,
         Cpl::Dm::Mp::Bool&                   mpSimEnabled,
         Cpl::Dm::Mp::Int32&                  mpIndoorTemperature,
         Cpl::Dm::Mp::Double&                 mpOutdoorTemperature ) noexcept;


public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept;

protected:
    /// MP: Indoor/space temperature
    Cpl::Dm::Mp::Int32&     m_mpIdt;

    /// MP: Enable switch
    Cpl::Dm::Mp::Bool&      m_mpSimEnabled;

    /// MP: Outdoor temperature 
    Cpl::Dm::Mp::Double&    m_mpOutdoorTemperature;

};


};          // end namespaces
};
};
#endif      // end header latch
