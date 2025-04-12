#ifndef Ajax_TShell_Ui_h
#define Ajax_TShell_Ui_h
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
#include "Cpl/Container/RingBufferMP.h"
#include "Ajax/ScreenMgr/Event.h"

///
namespace Ajax
{
///
namespace TShell
{



/** This class implements a TShell command that generates a UI event

 */
class Ui : public Cpl::TShell::Cmd::Command
{
public:
    /// The command verb/identifier
    static constexpr const char* verb = "ui";

    /// The command usage string
    static constexpr const char* usage = "ui <event>";

    /** The command detailed help string (recommended that lines do not exceed 80 chars)
                                                          1         2         3         4         5         6         7         8
                                                 12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* detailedHelp = "  Generates UI events. Supported events are:\n"
                                                "    btn-a   -->AJAX_UI_EVENT_BUTTON_A\n"
                                                "    btn-b   -->AJAX_UI_EVENT_BUTTON_B\n"
                                                "    btn-x   -->AJAX_UI_EVENT_BUTTON_X\n"
                                                "    btn-y   -->AJAX_UI_EVENT_BUTTON_Y\n"
                                                "    btn-esc -->AJAX_UI_EVENT_BUTTON_ESC\n"
                                                "    nop     -->AJAX_UI_EVENT_NO_EVENT";


public:
    /// See Cpl::TShell::Command                                                               `
    const char* getUsage() const noexcept { return usage; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return detailedHelp; }


public:
    /// Constructor
    Ui( Cpl::Container::SList<Cpl::TShell::Command>&          commandList,
        Cpl::Container::RingBufferMP<AjaxScreenMgrEvent_T>& uiEventRingBuffer,
        Cpl::TShell::Security::Permission_T                 minPermLevel=OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL ) noexcept;


public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept;

protected:
    /// UI Event FIFO
    Cpl::Container::RingBufferMP<AjaxScreenMgrEvent_T>& m_uiEventFIFO;
};

}       // end namespaces
}
#endif  // end header latch
