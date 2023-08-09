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
#include "Ajax/Ui/Splash/Screen.h"
#include "Ajax/Ui/Shutdown/Screen.h"
#include "Ajax/Ui/Home/Screen.h"
#include "Ajax/Ui/LogicalButtons.h"


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

// Graphics library: Use RGB332 mode (256 colours) on the Target to limit RAM usage canvas 
static Cpl::Dm::MailboxServer                   uiMboxServer_;
pimoroni::PicoGraphics_PenRGB332                Ajax::Main::g_graphics( OPTION_DRIVER_PICO_DISPLAY_LCD_WIDTH, OPTION_DRIVER_PICO_DISPLAY_LCD_HEIGHT, nullptr );
static Ajax::Ui::PicoDisplay                    uiDisplay_( g_graphics );
static Ajax::ScreenMgr::Api::NavigationElement  uiMemoryNavStack_[OPTION_AJAX_SCREEN_MGR_NAV_STACK_SIZE];
static AjaxScreenMgrEvent_T                     memoryForUiEvents_[OPTION_AJAX_SCREEN_MGR_EVENT_QUEUE_SIZE + 1];
static Cpl::Container::RingBufferMP<AjaxScreenMgrEvent_T> uiEventRingBuffer_( sizeof( memoryForUiEvents_ ) / sizeof( AjaxScreenMgrEvent_T ),
                                                                              memoryForUiEvents_,
                                                                              mp::uiEventQueueCount );
static Ajax::Ui::LogicalButtons        buttonEvents_( uiMboxServer_, uiEventRingBuffer_ );
static Ajax::ScreenMgr::Api            screenMgr_( uiMboxServer_,
                                                   mp::homeScrPtr,
                                                   mp::errorScrPtr,
                                                   mp::displaySleepTrigger,
                                                   mp::shutdownScrPtr,
                                                   uiDisplay_,
                                                   uiMemoryNavStack_,
                                                   sizeof( uiMemoryNavStack_ ) / sizeof( Ajax::ScreenMgr::Api::NavigationElement ),
                                                   uiEventRingBuffer_ );
Ajax::ScreenMgr::Navigation&      Ajax::Main::g_screenNav = screenMgr_;
static Ajax::Ui::Splash::Screen   splashScreen_( g_graphics );
static Ajax::Ui::Shutdown::Screen shutdownScreen_( g_graphics );


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

    // Create the UI Thread - and display the splash screen
    Cpl::System::Thread* uiThreadPtr = Cpl::System::Thread::create( uiMboxServer_, "UI", OPTION_AJAX_MAIN_THREAD_PRIORITY_UI );
    Driver::PicoDisplay::Api::rgbLED().setOff();
    Driver::PicoDisplay::Api::rgbLED().setBrightness( 64 );
    screenMgr_.open( &splashScreen_ );
    uint32_t uiStartTime = Cpl::System::ElapsedTime::milliseconds();

    platform_open0();
    
    appvariant_open0();

    buttonEvents_.open();

    // Start the shell
    shell_.launch( infd, outfd );

    // Splash screen must stay visible for at least N seconds per the requirements
    uint32_t now = Cpl::System::ElapsedTime::milliseconds();
    while ( !Cpl::System::ElapsedTime::expiredMilliseconds( uiStartTime, OPTION_AJAX_MAIN_MIN_SPLASH_TIME_MS, now ) )
    {
        Cpl::System::Api::sleep( 50 );
        now = Cpl::System::ElapsedTime::milliseconds();
    }
    appvariant_launchHomeScreen();

    /*
    ** RUNNING...
    */
    waitForShutdown_.wait(); // Wait for the Application to be shutdown
    mp::shutdownScrPtr.write( &shutdownScreen_ );

    /*
    ** SHUTTING DOWN...
    */
    // close() calls are the reverse order of the open() calls
    buttonEvents_.close();

    appvariant_close0();

    platform_close0();

    // DELETE-ME: For testing to see the shutdown screen.
    Cpl::System::Api::sleep( 1000 );

    screenMgr_.close();

    // Delete UI Thread
    uiMboxServer_.pleaseStop();
    Cpl::System::Api::sleep( 100 ); // Allow time for the thread so self terminate
    Cpl::System::Thread::destroy( *uiThreadPtr );

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
