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


#ifndef FSM_AJAX_HEATING_SUPERVISOR_H
#define FSM_AJAX_HEATING_SUPERVISOR_H

/** \class Ajax::Heating::Supervisor::Fsm

Here is the graph that shows the state machine this class implements

\dot
digraph G {
rankdir=TB;
bgcolor=transparent;
node[fontsize=14 shape=box];
edge[fontsize=14; arrowsize=0.7];
compound=true;
remincross=true;
nodesep=0.5;
ranksep=0.5;
structOff->structFailedSafeOff[minlen=1 label=<evHiTemp&#47; <br ALIGN="LEFT"/>raiseHiTempAlert();>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Off FailedSafeOff
structFailedSafeOff->structOff[minlen=1 label=<evSafeTemp&#47; <br ALIGN="LEFT"/>clearHiTempAlert();>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)FailedSafeOff Off
structOff -> structHeating[minlen=1 labeldistance=2.0 , label=<evEnabled>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterOn];// (D4)Off On
structOff->structOff[minlen=1 label=<evSensorAvailable&#47; <br ALIGN="LEFT"/>clearSensorA...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Off Off
structHeating->structWaitingForSensor[minlen=1 label=<evNoTempSensor&#47; <br ALIGN="LEFT"/>raiseSensorAler...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Heating WaitingForSensor
structWaitingForSensor->structHeating[minlen=1 label=<evSensorAvailable&#47; <br ALIGN="LEFT"/>clearSensorA...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)WaitingForSensor Heating
struct__C0->structWaitingForSensor[minlen=1 label=<&#91;!isSensorAvailable()&#93;&#47; <br ALIGN="LEFT"/>raiseSe...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)__C0 WaitingForSensor
struct__C0->structHeating[minlen=1 label=<&#91;else&#93;>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)__C0 Heating
structFailedSafeOff->structFailedSafeOn[minlen=1 label=<evEnabled>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)FailedSafeOff FailedSafeOn
structDefaultOn->structFailedSafeOn[minlen=1 labeldistance=2.0 , headlabel=<evHiTemp&#47; <br ALIGN="LEFT"/>raiseHiTempAlert();<br ALIGN="LEFT"/>h...>  color=black, fontname=Arial; fontsize=14, fontcolor=black ltail=clusterOn];//  (I1)On structFailedSafeOn
structDefaultOn->struct__C0[minlen=1 label=<  > fontname=Arial; fontsize=14, style=dotted]; // (A) 
structFailedSafeOn->structFailedSafeOff[minlen=1 label=<evDisabled>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)FailedSafeOn FailedSafeOff
structFailedSafeOn -> structHeating[minlen=1 labeldistance=2.0 , label=<evSafeTemp&#47; <br ALIGN="LEFT"/>clearHiTempAlert();>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterOn];// (D4)FailedSafeOn On
structFailedSafeOff->structFailedSafeOff[minlen=1 label=<evSensorAvailable&#47; <br ALIGN="LEFT"/>clearSensorA...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)FailedSafeOff FailedSafeOff
structDefaultroot->structOff[minlen=1 label=<  > fontname=Arial; fontsize=14, style=dotted]; // (A) 
structFailedSafeOn->structFailedSafeOn[minlen=1 label=<evSensorAvailable&#47; <br ALIGN="LEFT"/>clearSensorA...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)FailedSafeOn FailedSafeOn
structOff->structOff[minlen=1 label=<evNoTempSensor&#47; <br ALIGN="LEFT"/>raiseSensorAler...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Off Off
structFailedSafeOn->structFailedSafeOn[minlen=1 label=<evNoTempSensor&#47; <br ALIGN="LEFT"/>raiseSensorAler...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)FailedSafeOn FailedSafeOn
structFailedSafeOff->structFailedSafeOff[minlen=1 label=<evNoTempSensor&#47; <br ALIGN="LEFT"/>raiseSensorAler...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)FailedSafeOff FailedSafeOff

structOff[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Off</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];


structFailedSafeOff[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">FailedSafeOff</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];




subgraph "cluster0"{ style=invis;label=""
subgraph "cluster1"{ style=invis;label=""
subgraph "cluster2"{ style=invis;label=""
subgraph "cluster3"{ style=invis;label=""
subgraph "cluster4"{ style=invis;label=""
subgraph "cluster5"{ style=invis;label=""
subgraph "cluster6"{ style=invis;label=""
subgraph "cluster7"{ style=invis;label=""
subgraph "clusterOn"{ margin=10 fontname=Arial; fontsize=14; fontsize=14
color=black; style=rounded;
 label=< <TABLE BORDER="0" ><TR><TD PORT="W"></TD><TD PORT="N"> On </TD><TD PORT="E"></TD></TR><HR/><TR><TD> </TD></TR>
</TABLE> >;

structHeating[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Heating</TD><TD PORT="E"></TD></TR>
<TR><TD COLSPAN="3" ALIGN="LEFT">Do:<br ALIGN="LEFT"/>runHeatingAlgo();<br ALIGN="LEFT"/></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];


structWaitingForSensor[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">WaitingForSensor</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];

structDefaultOn[shape=circle, fontsize=14, height=0.3, width=0.3, fixedsize=true, fontname=Arial; fontsize=14, fontcolor=white,fillcolor=black,  style=filled label=< <TABLE BORDER="0"><TR><TD PORT="NW"></TD> <TD PORT="N"></TD> <TD PORT="NE"></TD></TR><TR><TD PORT="W"> </TD> <TD BORDER="0" ALIGN="CENTER"> <FONT COLOR="white"> </FONT>  </TD> <TD PORT="E"></TD></TR><TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR></TABLE> >];struct__C0 [label = "" xlabel="__C0" fontcolor=black  shape=diamond, fixedsize=true, height=0.2, width=0.2, fontname=Arial; fontsize=14 color=black];
};
};
};
};
};
};
};
};
};


structFailedSafeOn[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">FailedSafeOn</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];

structDefaultroot[shape=circle, fontsize=14, height=0.3, width=0.3, fixedsize=true, fontname=Arial; fontsize=14, fontcolor=white,fillcolor=black,  style=filled label=< <TABLE BORDER="0"><TR><TD PORT="NW"></TD> <TD PORT="N"></TD> <TD PORT="NE"></TD></TR><TR><TD PORT="W"> </TD> <TD BORDER="0" ALIGN="CENTER"> <FONT COLOR="white"> </FONT>  </TD> <TD PORT="E"></TD></TR><TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR></TABLE> >];}
\enddot
*/

