#ifndef Ajax_ScreeMgr_Api_h_
#define Ajax_ScreeMgr_Api_h_
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
#include "Cpl/Dm/MailboxServer.h"
#include "Cpl/Itc/CloseSync.h"
#include "Ajax/ScreenMgr/Event.h"
#include "Ajax/ScreenMgr/Navigation.h"
#include "Ajax/ScreenMgr/MpScreenApiPtr.h"
#include "Ajax/ScreenMgr/MpStaticScreenApiPtr.h"
#include "Ajax/ScreenMgr/DisplayApi.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/SubscriberComposer.h"
#include "Cpl/System/Timer.h"
#include "Cpl/Container/RingBufferMP.h"
#include "Cpl/Container/DList.h"
#include "Cpl/Container/Item.h"



/** The timer count down time, in milliseconds for the 20Hz tick timer 
 */
#ifndef OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS
#define OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS     50
#endif

/// 
namespace Ajax {
/// 
namespace ScreenMgr {

/** This class implements the Screen Manager.  The screen manager is responsible
    for navigation mechanics and handling of the Splash, Shutting-Down, and
    Error-Halt screens
    
    The Screen Manager is the owner/Runnable object for the UI thread
 */
class Api : public Cpl::Dm::MailboxServer,
    public Cpl::Itc::CloseSync,
    public Navigation
{
public:
    /// Type for 'elements' in the Navigation Stack
    class NavigationElement : public Cpl::Container::ExtendedItem
    {
    public:
        /// Constructor
        NavigationElement() :m_screenPtr( nullptr ){}
        
        /// Screen Pointer
        ScreenApi*  m_screenPtr;
    };

public:
    /** Constructor.  THe application is responsible for populating the 
        'freeMemoryForNavigationStack' with 'NullScreen' instances.  The
        number of elements in the list determines the depth of the navigation
        stack.
     */
    Api( Cpl::Dm::MailboxServer& uiMbox,
         MpScreenApiPtr&         homeScreenMP,
         MpStaticScreenApiPtr&   haltUiMP,
         Cpl::Dm::Mp::Bool&      displaySleepRequestMP,
         MpStaticScreenApiPtr&   shutdownMP,
         DisplayApi&             display,
         Cpl::Container::DList<NavigationElement>&           freeMemoryForNavigationStack,
         Cpl::Container::RingBufferMP<AjaxScreenMgrEvent_T>& eventRingBuffer );

public:
    /** Starts/initializes the manager.  The open() must pass a non-null
        pointer to the splash screen (StaticScreenApi*) as the 'args' parameter.
     */
    void request( OpenMsg& msg );

    /// Shutdowns the manager
    void request( CloseMsg& msg );

public:
    /// See Ajax::ScreenMgr::Navigation
    void push( ScreenApi& newScreen ) noexcept;
    
    /// See Ajax::ScreenMgr::Navigation
    void pop( unsigned count=1 ) noexcept;
    
    /// See Ajax::ScreenMgr::Navigation
    void popTo( ScreenApi& returnToScreen ) noexcept;
    
    /// See Ajax::ScreenMgr::Navigation
    void popToHome() noexcept;


protected:
    /// MP Change notification
    void homeScreenMp_changed( MpScreenApiPtr& modelPointThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// MP Change notification
    void haltUiMp_changed( MpStaticScreenApiPtr& modelPointThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// MP Change notification
    void sleepRequestMp_changed( Cpl::Dm::Mp::Bool& modelPointThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// MP Change notification
    void shutdownMp_changed( MpStaticScreenApiPtr& modelPointThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// MP Change notification
    void eventQueueCountMp_changed( Cpl::Dm::Mp::Uint32& modelPointThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// 20Hz timer expired
    void timerExpired( void );

protected:
    /// Subscriber
    Cpl::Dm::SubscriberComposer<Api, MpScreenApiPtr>        m_obHomeScreenMP;

    /// Subscriber
    Cpl::Dm::SubscriberComposer<Api, MpStaticScreenApiPtr>  m_obHaltUimMP;

    /// Subscriber
    Cpl::Dm::SubscriberComposer<Api, Cpl::Dm::Mp::Bool>     m_obSleepReqMP;

    /// Subscriber
    Cpl::Dm::SubscriberComposer<Api, MpStaticScreenApiPtr>  m_obShutdownReqMP;

    /// Subscriber
    Cpl::Dm::SubscriberComposer<Api, Cpl::Dm::Mp::Uint32>   m_obEventQueueCountMP;

    /// 20Hz tick timer
    Cpl::System::TimerComposer<Api>                         m_timer;

    /// Home screen MP
    MpScreenApiPtr&         m_mpHomeScreen;

    /// Halt screen MP
    MpStaticScreenApiPtr&   m_mpHaltUi;

    /// Sleep request MP (true:= turn off the display)
    Cpl::Dm::Mp::Bool&      m_mpDisplaySleepRequest;
        
    /// Shutdown request MP 
    MpStaticScreenApiPtr&   m_mpShutdownScreen;

    /// Current Home Screen Handle
    ScreenApi*              m_homeScreenHdl;

    /// Handle to the 'display'
    DisplayApi&             m_display;

    /// Thread safe Event queue
    Cpl::Container::RingBufferMP<AjaxScreenMgrEvent_T>& m_eventQueue;

    /// Free 'memory' for the navigation stack
    Cpl::Container::DList<NavigationElement>&           m_freeStackMemoryList;

    /// The Navigation stack
    Cpl::Container::DList<NavigationElement>            m_navigationStack;

    /// Current Screen
    ScreenApi*              m_curScreenHdl;

    /// Timer marker of last Tick call
    uint32_t                m_timerMarker;

    /// Shutdown flag
    bool                    m_shuttingDown;

    /// My open state
    bool                    m_opened;
};


}       // end namespaces
}
#endif  // end header latch

