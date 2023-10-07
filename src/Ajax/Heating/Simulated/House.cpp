/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2020  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "House.h"
#include "ModelPoints.h"
#include "mp/ModelPoints.h"
#include "Cpl/System/Trace.h"

using namespace Ajax::Heating::Simulated;

#define SECT_ "Ajax::Heating::Simulated"

#define RESISTANCE_NO_CAPACITY          50
#define RESISTANCE_COOLING_CAPCITY      (RESISTANCE_NO_CAPACITY*2.0/3.0)
#define RESISTANCE_HEATING_CAPCITY      (RESISTANCE_COOLING_CAPCITY*3.0)

//////////////////////
House::House( Cpl::Dm::MailboxServer&  myMbox,
              Cpl::Dm::Mp::Bool&       mpSimEnabled,
              Cpl::Dm::Mp::Double&     mpOutdoorTemperature )
    : Api( myMbox )
    , m_sim( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS / 1000.0    // Convert to seconds
             , 70.0                         // ODT
             , 120.0                        // max odt
             , -20.0                        // min odt
             , 0.33                         // ODT Cooling load rating
             , 0.80                         // ODT Heating load rating
             , RESISTANCE_NO_CAPACITY       // systemEnvResistance
             , RESISTANCE_COOLING_CAPCITY   // systemCoolingEnvResistance
             , RESISTANCE_HEATING_CAPCITY ) // systemHeatingEnvResistance
    , m_mpSimEnabled( mpSimEnabled )
    , m_mpOutdoorTemperature( mpOutdoorTemperature )
{
}


void House::intervalExpired() noexcept
{
    executeSimulation();
    Api::intervalExpired(); // Call my parent to run the algorithm
}

void House::executeSimulation()
{
    double    odt;
    bool      simEnabled;
    uint32_t  heaterPWM;
    uint32_t  fanPWM;
    if ( m_mpOutdoorTemperature.read( odt ) == true &&
         mp::cmdHeaterPWM.read( heaterPWM ) == true &&
         mp::cmdFanPWM.read( fanPWM ) == true &&
         m_mpSimEnabled.read( simEnabled ) == true && simEnabled )
    {
        
        // Heating capacity
        bool   safetyTrip;
        double heater = heaterPWM / 65536.0;
        if ( mp::hwSafetyLimit.read(safetyTrip) && safetyTrip  )
        {
            // If there is temperature safety limit trip -->then the actual heat 
            // output is ABOVE the designed limit (but the algorithm has turned 
            // off the command PWM value)
            heater = 1.0;
        }
        
        // Final capacity is function of the fan speed
        double fan       = fanPWM / 65536.0;
        double capacity  = heater * fan;
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "hpwm=%lu, fpwm=%lu, capacity=%g, heater=%g, fan=%g", 
                                       heaterPWM,
                                       fanPWM,
                                       capacity,
                                       heater, 
                                       fan 
                                       ));

        // Run the simulation
        double idt = m_sim.tick( odt, capacity, false );

        // Update Indoor temperature (convert to hundredths of a degree)
        mp::onBoardIdt.write( (int32_t) ((idt + 0.5)* 100) );
    }
}
