#ifndef Ajax_Main_appmain_h_
#define Ajax_Main_appmain_h_
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
/** @file

    This file defines the hardware/platform independent start-up interface(s)
    for the Ajax application
 */

#include "Cpl/Io/Input.h"
#include "Cpl/Io/Output.h"
#include "Cpl/Container/Map.h"
#include "Cpl/TShell/Command.h"


///
namespace Ajax {
///
namespace Main {

 /** This method launches the start-up sequence, then runs the application, followed
     by the shutdown sequence.  The 'infd' and 'outfd' are the file descriptors
     to be used for the TShell/Command-Line-Interface.

     NOTES:
         o The caller is responsible for calling Cpl::System::Api::initialize()
         o The caller of method MUST be executing in Cpl::System::Thread
         o This method ONLY returns once the shutdown sequence has
           been completed.
         o This method assumes that its thread is DEDICATED for performing the
           start-up/shutdown sequences. Once the start-up sequence has been
           completed, this thread stays blocked/in-the-waiting state until the
           Cpl::System::Shutdown interface is called to exit the application.
           All "shutdown handlers" will execute in this thread.
  */
int runTheApplication( Cpl::Io::Input& infd, Cpl::Io::Output& outfd );


/** Expose the TShell command list to facilitate creating platform specific
    TShell commands.
 */
extern Cpl::Container::Map<Cpl::TShell::Command>    g_cmdlist;


/*
** Thread Priorities
*/

/// Thread priority
#ifndef OPTION_AJAX_MAIN_THREAD_PRIORITY_CONSOLE
#define OPTION_AJAX_MAIN_THREAD_PRIORITY_CONSOLE       (CPL_SYSTEM_THREAD_PRIORITY_NORMAL + ( 2* CPL_SYSTEM_THREAD_PRIORITY_LOWER) )
#endif



};      // end namespaces
};
#endif  // end header latch