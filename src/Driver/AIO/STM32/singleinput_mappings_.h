#ifndef Driver_AIO_STM32_halsingleinput_mappings_x_h_
#define Driver_AIO_STM32_halsingleinput_mappings_x_h_
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


/// STM32 Mapping
#define DriverAIOHalSingleInput_T_MAP               int  


/// STM32 Mapping
#define Driver_AIO_HalSingleInput_sample_MAP        driverAIOHalSingleInputStm32_sample

/// STM32 Mapping
#define Driver_AIO_HalSingleInput_setADCSize_MAP    driverAIOHalSingleInputStm32_setADCSize

/// STM32 specific implementation
bool driverAIOHalSingleInputStm32_sample( int inputToSampleHdl, uint32_t& dstADCBits );

/// STM32 specific implementation
uint8_t driverAIOHalSingleInputStm32_setADCSize( int inputPinToSampleHdl, uint8_t numADCBits );

#endif  // end header latch
