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
#include "Ajax/Heating/Flc/Api.h"    
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Ajax/ScreenMgr/Api.h"
#include "Ajax/ScreenMgr/MockScreen.h"
#include "common.h"

using namespace Ajax::ScreenMgr;

// Mock display
namespace { // begin anonymous namespace

class MockDisplay : public DisplayApi
{
public:
    /// counters
    unsigned m_startCount;
    unsigned m_stopCount;
    unsigned m_udpateCount;
    unsigned m_turnOnCount;
    unsigned m_turnOffCount;
    bool     m_returnResult;

public:
    MockDisplay()
        : m_startCount( 0 )
        , m_stopCount( 0 )
        , m_udpateCount( 0 )
        , m_turnOnCount( 0 )
        , m_turnOffCount( 0 )
        , m_returnResult( true )
    {
    }

    bool start() noexcept{ m_startCount++; return m_returnResult; }
    void stop() noexcept{ m_stopCount++;  }
    bool update() noexcept{ m_udpateCount++; return m_returnResult;}
    bool turnOff() noexcept{ m_turnOnCount++; return m_returnResult;}
    bool turnOn() noexcept{ m_turnOffCount++; return m_returnResult;}
};

} // end anonymous namespace

// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase   modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );
static MpScreenApiPtr           mp_homeScrPtr( modelDb_, "homeScreen" );
static MpStaticScreenApiPtr     mp_errorScrPtr( modelDb_, "errorScreen" );
static MpStaticScreenApiPtr     mp_shutdownScrPtr( modelDb_, "shutdownScreen" );
static Cpl::Dm::Mp::Bool        mp_sleepRequst( modelDb_, "sleepReq" );
static Cpl::Dm::Mp::Uint32      mp_eventBuffferCount( modelDb_, "eventQueCount" );

// Memory for the Navigation stack
#define SIZE_NAV_STACK  3
static Api::NavigationElement   memoryNavStack_[SIZE_NAV_STACK];

// Create the event queue. Note: AjaxScreenMgrEvent_T maps to a: uint32_t
#define NUM_EVENTS      3
static AjaxScreenMgrEvent_T memoryForEvents_[NUM_EVENTS + 1];
static Cpl::Container::RingBufferMP< AjaxScreenMgrEvent_T> eventRingBuffer_( sizeof( memoryForEvents_ ) / sizeof( AjaxScreenMgrEvent_T ),
                                                                             memoryForEvents_,
                                                                             mp_eventBuffferCount );

// Mock display
static MockDisplay mockDisplay_;

// Don't let the Runnable object go out of scope before its thread has actually terminated!
//static Cpl::Dm::MailboxServer   uiMbox_;


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "api" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Cpl::Dm::MailboxServer uiMbox;
    Cpl::System::Thread*   t1 = Cpl::System::Thread::create( uiMbox, "UI" );

    memset( memoryNavStack_, 0, sizeof( memoryNavStack_ ) ); // Reset Navigation Elements

    mp_homeScrPtr.setInvalid();
    mp_errorScrPtr.setInvalid();
    mp_sleepRequst.setInvalid();
    mp_shutdownScrPtr.setInvalid();

    // Mock screens
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
             mp_sleepRequst,
             mp_shutdownScrPtr,
             mockDisplay_,
             memoryNavStack_, sizeof( memoryNavStack_ ) / sizeof( Api::NavigationElement ),
             eventRingBuffer_ );


    SECTION( "start-up" )
    {
        // Splash
        uut.open( &splashScreen );
        
        // Home
        REQUIRE( splashScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 0 );
        REQUIRE( homeScreen.m_refreshCount == 0 );
        REQUIRE( homeScreen.m_exitCount == 0 );
        REQUIRE( homeScreen.m_tickCount == 0 );
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 + 2*2* OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS ); // Allow time for the change notification to propagate + Ext
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
        // Splash
        uut.close();
        uut.open( &splashScreen );
        uut.open( &splashScreen );

        // Home
        REQUIRE( splashScreen.m_paintCount == 1 );
        REQUIRE( homeScreen.m_enterCount == 0 );
        REQUIRE( homeScreen.m_refreshCount == 0 );
        REQUIRE( homeScreen.m_exitCount == 0 );
        REQUIRE( homeScreen.m_tickCount == 0 );
        mp_homeScrPtr.write( &homeScreen );
        Cpl::System::Api::sleep( 200 + 2 * 2 * OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS ); // Allow time for the change notification to propagate + Ext
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


    // Shutdown threads
    uiMbox.pleaseStop();
    WAIT_FOR_THREAD_TO_STOP( t1 );
    Cpl::System::Thread::destroy( *t1 );
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}