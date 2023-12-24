#ifndef Ajax_Alerts_Summary_h_
#define Ajax_Alerts_Summary_h_
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
#include "Ajax/Dm/MpAlert.h"
#include "Cpl/Dm/SubscriberComposer.h"
#include "Cpl/Memory/Aligned.h"

/// 
namespace Ajax
{
/// 
namespace Alerts
{


/** This class manages the content of the Alert Summary model point
 */
class Summary : public Cpl::Itc::CloseSync
{
public:
    /// Convenience type
    typedef Cpl::Dm::SubscriberComposer<Summary, Ajax::Dm::MpAlert> AlertObserver;

    /// Type for allocating memory for Alert Observer composer instances
    typedef Cpl::Memory::AlignedClass<AlertObserver> AlertComposerMemory;

public:
    /** Constructor. Note: The number of elements in the two array argument
        MUST equal Ajax::Type::Alert::NUM_ALERTS
     */
    Summary( Cpl::Dm::MailboxServer& myMbox,
             Ajax::Dm::MpAlert*      alertList[] );

public:
    /// Starts the component
    void request( OpenMsg& msg );

    /// Shutdowns the component
    void request( CloseMsg& msg );

protected:
    /// Change notification: Alerts 
    void alertChanged( Ajax::Dm::MpAlert& mpThatChanged, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;


protected:
    /// Observer for change notifications
    AlertComposerMemory     m_observers[Ajax::Type::Alert::NUM_ALERTS];

    /// Array of Alert Model Point pointers. 
    Ajax::Dm::MpAlert**     m_alertList;

    /// Reference to the Event loop (needed when constructing Observers)
    Cpl::Dm::EventLoop&     m_eventLoop;

    /// open/close state
    bool                    m_opened;
};

}       // end namespaces
}
#endif  // end header latch

