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

#include "Driver/PicoDisplay/Api.h"     // FIXME: Must be first because of the pimoroni includes
#include "LogicalButtons.h"
#include "Cpl/System/Trace.h"

#define SECT_ "Ajax::Ui::LogicalButtons"

using namespace Ajax::Ui;

#define STATE_IDLE                  0
#define STATE_KEYPRESSED            1    
#define STATE_HELD_DOWN             2
#define STATE_WAITING_ALL_RELEASED  3

#define EDGE_NONE                   0
#define EDGE_PRESSED                -1
#define EDGE_RELEASED               1
#define EDGE_TIME                   2   // Pseudo edge

///////////////////////////
LogicalButtons::LogicalButtons( Cpl::Dm::MailboxServer& myMbox,
                                Cpl::Container::RingBufferMP<AjaxUiEvent_T>& eventRingBuffer )
    : CloseSync( myMbox )
    , Cpl::System::Timer( myMbox )
    , m_eventQueue( eventRingBuffer )
    , m_opened( false )
{
}

void LogicalButtons::request( OpenMsg& msg )
{
    // Do nothing if already started
    if ( m_opened )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("request(open) when already opened") );
        msg.returnToSender();
        return;
    }

    m_opened         = true;
    m_state          = STATE_IDLE;
    m_buttonAPressed = false;
    m_buttonBPressed = false;
    m_buttonXPressed = false;
    m_buttonYPressed = false;
    m_releaseEvent   = false;
    m_pressedEvent   = false;
    m_latchedKey     = AJAX_UI_EVENT_NO_EVENT;
    m_delayCounter   = OPTION_AJAX_UI_LOGICAL_BUTTON_HOLD_COUNT;

    Timer::start( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS );
    msg.returnToSender();
}

void LogicalButtons::request( CloseMsg& msg )
{
    // Do nothing if already stopped
    if ( !m_opened )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("request(closed) when already closed") );
        msg.returnToSender();
        return;
    }

    Timer::stop();
    msg.returnToSender();
}
void LogicalButtons::expired( void ) noexcept
{
    Driver::PicoDisplay::Api::buttonA().sample();
    Driver::PicoDisplay::Api::buttonB().sample();
    Driver::PicoDisplay::Api::buttonX().sample();
    Driver::PicoDisplay::Api::buttonY().sample();

    // Detect release events
    m_releaseEvent = false;
    m_pressedEvent = false;
    int edgeA      = detectEdges( Driver::PicoDisplay::Api::buttonA().isPressed(), m_buttonAPressed );
    int edgeB      = detectEdges( Driver::PicoDisplay::Api::buttonB().isPressed(), m_buttonBPressed );
    int edgeX      = detectEdges( Driver::PicoDisplay::Api::buttonX().isPressed(), m_buttonXPressed );
    int edgeY      = detectEdges( Driver::PicoDisplay::Api::buttonY().isPressed(), m_buttonYPressed );

    // Convert to button events
    m_isSingleKeyEvent = true;
    switch ( m_state )
    {
    case STATE_IDLE:
        if ( m_pressedEvent )
        {
            m_delayCounter = OPTION_AJAX_UI_LOGICAL_BUTTON_HOLD_COUNT;
            m_state        = STATE_KEYPRESSED;
        }
        break;

    case STATE_KEYPRESSED:
        if ( m_releaseEvent )
        {
            generateKeyEvent( getLogicalKey( edgeA, edgeB, edgeX, edgeY ) );
            m_state = m_isSingleKeyEvent ? STATE_IDLE : STATE_WAITING_ALL_RELEASED;
        }
        else if ( --m_delayCounter == 0 )
        {
            m_latchedKey   = getLogicalKey( EDGE_TIME, edgeB, edgeX, edgeY );
            generateKeyEvent( m_latchedKey );
            m_delayCounter = OPTION_AJAX_UI_LOGICAL_BUTTON_REPEAT_COUNT;
            m_state        = STATE_HELD_DOWN;
        }
        break;

    case STATE_WAITING_ALL_RELEASED:
        if ( !Driver::PicoDisplay::Api::buttonB().isPressed() &&
             !Driver::PicoDisplay::Api::buttonX().isPressed() &&
             !Driver::PicoDisplay::Api::buttonY().isPressed() )
        {
            m_state = STATE_IDLE;
        }
        break;

    case STATE_HELD_DOWN:
        if ( m_releaseEvent )
        {
            generateKeyEvent( m_latchedKey );
            m_state = m_isSingleKeyEvent ? STATE_IDLE : STATE_WAITING_ALL_RELEASED;
        }
        break;
    }

    // Restart the timer
    Timer::start( OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS );

}

