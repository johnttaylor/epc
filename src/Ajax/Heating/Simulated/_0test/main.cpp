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

#include "colony_config.h"
#include "Cpl/TShell/Cmd/Tick.h"
#include "Cpl/TShell/Cmd/Threads.h"
#include "Cpl/TShell/Cmd/Help.h"
#include "Cpl/TShell/Cmd/Bye.h"
#include "Cpl/TShell/Cmd/Trace.h"
#include "Cpl/TShell/Cmd/TPrint.h"
#include "Cpl/Dm/TShell/Dm.h"
#include "Cpl/Dm/Mp/Double.h"
#include "Cpl/TShell/Maker.h"
#include "Cpl/Dm/MailboxServer.h"
#include "Cpl/System/Shutdown.h"
#include "Cpl/System/Thread.h"
#include "Cpl/TShell/Stdio.h"
#include "Cpl/System/EventLoop.h"
#include "mp/ModelPoints.h"
#include "Ajax/Heating/Simulated/Cmd.h"
#include "Ajax/Heating/Simulated/House.h"
#include "Ajax/Heating/Supervisor/Api.h"
#include "Ajax/Constants.h"
#include "Ajax/Main/UserRecord.h"
#include "Ajax/Main/MetricsRecord.h"
#include "Ajax/Main/PersonalityRecord.h"
#include "Ajax/Main/StorageMap_.h"
#include "Cpl/Logging/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Persistent/NVAdapter.h"
#include "Cpl/Persistent/MirroredChunk.h"
#include "Cpl/Persistent/CrcChunk.h"
#include "Cpl/Persistent/RecordServer.h"
#include "Cpl/Persistent/IndexedEntryRecord.h"
#include "Cpl/Persistent/IndexedEntryServer.h"
#include "Cpl/Persistent/IndexRecord.h"
#include "Cpl/Logging/TShell/Log.h"
#include "Ajax/Logging/Api.h"
#include "Driver/NV/File/Cpl/Api.h"
#include "Catch/helpers.h"
#include "Ajax/TShell/Provision.h"
#include "Ajax/TShell/State.h"
#include "Driver/Crypto/Orlp/Sha512.h"
#include "Cpl/System/SimTick.h"

/// 
extern int algorithmTest( Cpl::Io::Input& infd, Cpl::Io::Output& outfd );

/// Number of entries for the in-RAM logging buffer
#ifndef OPTION_MAX_LOGGING_BUFFER_ENTRIES
#define OPTION_MAX_LOGGING_BUFFER_ENTRIES     20  // (20+1) * 159 = 3339 = 3.3K
#endif

#ifndef DRIVER_NV_FILE_NAME     
#define DRIVER_NV_FILE_NAME         "eeprom.bin"
#endif
#ifndef DRIVER_NV_NUM_PAGES
#define DRIVER_NV_NUM_PAGES         512
#endif
#ifndef DRIVER_NV_BYTES_PER_PAGE
#define DRIVER_NV_BYTES_PER_PAGE    128
#endif

////////////////////////////////////////////////////////////////////////////////
namespace mp
{
Cpl::Dm::Mp::Bool   simEnable( mp::g_modelDatabase, "simEnable" );
Cpl::Dm::Mp::Double simOdt( mp::g_modelDatabase, "simOdt" );
}

static Cpl::System::Semaphore       waitForShutdown_;
static volatile int                 exitCode_;
static int runShutdownHandlers() noexcept;

static Driver::Crypto::Orlp::SHA512 sha512_;

#define LOG_BUFFER_SIZE (OPTION_MAX_LOGGING_BUFFER_ENTRIES+1)
static Cpl::Logging::EntryData_T                                memoryEntryBuffer_[LOG_BUFFER_SIZE];
static Cpl::Container::RingBufferMP<Cpl::Logging::EntryData_T>  logEntryBuffer_( LOG_BUFFER_SIZE, memoryEntryBuffer_, mp::loggingQueCount );


static Driver::NV::File::Cpl::Api           nvDriver_( DRIVER_NV_NUM_PAGES, DRIVER_NV_BYTES_PER_PAGE, DRIVER_NV_FILE_NAME );

static Cpl::Persistent::NVAdapter           fd1LogIndexRec_( nvDriver_, AJAX_MAIN_LOGINDEX_REGION_A_START_ADDRESS, AJAX_MAIN_LOGINDEX_REGION_LENGTH );
static Cpl::Persistent::NVAdapter           fd2LogIndexRec_( nvDriver_, AJAX_MAIN_LOGINDEX_REGION_B_START_ADDRESS, AJAX_MAIN_LOGINDEX_REGION_LENGTH );
static Cpl::Persistent::MirroredChunk       logIndexRecChunk_( fd1LogIndexRec_, fd2LogIndexRec_ );
static Cpl::Persistent::IndexRecord			logIndexRecord_( logIndexRecChunk_ );

