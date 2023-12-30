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

#include "Driver/AIO/HalSingleInput.h"
#include "HalSingleInput.h"
#include <stdint.h>

void driverAIOHalSingleInputArduino_initialize( int analogInputPin, eAnalogReference avref )
{
    analogReference( avref );
}

bool driverAIOHalSingleInputArduino_sample( int inputPinToSampleHdl, uint32_t& dstADCBits )
{
    dstADCBits = analogRead( inputPinToSampleHdl );
    return true;
}

uint8_t driverAIOHalSingleInputArduino_setADCSize( int inputToSampleHdl, uint8_t numADCBits )
{
    if ( numADCBits == 12 && HAS_DRIVER_AIO_ARDUINO_HAL_SINGLE_INPUT_12ADC )
    {
        analogReadResolution( numADCBits );
    }
    else if ( numADCBits == 16 && HAS_DRIVER_AIO_ARDUINO_HAL_SINGLE_INPUT_16ADC )
    {
        analogReadResolution( numADCBits );
    }
    
    // Arduino default is 10bit ADC
    else
    {
        numADCBits = 10;
    }

    return numADCBits;
}

