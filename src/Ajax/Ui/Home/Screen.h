#ifndef Ajax_Ui_Home_Screen_h_
#define Ajax_Ui_Home_Screen_h_
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

#include "pimoroni/libraries/pico_graphics/pico_graphics.hpp"
#include "Ajax/ScreenMgr/ScreenApi.h"
#include "Ajax/ScreenMgr/Api.h"
#include "Cpl/Dm/Mp/Int32.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/SubscriberComposer.h"
#include "Ajax/Dm/MpFanMode.h"
#include "Ajax/Dm/MpAlertSummary.h"

/// Polling rate, in milliseconds, for sampling the space temperature
#ifndef OPTION_AJAX_UI_HOME_SCREEN_POLLING_MS
#define OPTION_AJAX_UI_HOME_SCREEN_POLLING_MS       2000     // 2 second
#endif


/// 
namespace Ajax
{
/// 
namespace Ui
{
/// 
namespace Home
{


/** This class implements the Home screen
 */
class Screen : public Ajax::ScreenMgr::ScreenApi
{
public:
    /// Constructor
    Screen( Ajax::ScreenMgr::Navigation&  screenMgr,
            pimoroni::PicoGraphics&       graphics,
            Cpl::Dm::MailboxServer&       myMbox,
            Cpl::Dm::Mp::Int32&           mpSpaceTemperature );

public:
    /// See Ajax::ScreenMgr::ScreenApi
    void enter( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    void exit( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    void sleep( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    void wake( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    void dispatch( AjaxScreenMgrEvent_T event, Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    bool tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    bool refresh( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

protected:
    /// Change notification
    void heatingModeChanged( Cpl::Dm::Mp::Bool& mpThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// Change notification
    void fanModeChanged( Ajax::Dm::MpFanMode& mpThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// Change notification
    void setpointChanged( Cpl::Dm::Mp::Int32& mpThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// Change notification
    void alertSummaryChanged( Ajax::Dm::MpAlertSummary& mpThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

protected:
    /// Helper method to get and break down the current IDT into integer and fractional components
    bool getDisplayIdt( int32_t& dstInteger, int32_t& dstFractional ) noexcept;

protected:
    /// Handle to the screen manager
    Ajax::ScreenMgr::Navigation&    m_screenMgr;

    /// Graphic library handle
    pimoroni::PicoGraphics&         m_graphics;

    /// Current space temperature
    Cpl::Dm::Mp::Int32&             m_mpIdt;

    /// Subscriber heating mode
    Cpl::Dm::SubscriberComposer<Screen, Cpl::Dm::Mp::Bool> m_obHeatMode;

    /// Subscriber fan mode
    Cpl::Dm::SubscriberComposer<Screen, Ajax::Dm::MpFanMode> m_obFanMode;

    /// Subscriber setpoint
    Cpl::Dm::SubscriberComposer<Screen, Cpl::Dm::Mp::Int32> m_obSetpoint;

    /// Subscriber Alert Summary
    Cpl::Dm::SubscriberComposer<Screen, Ajax::Dm::MpAlertSummary> m_obAlertSummary;

    /// Current space temperature
    int32_t                         m_currentIdt;

    /// Time marker used to trigger 1second polling of the space temperature
    uint32_t                        m_timerMarker;

    /// Current Alert index
    unsigned                        m_alertIndex;

    /// Dirty flag (i.e. need the screen manager to call refresh())
    bool                            m_stale;
};

}       // end namespaces
}
}
#endif  // end header latch

