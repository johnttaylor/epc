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

#include "Api.h"
#include "Ajax/Logging/Api.h"
#include "StaticScreenApi.h"
#include "Cpl/System/Assert.h"

using namespace Ajax::ScreenMgr;



/////////////////////////////////////////////////////////////////////////////
Api::Api( Cpl::Dm::MailboxServer& uiMbox,
          MpScreenApiPtr&         homeScreenMP,
          MpStaticScreenApiPtr&   haltUiMP,
          Cpl::Dm::Mp::Bool&      displaySleepRequestMP,
          MpStaticScreenApiPtr&   shutdownMP,
          DisplayApi&             display,
          NavigationElement       memoryForNavigationStack[],
          size_t                  numElementsInNavigtationStackArray,
          Cpl::Container::RingBufferMP<AjaxScreenMgrEvent_T>& eventRingBuffer )
    : Cpl::Itc::CloseSync( uiMbox )
    , m_obHomeScreenMP( uiMbox, *this, &Api::homeScreenMp_changed )
    , m_obHaltUimMP( uiMbox, *this, &Api::haltUiMp_changed )
    , m_obSleepReqMP( uiMbox, *this, &Api::sleepRequestMp_changed )
    , m_obShutdownReqMP( uiMbox, *this, &Api::shutdownMp_changed )
    , m_obEventQueueCountMP( uiMbox, *this, &Api::eventQueueCountMp_changed )
    , m_timer( uiMbox, *this, &Api::timerExpired )
    , m_mpHomeScreen( homeScreenMP )
    , m_mpHaltUi( haltUiMP )
    , m_mpDisplaySleepRequest( displaySleepRequestMP )
    , m_mpShutdownScreen( shutdownMP )
    , m_homeScreenHdl( nullptr )
    , m_display( display )
    , m_eventQueue( eventRingBuffer )
    , m_curScreenHdl( nullptr )
    , m_shuttingDown( false )
    , m_opened( false )
{
    // Populate the free list
    for ( size_t i=0; i < numElementsInNavigtationStackArray; i++ )
    {
        m_freeStackMemoryList.put( memoryForNavigationStack[i] );
    }
}

void Api::request( OpenMsg& msg )
{
    msg.getPayload().m_success = true;

    if ( m_opened )
    {
        Ajax::Logging::logf( Ajax::Logging::WarningMsg::OPEN_CLOSE, "open: Ajax::ScreenMgr::Api" );
        msg.returnToSender();
        return;
    }

    // Start the display/graphics engine and paint the Splash screen
    StaticScreenApi* splash = (StaticScreenApi*) msg.getPayload().m_args;
    CPL_SYSTEM_ASSERT( splash );
    if ( !m_display.start() )
    {
        Ajax::Logging::logf( Ajax::Logging::CriticalMsg::DRIVER, "Ajax::ScreenMgr::Api - PicoDisplay driver failed to start" );
        msg.getPayload().m_success = false;
        msg.returnToSender();
        return;
    }
    splash->paint( Cpl::System::ElapsedTime::precision() );
    if ( !m_display.update() )
    {
        Ajax::Logging::logf( Ajax::Logging::CriticalMsg::DRIVER, "Ajax::ScreenMgr::Api - PicoDisplay update() failed" );
        msg.getPayload().m_success = false;
        msg.returnToSender();
        return;
    }

    // Housekeeping
    m_opened         = true;
    m_halted         = false;
    m_shuttingDown   = false;
    m_homeScreenHdl  = nullptr;
    m_curScreenHdl   = nullptr;
    m_pendingHaltScr = nullptr;

    // Enable my 'events'
    m_mpHomeScreen.attach( m_obHomeScreenMP );
    m_mpHaltUi.attach( m_obHaltUimMP );
    m_mpShutdownScreen.attach( m_obShutdownReqMP );
    m_mpDisplaySleepRequest.attach( m_obSleepReqMP );
    m_eventQueue.m_mpElementCount.attach( m_obEventQueueCountMP );
    m_timerMarker = Cpl::System::ElapsedTime::milliseconds();

    // All done!
    msg.returnToSender();
}

