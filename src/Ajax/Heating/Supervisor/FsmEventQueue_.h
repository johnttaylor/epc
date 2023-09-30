#ifndef Ajax_Heating_Supervisor_FsmEventQueue_x_h_
#define Ajax_Heating_Supervisor_FsmEventQueue_x_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2020  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */



/* This file is auto-generated DO NOT MANUALLY EDIT this file! */


#include "Ajax/Heating/Supervisor/Fsm_.h"
#include "Cpl/Container/RingBuffer.h"


///
namespace Ajax { namespace Heating { namespace Supervisor { 


/// Event Queue for FSM events.
class FsmEventQueue_: public Fsm, public Cpl::Container::RingBuffer<FSM_EVENT_T>
{
public:
    /// Define callback function that is called when an event has completed
    typedef void ( *EventCompletedCbFunc_T )( FSM_EVENT_T proceessedMsg );

protected:
    /// Optional Callback function for event-completed (typically used for unit testing purposes)
    EventCompletedCbFunc_T  m_eventCompletedCallback;

    /// Memory for Event queue
    FSM_EVENT_T m_eventQueMemory[4];

    /// Flag for tracking re-entrant events
    bool        m_processingFsmEvent;

public:
    /// Constructor
    FsmEventQueue_( EventCompletedCbFunc_T eventCompletedCallback = nullptr );

public:
    /// This method properly queues and process event messages
    virtual void generateEvent( FSM_EVENT_T msg );
};

};};};  // end namespace(s)
#endif /// end header latch
