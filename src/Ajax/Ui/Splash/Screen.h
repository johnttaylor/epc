#ifndef Ajax_Ui_Splash_Screen_h_
#define Ajax_Ui_Splash_Screen_h_
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

#include "Ajax/ScreenMgr/StaticScreenApi.h"
#include "pimoroni/libraries/pico_graphics/pico_graphics.hpp"

/// 
namespace Ajax {
/// 
namespace Ui {
/// 
namespace Splash {


/** This class implements the splash screen
 */
class Screen: public Ajax::ScreenMgr::StaticScreenApi
{
public:
    /// Constructor
    Screen( pimoroni::PicoGraphics& graphics );

public:
    /// Ajax::ScreenMgr::StaticScreenApi
    void paint( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

protected:
    /// Graphic library handle
    pimoroni::PicoGraphics& m_graphics;
};

}       // end namespaces
}
}
#endif  // end header latch

