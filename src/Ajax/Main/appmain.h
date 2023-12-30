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

#include "Ajax/Ui/PicoDisplay.h"        // FIXME: This header file (because it include pimoroni header files) MUST be first :(
#include "colony_config.h"
#include "Cpl/Io/Input.h"
#include "Cpl/Io/Output.h"
#include "Cpl/Container/Map.h"
#include "Cpl/TShell/Command.h"
#include "Ajax/ScreenMgr/Navigation.h"
#include "Driver/NV/Api.h"
#include "Driver/Crypto/Hash.h"
#include "Cpl/Dm/MailboxServer.h"
#include "Driver/AIO/HalSingleInput.h"


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

/// Expose the Graphic library (to faciliate static screen creation)
extern pimoroni::PicoGraphics_PenRGB332 g_graphics;

/// Expose a handle to the global Screen Navigation instance
extern Ajax::ScreenMgr::Navigation&     g_screenNav;

/// Expose the handle to the NV driver
extern Driver::NV::Api&                 g_nvramDriver;

/// Expose the Console hash function
extern Driver::Crypto::Hash*            g_sha512Ptr;

/// Expose the mailbox for the "application" thread (for Ajax this is the Algorithm thread)
extern Cpl::Dm::MailboxServer           g_appMbox;

/// Expose the mailbox for the "UI" thread 
extern Cpl::Dm::MailboxServer           g_uiMbox;

/// Expose the low-level ADC driver/handle
extern DriverAIOHalSingleInput_T        g_thermistorHdl;

/*
** Thread Priorities
*/

/// Thread priority
#ifndef OPTION_AJAX_MAIN_THREAD_PRIORITY_UI
#define OPTION_AJAX_MAIN_THREAD_PRIORITY_UI             (CPL_SYSTEM_THREAD_PRIORITY_NORMAL +  (CPL_SYSTEM_THREAD_PRIORITY_RAISE) )
#endif

/// Thread priority
#ifndef OPTION_AJAX_MAIN_THREAD_PRIORITY_APPLICATION
#define OPTION_AJAX_MAIN_THREAD_PRIORITY_APPLICATION   (CPL_SYSTEM_THREAD_PRIORITY_NORMAL)
#endif

/// Thread priority
#ifndef OPTION_AJAX_MAIN_THREAD_PRIORITY_STORAGE
#define OPTION_AJAX_MAIN_THREAD_PRIORITY_STORAGE       (CPL_SYSTEM_THREAD_PRIORITY_NORMAL + ( 2* CPL_SYSTEM_THREAD_PRIORITY_LOWER) )
#endif

/// Thread priority
#ifndef OPTION_AJAX_MAIN_THREAD_PRIORITY_CONSOLE
#define OPTION_AJAX_MAIN_THREAD_PRIORITY_CONSOLE       (CPL_SYSTEM_THREAD_PRIORITY_NORMAL + ( 3* CPL_SYSTEM_THREAD_PRIORITY_LOWER) )
#endif


/*
** Magic values
*/
/// Minimum amount of time (in milliseconds) the splash screen is displayed
#ifndef OPTION_AJAX_MAIN_MIN_SPLASH_TIME_MS
#define OPTION_AJAX_MAIN_MIN_SPLASH_TIME_MS     (2*1000)
#endif

/// Number of entries for the in-RAM logging buffer
#ifndef OPTION_AJAX_MAIN_MAX_LOGGING_BUFFER_ENTRIES
#define OPTION_AJAX_MAIN_MAX_LOGGING_BUFFER_ENTRIES     20  // (20+1) * 159 = 3339 = 3.3K
#endif
};      // end namespaces
};
#endif  // end header latch