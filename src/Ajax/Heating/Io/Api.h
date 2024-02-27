#ifndef Ajax_Heating_Io_Api_h_
#define Ajax_Heating_Io_Api_h_
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
#include "Ajax/Heating/Supervisor/Api.h"
#include "Driver/DIO/Pwm.h"
#include "Driver/DIO/In.h"
#include "Cpl/Dm/Mp/Uint32.h"

/// Periodic interval (in milliseconds) for sampling/outputting hardware signals
#ifndef OPTION_AJAX_HEATING_IO_ALGO_INTERVAL_MS
#define OPTION_AJAX_HEATING_IO_ALGO_INTERVAL_MS         (OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS/4)
#endif

/// 
namespace Ajax {
/// 
namespace Heating {
/// 
namespace Io {

/** This class is responsible for 'translating' physical hardware signal 
    to/from model point values.  The implementation assumes the 
    existence of the following MP:

        - cmdHeaterPWM
        - cmdFanPWM
        - hwSafetyLimit
 */
class Api: public Cpl::Itc::CloseSync, public Cpl::System::Timer
{
public:
    /// Constructor
    Api( Cpl::Dm::MailboxServer&  myMbox,
         Driver::DIO::Pwm&        driverHeaterPwm,
         Driver::DIO::Pwm&        driverFanPwm,
         Driver::DIO::In&         driverHwSafetyIn
         ) noexcept;

public:
    /// This method starts the supervisor (See Cpl::Itc::OpenSync)
    void request( OpenMsg& msg );

    /// This method stops the supervisor (See Cpl::Itc::CloseSync)
    void request( CloseMsg& msg );

protected:
    /// Change notification 
    void heaterPWMChanged( Cpl::Dm::Mp::Uint32& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// Change notification 
    void fanPWMChanged( Cpl::Dm::Mp::Uint32& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept;

    /// See Cpl::System::Timer (timer expired callback)
    void expired() noexcept;
    
protected:
    /// Helper method than reads/writes the hardware
    void intervalExpired() noexcept;

protected:
    /// Observer for change notification 
    Cpl::Dm::SubscriberComposer<Api, Cpl::Dm::Mp::Uint32>    m_obHeaterPWM;

    /// Observer for change notification 
    Cpl::Dm::SubscriberComposer<Api, Cpl::Dm::Mp::Uint32>    m_obFanPWM;

    /// Driver handle
    Driver::DIO::Pwm&   m_driverHeaterPwm;
    
    /// Driver handle
    Driver::DIO::Pwm&   m_driverFanPwm;
    
    /// Driver handle
    Driver::DIO::In&    m_driverHwSafetyIn;

    /// Period time marker
    uint32_t    m_timeMarker;

    /// Flag for initial execution pass
    bool        m_firstExecution;

    /// Open state
    bool        m_opened;
};


}       // end namespaces
} 
} 
#endif  // end header latch

