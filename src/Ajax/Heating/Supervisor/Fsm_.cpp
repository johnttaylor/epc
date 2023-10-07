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
/** @file */

/* Command line options: -p CADIFRA -doxygen -o Fsm -l cppx -Trace Fsm.cdd   */
/* This file is generated from Fsm.cdd - do not edit manually  */
/* Generated on:  Version 6.0.2.3601 */


/* Include default headers (no header definition in UML state-chart provided) */
/* Define a header comment in your statediagram.model to define own includes here. */
#include <cstdint>
#include "Ajax/Heating/Supervisor/Fsm_ext_.h"
#include "Ajax/Heating/Supervisor/Fsm_.h"
#include "Ajax/Heating/Supervisor/Fsm_trace_.h"

namespace Ajax { namespace Heating { namespace Supervisor  {

    Fsm::Fsm(void)
    {
        m_initialized=static_cast<int>(0U);
    }



    /* State names */
    const char states[]=
        "Off\0On\0Heating\0FailedSafeOn\0FailedSafeOff\0WaitingForSensor\0";

    const unsigned short state_idx[]={
        0,4,7,15,28,42,59};

    /* Event names */
    const char events[]=
        "Fsm_evEnabled\0Fsm_evHiTemp\0Fsm_evNoTempSensor\0Fsm_evDisabled\0Fsm_evSafeTemp\0Fsm_evSensorAvailable\0NO_MSG\0";

    const unsigned short evt_idx[]={
        0,14,27,46,61,76,98};

    const char* Fsm::getNameByState(const unsigned short state) {
        return (states+state_idx[state]);
    }

    const char* Fsm::getNameByEvent(const FSM_EVENT_T evt) {
        return (events+evt_idx[evt]);
    }


    // Helper(s) to reset history
    void Fsm::resetHistoryOn(void){
        stateVars.stateVarOn=Heating;
    }


    // Helper(s) to find out if the machine is in a certain state

    bool Fsm::isInOff(void) const {return(((stateVars.stateVar== Off)) ? (true) : (false));}
    bool Fsm::isInOn(void) const {return(((stateVars.stateVar== On)) ? (true) : (false));}
    bool Fsm::isInHeating(void) const {return(((stateVars.stateVarOn== Heating)&&(stateVars.stateVar== On)) ? (true) : (false));}
    bool Fsm::isInFailedSafeOn(void) const {return(((stateVars.stateVar== FailedSafeOn)) ? (true) : (false));}
    bool Fsm::isInFailedSafeOff(void) const {return(((stateVars.stateVar== FailedSafeOff)) ? (true) : (false));}
    bool Fsm::isInWaitingForSensor(void) const {return(((stateVars.stateVarOn== WaitingForSensor)&&(stateVars.stateVar== On)) ? (true) : (false));}

    // Helper to get id of innermost active state

    Fsm::States Fsm::getInnermostActiveState(void) const {

        Fsm::States state = NUM_STATES;

        if(isInWaitingForSensor()){
            state = WaitingForSensor;
        }else if(isInHeating()){
            state = Heating;
        }else if(isInFailedSafeOff()){
            state = FailedSafeOff;
        }else if(isInFailedSafeOn()){
            state = FailedSafeOn;
        }else if(isInOff()){
            state = Off;
        }else{
            // intentionally left blank
        }
        return (state);
    }

    // Initialize method. Must be called once to init the machine
    void Fsm::initialize(void){
        FsmTraceEvent(6U);
        //call on entry code of default states
        if(m_initialized==static_cast<int>(0U)){
            m_initialized=static_cast<int>(1U);

            //Create copy of statevar
            stateVarsCopy = stateVars;

            // Set state vars to default states

            stateVarsCopy.stateVar = Off; /* set init state of top state */
            stateVarsCopy.stateVarOn = Heating; /* set init state of On */




            // Copy state variables back
            stateVars = stateVarsCopy;

        }

    }


