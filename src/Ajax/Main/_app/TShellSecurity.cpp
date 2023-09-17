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

#include "TShellSecurity.h"
#include "Ajax/Constants.h"
#include "Cpl/System/Api.h"
#include "mp/ModelPoints.h"
#include "Driver/Crypto/PasswordHash/Api.h"
#include <string.h>
#include <stdint.h>

using namespace Ajax::Main;

#define ENFORCED_DELAY_MS       2000


///////////////////
Cpl::TShell::Security::Permission_T TShellSecurity::validateCredentials( const char* username, const char* password ) noexcept
{
    // Filter out bad user names and password lengths
    size_t passwordLen = strlen( password );
    if ( strcmp( username, OPTION_AJAX_MAIN_TSHELL_SECURITY_USER_NAME ) != 0 ||
         passwordLen > OPTION_AJAX_PASSWORD_MAX_LENGTH ||
         passwordLen < OPTION_AJAX_PASSWORD_MIN_LENGTH )
    {
        Cpl::System::Api::sleep( ENFORCED_DELAY_MS );
        return ePUBLIC;
    }

    // Get the salt
    uint8_t salt[OPTION_AJAX_HASHED_PASSWORD_SALT_SIZE];
    if ( !mp::consolePwdSalt.read( salt, sizeof(salt) ) )
    {
        Cpl::System::Api::sleep( ENFORCED_DELAY_MS );
        return ePUBLIC;
    }

    // Hash the password
    uint8_t workBuffer[OPTION_AJAX_PASSWORD_MAX_LENGTH + OPTION_AJAX_HASHED_PASSWORD_SIZE];
    uint8_t workDigest[OPTION_AJAX_HASHED_PASSWORD_SIZE];
    uint8_t outputBuffer[OPTION_AJAX_HASHED_PASSWORD_SIZE];
    DriverCryptoStatus_T result = Driver::Crypto::Password::hash( password,
                                                                  passwordLen,
                                                                  salt,
                                                                  sizeof( salt ),
                                                                  workBuffer,
                                                                  sizeof( workBuffer),
                                                                  workDigest,
                                                                  sizeof( workDigest),
                                                                  m_sha512,
                                                                  OPTION_AJAX_HASHED_PASSWORD_ITERATIONS,
                                                                  outputBuffer,
                                                                  sizeof(outputBuffer) );
    if ( result != DRIVER_CRYPTO_SUCCESS )
    {
        return ePUBLIC;
    }

    // Get and compare the expected hash
    if ( !mp::consolePwdHash.read( workDigest, sizeof( workDigest ) ) ||
         memcmp( workDigest, outputBuffer, sizeof(workDigest)) != 0 )
    {
        return ePUBLIC;
    }

    // If I get here -->the password is valid
    return eADMIN;
}
