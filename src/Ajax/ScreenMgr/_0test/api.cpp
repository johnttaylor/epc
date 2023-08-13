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

#include "Catch/catch.hpp"
#include "Catch/helpers.h"
#include "Ajax/Heating/Flc/Api.h"    
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Ajax/ScreenMgr/Api.h"
#include "Ajax/ScreenMgr/MockScreen.h"
#include "common.h"

using namespace Ajax::ScreenMgr;

#define SECT_   "_0test"


// Mock display
namespace { // begin anonymous namespace

class MockDisplay : public DisplayApi
{
public:
    /// counters
    int      m_startCount;
    int      m_stopCount;
    int      m_updateCount;
    int      m_turnOnCount;
    int      m_turnOffCount;
    bool     m_returnResult;

public:
    MockDisplay()
        : m_startCount( 0 )
        , m_stopCount( 0 )
        , m_updateCount( 0 )
        , m_turnOnCount( 0 )
        , m_turnOffCount( 0 )
        , m_returnResult( true )
    {
    }

    bool start() noexcept{ m_startCount++; return m_returnResult; }
    void stop() noexcept{ m_stopCount++; }
    bool update() noexcept{ m_updateCount++; return m_returnResult; }
    bool turnOff() noexcept{ m_turnOffCount++; return m_returnResult; }
    bool turnOn() noexcept{ m_turnOnCount++; return m_returnResult; }
};

} // end anonymous namespace

// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase   modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );
static MpScreenApiPtr           mp_homeScrPtr( modelDb_, "homeScreen" );
static MpStaticScreenApiPtr     mp_errorScrPtr( modelDb_, "errorScreen" );
static MpStaticScreenApiPtr     mp_shutdownScrPtr( modelDb_, "shutdownScreen" );
static Cpl::Dm::Mp::Bool        mp_sleepRequest( modelDb_, "sleepReq" );
static Cpl::Dm::Mp::Uint32      mp_eventBuffferCount( modelDb_, "eventQueCount" );

// Memory for the Navigation stack
#define SIZE_NAV_STACK  3
static Api::NavigationElement   memoryNavStack_[SIZE_NAV_STACK];

// Create the event queue. Note: AjaxScreenMgrEvent_T maps to a: uint32_t
#define NUM_EVENTS      4
static AjaxScreenMgrEvent_T memoryForEvents_[NUM_EVENTS + 1];
static Cpl::Container::RingBufferMP< AjaxScreenMgrEvent_T> eventRingBuffer_( sizeof( memoryForEvents_ ) / sizeof( AjaxScreenMgrEvent_T ),
                                                                             memoryForEvents_,
                                                                             mp_eventBuffferCount );

// Don't let the Runnable object go out of scope before its thread has actually terminated!
//static Cpl::Dm::MailboxServer   uiMbox_;


/// Handshake semaphore for the Test thread 
static Cpl::System::Semaphore semaUi_;

#define DISPLAY_CURRENT_SCREEN()    mockScrPtr = (MockScreen*) uut.getCurrentScreen(); CPL_SYSTEM_TRACE_MSG( SECT_, ("current screen=%s", mockScrPtr->m_name ))

