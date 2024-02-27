#ifndef Ajax_Ui_StatusIndicator_Api_h_
#define Ajax_Ui_StatusIndicator_Api_h_
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

#include "Driver/LED/RedGreenBlue.h"
#include "Cpl/Dm/MailboxServer.h"
#include "Cpl/Itc/CloseSync.h"
#include "Ajax/Dm/MpAlertSummary.h"
#include "Ajax/ScreenMgr/Navigation.h"
#include "Cpl/Dm/Mp/Uint32.h"
#include "Cpl/Dm/SubscriberComposer.h"
#include "Cpl/System/Timer.h"


/// 
namespace Ajax
{
/// 
namespace Ui
{
/// 
namespace StatusIndicator
{


/** This class manages the what color/on-off-state of the RGB LED
 */
class Api : public Cpl::Itc::CloseSync, public Cpl::System::Timer
{
public:
    /// Constructor
    Api( Cpl::Dm::MailboxServer&        myMbox,
         Driver::LED::RedGreenBlue&     statusLED,
         Ajax::ScreenMgr::Navigation&   scrMgr );

public:
    /// Starts the component
    void request( OpenMsg& msg );

    /// Shutdowns the component
    void request( CloseMsg& msg );

protected:
    /// Software timer expired
    void expired() noexcept;

protected:
    /// Helper method that set the actual LED state (return true when the error condition is present)
    bool setHaltError( uint32_t now ) noexcept;

    /// Helper method that set the actual LED state
    void setStatus( uint32_t now ) noexcept;

protected:
    /// Handle to the RGD LED driver
    Driver::LED::RedGreenBlue&    m_ledDriver;

    /// Handle to the screen manager
    Ajax::ScreenMgr::Navigation&  m_scrMgr;

    /// Period time marker
    uint32_t    m_timeMarker2Hz;

    /// Period time marker
    uint32_t    m_timeMarker1Hz;

    /// Period time marker
    uint32_t    m_timeMarkerQuarterHz;

    /// Current indicator state (when NOT in a 'hard-error')
    unsigned    m_state;

    /// Current 'flash edge'
    bool        m_2HzOnCycle;

    /// Current 'flash edge'
    bool        m_1HzOnCycle;

    /// Current 'flash edge'
    bool        m_quarterHzOnCycle;

    /// Flag for initial execution pass
    bool        m_firstExecution;
    
    /// open/close state
    bool        m_opened;
};

}       // end namespaces
}
}
#endif  // end header latch

