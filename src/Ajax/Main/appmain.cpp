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

#include "appmain.h"
#include "platform.h"
#include "application.h"
#include "mp/ModelPoints.h"
#include "Cpl/System/Shutdown.h"
#include "Cpl/System/Semaphore.h"
#include "Cpl/TShell/Cmd/Help.h"
#include "Cpl/TShell/Cmd/Bye.h"
#include "Cpl/TShell/Cmd/Trace.h"
#include "Cpl/TShell/Cmd/TPrint.h"
#include "Cpl/Dm/TShell/Dm.h"
#include "Cpl/TShell/Maker.h"
#include "Cpl/TShell/Stdio.h"

using namespace Ajax::Main;

static Cpl::System::Semaphore       waitForShutdown_;
static volatile int                 exitCode_;
static int runShutdownHandlers() noexcept;

Cpl::Container::Map<Cpl::TShell::Command>    Ajax::Main::g_cmdlist( "ignoreThisParameter_usedToCreateAUniqueConstructor" );
static Cpl::TShell::Maker                    cmdProcessor_( g_cmdlist );
static Cpl::TShell::Stdio                    shell_( cmdProcessor_, "TShell", OPTION_AJAX_MAIN_THREAD_PRIORITY_CONSOLE );
static Cpl::TShell::Cmd::Help	             helpCmd_( g_cmdlist );
static Cpl::TShell::Cmd::Bye	             byeCmd_( g_cmdlist );
static Cpl::TShell::Cmd::Trace	             traceCmd_( g_cmdlist );
static Cpl::TShell::Cmd::TPrint	             tprintCmd_( g_cmdlist );
static Cpl::Dm::TShell::Dm	                 dmCmd_( g_cmdlist, mp::g_modelDatabase );


/////////////////////////////
int Ajax::Main::runTheApplication( Cpl::Io::Input& infd, Cpl::Io::Output& outfd )
{
    /*
    ** STARTING UP...
    */
    platform_initialize0();
    appvariant_initialize0();

    platform_initializeModelPoints0();
    appvariant_initializeModelPoints0();

    platform_open0();
    appvariant_open0();

    // Start the shell
    shell_.launch( infd, outfd );


    /*
    ** RUNNING...
    */
    waitForShutdown_.wait(); // Wait for the Application to be shutdown


    /*
    ** SHUTTING DOWN...
    */
    appvariant_close0();
    platform_close0();

    // Run any/all register shutdown handlers (as registered by the Cpl::System::Shutdown interface) and then exit
    return runShutdownHandlers();
}


///////////////////////////////////////////////////////////////////////////////
int runShutdownHandlers() noexcept
{
    exitCode_ = Cpl::System::Shutdown::notifyShutdownHandlers_( exitCode_ );
    return platform_exit( exitCode_ );
}

int Cpl::System::Shutdown::success( void )
{
    exitCode_ = OPTION_CPL_SYSTEM_SHUTDOWN_SUCCESS_ERROR_CODE;
    waitForShutdown_.signal();
    return OPTION_CPL_SYSTEM_SHUTDOWN_SUCCESS_ERROR_CODE;
}

int Cpl::System::Shutdown::failure( int exit_code )
{
    exitCode_ = exit_code;
    waitForShutdown_.signal();
    return exit_code;
}
