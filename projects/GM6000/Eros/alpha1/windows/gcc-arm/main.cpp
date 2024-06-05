
#include "Ajax/Main/appmain.h"
#include "Bsp/Api.h"
#include <stdio.h>
#include <stdint.h>
#include "Cpl/System/Api.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/TShell/Cmd/FreeRTOS/Threads.h"
#include "Bsp/Initech/alpha1/console/Output.h"

static Cpl::TShell::Cmd::FreeRTOS::Threads cmdThreads_( Ajax::Main::g_cmdlist );

/// Thread for the Main Pattern
class ThreadMain : public Cpl::System::Runnable
{
public:
    ThreadMain() {};

protected:
    void appRun()
    {
        Ajax::Main::runTheApplication( g_bspConsoleStream, g_bspConsoleStream );
    }
};

/* NOTE: The way FreeRTOS start the 'first' task - corrupts the raw main
         stack -->this means we cannot allocate anything on the raw main
         stack and expect it to stay in scope for duration of the application
*/
static ThreadMain runnable_;

int main( void )
{
    // Initialize the board
    Bsp_Api_initialize();

    printf( "\n**** APPLICATION STARTED ****\n\n" );

    // Initialize CPL
    Cpl::System::Api::initialize();

    //// Create the main thread
    Cpl::System::Thread* t1 = Cpl::System::FreeRTOS::Thread::create( runnable_, "main", CPL_SYSTEM_THREAD_PRIORITY_NORMAL );
    if ( t1 == nullptr )
    {
        printf( "Failed to create the main thread\n" );
        while ( 1 );
    }

    // Start the scheduler
    Cpl::System::Api::enableScheduling(); // Enable scheduling NEVER return!
    return 0;
}


