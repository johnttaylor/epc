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


#include "Api.h"
#include "Driver/Crypto/Random.h"
#include <stdint.h>

DriverCryptoStatus_T Driver::Crypto::generateRandom( void* dstBuffer, size_t numBytesToGenerate ) noexcept
{
    if ( dstBuffer == nullptr )
    {
        return 1;
    }

    uint8_t* ptr     = (uint8_t*) dstBuffer;
    uint32_t rnum    = TRNG->DATA.reg; // Read random number;
    uint8_t  byteNum = 0;
    while ( numBytesToGenerate )
    {
        // Get a new random DWORD
        if ( byteNum == 4 )
        {
            rnum    = TRNG->DATA.reg;
            byteNum = 0;
        }

        *ptr++ = (rnum >> (byteNum * 8)) & 0xFF;
        byteNum++;
        numBytesToGenerate--;
    }

    return DRIVER_CRYPTO_SUCCESS;
}
