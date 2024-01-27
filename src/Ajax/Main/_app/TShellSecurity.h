#ifndef Ajax_Main_app_TShell_Security_h_
#define Ajax_Main_app_TShell_Security_h_
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
/** @file */


#include "colony_config.h"
#include "Cpl/TShell/Security.h"
#include "Driver/Crypto/Hash.h"


/// The one and only valid user account
#ifndef OPTION_AJAX_MAIN_TSHELL_SECURITY_USER_NAME
#define OPTION_AJAX_MAIN_TSHELL_SECURITY_USER_NAME  "dilbert"
#endif


///
namespace Ajax {
///
namespace Main {

/// Validates the TShell login password
class TShellSecurity: public Cpl::TShell::Security
{
public:
    /// Constructor
    TShellSecurity( Driver::Crypto::Hash& sha512HashFunciton ): m_sha512( sha512HashFunciton) {}

public:
    /// Cpl::TShell::Security
    Permission_T validateCredentials( const char* username, const char* password ) noexcept;

protected:
    /// Hash function
    Driver::Crypto::Hash& m_sha512;
};


};      // end namespaces
};
#endif  // end header latch
