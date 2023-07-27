/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2022  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "../../app.h"
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/TShell/Cmd/Threads.h"
#include "Driver/PicoDisplay/STM32/Api.h"
#include "Bsp/Initech/alpha1/console/Output.h"



/// Allocate memory for the console stream pointers
Cpl::Io::Input*  g_consoleInputFdPtr;
Cpl::Io::Output* g_consoleOutputFdPtr;

/// Generic thread TShell command
static Cpl::TShell::Cmd::Threads threadsCmd_( g_cmdlist );

/*-----------------------------------------------------------*/
int main( void )
{
    // Initialize CPL
    Cpl::System::Api::initialize();

    // Initialize the board
    Bsp_Api_initialize();
    Driver::PicoDisplay::STM32::initialize();

    // Set the stream of the console
    g_consoleInputFdPtr  = &g_bspConsoleStream;
    g_consoleOutputFdPtr = &g_bspConsoleStream;

    // Start the application
    runApplication();         // This method should never return
    return 0;
}
