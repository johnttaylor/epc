#ifndef Ajax_Type_Alert_h_
#define Ajax_Type_Alert_h_
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

#include "Cpl/Type/enum.h"


///
namespace Ajax {
///
namespace Type {

/** This enumeration defines the name/identifiers for the individual Alerts

    @param Alert                            Enum

    @param eONBOARD_SENSOR_FAILED           The onboard temperature is not working
    @param eHITEMP_HEATER_FAILSAFE          The hardware high-temperature failsafe for the Heater has tripped
    @param NUM_ALERTS                       The total number of alert instances
 */
BETTER_ENUM( Alert, unsigned
             , eONBOARD_SENSOR_FAILED = 0
             , eHITEMP_HEATER_FAILSAFE
             , NUM_ALERTS
);

};      // end namespaces
};
#endif  // end header latch