    // State machine event handler
    int Fsm::processEvent(const FSM_EVENT_T msg){

        int evConsumed = 0U;

    

        if(m_initialized==static_cast<int>(0U)){
            return static_cast<int>(0);
        }

        //Create copy of statevar
        stateVarsCopy = stateVars;


        switch (stateVars.stateVar) {

            case Off:
                if(msg==Fsm_evEnabled){
                    /* Transition from Off to On */
                    evConsumed=1U;

                    stateVarsCopy.stateVar = On; /* Default in entry chain  */
                    
                    /* Init ends in choice, select default state based on given guards  */
                    if(!isSensorAvailable()){
                        allOff();
                        stateVarsCopy.stateVarOn = WaitingForSensor; /* Selective default in entry chain  */
                    }else{
                        stateVarsCopy.stateVarOn = Heating; /* Selective default in entry chain  */
                    }

                    FsmTraceEvent(2U);
                }else if(msg==Fsm_evHiTemp){
                    /* Transition from Off to FailedSafeOff */
                    evConsumed=1U;

                    /* Action code for transition  */
                    fanOn();


                    /* adjust state variables  */
                    stateVarsCopy.stateVar = FailedSafeOff;
                    FsmTraceEvent(0U);
                }else{
                    /* Intentionally left blank */
                } /*end of event selection */
            break; /* end of case Off  */

            case FailedSafeOff:
                if(msg==Fsm_evEnabled){
                    /* Transition from FailedSafeOff to FailedSafeOn */
                    evConsumed=1U;


                    /* adjust state variables  */
                    stateVarsCopy.stateVar = FailedSafeOn;
                    FsmTraceEvent(2U);
                }else if(msg==Fsm_evSafeTemp){
                    /* Transition from FailedSafeOff to Off */
                    evConsumed=1U;

                    /* Action code for transition  */
                    fanOff();


                    /* adjust state variables  */
                    stateVarsCopy.stateVar = Off;
                    FsmTraceEvent(1U);
                }else{
                    /* Intentionally left blank */
                } /*end of event selection */
            break; /* end of case FailedSafeOff  */

            case On:

                switch (stateVars.stateVarOn) {

                    case Heating:
                        /* action code  */
                        runHeatingAlgo();


                        if(msg==Fsm_evNoTempSensor){
                            /* Transition from Heating to WaitingForSensor */
                            evConsumed=1U;

                            /* Action code for transition  */
                            allOff();


                            /* adjust state variables  */
                            stateVarsCopy.stateVarOn = WaitingForSensor;
                            FsmTraceEvent(4U);
                        }else{
                            /* Intentionally left blank */
                        } /*end of event selection */
                    break; /* end of case Heating  */

                    case WaitingForSensor:
                        /* action code  */
                        checkForSensor();


                        if(msg==Fsm_evSensorAvailable){
                            /* Transition from WaitingForSensor to Heating */
                            evConsumed=1U;


                            /* adjust state variables  */
                            stateVarsCopy.stateVarOn = Heating;
                            FsmTraceEvent(5U);
                        }else{
                            /* Intentionally left blank */
                        } /*end of event selection */
                    break; /* end of case WaitingForSensor  */

                    default:
                        /* Intentionally left blank */
                    break;
                } /* end switch On */

                /* Check if event was already processed  */
                if(evConsumed==0){

                    if(msg==Fsm_evDisabled){
                        /* Transition from On to Off */
                        evConsumed=1U;
                        
                        /* Action code for transition  */
                        allOff();


                        /* adjust state variables  */
                        stateVarsCopy.stateVar = Off;
                        FsmTraceEvent(3U);
                    }else if(msg==Fsm_evHiTemp){
                        /* Transition from On to FailedSafeOn */
                        evConsumed=1U;
                        
                        /* Action code for transition  */
                        heatOff();


                        /* adjust state variables  */
                        stateVarsCopy.stateVar = FailedSafeOn;
                        FsmTraceEvent(0U);
                    }else{
                        /* Intentionally left blank */
                    } /*end of event selection */
                }
            break; /* end of case On  */

            case FailedSafeOn:
                if(msg==Fsm_evDisabled){
                    /* Transition from FailedSafeOn to FailedSafeOff */
                    evConsumed=1U;


                    /* adjust state variables  */
                    stateVarsCopy.stateVar = FailedSafeOff;
                    FsmTraceEvent(3U);
                }else if(msg==Fsm_evSafeTemp){
                    /* Transition from FailedSafeOn to On */
                    evConsumed=1U;

                    stateVarsCopy.stateVar = On; /* Default in entry chain  */
                    
                    /* Init ends in choice, select default state based on given guards  */
                    if(!isSensorAvailable()){
                        allOff();
                        stateVarsCopy.stateVarOn = WaitingForSensor; /* Selective default in entry chain  */
                    }else{
                        stateVarsCopy.stateVarOn = Heating; /* Selective default in entry chain  */
                    }

                    FsmTraceEvent(1U);
                }else{
                    /* Intentionally left blank */
                } /*end of event selection */
            break; /* end of case FailedSafeOn  */

            default:
                /* Intentionally left blank */
            break;
        } /* end switch stateVar_root */

        // Copy state variables back
        stateVars = stateVarsCopy;
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "  New State=%s", getNameByState(getInnermostActiveState()) ));

        return (evConsumed);
    } // end processEvent







}}} //namespace Ajax { namespace Heating { namespace Supervisor 

