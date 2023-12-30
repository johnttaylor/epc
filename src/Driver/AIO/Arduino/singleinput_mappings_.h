#ifndef Driver_AIO_Arduino_halsingleinput_mappings_x_h_
#define Driver_AIO_Arduino_halsingleinput_mappings_x_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2022  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file
 */

#include <stdint.h>


/// Arduino Mapping
#define DriverAIOHalSingleInput_T_MAP               int  


/// Arduino Mapping
#define Driver_AIO_HalSingleInput_sample_MAP        driverAIOHalSingleInputArduino_sample

/// Arduino Mapping
#define Driver_AIO_HalSingleInput_setADCSize_MAP    driverAIOHalSingleInputArduino_setADCSize

/// Arduino specific implementation
bool driverAIOHalSingleInputArduino_sample( int inputToSampleHdl, uint32_t& dstADCBits );

/// Arduino specific implementation
uint8_t driverAIOHalSingleInputArduino_setADCSize( int inputPinToSampleHdl, uint8_t numADCBits );

#endif  // end header latch
