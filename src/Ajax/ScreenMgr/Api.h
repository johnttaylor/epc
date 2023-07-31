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

#include "Cpl/Dm/MailboxServer.h"
#include "Cpl/Itc/CloseSync.h"
#include "Ajax/ScreenMgr/EventQueue.h"
#include "Ajax/ScreenMgr/Navigation.h"
#include "Ajax/ScreenMgr/MpScreenApiPtr.h"
#include "Ajax/ScreenMgr/MpStaticScreenApiPtr.h"
#include "Cpl/Dm/Mp/Bool.h"

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
    public EventQueue,
    public Navigation
{
public:
    /// Constructor
    Api( Cpl::Dm::MailboxServer& uiMbox,
         MpScreenApiPtr&         homeScreenMP,
         MpStaticScreenApiPtr&   haltUiMP,
         Cpl::Dm::Mp::Bool&      displaySleepRequestMP );

public:
    /** This struct contains the parameters that MUST passed as the 'args' argument
        to the open() call.
     */
    struct OpenArgs_T
    {
        StaticScreenApi* splashScreen;  //!< Pointer to the Splash screen
        StaticScreenApi* haltScreen;    //!< Pointer to the 'default' error/halt screen (i.e. used when haltUiMP is invalid)
    };

    /// Starts/initializes the manager
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

public:
    /// See Ajax::ScreenMgr::EventQueue
    bool add( AjaxScreenMgrEvent_T event ) noexcept;

protected:
    /// Home screen MP
    MpScreenApiPtr&         m_mpHomeScreen;

    /// Halt screen MP
    MpStaticScreenApiPtr&   m_mpHaltUi;

    /// Sleep request MP (true:= turn off the display)
    Cpl::Dm::Mp::Bool&      m_mpDisplaySleepRequest;
        
    /// Splash screen handle
    StaticScreenApi*        m_splashScreenHdl;

    /// Default Halt screen handle (used no the HalUi MP is invalid)
    StaticScreenApi*        m_haltScreenHdl;

    /// Current Home Screen Handle
    ScreenApi*              m_homeScreenHdl;

    /// My open state
    bool                    m_opened;
};


}       // end namespaces
}
#endif  // end header latch