static Cpl::Persistent::NVAdapter           fdLogEntriesRec_( nvDriver_, AJAX_MAIN_LOGENTRIES_REGION_START_ADDRESS, AJAX_MAIN_LOGENTRIES_REGION_DATA_LEN );
static Cpl::Persistent::CrcChunk            logEntriesChunk_( fdLogEntriesRec_ );
static Cpl::Persistent::IndexedEntryRecord	logEntryRecord_( logEntriesChunk_, Cpl::Logging::EntryData_T::entryLen, fdLogEntriesRec_, logIndexRecord_, mp::latestLoggingEntryKey );

static Cpl::Persistent::NVAdapter           fd1UserRec_( nvDriver_, AJAX_MAIN_USER_REGION_A_START_ADDRESS, AJAX_MAIN_USER_REGION_LENGTH );
static Cpl::Persistent::NVAdapter           fd2UserRec_( nvDriver_, AJAX_MAIN_USER_REGION_B_START_ADDRESS, AJAX_MAIN_USER_REGION_LENGTH );
static Cpl::Persistent::MirroredChunk       chunkUserRec_( fd1UserRec_, fd2UserRec_ );
static Ajax::Main::UserRecord               userRec_( chunkUserRec_ );

static Cpl::Persistent::NVAdapter           fd1MetricsRec_( nvDriver_, AJAX_MAIN_METRICS_REGION_A_START_ADDRESS, AJAX_MAIN_METRICS_REGION_LENGTH );
static Cpl::Persistent::NVAdapter           fd2MetricsRec_( nvDriver_, AJAX_MAIN_METRICS_REGION_B_START_ADDRESS, AJAX_MAIN_METRICS_REGION_LENGTH );
static Cpl::Persistent::MirroredChunk       chunkMetricsRec_( fd1MetricsRec_, fd2MetricsRec_ );
static Ajax::Main::MetricsRecord            metricsRec_( chunkMetricsRec_ );

static Cpl::Persistent::NVAdapter           fd1PersonalityRec_( nvDriver_, AJAX_MAIN_PERSONALITY_REGION_A_START_ADDRESS, AJAX_MAIN_PERSONALITY_REGION_LENGTH );
static Cpl::Persistent::NVAdapter           fd2PersonalityRec_( nvDriver_, AJAX_MAIN_PERSONALITY_REGION_B_START_ADDRESS, AJAX_MAIN_PERSONALITY_REGION_LENGTH );
static Cpl::Persistent::MirroredChunk       chunkPersonalityRec_( fd1PersonalityRec_, fd2PersonalityRec_ );
static Ajax::Main::PersonalityRecord        personalityRec_( chunkPersonalityRec_ );

static Cpl::Persistent::Record*                                         records_[4 + 1] ={ &logEntryRecord_, &userRec_, &metricsRec_, &personalityRec_, 0 };
static Cpl::Persistent::RecordServer                                    recordServer_( records_ );
static Cpl::Persistent::IndexedEntryServer<Cpl::Logging::EntryData_T>   logServer_( recordServer_, logEntryRecord_, logEntryBuffer_ );

static Cpl::Container::Map<Cpl::TShell::Command>  cmdlist_( "ignore_this_parameter-used to invoke the static constructor" );
static Cpl::TShell::Maker                         cmdProcessor_( cmdlist_ );
static Cpl::TShell::Stdio                         shell_( cmdProcessor_ );

static Cpl::TShell::Cmd::Tick	       tick_( cmdlist_ );
static Cpl::TShell::Cmd::Threads	   threads_( cmdlist_ );
static Cpl::TShell::Cmd::Help	       helpCmd_( cmdlist_ );
static Cpl::TShell::Cmd::Bye	       byeCmd_( cmdlist_ );
static Cpl::TShell::Cmd::Trace	       traceCmd_( cmdlist_ );
static Cpl::TShell::Cmd::TPrint	       tprintCmd_( cmdlist_ );
static Cpl::Dm::TShell::Dm	           dmCmd_( cmdlist_, mp::g_modelDatabase, "dm" );
static Cpl::Logging::TShell::Log       logCmd_( cmdlist_, recordServer_, logServer_ );
static Ajax::Heating::Simulated::Cmd   simCmd_( cmdlist_, mp::simEnable, mp::onBoardIdt, mp::simOdt );
static Ajax::TShell::Provision         provCmd_( cmdlist_, personalityRec_, recordServer_, sha512_ );
static Ajax::TShell::State             stateCmd_( cmdlist_ );
static Cpl::Dm::MailboxServer          algoMbox_;
static Ajax::Heating::Simulated::House heatingAlgo_( algoMbox_, mp::simEnable, mp::simOdt );

