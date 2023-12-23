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
#include "Cpl/Dm/Mp/Uint32.h"


/// 
namespace Ajax {
/// 
namespace Ui {
/// 
namespace StatusIndicator {


/** This class manages the what color/on-off-state of the RGB LED
 */
class Api : public Cpl::Itc::CloseSync
{
public:
    /// Constructor
    Api( Cpl::Dm::MailboxServer&       myMbox,
            Driver::LED::RedGreenBlue& statusLED );

public:
    /// Starts the component
    void request( OpenMsg& msg );

    /// Shutdowns the component
    void request( CloseMsg& msg );

protected:
    /// Change notification: Alert Summary
    void alertSummaryChanged( Aj
protected:
    /// Handle to the RGD LED driver
    Driver::LED::RedGreenBlue&    m_ledDriver;
};

}       // end namespaces
}
}
#endif  // end header latch