#define PUSH_AND_TEST( screenToPush )   uiMbox.freeze();\
                                        uut.push( screenToPush ); \
                                        uiMbox.thaw(); \
                                        DISPLAY_CURRENT_SCREEN(); \
                                        REQUIRE( uut.getCurrentScreen() == &screenToPush )


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "api" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    DmMailbox              uiMbox( semaUi_ );
    Cpl::System::Thread*   t1 = Cpl::System::Thread::create( uiMbox, "UI" );
    MockScreen* mockScrPtr;

    memset( memoryNavStack_, 0, sizeof( memoryNavStack_ ) ); // Reset Navigation Elements

    mp_homeScrPtr.setInvalid();
    mp_errorScrPtr.setInvalid();
    mp_sleepRequest.setInvalid();
    mp_shutdownScrPtr.setInvalid();

    // Mock screens
    MockDisplay mockDisplay;
    MockScreen  homeScreen( "home" );
    MockScreen  appleScreen( "apple" );
    MockScreen  cherryScreen( "cherry" );
    MockScreen  orangeScreen( "orange" );
    MockScreen  plumScreen( "plum" );
    MockScreen  grapeScreen( "grape" );
    MockStaticScreen splashScreen( "splash" );
    MockStaticScreen shutdownScreen( "shutdown" );
    MockStaticScreen errorScreen( "error" );

    Api uut( uiMbox,
             mp_homeScrPtr,
             mp_errorScrPtr,
             mp_sleepRequest,
             mp_shutdownScrPtr,
             mockDisplay,
             memoryNavStack_, sizeof( memoryNavStack_ ) / sizeof( Api::NavigationElement ),
             eventRingBuffer_ );


    SECTION( "events" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: events") );
        uint32_t eventCount;

        // Wait for the UI/Test thread to actual start
        semaUi_.wait();

        // Splash
        uut.open( &splashScreen );

        // Generate an event - should not be consumed yet
        eventRingBuffer_.add( 42 );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate  (Should NOT be a change)
        REQUIRE( mp_eventBuffferCount.read( eventCount ) );
        REQUIRE( eventCount == 1 );
        REQUIRE( homeScreen.m_dispatchCount == 0 );

        // Set Home screen
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        mp_sleepRequest.write( true );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 0 );
        REQUIRE( homeScreen.m_tickCount > 2 );
        REQUIRE( homeScreen.m_wakeCount == 0 );
        REQUIRE( homeScreen.m_sleepCount == 0 );
        REQUIRE( homeScreen.m_dispatchCount == 1 );
        REQUIRE( homeScreen.m_lastEvent == 42 );
        REQUIRE( uut.getCurrentScreen() == &homeScreen );
        REQUIRE( mp_eventBuffferCount.read( eventCount ) );
        REQUIRE( eventCount == 0 );

        // New screen
        PUSH_AND_TEST( orangeScreen );
        eventRingBuffer_.add( 55 );
        eventRingBuffer_.add( 56 );
        eventRingBuffer_.add( 7 );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        REQUIRE( homeScreen.m_lastEvent == 42 );
        REQUIRE( homeScreen.m_dispatchCount == 1 );
        REQUIRE( orangeScreen.m_lastEvent == 7 );
        REQUIRE( orangeScreen.m_dispatchCount == 3 );

        // Close
        uut.close();
    }

    SECTION( "sleep" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: sleep") );

        // Wait for the UI/Test thread to actual start
        semaUi_.wait();

        // Splash
        uut.open( &splashScreen );

        // Sleep (can't when on splash/halt/shutdown)
        REQUIRE( mockDisplay.m_startCount == 1 );
        REQUIRE( mockDisplay.m_updateCount == 1 );
        REQUIRE( mockDisplay.m_turnOffCount == 0 );
        REQUIRE( mockDisplay.m_turnOnCount == 0 );
        mp_sleepRequest.write( true );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        REQUIRE( mockDisplay.m_startCount == 1 );
        REQUIRE( mockDisplay.m_updateCount == 1 );
        REQUIRE( mockDisplay.m_turnOffCount == 0 );
        REQUIRE( mockDisplay.m_turnOnCount == 0 );
        REQUIRE( uut.getCurrentScreen() == nullptr );
        mp_sleepRequest.setInvalid();

        // Sleep 
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 300 ); // Allow time for the change notification to propagate 
        mp_sleepRequest.write( true );
        Cpl::System::Api::sleep( 300 ); // Allow time for the change notification to propagate 
        REQUIRE( mockDisplay.m_startCount == 1 );
        REQUIRE( mockDisplay.m_updateCount == 2 );
        REQUIRE( mockDisplay.m_turnOffCount == 1 );
        REQUIRE( mockDisplay.m_turnOnCount == 0 );
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Wake
        REQUIRE( mockDisplay.m_startCount == 1 );
        REQUIRE( mockDisplay.m_updateCount == 2 );
        REQUIRE( mockDisplay.m_turnOffCount == 1 );
        REQUIRE( mockDisplay.m_turnOnCount == 0 );
        mp_sleepRequest.write( false );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        REQUIRE( mockDisplay.m_startCount == 1 );
        REQUIRE( mockDisplay.m_updateCount == 2 );
        REQUIRE( mockDisplay.m_turnOffCount == 1 );
        REQUIRE( mockDisplay.m_turnOnCount == 1 );
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Close
        uut.close();
    }

    SECTION( "halt#1" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: halt#1") );

        // Wait for the UI/Test thread to actual start
        semaUi_.wait();

        // Splash
        uut.open( &splashScreen );

        // Home
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Push
        PUSH_AND_TEST( orangeScreen );
        PUSH_AND_TEST( appleScreen );
        PUSH_AND_TEST( cherryScreen );

        // Signal halt
        REQUIRE( shutdownScreen.m_paintCount == 0 );
        mp_errorScrPtr.write( &errorScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        REQUIRE( uut.getCurrentScreen() == nullptr );
        REQUIRE( errorScreen.m_paintCount == 1 );

        REQUIRE( plumScreen.m_enterCount == 0 );
        uiMbox.freeze();
        uut.push( plumScreen );
        uiMbox.thaw();
        REQUIRE( errorScreen.m_paintCount == 1 );
        REQUIRE( plumScreen.m_enterCount == 0 );
        REQUIRE( uut.getCurrentScreen() == nullptr );

        // Close
        uut.close();
    }
    SECTION( "shutdown#2" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: shutdown#2") );

        // Wait for the UI/Test thread to actual start
        semaUi_.wait();

        // Splash
        uut.open( &splashScreen );

        // Home
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Push
        PUSH_AND_TEST( orangeScreen );
        PUSH_AND_TEST( appleScreen );
        PUSH_AND_TEST( cherryScreen );

        // Signal shutdown
        REQUIRE( shutdownScreen.m_paintCount == 0 );
        mp_shutdownScrPtr.write( &shutdownScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        REQUIRE( uut.getCurrentScreen() == nullptr );
        REQUIRE( shutdownScreen.m_paintCount == 1 );

        REQUIRE( plumScreen.m_enterCount == 0 );
        uiMbox.freeze();
        uut.push( plumScreen );
        uiMbox.thaw();
        REQUIRE( shutdownScreen.m_paintCount == 1 );
        REQUIRE( plumScreen.m_enterCount == 0 );
        REQUIRE( uut.getCurrentScreen() == nullptr );

        // Close
        uut.close();
    }

    SECTION( "shutdown#1" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: shutdown#1") );

        // Wait for the UI/Test thread to actual start
        semaUi_.wait();

        // Splash
        uut.open( &splashScreen );

        // Home
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Push
        PUSH_AND_TEST( orangeScreen );
        PUSH_AND_TEST( appleScreen );
        PUSH_AND_TEST( cherryScreen );

        // Close - without popping the nav stack
        uut.close();
    }

    SECTION( "popto" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: popto") );

        // Wait for the UI/Test thread to actual start
        semaUi_.wait();

        // Splash
        uut.open( &splashScreen );

        // Home
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Push
        PUSH_AND_TEST( orangeScreen );
        PUSH_AND_TEST( appleScreen );
        PUSH_AND_TEST( cherryScreen );

        // Pop-to
        uiMbox.freeze();
        uut.popTo( orangeScreen );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( uut.getCurrentScreen() == &orangeScreen );

        // Push
        PUSH_AND_TEST( appleScreen );
        PUSH_AND_TEST( cherryScreen );
        PUSH_AND_TEST( plumScreen );

        // Pop-to
        uiMbox.freeze();
        uut.popTo( cherryScreen );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( uut.getCurrentScreen() == &cherryScreen );

        // Push
        PUSH_AND_TEST( appleScreen );

        // Pop-to
        uiMbox.freeze();
        uut.popTo( homeScreen );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Push
        PUSH_AND_TEST( appleScreen );
        PUSH_AND_TEST( cherryScreen );
        PUSH_AND_TEST( plumScreen );

        // Pop-to
        uiMbox.freeze();
        uut.popTo( orangeScreen );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Shutdown
        REQUIRE( shutdownScreen.m_paintCount == 0 );
        mp_shutdownScrPtr.write( &shutdownScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate
        REQUIRE( shutdownScreen.m_paintCount == 1 );

        uut.close();
    }

    SECTION( "pop-push" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: pop-push") );

        // Wait for the UI/Test thread to actual start
        semaUi_.wait();

        // Splash
        uut.open( &splashScreen );

        // Home
        REQUIRE( splashScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 0 );
        REQUIRE( homeScreen.m_refreshCount == 0 );
        REQUIRE( homeScreen.m_exitCount == 0 );
        REQUIRE( homeScreen.m_tickCount == 0 );
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate 
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( splashScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 0 );
        REQUIRE( homeScreen.m_wakeCount == 0 );
        REQUIRE( homeScreen.m_sleepCount == 0 );

        // Child screen
        REQUIRE( orangeScreen.m_enterCount == 0 );
        REQUIRE( orangeScreen.m_exitCount == 0 );
        REQUIRE( orangeScreen.m_refreshCount == 0 );
        uiMbox.freeze();
        REQUIRE( uut.getCurrentScreen() == &homeScreen );
        uut.push( orangeScreen );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_enterCount == 1 );
        REQUIRE( orangeScreen.m_exitCount == 0 );
        REQUIRE( orangeScreen.m_refreshCount == 1 );

        // Grand Child screen
        REQUIRE( appleScreen.m_enterCount == 0 );
        REQUIRE( appleScreen.m_exitCount == 0 );
        REQUIRE( appleScreen.m_refreshCount == 0 );
        uiMbox.freeze();
        uut.push( appleScreen );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_enterCount == 1 );
        REQUIRE( orangeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_refreshCount == 1 );
        REQUIRE( appleScreen.m_enterCount == 1 );
        REQUIRE( appleScreen.m_exitCount == 0 );
        REQUIRE( appleScreen.m_refreshCount == 1 );

        // Great Grand Child screen
        REQUIRE( cherryScreen.m_enterCount == 0 );
        REQUIRE( cherryScreen.m_exitCount == 0 );
        REQUIRE( cherryScreen.m_refreshCount == 0 );
        uiMbox.freeze();
        uut.push( cherryScreen );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_enterCount == 1 );
        REQUIRE( orangeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_refreshCount == 1 );
        REQUIRE( appleScreen.m_enterCount == 1 );
        REQUIRE( appleScreen.m_exitCount == 1 );
        REQUIRE( appleScreen.m_refreshCount == 1 );
        REQUIRE( cherryScreen.m_enterCount == 1 );
        REQUIRE( cherryScreen.m_exitCount == 0 );
        REQUIRE( cherryScreen.m_refreshCount == 1 );

        // Great Great Grand Child screen
        REQUIRE( plumScreen.m_enterCount == 0 );
        REQUIRE( plumScreen.m_exitCount == 0 );
        REQUIRE( plumScreen.m_refreshCount == 0 );
        uiMbox.freeze();
        uut.push( plumScreen );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_enterCount == 1 );
        REQUIRE( orangeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_refreshCount == 1 );
        REQUIRE( appleScreen.m_enterCount == 1 );
        REQUIRE( appleScreen.m_exitCount == 1 );
        REQUIRE( appleScreen.m_refreshCount == 1 );
        REQUIRE( cherryScreen.m_enterCount == 1 );
        REQUIRE( cherryScreen.m_exitCount == 1 );
        REQUIRE( cherryScreen.m_refreshCount == 1 );
        REQUIRE( plumScreen.m_enterCount == 1 );
        REQUIRE( plumScreen.m_exitCount == 0 );
        REQUIRE( plumScreen.m_refreshCount == 1 );

        // Pop Plum
        uiMbox.freeze();
        uut.pop();
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_enterCount == 1 );
        REQUIRE( orangeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_refreshCount == 1 );
        REQUIRE( appleScreen.m_enterCount == 1 );
        REQUIRE( appleScreen.m_exitCount == 1 );
        REQUIRE( appleScreen.m_refreshCount == 1 );
        REQUIRE( cherryScreen.m_enterCount == 2 );
        REQUIRE( cherryScreen.m_exitCount == 1 );
        REQUIRE( cherryScreen.m_refreshCount == 2 );
        REQUIRE( plumScreen.m_enterCount == 1 );
        REQUIRE( plumScreen.m_exitCount == 1 );
        REQUIRE( plumScreen.m_refreshCount == 1 );

        // Pop Cherry
        uiMbox.freeze();
        uut.pop();
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_enterCount == 1 );
        REQUIRE( orangeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_refreshCount == 1 );
        REQUIRE( appleScreen.m_enterCount == 2 );
        REQUIRE( appleScreen.m_exitCount == 1 );
        REQUIRE( appleScreen.m_refreshCount == 2 );
        REQUIRE( cherryScreen.m_enterCount == 2 );
        REQUIRE( cherryScreen.m_exitCount == 2 );
        REQUIRE( cherryScreen.m_refreshCount == 2 );
        REQUIRE( plumScreen.m_enterCount == 1 );
        REQUIRE( plumScreen.m_exitCount == 1 );
        REQUIRE( plumScreen.m_refreshCount == 1 );

        // Pop Apple
        uiMbox.freeze();
        uut.pop();
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_enterCount == 2 );
        REQUIRE( orangeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_refreshCount == 2 );
        REQUIRE( appleScreen.m_enterCount == 2 );
        REQUIRE( appleScreen.m_exitCount == 2 );
        REQUIRE( appleScreen.m_refreshCount == 2 );
        REQUIRE( cherryScreen.m_enterCount == 2 );
        REQUIRE( cherryScreen.m_exitCount == 2 );
        REQUIRE( cherryScreen.m_refreshCount == 2 );
        REQUIRE( plumScreen.m_enterCount == 1 );
        REQUIRE( plumScreen.m_exitCount == 1 );
        REQUIRE( plumScreen.m_refreshCount == 1 );

        // Pop Orange
        uiMbox.freeze();
        uut.pop();
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( homeScreen.m_enterCount == 2 );
        REQUIRE( homeScreen.m_refreshCount == 2 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( orangeScreen.m_enterCount == 2 );
        REQUIRE( orangeScreen.m_exitCount == 2 );
        REQUIRE( orangeScreen.m_refreshCount == 2 );
        REQUIRE( appleScreen.m_enterCount == 2 );
        REQUIRE( appleScreen.m_exitCount == 2 );
        REQUIRE( appleScreen.m_refreshCount == 2 );
        REQUIRE( cherryScreen.m_enterCount == 2 );
        REQUIRE( cherryScreen.m_exitCount == 2 );
        REQUIRE( cherryScreen.m_refreshCount == 2 );
        REQUIRE( plumScreen.m_enterCount == 1 );
        REQUIRE( plumScreen.m_exitCount == 1 );
        REQUIRE( plumScreen.m_refreshCount == 1 );

        // Push
        PUSH_AND_TEST( appleScreen );
        PUSH_AND_TEST( cherryScreen );
        PUSH_AND_TEST( plumScreen );
        PUSH_AND_TEST( orangeScreen );

        // Pop 3
        uiMbox.freeze();
        uut.pop( 3 );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( mockScrPtr == mockScrPtr );    // NOTE: This is need to avoid warnings when trace is disabled (not really part of the 'test')
        REQUIRE( uut.getCurrentScreen() == mockScrPtr );

        // Push error
        PUSH_AND_TEST( plumScreen );
        REQUIRE( uut.getCurrentScreen() == &plumScreen );
        PUSH_AND_TEST( homeScreen );
        uiMbox.freeze();
        uut.pop( 3 );
        uiMbox.thaw();
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Push error
        PUSH_AND_TEST( appleScreen );
        PUSH_AND_TEST( cherryScreen );
        PUSH_AND_TEST( plumScreen );
        PUSH_AND_TEST( orangeScreen );
        uiMbox.freeze();
        uut.push( grapeScreen );
        uiMbox.thaw();
        DISPLAY_CURRENT_SCREEN();
        REQUIRE( uut.getCurrentScreen() == &homeScreen );

        // Shutdown
        REQUIRE( shutdownScreen.m_paintCount == 0 );
        mp_shutdownScrPtr.write( &shutdownScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate
        REQUIRE( shutdownScreen.m_paintCount == 1 );

        uut.close();
    }

    SECTION( "start-up" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: start-up") );

        // Wait for the UI/Test thread to actual start
        semaUi_.wait();

        // Splash
        uut.open( &splashScreen );

        // Home
        REQUIRE( splashScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 0 );
        REQUIRE( homeScreen.m_refreshCount == 0 );
        REQUIRE( homeScreen.m_exitCount == 0 );
        REQUIRE( homeScreen.m_tickCount == 0 );
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 + 5 * 2 * OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS ); // Allow time for the change notification to propagate + Extra for the timer
        REQUIRE( splashScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_tickCount > 2 );
        REQUIRE( homeScreen.m_exitCount == 0 );
        REQUIRE( homeScreen.m_wakeCount == 0 );
        REQUIRE( homeScreen.m_sleepCount == 0 );

        // Shutdown
        REQUIRE( shutdownScreen.m_paintCount == 0 );
        mp_shutdownScrPtr.write( &shutdownScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate
        REQUIRE( shutdownScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( homeScreen.m_tickCount > 2 );
        REQUIRE( homeScreen.m_wakeCount == 0 );
        REQUIRE( homeScreen.m_sleepCount == 0 );

        uut.close();
    }

    SECTION( "error-case1" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: error-case1") );

        // Wait for the UI/Test thread to actual start
        semaUi_.wait();

        // Splash
        uut.close();
        REQUIRE( mockDisplay.m_stopCount == 0 );
        REQUIRE( mockDisplay.m_startCount == 0 );
        uut.open( &splashScreen );
        REQUIRE( mockDisplay.m_startCount == 1 );
        REQUIRE( mockDisplay.m_updateCount == 1 );

        // Home
        REQUIRE( mockDisplay.m_updateCount == 1 );
        REQUIRE( splashScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 0 );
        REQUIRE( homeScreen.m_refreshCount == 0 );
        REQUIRE( homeScreen.m_exitCount == 0 );
        REQUIRE( homeScreen.m_tickCount == 0 );
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 + 2 * 2 * OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS ); // Allow time for the change notification to propagate + Ext
        REQUIRE( mockDisplay.m_updateCount == 2 );
        REQUIRE( splashScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_tickCount > 2 );
        REQUIRE( homeScreen.m_exitCount == 0 );
        REQUIRE( homeScreen.m_wakeCount == 0 );
        REQUIRE( homeScreen.m_sleepCount == 0 );

        // Should be a NOP
        uut.open( &splashScreen );
        REQUIRE( mockDisplay.m_startCount == 1 );
        REQUIRE( mockDisplay.m_updateCount == 2 );

        // Shutdown
        REQUIRE( shutdownScreen.m_paintCount == 0 );
        mp_shutdownScrPtr.write( &shutdownScreen );
        Cpl::System::Api::sleep( 200 ); // Allow time for the change notification to propagate
        REQUIRE( mockDisplay.m_updateCount == 3 );
        REQUIRE( shutdownScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 1 );
        REQUIRE( homeScreen.m_refreshCount == 1 );
        REQUIRE( homeScreen.m_exitCount == 1 );
        REQUIRE( homeScreen.m_tickCount > 2 );
        REQUIRE( homeScreen.m_wakeCount == 0 );
        REQUIRE( homeScreen.m_sleepCount == 0 );

        uut.close();
        REQUIRE( mockDisplay.m_stopCount == 1 );
        REQUIRE( mockDisplay.m_startCount == 1 );

    }


    // Shutdown threads
    uiMbox.pleaseStop();
    WAIT_FOR_THREAD_TO_STOP( t1 );
    Cpl::System::Thread::destroy( *t1 );
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}