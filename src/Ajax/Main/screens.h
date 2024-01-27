#ifndef Ajax_Main_screens_h
#define Ajax_Main_screens_h
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
/** @file

    This file exposes handles to Screen instances for the Ajax Application
 */

#include "Ajax/Ui/Home/Screen.h"
#include "Ajax/Ui/About/Screen.h"
#include "Ajax/Ui/EditSetpt/Screen.h"
#include "Ajax/Ui/Error/Screen.h"

///
namespace Ajax {
///
namespace Main {

/// Home Screen
extern Ajax::Ui::Home::Screen       g_homeScreen_;

/// Edit setpoint Screen
extern Ajax::Ui::EditSetpt::Screen  g_editSetptScreen_;

/// About Screen
extern Ajax::Ui::About::Screen      g_aboutScreen_;

/// Error Screen
extern Ajax::Ui::Error::Screen      g_errorScreen_;

}       // end namespaces
}
#endif  // end header latch
