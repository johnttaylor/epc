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

#include "Driver/Button/Hal.h"
#include "Hal.h"
#include <stdint.h>

void driverButtonHalArduino_initialize( DriverButtonPinHalArduino_T buttonHdl )
{
    pinMode( buttonHdl.pin, buttonHdl.mode );
}

bool driverButtonHalArduino_getRawPressState( DriverButtonPinHalArduino_T pinHandle )
{
    uint32_t phy = digitalRead( pinHandle.pin );
    bool     log = phy == HIGH ? true : false;
    return pinHandle.activeLow ? !log : log;
}
