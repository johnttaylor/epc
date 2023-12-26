#ifndef Ajax_Ui_Error_errors_h_
#define Ajax_Ui_Error_errors_h_
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

/// 
namespace Ajax {
/// 
namespace Ui {
/// 
namespace Error {


/// Error Code for Alert
static constexpr const char* errCode_NOT_PROVISIONED = "Error Code: 01";

/// Error Message for Alert
static constexpr const char* errMsg_NOT_PROVISIONED  = "Manufacture error";


/// Error Code for Alert
static constexpr const char* errCode_POST_FAILURE    = "Error Code: 02";

/// Error Message for Alert
static constexpr const char* errMsg_POST_FAILURE     = "Hardware error";


/// Error Code for Alert - for all other Alerts (in theory should not happen)
static constexpr const char* errCode_OTHER_FAILURES  = "Error Code: 99";

/// Error Code for Alert - for all other Alerts (in theory should not happen)
/// Error Message for Alert
static constexpr const char* errMsg_OTHER_FAILURES   = "Software error";

}       // end namespaces
}
}
#endif  // end header latch

