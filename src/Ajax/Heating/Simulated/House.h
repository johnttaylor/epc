#ifndef Ajax_Heating_Simulated_House_h_
#define Ajax_Heating_Simulated_House_h_
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

#include "colony_config.h"
#include "Ajax/SimHouse/House.h"
#include "Ajax/Heating/Supervisor/Api.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/Mp/Double.h"


///
namespace Ajax
{
///
namespace Heating
{
///
namespace Simulated
{


/** This class glues the House Simulation into the Heater Application.
    The specified IDT model point is updated with the output of simulation - when
    the 'mpSimEnabled' MP is true.  Requires an outdoor temperature model point
    as an input
 */
class House : public Ajax::Heating::Supervisor::Api
{
public:
    /// Constructor
    House( Cpl::Dm::MailboxServer& myMbox, 
           Cpl::Dm::Mp::Bool&      mpSimEnabled,
           Cpl::Dm::Mp::Double&    mpOutdoorTemperature );

protected:
    /// Override my parent class to hook into the Algorithm periodic timing
    void intervalExpired() noexcept;

    /// Helper method
    void executeSimulation();


protected:
    /// The underlying house simulation
    Ajax::SimHouse::House   m_sim;

    /// INPUT: Enable switch
    Cpl::Dm::Mp::Bool&      m_mpSimEnabled;

    /// INPUT: Outdoor temperature 
    Cpl::Dm::Mp::Double&    m_mpOutdoorTemperature;
};

};      // end namespace(s)
};
};

#endif  // end header latch
