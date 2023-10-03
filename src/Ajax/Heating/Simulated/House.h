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
#include "Cpl/System/EventLoop.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/Mp/Float.h"
#include "Cpl/Dm/Mp/Uint32.h"
#include "Cpl/Dm/Mp/Int32.h"

/** The periodic timing, in milliseconds, to run the house simulator
 */
#ifndef OPTION_AJAX_HEATING_SIMULATED_HOUSE_SIM_TIMING_MS
#define OPTION_AJAX_HEATING_SIMULATED_HOUSE_SIM_TIMING_MS        1000
#endif

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
class House : public Cpl::System::EventLoop
{
public:
    /// Constructor
    House( Cpl::Dm::Mp::Bool&                   mpSimEnabled,
           Cpl::Dm::Mp::Int32&                  mpIndoorTemperature,
           Cpl::Dm::Mp::Float&                  mpOutdoorTemperature,
           Cpl::Dm::Mp::Uint32&                 mpHeaterPWMOutput,
           uint32_t                             maxHeatingPWMValue,
           Cpl::Dm::Mp::Uint32&                 mpFanPWMOutput,
           uint32_t                             maxFanPWMValue,
           unsigned long                        timeOutPeriodInMsec = OPTION_AJAX_HEATING_SIMULATED_HOUSE_SIM_TIMING_MS,
           Cpl::System::SharedEventHandlerApi*  eventHandler        = 0 );

protected:
    /// Override base implementation to access the event-loop timing directly
    void appRun();

    /// Helper method
    void executeSimulation();


protected:
    /// The underlying house simulation
    Ajax::SimHouse::House   m_sim;

    /// OUTPUT: Indoor/space temperature
    Cpl::Dm::Mp::Int32&     m_mpIdt;

    /// INPUT: Enable switch
    Cpl::Dm::Mp::Bool&      m_mpSimEnabled;

    /// INPUT: Outdoor temperature 
    Cpl::Dm::Mp::Float&     m_mpOutdoorTemperature;

    /// INPUT: Requested heater capacity
    Cpl::Dm::Mp::Uint32&    m_mpHeaterPWMOutput;

    /// INPUT: Requested fan speed
    Cpl::Dm::Mp::Uint32&    m_mpFanPWMOutput;

    /// Heater: Max PWM duty cycle value
    uint32_t                m_maxHeatingPWMValue;

    /// Heater: Max PWM duty cycle value
    uint32_t                m_maxFanPWMValue;
};

};      // end namespace(s)
};
};

#endif  // end header latch
