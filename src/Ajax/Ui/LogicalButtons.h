#ifndef Ajax_Ui_LogicalButtons_h_
#define Ajax_Ui_LogicalButtons_h_
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


#include "colony_config.h"
#include "Ajax/Ui/Events.h"
#include "Cpl/Itc/CloseSync.h"
#include "Cpl/System/Timer.h"
#include "Cpl/Dm/MailboxServer.h"
#include "Cpl/Container/RingBufferMP.h"


/// Polling rate, in milliseconds
#ifndef OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS
#define OPTION_AJAX_UI_LOGICAL_BUTTON_POLLING_RATE_MS   10      // 100Hz
#endif

/** Specifies the initial hold time, in interval counts, before
    a pressed button generates a repeat event
 */
#ifndef OPTION_AJAX_UI_LOGICAL_BUTTON_HOLD_COUNT
#define OPTION_AJAX_UI_LOGICAL_BUTTON_HOLD_COUNT     50  // 00.5 sec @100Hz
#endif

 /** Specifies the repeat hold time, in interval counts, between
     a pressed button repeat events
  */
#ifndef OPTION_AJAX_UI_LOGICAL_BUTTON_REPEAT_COUNT
#define OPTION_AJAX_UI_LOGICAL_BUTTON_REPEAT_COUNT   10  // 0.1 sec @100Hz
#endif

  /// 
namespace Ajax {
/// 
namespace Ui {



/** This concrete class translates physical button actions into logical
    UI Button events.

    The class used the Driver::PicoDisplay interface for debouncing
    the physical buttons
 */
class LogicalButtons : public Cpl::Itc::CloseSync, public Cpl::System::Timer
{
public:
    /// Constructor
    LogicalButtons( Cpl::Dm::MailboxServer& myMbox,
                    Cpl::Container::RingBufferMP<AjaxUiEvent_T>& eventRingBuffer );

public:
    /// See Cpl::Itc::OpenSync
    void request( OpenMsg& msg );

    /// See Cpl::Itc::CloseSync
    void request( CloseMsg& msg );


protected:
    /// Timer expired callback
    void expired( void ) noexcept;

protected:
    /// Helper method
    AjaxUiEvent_T  getLogicalKey( int edgeA, int edgeB, int edgeX, int edgeY ) noexcept;

    /// Helper method
    AjaxUiEvent_T lookUpLogicalKey( bool pressedA, bool pressedB, bool pressedX, bool pressedY ) noexcept;

    /// Helper method
    void generateKeyEvent( AjaxUiEvent_T event ) noexcept;

    /// Helper method
    int detectEdges( bool currentPressedState, bool& prevPressedSate ) noexcept;


protected:
    /// The event queue
    Cpl::Container::RingBufferMP<AjaxUiEvent_T>& m_eventQueue;

    /// Internal button state
    int         m_state;

    /// Track time by tick counts
    unsigned    m_delayCounter;

    /// The current detected key event
    AjaxUiEvent_T m_latchedKey;

    /// Divider to 10Hz 
    unsigned    m_10HzDividerCounter;

    /// Track when the button has been pressed
    bool        m_buttonAPressed;
    
    /// Track when the button has been pressed
    bool        m_buttonBPressed;
    
    /// Track when the button has been pressed
    bool        m_buttonXPressed;
    
    /// Track when the button has been pressed
    bool        m_buttonYPressed;
    
    /// Track the release action
    bool        m_releaseEvent;
    
    /// Track the pressed action
    bool        m_pressedEvent;

    /// Track single vs multi key events
    bool        m_isSingleKeyEvent;

    /// Track my open/close state
    bool        m_opened;
};

}      // end namespaces
}
#endif  // end header latch

