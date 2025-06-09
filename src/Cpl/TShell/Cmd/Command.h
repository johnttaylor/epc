#ifndef Cpl_TShell_Cmd_Command_h_
#define Cpl_TShell_Cmd_Command_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2025  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "colony_config.h"
#include "Cpl/TShell/Command.h"
#include "Cpl/TShell/Context_.h"

/** Default Permission level for all commands
 */
#ifndef OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL
#define OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL Cpl::TShell::Security::ePUBLIC
#endif


///
namespace Cpl {
///
namespace TShell {
///
namespace Cmd {

/** This partially concrete class implements infrastructure and/or common
	functionality for a  Shell command.
 */
class Command : public Cpl::TShell::Command
{
protected:
    /// Result when processing a conditional statement
    enum CondResult_T
    {
        eFALSE,  //!< The conditional statement evaluated to false
        eTRUE,   //!< The conditional statement evaluated to true
        eERROR   //!< Error occurring during the evaluation of the conditional
    };

protected:
    /// Command verb
    const char* m_verb;

    /// Minimum required user permission need to execute me
    Security::Permission_T m_minPermissionLevel;

protected:
    /// Constructor
    Command( Cpl::Container::SList<Cpl::TShell::Command>& commandList,
             const char*                                  verb,
             Security::Permission_T                       minPermLevel = OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL ) noexcept;


public:
    /// See Cpl::TShell::Command
    const char* getVerb() const noexcept;

    /// See Cpl::TShell::Command
    Security::Permission_T getMinPermissionRequired() const noexcept;
};

};  // end namespaces
};
};
#endif  // end header latch
