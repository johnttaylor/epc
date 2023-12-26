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
#define SECT_   "INFO"

#include "appmain.h"
#include "platform.h"
#include "application.h"
#include "UserRecord.h"
#include "MetricsRecord.h"
#include "PersonalityRecord.h"
#include "StorageMap_.h"
#include "mp/ModelPoints.h"
#include "Cpl/System/Shutdown.h"
#include "Cpl/System/Semaphore.h"
#include "Cpl/TShell/Cmd/Help.h"
#include "Cpl/TShell/Cmd/Bye.h"
#include "Cpl/TShell/Cmd/Trace.h"
#include "Cpl/TShell/Cmd/TPrint.h"
#include "Cpl/Dm/TShell/Dm.h"
#include "Cpl/Logging/TShell/Log.h"
#include "Cpl/TShell/Maker.h"
#include "Cpl/TShell/Stdio.h"
#include "Ajax/Ui/Splash/Screen.h"
#include "Ajax/Ui/Shutdown/Screen.h"
#include "Ajax/Ui/Home/Screen.h"
#include "Ajax/Ui/Error/Screen.h"
#include "Ajax/Ui/LogicalButtons.h"
#include "Ajax/Logging/Api.h"
#include "Ajax/TShell/Provision.h"
#include "Ajax/TShell/Ui.h"
#include "Cpl/Logging/Api.h"
#include "Cpl/Persistent/NVAdapter.h"
#include "Cpl/Persistent/MirroredChunk.h"
#include "Cpl/Persistent/CrcChunk.h"
#include "Cpl/Persistent/RecordServer.h"
#include "Cpl/Persistent/IndexedEntryRecord.h"
#include "Cpl/Persistent/IndexedEntryServer.h"
#include "Cpl/Persistent/IndexRecord.h"
#include "Cpl/System/Trace.h"
#include "Driver/Crypto/Api.h"
#include "Driver/Crypto/TShell/Random.h"
#include "Driver/Crypto/Orlp/Sha512.h"
#include "Driver/NV/_tshell/Cmd.h"
#include "screens.h"

using namespace Ajax::Main;

static Cpl::System::Semaphore       waitForShutdown_;
static volatile int                 exitCode_;
static int runShutdownHandlers() noexcept;

Cpl::Dm::MailboxServer              Ajax::Main::g_appMbox;

static Driver::Crypto::Orlp::SHA512 sha512_;
Driver::Crypto::Hash*               Ajax::Main::g_sha512Ptr = &sha512_;


// Graphics library: Use RGB332 mode (256 colours) on the Target to limit RAM usage canvas 
Cpl::Dm::MailboxServer                          Ajax::Main::g_uiMbox;
pimoroni::PicoGraphics_PenRGB332                Ajax::Main::g_graphics( OPTION_DRIVER_PICO_DISPLAY_LCD_WIDTH, OPTION_DRIVER_PICO_DISPLAY_LCD_HEIGHT, nullptr );
static Ajax::Ui::PicoDisplay                    uiDisplay_( g_graphics );
static Ajax::ScreenMgr::Api::NavigationElement  uiMemoryNavStack_[OPTION_AJAX_SCREEN_MGR_NAV_STACK_SIZE];
static AjaxScreenMgrEvent_T                     memoryForUiEvents_[OPTION_AJAX_SCREEN_MGR_EVENT_QUEUE_SIZE + 1];
static Cpl::Container::RingBufferMP<AjaxScreenMgrEvent_T> uiEventRingBuffer_( sizeof( memoryForUiEvents_ ) / sizeof( AjaxScreenMgrEvent_T ),
                                                                              memoryForUiEvents_,
                                                                              mp::uiEventQueueCount );
