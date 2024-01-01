#ifndef Driver_AIO_Eros_ModelPoints_h_
#define Driver_AIO_Eros_ModelPoints_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015-2023  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file

    This file defines all of the Model Points for Ajax application.

    All of the model points are placed into the 'mp' namespace.  The model
    point names map 1-to-1 with the instance names.

    The Application/Client is RESPONSIBLE for ensuring input values are with
    the defined range for the model point instance.
 */

#include "Cpl/Dm/ModelDatabase.h"
#include "Cpl/Dm/Mp/Array.h"

/** Encapsulate all Model Points in the 'mp' namespace to prevent polluting
    the global name space
 */
namespace mp {

/** This model point holds the low level ADC Bit value and its corresponding
    Temperature value in degrees Fahrenheit.

    \b Units: Array

    \b Range: Index[0] = ADC Bits
              Index[1] = Temperature 'F

    \b Notes: 
        n/a
*/
extern Cpl::Dm::Mp::ArrayInt32<2> erosThermistorSample;


/*---------------------------------------------------------------------------*/
/// The Application's Model Point Database
extern Cpl::Dm::ModelDatabase   g_modelDatabase;


}       // end namespace
#endif  // end header latch
