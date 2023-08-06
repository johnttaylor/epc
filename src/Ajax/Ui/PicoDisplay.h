#ifndef Ajax_Ui_PicoDisplay_h_
#define Ajax_Ui_PicoDisplay_h_
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


#include "Ajax/ScreenMgr/DisplayApi.h"
#include "Driver/PicoDisplay/Api.h"

/// 
namespace Ajax {
/// 
namespace Ui {


 
/** This concrete class implements the Ajax::ScreenMgr::DisplayApi using the
    Driver::PicoDisplay driver
 */
class PicoDisplay : public Ajax::ScreenMgr::DisplayApi
{
public:
    /// Constructor
    PicoDisplay( pimoroni::PicoGraphics& graphicsLibrary )
        : m_graphicsLibrary( graphicsLibrary )
    {
    }

public:
    /// See Ajax::ScreenMsg::DisplayApi
    bool start() noexcept
    {
        // The driver is required to be initialized by the application prior to starting the UI sub-system
        return true;
    }
    
    /// See Ajax::ScreenMsg::DisplayApi
    void stop() noexcept
    {
        // Do nothing - Driver does not support stop/shutdown
    }
    
    /// See Ajax::ScreenMsg::DisplayApi
    bool update() noexcept
    {
        Driver::PicoDisplay::Api::updateLCD( m_graphicsLibrary );
        return true;
    }
    
    /// See Ajax::ScreenMsg::DisplayApi
    bool turnOff() noexcept
    {
        // Do nothing - NOT support by the driver
        return true;
    }
    
    /// See Ajax::ScreenMsg::DisplayApi
    bool turnOn() noexcept
    {
        // Do nothing - NOT support by the driver
        return true;
    }


protected:
    /// Graphic library instance
    pimoroni::PicoGraphics&   m_graphicsLibrary;
};

}      // end namespaces
}
#endif  // end header latch

