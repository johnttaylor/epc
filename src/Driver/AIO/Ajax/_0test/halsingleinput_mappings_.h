#ifndef Driver_AIO_Ajax_0test_halsingleinput_mappings_x_h_
#define Driver_AIO_Ajax_0test_halsingleinput_mappings_x_h_
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

    This file provides the mappings for unit test specific types, etc. for the
    Driver::AIO::HalSingleInput HAL

 */

#include <stdint.h>

 /// Unit test Mapping
#define DriverAIOHalSingleInput_T_MAP                   int  

#define Driver_AIO_HalSingleInput_sample_MAP            mockedHalSingleInput
#define Driver_AIO_HalSingleInput_setADCSize_MAP        mockedHalSingleInput_setADCSize
/// Unit test Mapping
bool mockedHalSingleInput( int, uint32_t& dstADCBits );

/// Unit test Mapping
uint8_t mockedHalSingleInput_setADCSize( int, uint8_t numADCBits );


#endif  // end header latch