// User defined includes
#include <stdint.h>
#include <stdbool.h>

#include "Ajax/Heating/Supervisor/Fsm_ext_.h"
#include "Ajax/Heating/Supervisor/FsmContext_.h"

#ifndef DOXYGEN_WILL_SKIP_THIS

namespace Ajax { namespace Heating { namespace Supervisor  {

    class Fsm: public FsmContext_
    {
        public:
            explicit Fsm(void);


            int processEvent(const FSM_EVENT_T msg);

            void initialize(void);

            // Helper(s) to reset history
            void resetHistoryOn(void);

            // Helper(s) to find out if the machine is in a certain state
            bool isInOff(void) const;
            bool isInOn(void) const;
            bool isInHeating(void) const;
            bool isInFailedSafeOn(void) const;
            bool isInFailedSafeOff(void) const;
            bool isInWaitingForSensor(void) const;


            // Helper returning the innermost active state id.
            enum States{
                Off,
                On,
                Heating,
                FailedSafeOn,
                FailedSafeOff,
                WaitingForSensor,
                NUM_STATES  // number of states in the machine
            };

            States getInnermostActiveState(void) const;


            // Returns the state name as string
            static const char* getNameByState(const unsigned short state);

            // Returns the event as string
            static const char* getNameByEvent(const FSM_EVENT_T evt);

        protected:


            int m_initialized;



        protected:
            // State variables
            struct stateVarsT{
                States stateVar;
                States stateVarOn;
            };

            stateVarsT stateVars;
            stateVarsT stateVarsCopy;






    };

}}} //namespace Ajax { namespace Heating { namespace Supervisor 


#endif // !DOXYGEN_WILL_SKIP_THIS

#endif // __FSM_AJAX_HEATING_SUPERVISOR_H__