void Api::request( CloseMsg& msg )
{
    if ( !m_opened )
    {
        Ajax::Logging::logf( Ajax::Logging::WarningMsg::OPEN_CLOSE, "close: Ajax::ScreenMgr::Api" );
        msg.returnToSender();
        return;
    }

    // Housekeeping
    m_opened       = false;
    m_curScreenHdl = nullptr;

    // Stop my timer and change notifications
    m_timer.stop();
    m_mpHomeScreen.detach( m_obHomeScreenMP );
    m_mpHaltUi.detach( m_obHaltUimMP );
    m_mpShutdownScreen.detach( m_obShutdownReqMP );
    m_mpDisplaySleepRequest.detach( m_obSleepReqMP );
    m_eventQueue.m_mpElementCount.detach( m_obEventQueueCountMP );

    // Restore the Navigation free list
    NavigationElement* item;
    while ( (item=m_navigationStack.get()) )
    {
        m_freeStackMemoryList.put( *item );
    }

    // Shutdown the display
    m_display.stop();
    msg.returnToSender();
}

/////////////////////////////
void Api::homeScreenMp_changed( MpScreenApiPtr& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    ScreenApi* homePtr;
    if ( mp.readAndSync( homePtr, clientObserver ) && homePtr != nullptr && !m_halted )
    {
        // Check for a pending transition to the halt screen
        if ( m_pendingHaltScr )
        {
            transitionToHaltScreen();
            return;
        }

        bool firstHome  = m_homeScreenHdl == nullptr;
        m_homeScreenHdl = homePtr;

        // Transition from the splash screen to the Home screen
        if ( firstHome )
        {
            Cpl::System::ElapsedTime::Precision_T now = Cpl::System::ElapsedTime::precision();
            m_curScreenHdl = m_homeScreenHdl;
            m_curScreenHdl->enter( now );
            eventQueueCountMp_changed( m_eventQueue.m_mpElementCount, m_obEventQueueCountMP );  // Drain any events that occurred before the Home screen was launched
            bool dirty = m_curScreenHdl->refresh( now );

            // Update the physical display
            if ( dirty )
            {
                if ( !m_display.update() )
                {
                    Ajax::Logging::logf( Ajax::Logging::CriticalMsg::DRIVER, "PicoDisplay update() failed" );
                }
            }
            m_timer.start( OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS );
        }
    }
}

void Api::haltUiMp_changed( MpStaticScreenApiPtr& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    StaticScreenApi* haltPtr;
    if ( mp.readAndSync( haltPtr, clientObserver ) && haltPtr != nullptr )
    {
        m_pendingHaltScr = haltPtr;
        
        // If NOT on the Splash screen -->transition immediately to the Halt/Error Screen
        if ( m_curScreenHdl != nullptr )
        {
            transitionToHaltScreen();
        }
    }
}
void Api::transitionToHaltScreen() noexcept
{
    // Transition to the Halt screen
    Cpl::System::ElapsedTime::Precision_T now = Cpl::System::ElapsedTime::precision();
    if ( m_curScreenHdl )
    {
        m_curScreenHdl->exit( now );
    }
    m_pendingHaltScr->paint( now );
    m_halted       = true;
    m_curScreenHdl = nullptr;
    if ( !m_display.update() )
    {
        Ajax::Logging::logf( Ajax::Logging::CriticalMsg::DRIVER, "PicoDisplay update() failed" );
    }
}

void Api::sleepRequestMp_changed( Cpl::Dm::Mp::Bool& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    bool sleepRequest;
    if ( mp.readAndSync( sleepRequest, clientObserver ) && m_curScreenHdl != nullptr )
    {
        Cpl::System::ElapsedTime::Precision_T now = Cpl::System::ElapsedTime::precision();

        if ( sleepRequest )
        {
            if ( !m_display.turnOff() )
            {
                Ajax::Logging::logf( Ajax::Logging::CriticalMsg::DRIVER, "PicoDisplay turnOff() failed" );
            }
            m_curScreenHdl->sleep( now );
        }
        else
        {
            m_curScreenHdl->wake( now );
            m_curScreenHdl->refresh( now );
            if ( !m_display.turnOn() )
            {
                Ajax::Logging::logf( Ajax::Logging::CriticalMsg::DRIVER, "PicoDisplay turnOn() failed" );
            }
        }
    }
}

