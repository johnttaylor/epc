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
    Api( Cpl::Dm::MailboxServer&  myMbox,
         Ajax::Heating::Flc::Api& heatingController,
         size_t                   maxHeaterPWM,
         size_t                   maxFanPWM ) noexcept;

public:
    /// This method starts the supervisor (See Cpl::Itc::OpenSync)
    void request( OpenMsg& msg );

    /// This method stops the supervisor (See Cpl::Itc::CloseSync)
    void request( CloseMsg& msg );


protected:
    /// Action
    void clearHiTempAlert() noexcept;

    /// Action
    void clearSensorAlert() noexcept;

    /// Action
    void heatOff() noexcept;

    /// Action
    void raiseHiTempAlert() noexcept;

    /// Action
    void raiseSensorAlert() noexcept;

    /// Action
    void runHeatingAlgo() noexcept;

public:
    /// Guard
    bool isSensorAvailable() noexcept;


protected:
    /// See Cpl::System::Timer (timer expired callback)
    void expired() noexcept;
    
    /// Helper method to select temperature source.  Returns false if there is no valid temperature source
    bool getTemperature( int32_t& idt ) noexcept;

protected:
    /// Heating controller
    Ajax::Heating::Flc::Api& m_flcController;

    /// Maximum PWM output value (i.e 100% duty cycle) for the Heater
    size_t                   m_maxHeaterPWM;

    /// Maximum PWM output value (i.e 100% duty cycle) for the Fan motor
    size_t                   m_maxFanPWM;

    /// Heater PWM output value
    int32_t                  m_heaterOutPWM;

    /// Open state
    bool                     m_opened;
};


}       // end namespaces
} 
} 
#endif  // end header latch