static Ajax::Ui::LogicalButtons        buttonEvents_( Ajax::Main::g_uiMbox, uiEventRingBuffer_ );
static Ajax::ScreenMgr::Api            screenMgr_( Ajax::Main::g_uiMbox,
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


#define LOG_BUFFER_SIZE (OPTION_AJAX_MAIN_MAX_LOGGING_BUFFER_ENTRIES+1)
static Cpl::Logging::EntryData_T                                memoryEntryBuffer_[LOG_BUFFER_SIZE];
static Cpl::Container::RingBufferMP<Cpl::Logging::EntryData_T>  logEntryBuffer_( LOG_BUFFER_SIZE, memoryEntryBuffer_, mp::loggingQueCount );

static Cpl::Persistent::NVAdapter           fd1LogIndexRec_( g_nvramDriver, AJAX_MAIN_LOGINDEX_REGION_A_START_ADDRESS, AJAX_MAIN_LOGINDEX_REGION_LENGTH );
static Cpl::Persistent::NVAdapter           fd2LogIndexRec_( g_nvramDriver, AJAX_MAIN_LOGINDEX_REGION_B_START_ADDRESS, AJAX_MAIN_LOGINDEX_REGION_LENGTH );
static Cpl::Persistent::MirroredChunk       logIndexRecChunk_( fd1LogIndexRec_, fd2LogIndexRec_ );
static Cpl::Persistent::IndexRecord			logIndexRecord_( logIndexRecChunk_ );

static Cpl::Persistent::NVAdapter           fdLogEntriesRec_( g_nvramDriver, AJAX_MAIN_LOGENTRIES_REGION_START_ADDRESS, AJAX_MAIN_LOGENTRIES_REGION_DATA_LEN );
static Cpl::Persistent::CrcChunk            logEntriesChunk_( fdLogEntriesRec_ );
static Cpl::Persistent::IndexedEntryRecord	logEntryRecord_( logEntriesChunk_, Cpl::Logging::EntryData_T::entryLen, fdLogEntriesRec_, logIndexRecord_, mp::latestLoggingEntryKey );

static Cpl::Persistent::NVAdapter           fd1UserRec_( g_nvramDriver, AJAX_MAIN_USER_REGION_A_START_ADDRESS, AJAX_MAIN_USER_REGION_LENGTH );
static Cpl::Persistent::NVAdapter           fd2UserRec_( g_nvramDriver, AJAX_MAIN_USER_REGION_B_START_ADDRESS, AJAX_MAIN_USER_REGION_LENGTH );
static Cpl::Persistent::MirroredChunk       chunkUserRec_( fd1UserRec_, fd2UserRec_ );
static Ajax::Main::UserRecord               userRec_( chunkUserRec_ );

static Cpl::Persistent::NVAdapter           fd1MetricsRec_( g_nvramDriver, AJAX_MAIN_METRICS_REGION_A_START_ADDRESS, AJAX_MAIN_METRICS_REGION_LENGTH );
static Cpl::Persistent::NVAdapter           fd2MetricsRec_( g_nvramDriver, AJAX_MAIN_METRICS_REGION_B_START_ADDRESS, AJAX_MAIN_METRICS_REGION_LENGTH );
static Cpl::Persistent::MirroredChunk       chunkMetricsRec_( fd1MetricsRec_, fd2MetricsRec_ );
static Ajax::Main::MetricsRecord            metricsRec_( chunkMetricsRec_ );

static Cpl::Persistent::NVAdapter           fd1PersonalityRec_( g_nvramDriver, AJAX_MAIN_PERSONALITY_REGION_A_START_ADDRESS, AJAX_MAIN_PERSONALITY_REGION_LENGTH );
static Cpl::Persistent::NVAdapter           fd2PersonalityRec_( g_nvramDriver, AJAX_MAIN_PERSONALITY_REGION_B_START_ADDRESS, AJAX_MAIN_PERSONALITY_REGION_LENGTH );
static Cpl::Persistent::MirroredChunk       chunkPersonalityRec_( fd1PersonalityRec_, fd2PersonalityRec_ );
static Ajax::Main::PersonalityRecord        personalityRec_( chunkPersonalityRec_ );

static Cpl::Persistent::Record*                                         records_[4 + 1] ={ &logEntryRecord_, &userRec_, &metricsRec_, &personalityRec_, 0 };
static Cpl::Persistent::RecordServer                                    recordServer_( records_ );
static Cpl::Persistent::IndexedEntryServer<Cpl::Logging::EntryData_T>   logServer_( recordServer_, logEntryRecord_, logEntryBuffer_ );

Cpl::Container::Map<Cpl::TShell::Command>    Ajax::Main::g_cmdlist( "ignoreThisParameter_usedToCreateAUniqueConstructor" );
static Cpl::TShell::Maker                    cmdProcessor_( g_cmdlist );
static Cpl::TShell::Stdio                    shell_( cmdProcessor_, "TShell", OPTION_AJAX_MAIN_THREAD_PRIORITY_CONSOLE );
static Cpl::TShell::Cmd::Help	             helpCmd_( g_cmdlist );
static Cpl::TShell::Cmd::Bye	             byeCmd_( g_cmdlist );
static Cpl::TShell::Cmd::Trace	             traceCmd_( g_cmdlist );
static Cpl::TShell::Cmd::TPrint	             tprintCmd_( g_cmdlist );
static Cpl::Dm::TShell::Dm	                 dmCmd_( g_cmdlist, mp::g_modelDatabase );
static Cpl::Logging::TShell::Log             logCmd_( g_cmdlist, recordServer_, logServer_ );
static Driver::Crypto::TShell::Random        randomCmd_( g_cmdlist );
static Ajax::TShell::Ui                      uiEventCmd_( g_cmdlist, uiEventRingBuffer_ );

// Only include the Provision command in Ajax Debug build AND ALL Eros builds
#if defined(DEBUG_BUILD) || defined(I_AM_EROS)
static Ajax::TShell::Provision               provCmd_( g_cmdlist, personalityRec_, recordServer_, *g_sha512Ptr );
static Driver::NV::Cmd                       eepromCmd_( g_cmdlist, g_nvramDriver );
#endif


static void displayRecordSizes();

/////////////////////////////
int Ajax::Main::runTheApplication( Cpl::Io::Input& infd, Cpl::Io::Output& outfd )
{
    /// DELETE-ME.  For debugging during development
    CPL_SYSTEM_TRACE_ENABLE();
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "CRITICAL" );  // Enable trace for the log statements
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "WARNING" );
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "ALERT" );
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "EVENT" );
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "INFO" );
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "METRICS" );

    /*
    ** STARTING UP...
    */
    Cpl::Logging::initialize( logEntryBuffer_,
                              Ajax::Logging::CategoryId::WARNING,
                              (+Ajax::Logging::CategoryId::WARNING)._to_string(),
                              Ajax::Logging::WarningMsg::LOGGING_OVERFLOW,
                              (+Ajax::Logging::WarningMsg::LOGGING_OVERFLOW)._to_string() );
    platform_initialize0();
    appvariant_initialize0();
    Driver::Crypto::initialize();

    // Run Power On Self Test(s)
    if ( !platform_runPOST() )
    {
        mp::postFailedAlert.raiseAlert();
    }

    // Turn off the RGB LED (and set the default brightness)
    Driver::PicoDisplay::Api::setLCDBrightness( 0 );
    Driver::PicoDisplay::Api::rgbLED().setOff();
    Driver::PicoDisplay::Api::rgbLED().setBrightness( 65 );

    platform_initializeModelPoints0();
    appvariant_initializeModelPoints0();

    // Create Threads
    Cpl::System::Thread* appThreadPtr     = Cpl::System::Thread::create( g_appMbox, "APP", OPTION_AJAX_MAIN_THREAD_PRIORITY_APPLICATION );
    Cpl::System::Thread* uiThreadPtr      = Cpl::System::Thread::create( g_uiMbox, "UI", OPTION_AJAX_MAIN_THREAD_PRIORITY_UI );
    Cpl::System::Thread* storageThreadPtr = Cpl::System::Thread::create( recordServer_, "NVRAM", OPTION_AJAX_MAIN_THREAD_PRIORITY_STORAGE );

    recordServer_.open();               // Start the Persistent server as soon as possible AND before the splash screen
    metricsRec_.flush( recordServer_ ); // Immediate flush the metrics record so the new boot counter value is updated (see MetricsRecord for where the counter gets incremented)

    // Display the splash screen
    screenMgr_.open( &splashScreen_ );
    uint32_t uiStartTime = Cpl::System::ElapsedTime::milliseconds();


    // Complete "starting" the application
    platform_open0();

    uint32_t bootCounter;
    mp::metricBootCounter.read( bootCounter );
    Ajax::Logging::logf( Ajax::Logging::MetricsMsg::POWER_ON, "Boot count = %lu", bootCounter );
    logServer_.open();

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
    displayRecordSizes();
    waitForShutdown_.wait(); // Wait for the Application to be shutdown
    mp::shutdownScrPtr.write( &shutdownScreen_ );

    /*
    ** SHUTTING DOWN...
    */
    Ajax::Logging::logf( Ajax::Logging::MetricsMsg::SHUTDOWN, "Boot count = %lu", bootCounter );

    // close() calls are the reverse order of the open() calls
    buttonEvents_.close();

    appvariant_close0();

    logServer_.close();

    platform_close0();

    // DELETE-ME: For testing to see the shutdown screen.
    Cpl::System::Api::sleep( 1000 );

    recordServer_.close();
    screenMgr_.close();

    Driver::Crypto::shutdown();

    // Delete UI Thread
    recordServer_.pleaseStop();
    g_uiMbox.pleaseStop();
    g_appMbox.pleaseStop();
    Cpl::System::Api::sleep( 100 ); // Allow time for the thread so self terminate
    Cpl::System::Thread::destroy( *uiThreadPtr );
    Cpl::System::Thread::destroy( *storageThreadPtr );
    Cpl::System::Thread::destroy( *appThreadPtr );

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


void displayRecordSizes()
{
    CPL_SYSTEM_TRACE_MSG( SECT_, ("User Record size:        %lu (%lu) ", userRec_.getRecordSize(), userRec_.getRecordSize() + chunkUserRec_.getMetadataLength()) );
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Metrics Record size:     %lu (%lu) ", metricsRec_.getRecordSize(), metricsRec_.getRecordSize() + chunkMetricsRec_.getMetadataLength()) );
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Personality Record size: %lu (%lu) ", personalityRec_.getRecordSize(), personalityRec_.getRecordSize() + chunkPersonalityRec_.getMetadataLength()) );
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Log Entry size:          %lu", logEntryRecord_.getMetadataLength() + Cpl::Logging::EntryData_T::entryLen + logEntriesChunk_.getMetadataLength()) );
}