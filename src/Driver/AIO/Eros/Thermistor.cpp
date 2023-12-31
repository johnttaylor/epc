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

#include "Thermistor.h"
#include "ModelPoints.h"

#define SECT_ "Driver::AIO::Eros"


using namespace Driver::AIO::Eros;


////////////////////////////////////////////////
Thermistor::Thermistor( Cpl::Dm::MailboxServer &    myMbox,
                        DriverAIOHalSingleInput_T   adcDriver,
                        Cpl::Dm::Mp::Int32&         mpTemperatureOutput )
    : Driver::AIO::Ajax::Thermistor( myMbox, adcDriver, mpTemperatureOutput )
{
}


void Thermistor::hookAdcSample( uint32_t adcBits, int32_t temperatureF )
{
    int32_t values[2] ={ (int32_t) adcBits, temperatureF };
    mp::erosThermistorSample.write( values, 2 );
}
