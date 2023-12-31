#ifndef Ajax_Metrics_Api_h_
#define Ajax_Metrics_Api_h_
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
#include "Cpl/Dm/SubscriberComposer.h"
#include "Cpl/Itc/CloseSync.h"
#include "Ajax/Main/MetricsRecord.h"
#include "Cpl/System/Timer.h"
#include "Ajax/Dm/MpAlert.h"
#include "Cpl/Dm/Mp/Uint32.h"

/// Time, in milliseconds, between updates to persistent storage
#ifndef OPTION_AJAX_METRICS_INTERVAL_TIME_MS
#define OPTION_AJAX_METRICS_INTERVAL_TIME_MS        (15*60*1000)        // 15 min
#endif

/// 
namespace Ajax
{
/// 
namespace Metrics
{


/** This class manages the collection of Metrics and triggering their
    updates to persistent storage.
 */
class Api : public Cpl::Itc::CloseSync, public Cpl::System::Timer
{
public:
    /** Constructor. 
     */
    Api( Cpl::Dm::MailboxServer& myMbox);

public:
    /// Starts the component
    void request( OpenMsg& msg );

    /// Shutdowns the component
    void request( CloseMsg& msg );

protected:
    /// Software time expired
    void expired() noexcept;

    /// Change notification
    void fanPWMChanged( Cpl::Dm::Mp::Uint32& mpPWM, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// Change notification
    void heaterPWMChanged( Cpl::Dm::Mp::Uint32& mpPWM, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// Change notification
    void failsafeAlertChanged( Ajax::Dm::MpAlert& mpAlert, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// Helper method to increment metric mp
    void processFanMetric( uint32_t currentElapsedTimeMs, 
                           uint32_t pwm, 
                           uint32_t intervalTime = OPTION_AJAX_METRICS_INTERVAL_TIME_MS ) noexcept;

    /// Helper method to increment metric mp
    void processHeaterMetric( uint32_t currentElapsedTimeMs, 
                              uint32_t pwm, 
                              uint32_t intervalTime = OPTION_AJAX_METRICS_INTERVAL_TIME_MS  ) noexcept;

protected:
    /// Observer for change notifications
    Cpl::Dm::SubscriberComposer<Api, Cpl::Dm::Mp::Uint32> m_obFanPWM;

    /// Observer for change notifications
    Cpl::Dm::SubscriberComposer<Api, Cpl::Dm::Mp::Uint32> m_obHeaterPWM;

    /// Observer for change notifications
    Cpl::Dm::SubscriberComposer<Api, Ajax::Dm::MpAlert>   m_obFailsafeAlert;

    /// Time marker of when last updated the metrics to persistent storage
    uint32_t                        m_runtimeMarker;

    /// Time marker of when last updated the metrics to persistent storage
    uint32_t                        m_heaterMarker;

    /// Time marker of when last updated the metrics to persistent storage
    uint32_t                        m_fanMarker;

    /// Current on/off state
    bool                            m_fanOn;

    /// Current on/off state
    bool                            m_heaterOn;

    /// open/close state
    bool                            m_opened;
};

}       // end namespaces
}
#endif  // end header latch