static AsyncOpenClose asyncAlgo_( heatingAlgo_ );


int algorithmTest( Cpl::Io::Input& infd, Cpl::Io::Output& outfd )
{
    /// Output log messages
    CPL_SYSTEM_TRACE_ENABLE();
    CPL_SYSTEM_TRACE_SET_INFO_LEVEL( Cpl::System::Trace::eBRIEF );
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "CRITICAL" );  // Enable trace for the log statements
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "WARNING" );
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "ALERT" );
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "EVENT" );
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "INFO" );
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "METRICS" );
    //CPL_SYSTEM_TRACE_ENABLE_SECTION( "*Ajax::Heating" );

    /*
    ** STARTING UP...
    */
    Cpl::Logging::initialize( logEntryBuffer_,
                              Ajax::Logging::CategoryId::WARNING,
                              (+Ajax::Logging::CategoryId::WARNING)._to_string(),
                              Ajax::Logging::WarningMsg::LOGGING_OVERFLOW,
                              (+Ajax::Logging::WarningMsg::LOGGING_OVERFLOW)._to_string() );

    // Default some model points
    mp::onBoardIdt.write( 7000 ); // Matches the 'starting IDT' for the house sim
    mp::simEnable.write( false );

    // Start the shell
    shell_.launch( infd, outfd );

    // Extra thread for the 'async-opener-closer' helper
    Cpl::System::Thread* t1 = Cpl::System::Thread::create( asyncAlgo_, "ASYNC-HELPER" );

    // Create thread to run the Algorithm
    Cpl::System::Thread* algoThreadPtr = Cpl::System::Thread::create( algoMbox_, "Algorithm", CPL_SYSTEM_THREAD_PRIORITY_NORMAL + CPL_SYSTEM_THREAD_PRIORITY_RAISE );

    // Create thread for persistent storage. Note: Run in REAL-TIME (not simulated time)
    Cpl::System::Thread* storageThreadPtr = Cpl::System::Thread::create( recordServer_, "NVRAM", CPL_SYSTEM_THREAD_PRIORITY_NORMAL, 0, 0, false );

    recordServer_.open();               // Start Persistent server as soon as possible
    metricsRec_.flush( recordServer_ ); // Immediate flush the metrics record so the new boot counter value is updated (see MetricsRecord for where the counter gets incremented)
    uint32_t bootCounter;
    mp::metricBootCounter.read( bootCounter );
    Ajax::Logging::logf( Ajax::Logging::MetricsMsg::POWER_ON, "Boot count = %lu", bootCounter );
    logServer_.open();

    // Start the algorithm
    asyncAlgo_.openSubject();   // NOTE: Because I am using simulate time - we can't use synchronous semantics
    simAdvanceTillOpened( asyncAlgo_ );


    /*
    ** RUNNING...
    */
    waitForShutdown_.wait(); // Wait for the Application to be shutdown


    /*
    ** SHUTTING DOWN...
    */
    Ajax::Logging::logf( Ajax::Logging::MetricsMsg::SHUTDOWN, "Boot count = %lu", bootCounter );

    asyncAlgo_.closeSubject( true );      // NOTE: Because I am using simulate time - we can't use synchronous semantics
    simAdvanceTillClosed( asyncAlgo_ );


    logServer_.close();
    recordServer_.close();

    // Delete UI Thread
    recordServer_.pleaseStop();
    algoMbox_.pleaseStop();
    Cpl::System::SimTick::advance( 10 );    // Advance sim time to ensure that my simulated threads have a chance to self terminate
    Cpl::System::Api::sleep( 100 );         // Allow time for the thread so self terminate
    Cpl::System::Thread::destroy( *algoThreadPtr );
    Cpl::System::Thread::destroy( *storageThreadPtr );
    Cpl::System::Thread::destroy( *t1 );
    Cpl::System::Api::sleep( 100 ); // Allow time for the thread so self terminate

    // Run any/all register shutdown handlers (as registered by the Cpl::System::Shutdown interface) and then exit
    return runShutdownHandlers();
}


///////////////////////////////////////////////////////////////////////////////
int runShutdownHandlers() noexcept
{
    exitCode_ = Cpl::System::Shutdown::notifyShutdownHandlers_( exitCode_ );
    exit( exitCode_ );
    return 0; // Keep the compiler happy
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


