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

#include "Driver/PicoDisplay/Api.h"     // FIXME: Must be first because of the pimoroni includes
#include "Catch/catch.hpp"
#include "Catch/helpers.h"
#include "Ajax/Heating/Flc/Api.h"    
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Ajax/ScreenMgr/Api.h"
#include "Ajax/Ui/LogicalButtons.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/SimTick.h"

using namespace Ajax::Ui;

#define SECT_   "_0test"


/// Macro to help shutdown the observer thread
#define WAIT_FOR_THREAD_TO_STOP(t)      for ( int i=0; i < 200; i++ ) \
                                        { \
                                            Cpl::System::SimTick::advance( 50 ); \
                                            Cpl::System::Api::sleepInRealTime( 50 ); \
                                            if ( t->isRunning() == false ) \
                                            { \
                                                break; \
                                            } \
                                        }



// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase   modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );
static Cpl::Dm::Mp::Uint32      mp_eventBuffferCount( modelDb_, "eventQueCount" );

// Create the event queue. Note: AjaxScreenMgrEvent_T maps to a: AjaxUiEvents_T
#define NUM_EVENTS      4
static AjaxUiEvent_T    memoryForEvents_[NUM_EVENTS + 1];
static Cpl::Container::RingBufferMP<AjaxUiEvent_T> eventRingBuffer_( sizeof( memoryForEvents_ ) / sizeof( AjaxUiEvent_T ),
                                                                     memoryForEvents_,
                                                                     mp_eventBuffferCount );
static Cpl::Dm::MailboxServer   uiMbox_;

// Mocked Button Driver
#define BUTTON_IDX_A        0
#define BUTTON_IDX_B        1
#define BUTTON_IDX_X        2
#define BUTTON_IDX_Y        3
static bool rawButtonStates_[4]; // Index 0=A,1=B,2=X
bool mocked_getRawPressState( Driver_Button_Hal_T hdl )
{
    return rawButtonStates_[hdl];
}

// MOCKED PicoDisplay driver
static Driver::Button::PolledDebounced buttonA_( BUTTON_IDX_A, 1 ); // Value of '1' is 'two consecutive samples"
static Driver::Button::PolledDebounced buttonB_( BUTTON_IDX_B, 1 );
static Driver::Button::PolledDebounced buttonX_( BUTTON_IDX_X, 1 );
static Driver::Button::PolledDebounced buttonY_( BUTTON_IDX_Y, 1 );
Driver::Button::PolledDebounced& Driver::PicoDisplay::Api::buttonA() noexcept
{
    return buttonA_;
}
Driver::Button::PolledDebounced& Driver::PicoDisplay::Api::buttonB() noexcept
{
    return buttonB_;
}
Driver::Button::PolledDebounced& Driver::PicoDisplay::Api::buttonX() noexcept
{
    return buttonX_;
}
Driver::Button::PolledDebounced& Driver::PicoDisplay::Api::buttonY() noexcept
{
    return buttonY_;
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "LogicalButtons" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Cpl::Dm::MailboxServer  uiMbox;
    Cpl::System::Thread*    t1 = Cpl::System::Thread::create( uiMbox, "UITEST" );

    mp_eventBuffferCount.setInvalid();
    LogicalButtons uut( uiMbox, eventRingBuffer_ );
    uint32_t      eventCount = 0;
    AjaxUiEvent_T event      = AJAX_UI_EVENT_NO_EVENT;
    SECTION( "events" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: events") );

        // Start
        uut.open();
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS ); // Expire the first poll tick

        // Press A button
        REQUIRE( mp_eventBuffferCount.isNotValid() );
        rawButtonStates_[BUTTON_IDX_A] = true;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS*2 );
        rawButtonStates_[BUTTON_IDX_A] = false;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 2 );
        REQUIRE( mp_eventBuffferCount.read( eventCount ) );
        REQUIRE( eventCount == 1 );
        REQUIRE( eventRingBuffer_.remove( event ) );
        REQUIRE( event == AJAX_UI_EVENT_BUTTON_A );

        // Press B, X, Y buttons
        rawButtonStates_[BUTTON_IDX_B] = true;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 2 );
        rawButtonStates_[BUTTON_IDX_B] = false;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 2 );
        rawButtonStates_[BUTTON_IDX_X] = true;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 2 );
        rawButtonStates_[BUTTON_IDX_X] = false;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 2 );
        rawButtonStates_[BUTTON_IDX_Y] = true;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 2 );
        rawButtonStates_[BUTTON_IDX_Y] = false;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 2 );
        REQUIRE( mp_eventBuffferCount.read( eventCount ) );
        REQUIRE( eventCount == 3 );
        REQUIRE( eventRingBuffer_.remove( event ) );
        REQUIRE( event == AJAX_UI_EVENT_BUTTON_B );
        REQUIRE( eventRingBuffer_.remove( event ) );
        REQUIRE( event == AJAX_UI_EVENT_BUTTON_X );
        REQUIRE( eventRingBuffer_.remove( event ) );
        REQUIRE( event == AJAX_UI_EVENT_BUTTON_Y );

        // Press B+Y button
        rawButtonStates_[BUTTON_IDX_B] = true;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 1 );
        rawButtonStates_[BUTTON_IDX_Y] = true;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 2 );
        rawButtonStates_[BUTTON_IDX_B] = false;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 1 );
        rawButtonStates_[BUTTON_IDX_Y] = false;
        Cpl::System::SimTick::advance( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS * 1 );
        REQUIRE( mp_eventBuffferCount.read( eventCount ) );
        REQUIRE( eventCount == 1 );
        REQUIRE( eventRingBuffer_.remove( event ) );
        REQUIRE( event == AJAX_UI_EVENT_BUTTON_BACK );

        // Stop
        // NOTE: close() is NOT called on purpose because it result in deadlock when using simulated.  
        //       Since the close() method (currently) does not contain any business logic AND the
        //       fact the UUT is recreated every pass of the test loop - the loss of test coverage
        //       is minimal.
        //uut.close();
    }

    // Shutdown threads
    uiMbox.pleaseStop();
    WAIT_FOR_THREAD_TO_STOP( t1 );
    Cpl::System::Thread::destroy( *t1 );
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}