void Api::shutdownMp_changed( MpStaticScreenApiPtr& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    StaticScreenApi* shutdownPtr;
    if ( mp.readAndSync( shutdownPtr, clientObserver ) && shutdownPtr != nullptr )
    {
        // Transition to the shutting down screen
        Cpl::System::ElapsedTime::Precision_T now = Cpl::System::ElapsedTime::precision();
        if ( m_curScreenHdl )
        {
            m_curScreenHdl->exit( now );
        }
        shutdownPtr->paint( now );
        m_curScreenHdl = nullptr;
        if ( !m_display.update() )
        {
            Ajax::Logging::logf( Ajax::Logging::CriticalMsg::DRIVER, "PicoDisplay update() failed" );
        }
    }
}


void Api::eventQueueCountMp_changed( Cpl::Dm::Mp::Uint32& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    uint32_t count;
    if ( mp.readAndSync( count, clientObserver ) && m_curScreenHdl != nullptr )
    {
        if ( count > 0 )
        {
            // Un-subscribe so I don't get change notifications when removing elements
            m_eventQueue.m_mpElementCount.detach( m_obEventQueueCountMP );
            uint16_t finalSeqNum = 0;

            // Drain the event queue
            AjaxScreenMgrEvent_T event;
            while ( m_eventQueue.remove( event, finalSeqNum ) )
            {
                // Process the event
                m_curScreenHdl->dispatch( event, Cpl::System::ElapsedTime::milliseconds() );
            }

            // Re-subscribe
            m_eventQueue.m_mpElementCount.attach( m_obEventQueueCountMP, finalSeqNum );
        }
    }
}

void Api::timerExpired( void )
{
    // Do nothing if the splash/shutdown/error screen is active
    if ( m_curScreenHdl != nullptr )
    {
        bool stale = m_curScreenHdl->tick( Cpl::System::ElapsedTime::milliseconds() );

        // Restart the timer - and attempt to be rate monotonic
        Cpl::System::ElapsedTime::Precision_T precNow = Cpl::System::ElapsedTime::milliseconds();
        uint32_t now   = precNow.asMilliseconds();
        uint32_t delta = now - m_timerMarker;
        if ( delta > OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS )
        {
            delta = delta - OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS;
            if ( delta > OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS )
            {
                delta = OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS;
            }
        }
        else
        {
            delta = 0;
        }
        m_timerMarker = now;
        m_timer.start( OPTION_AJAX_SCREEN_MGR_TICK_TIME_MS - delta );

        if ( stale )
        {
            if ( m_curScreenHdl->refresh( precNow ) )
            {
                if ( !m_display.update() )

                {
                    Ajax::Logging::logf( Ajax::Logging::CriticalMsg::UX_ERROR, "tick.m_display.update() failed" );
                }
            }

        }
    }
}


////////////////////////////////
ScreenApi* Api::getCurrentScreen() noexcept
{
    return m_curScreenHdl;
}

bool Api::isCurrentScreenHaltError() noexcept
{
    return m_halted;
}

void Api::push( ScreenApi & newScreen ) noexcept
{
    // Do nothing if the splash/shutdown/error screen is active
    if ( m_curScreenHdl != nullptr )
    {
        // Handle the error case of pushing to the Home screen
        if ( &newScreen == m_homeScreenHdl )
        {
            popToHome();
            return;
        }

        // Push the current screen onto the Nav stack (if it is not the home screen)
        if ( m_curScreenHdl != nullptr && m_curScreenHdl != m_homeScreenHdl )
        {
            // Get a free element - 'throw an error' if no free memory
            NavigationElement* freeElem = m_freeStackMemoryList.get();
            if ( freeElem == nullptr )
            {
                Ajax::Logging::logf( Ajax::Logging::WarningMsg::OUT_OF_MEMORY, "Ui::Navigation.push() failed" );
                popToHome();
                return;
            }

            freeElem->m_screenPtr = m_curScreenHdl;
            m_navigationStack.putFirst( *freeElem );
        }

        // Do the screen transition
        Cpl::System::ElapsedTime::Precision_T now = Cpl::System::ElapsedTime::precision();
        m_curScreenHdl->exit( now );
        m_curScreenHdl = &newScreen;
        m_curScreenHdl->enter( now );
        bool dirty = m_curScreenHdl->refresh( now );
        if ( dirty )
        {
            if ( !m_display.update() )

            {
                Ajax::Logging::logf( Ajax::Logging::CriticalMsg::UX_ERROR, "push.m_display.update() failed" );
            }
        }
    }
}

