#ifndef Ajax_Heating_Supervisor_Api_h_
#define Ajax_Heating_Supervisor_Api_h_
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
#include "Cpl/System/Timer.h"
#include "Ajax/Heating/Supervisor/FsmEventQueue_.h"
#include "Ajax/Heating/Flc/Api.h"
#include "Cpl/Dm/SubscriberComposer.h"
#include "Cpl/Dm/Mp/Int32.h"
#include "Cpl/Dm/Mp/Bool.h"


/// Periodic interval (in milliseconds) for calling the FLC
#ifndef OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS
#define OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS         (2*1000)    // 2 seconds
#endif

/// 
namespace Ajax {
/// 
namespace Heating {
/// 
namespace Supervisor {

/** This class implements the Heating Algorithm Supervisor.  The supervisor
    is responsible for "managing/sequencing' the heating algorithm

    All inputs and outputs (including alerts) are done via Model Points.  The 
    model point references are via 'hard coded' model point names.

    The class is NOT thread safe.
 */
class Api: public Cpl::Itc::CloseSync, public Cpl::System::Timer, public FsmEventQueue_
{
public:
    /// Constructor
    Api( Cpl::Dm::MailboxServer&  myMbox ) noexcept;

public:
    /// This method starts the supervisor (See Cpl::Itc::OpenSync)
    void request( OpenMsg& msg );

    /// This method stops the supervisor (See Cpl::Itc::CloseSync)
    void request( CloseMsg& msg );


protected:
    /// Action
    void allOff() noexcept;

    /// Action
    void checkForSensor() noexcept;

    /// Action
    void fanOff() noexcept;
    
    /// Action
    void fanOn() noexcept;

    /// Action
    void heatOff() noexcept;

    /// Action
    void runHeatingAlgo() noexcept;

public:
    /// Guard
    bool isSensorAvailable() noexcept;


protected:
    /// See Cpl::System::Timer (timer expired callback)
    void expired() noexcept;
    
    /// Change notification
    void hwSafetyChanged( Cpl::Dm::Mp::Bool& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;
    
    /// Change notification
    void heatingEnabledChanged( Cpl::Dm::Mp::Bool& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

protected:
    /// Helper method to select temperature source.  Returns false if there is no valid temperature source
    bool getTemperature( int32_t& idt ) noexcept;

    /// Helper method that schedules when the algorithm executes
    void scheduleAlgorithm() noexcept;

    /** Helper method that is called when the algorithm's periodic interval has 
        expired, i.e. the periodic execution of the algorithm
     */
    virtual void intervalExpired() noexcept;

    /// Helper method that gets the fan setting
    uint32_t getFanPWM() noexcept;

protected:
    /// Heating controller
    Ajax::Heating::Flc::Api  m_flcController;

    /// Observer for change notification (to the hardware temperature safety limit)
    Cpl::Dm::SubscriberComposer<Api, Cpl::Dm::Mp::Bool>    m_obHwSafety;

    /// Observer for change notification (to the heating mode)
    Cpl::Dm::SubscriberComposer<Api, Cpl::Dm::Mp::Bool>    m_obHeatingEnabled;

    /// Accumulated capacity requests
    int32_t                  m_sumCapacityRequest;

    /// Timer marker of last processing cycle
    uint32_t                 m_timeMarker;

    /// Maximum capacity
    uint32_t                 m_maxCapacity;

    /// Flag for first execution of the algorithm
    bool                     m_firstExecution;

    /// Temperature sensor available
    bool                     m_temperatureSensorAvailable;

    /// Open state
    bool                     m_opened;
};


}       // end namespaces
} 
} 
#endif  // end header latch

