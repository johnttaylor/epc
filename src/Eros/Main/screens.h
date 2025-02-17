#ifndef Eros_Main_screens_h
#define Eros_Main_screens_h
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

    This file exposes handles to Screen instances for the Eros Application
 */

#include "Eros/Ui/Home/Screen.h"
#include "Eros/Ui/LcdTest/Screen.h"

///
namespace Eros {
///
namespace Main {

/// Home Screen
extern Eros::Ui::Home::Screen    g_homeScreen;

/// LCd TestScreen
extern Eros::Ui::LcdTest::Screen g_lcdTextScreen;


}       // end namespaces
}
#endif  // end header latch