/*---------------------------------------------------------------------------*/
int LogicalButtons::detectEdges( bool currentPressedState, bool& prevPressedSate ) noexcept
{
    int edge = EDGE_NONE;
    if ( currentPressedState && !prevPressedSate )
    {
        m_pressedEvent = true;
        edge           = EDGE_PRESSED;
    }
    else if ( !currentPressedState && prevPressedSate )
    {
        m_releaseEvent = true;
        edge           = EDGE_RELEASED;
    }
    prevPressedSate = currentPressedState;
    return edge;
}

AjaxUiEvent_T LogicalButtons::lookUpLogicalKey( bool pressedA, bool pressedB, bool pressedX, bool pressedY ) noexcept
{
    unsigned mask = (pressedA ? 1 : 0) | ((pressedB ? 1 : 0) << 1) | ((pressedX ? 1 : 0) << 2) | ((pressedY ? 1 : 0) << 3);
    switch ( mask )
    {
    case 0b0001: return AJAX_UI_EVENT_BUTTON_A;
    case 0b0010: return AJAX_UI_EVENT_BUTTON_B;
    case 0b0100: return AJAX_UI_EVENT_BUTTON_X;
    case 0b1000: return AJAX_UI_EVENT_BUTTON_Y;
    case 0b1010: m_isSingleKeyEvent = false;  return AJAX_UI_EVENT_BUTTON_BACK;
    default:
        return AJAX_UI_EVENT_NO_EVENT;
    }
}


AjaxUiEvent_T LogicalButtons::getLogicalKey( int edgeA, int edgeB, int edgeX, int edgeY ) noexcept
{
    if ( edgeA == EDGE_RELEASED )
    {
        return lookUpLogicalKey( true, Driver::PicoDisplay::Api::buttonB().isPressed(), Driver::PicoDisplay::Api::buttonX().isPressed(), Driver::PicoDisplay::Api::buttonY().isPressed() );
    }
    else if ( edgeB == EDGE_RELEASED )
    {
        return lookUpLogicalKey( Driver::PicoDisplay::Api::buttonA().isPressed(), true, Driver::PicoDisplay::Api::buttonX().isPressed(), Driver::PicoDisplay::Api::buttonY().isPressed() );
    }
    else if ( edgeX == EDGE_RELEASED )
    {
        return lookUpLogicalKey( Driver::PicoDisplay::Api::buttonA().isPressed(), Driver::PicoDisplay::Api::buttonB().isPressed(), true, Driver::PicoDisplay::Api::buttonY().isPressed() );
    }
    else if ( edgeY == EDGE_RELEASED )
    {
        return lookUpLogicalKey( Driver::PicoDisplay::Api::buttonA().isPressed(), Driver::PicoDisplay::Api::buttonB().isPressed(), Driver::PicoDisplay::Api::buttonX().isPressed(), true );
    }
    else if ( edgeA == EDGE_TIME )
    {
        return lookUpLogicalKey( Driver::PicoDisplay::Api::buttonA().isPressed(), Driver::PicoDisplay::Api::buttonB().isPressed(), Driver::PicoDisplay::Api::buttonX().isPressed(), Driver::PicoDisplay::Api::buttonY().isPressed() );
    }

    return AJAX_UI_EVENT_NO_EVENT;
}

void LogicalButtons::generateKeyEvent( AjaxUiEvent_T event ) noexcept
{
    if ( event != AJAX_UI_EVENT_NO_EVENT )
    {
        if ( !m_eventQueue.add( event ) )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ("Button Event - %s - was dropped because the event queue was full", event) );
        }
    }
}
