#ifndef Eros_Ui_Home_Screen_h_
#define Eros_Ui_Home_Screen_h_
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

#include "pimoroni/libraries/pico_graphics/pico_graphics.hpp"
#include "Ajax/ScreenMgr/ScreenApi.h"
#include "Ajax/ScreenMgr/Navigation.h"

/// 
namespace Eros {
/// 
namespace Ui {
/// 
namespace Home {


/** This class implements the Home screen
 */
class Screen : public Ajax::ScreenMgr::ScreenApi
{
public:
    /// Constructor
    Screen( Ajax::ScreenMgr::Navigation&    screenMgr, 
            pimoroni::PicoGraphics&         graphics );

public:
    /// See Ajax::ScreenMgr::ScreenApi
    void enter( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    void exit( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    void sleep( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    void wake( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    void dispatch( AjaxScreenMgrEvent_T event, Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    void tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

    /// See Ajax::ScreenMgr::ScreenApi
    bool refresh( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept;

protected:
    /// Handle to the screen manager
    Ajax::ScreenMgr::Navigation&    m_screenMgr;

    /// Graphic library handle
    pimoroni::PicoGraphics&         m_graphics;
};

}       // end namespaces
}
}
#endif  // end header latch