void Api::pop( unsigned count ) noexcept
{
    // Do nothing if currently on the home screen
    if ( m_curScreenHdl != nullptr && m_curScreenHdl != m_homeScreenHdl )
    {
        // Pop N elements
        bool stackEmpty               = false;
        NavigationElement* poppedElem = nullptr;
        while ( count-- )
        {
            poppedElem = m_navigationStack.getFirst();
            if ( poppedElem == nullptr )
            {
                stackEmpty = true;
                break;
            }
            m_freeStackMemoryList.put( *poppedElem );
        }

        // Exit the current screen
        Cpl::System::ElapsedTime::Precision_T now = Cpl::System::ElapsedTime::precision();
        m_curScreenHdl->exit( now );

        // Set the new current screen and transition to it
        m_curScreenHdl = stackEmpty ? m_homeScreenHdl : poppedElem->m_screenPtr;
        m_curScreenHdl->enter( now );
        bool dirty = m_curScreenHdl->refresh( now );
        if ( dirty )
        {
            if ( !m_display.update() )
            {
                Ajax::Logging::logf( Ajax::Logging::CriticalMsg::UX_ERROR, "pop.m_display.update() failed" );
            }
        }
    }
}

void Api::popTo( ScreenApi & returnToScreen ) noexcept
{
    // Do nothing if currently on the home screen
    if ( m_curScreenHdl != nullptr && m_curScreenHdl != m_homeScreenHdl )
    {
        // Pop till there is match
        bool stackEmpty = false;
        NavigationElement* poppedElem = nullptr;
        for ( ;;)
        {
            poppedElem = m_navigationStack.getFirst();
            if ( poppedElem == nullptr )
            {
                stackEmpty = true;
                break;
            }
            m_freeStackMemoryList.put( *poppedElem );
            if ( poppedElem->m_screenPtr == &returnToScreen )
            {
                break;
            }
        }

        // Exit the current screen
        Cpl::System::ElapsedTime::Precision_T now = Cpl::System::ElapsedTime::precision();
        m_curScreenHdl->exit( now );

        // Set the new current screen and transition to it
        m_curScreenHdl = stackEmpty ? m_homeScreenHdl : poppedElem->m_screenPtr;
        m_curScreenHdl->enter( now );
        bool dirty = m_curScreenHdl->refresh( now );
        if ( dirty )
        {
            if ( !m_display.update() )
            {
                Ajax::Logging::logf( Ajax::Logging::CriticalMsg::UX_ERROR, "popTo.m_display.update() failed" );
            }
        }
    }
}

void Api::popToHome() noexcept
{
    // Do nothing if the splash/shutdown/error screen is active
    if ( m_curScreenHdl != nullptr )
    {
        // Restore the Navigation free list
        NavigationElement* item;
        while ( (item=m_navigationStack.get()) )
        {
            m_freeStackMemoryList.put( *item );
        }

        // Transition to the home screen
        Cpl::System::ElapsedTime::Precision_T now = Cpl::System::ElapsedTime::precision();
        m_curScreenHdl->exit( now );
        m_curScreenHdl = m_homeScreenHdl;
        m_curScreenHdl->enter( now );
        bool dirty = m_curScreenHdl->refresh( now );
        if ( dirty )
        {
            if ( !m_display.update() )
            {
                Ajax::Logging::logf( Ajax::Logging::CriticalMsg::UX_ERROR, "popToHome.m_display.update() failed" );
            }
        }
    }
}

