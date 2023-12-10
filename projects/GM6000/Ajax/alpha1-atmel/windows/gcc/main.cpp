
#include "Ajax/Main/appmain.h"
#include "Bsp/Api.h"
#include <stdio.h>
#include <stdint.h>
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/TShell/Cmd/FreeRTOS/Threads.h"
#include "Bsp/Initech/alpha1-atmel/Api.h"

extern Cpl::Io::InputOutput& Bsp_Serial( void );
static Cpl::TShell::Cmd::FreeRTOS::Threads cmdThreads_( Ajax::Main::g_cmdlist );


// the setup function runs once when you press reset or power the board
// NOTE: FreeRTOS is RUNNING at this point
void setup()
{
    // Initialize the board (for use with CPL)
    Bsp_Api_initialize();
    Bsp_beginArduinoSerialObject( 115200, SERIAL_8N1 );

    printf( "\n**** APPLICATION STARTED ****\n\n" );

    // Initialize CPL
    Cpl::System::Api::initialize();

    // Make the current/main thread a CPL Thread
    Cpl::System::FreeRTOS::Thread::makeNativeMainThreadACplThread();
}


// This function is called repeatedly forever
void loop()
{
    Ajax::Main::runTheApplication( Bsp_Serial(), Bsp_Serial() ); // This method never returns
}


