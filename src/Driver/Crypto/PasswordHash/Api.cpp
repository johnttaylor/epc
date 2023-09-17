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
#include <string.h>


DriverCryptoStatus_T Driver::Crypto::Password::hash( const char*           plaintext,
                                                     size_t                plaintextLength,
                                                     const void*           salt,
                                                     size_t                saltLength,
                                                     uint8_t*              workBuffer,
                                                     size_t                workBufferLength,
                                                     uint8_t*              workDigest,
                                                     size_t                workDigestLength,
                                                     Driver::Crypto::Hash& hf,
                                                     size_t                numIterations,
                                                     void*                 dstOutputBuffer,
                                                     size_t                dstOutputBufferLen ) noexcept
{
    // Validate sizes
    if ( dstOutputBufferLen < hf.digestSize() )
    {
        return DRIVER_CRYPTO_PASSWORD_OUTPUT_BAD_SIZE;
    }
    if ( (plaintextLength + saltLength) > workBufferLength ||
         (plaintextLength + hf.digestSize()) > workBufferLength )
    {
        return DRIVER_CRYPTO_PASSWORD_WORK_BUFFER_BAD_SIZE;
    }
    if ( workDigestLength < hf.digestSize() )
    {
        return DRIVER_CRYPTO_PASSWORD_WORK_DIGEST_BAD_SIZE;
    }

    //
    // First Iteration
    //
    // x = plaintext + salt
    memcpy( workBuffer, plaintext, plaintextLength );
    memcpy( workBuffer + plaintextLength, salt, saltLength );

    // out = u = hf( x ).digest()
    hf.reset();
    hf.accumulate( workBuffer, plaintextLength + saltLength );
    if ( hf.finalize( workDigest, workDigestLength ) )
    {
        return DRIVER_CRYPTO_PASSWORD_HASH_FUNCTION_ERROR;
    }
    memcpy( dstOutputBuffer, workDigest, hf.digestSize() );

    // 
    // Perform N-1 iteration
    //
    for ( size_t c=1; c < numIterations; c++ )
    {
        // x = plaintext + u
        memcpy( workBuffer, plaintext, plaintextLength );
        memcpy( workBuffer + plaintextLength, workDigest, hf.digestSize() );

        // u = hf( x ).digest()
        hf.reset();
        hf.accumulate( workBuffer, plaintextLength + hf.digestSize() );
        if ( hf.finalize( workDigest, workDigestLength ) )
        {
            return DRIVER_CRYPTO_PASSWORD_HASH_FUNCTION_ERROR;
        }

        // XOR the current 'u' with 'u-1'
        // out = bytes( out ^ u for ( out, u ) in zip( out, u ) )
        uint8_t* out = (uint8_t*) dstOutputBuffer;
        for ( size_t idx=0; idx < hf.digestSize(); idx++ )
        {
            out[idx] ^= workDigest[idx];
        }
    }

    return DRIVER_CRYPTO_PASSWORD_SUCCESS;
}

/*
def calculate_pwd( plaintext_string, store_guid_string ):
    plaintext_bytes = bytes( plaintext_string, "utf-8" )
    store_id_bytes  = uuid.UUID( store_guid_string ).bytes

    # Calculate U1
    x   = plaintext_bytes + store_id_bytes
    u   = hashlib.md5( x ).digest()
    out = u

    # Calculate U2...U128
    for i in range(127):
        x = plaintext_bytes + u
        u = hashlib.md5( x ).digest()

        # XOR the current 'u' with 'u-1'
        out = bytes(out ^ u for (out, u) in zip(out, u))

    return out.hex()
*/
