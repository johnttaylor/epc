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

using namespace Ajax::Heating::Simulated;


#define RESISTANCE_NO_CAPACITY          50
#define RESISTANCE_COOLING_CAPCITY      (RESISTANCE_NO_CAPACITY*2.0/3.0)
#define RESISTANCE_HEATING_CAPCITY      (RESISTANCE_COOLING_CAPCITY*3.0)

//////////////////////
House::House( Cpl::Dm::Mp::Bool&                   mpSimEnabled,
              Cpl::Dm::Mp::Int32&                  mpIndoorTemperature,
              Cpl::Dm::Mp::Float&                  mpOutdoorTemperature,
              Cpl::Dm::Mp::Uint32&                 mpHeaterPWMOutput,
              uint32_t                             maxHeatingPWMValue,
              Cpl::Dm::Mp::Uint32&                 mpFanPWMOutput,
              uint32_t                             maxFanPWMValue,
              unsigned long                        timeOutPeriodInMsec,
              Cpl::System::SharedEventHandlerApi*  eventHandler )
    : EventLoop( timeOutPeriodInMsec, eventHandler )                        // Wake up once a second
    , m_sim( OPTION_AJAX_HEATING_SIMULATED_HOUSE_SIM_TIMING_MS / 1000.0     // Convert to seconds
             , 70.0                         // ODT
             , 120.0                        // max odt
             , -20.0                        // min odt
             , 0.33                         // ODT Cooling load rating
             , 0.80                         // ODT Heating load rating
             , RESISTANCE_NO_CAPACITY       // systemEnvResistance
             , RESISTANCE_COOLING_CAPCITY   // systemCoolingEnvResistance
             , RESISTANCE_HEATING_CAPCITY ) // systemHeatingEnvResistance
    , m_mpIdt( mpIndoorTemperature )
    , m_mpSimEnabled( mpSimEnabled )
    , m_mpOutdoorTemperature( mpOutdoorTemperature )
    , m_mpHeaterPWMOutput( mpHeaterPWMOutput )
    , m_mpFanPWMOutput( mpFanPWMOutput )
    , m_maxHeatingPWMValue( maxHeatingPWMValue )
    , m_maxFanPWMValue( maxFanPWMValue )
{
}


void House::appRun()
{
    startEventLoop();
    unsigned long timeMark = Cpl::System::ElapsedTime::milliseconds();

    bool run = true;
    while ( run )
    {
        run = waitAndProcessEvents();
        if ( run )
        {
            unsigned long timeNow = Cpl::System::ElapsedTime::milliseconds();
            while ( Cpl::System::ElapsedTime::deltaMilliseconds( timeMark, timeNow ) >= OPTION_AJAX_HEATING_SIMULATED_HOUSE_SIM_TIMING_MS )
            {
                timeMark += OPTION_AJAX_HEATING_SIMULATED_HOUSE_SIM_TIMING_MS;
                executeSimulation();
            }
        }
    }
}

void House::executeSimulation()
{
    float     odt;
    bool      simEnabled;
    uint32_t  heaterPWM;
    uint32_t  fanPWM;
    if ( m_mpOutdoorTemperature.read( odt ) == true &&
         m_mpHeaterPWMOutput.read( heaterPWM ) == true &&
         m_mpFanPWMOutput.read( fanPWM ) == true &&
         m_mpSimEnabled.read( simEnabled ) == true && simEnabled )
    {

        // Heating capacity
        double heater    = ((double) heaterPWM) / ((double) m_maxHeatingPWMValue);
        double fan       = ((double) fanPWM) / ((double) m_maxFanPWMValue);
        double capacity  = heater * fan;

        // Run the simulation
        double idt = m_sim.tick( odt, capacity, false );

        // Update Indoor temperature (convert to hundredths of a degree)
        m_mpIdt.write( (int32_t) ((idt + 0.5)* 100) );
    }
}
