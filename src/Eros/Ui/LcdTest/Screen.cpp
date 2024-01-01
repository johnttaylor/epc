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

#include "Screen.h"

using namespace Eros::Ui::LcdTest;



#define SET_PEN_RED()        m_graphics.set_pen( 255, 0, 0 );        
#define SET_PEN_GREEN()      m_graphics.set_pen( 0, 255, 0 );        
#define SET_PEN_BLUE()       m_graphics.set_pen( 0, 0, 255);        
#define SET_PEN_WHITE()      m_graphics.set_pen( 255, 255, 255 );        
#define SET_PEN_BLACK()      m_graphics.set_pen( 0, 0, 0  );   

///////////////////////////
Screen::Screen( Ajax::ScreenMgr::Navigation&    screenMgr,
                pimoroni::PicoGraphics&         graphics )
    : m_screenMgr( screenMgr )
    , m_graphics( graphics )
{
}

///////////////////////////
void Screen::enter( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    m_color = FIRST;
    m_stale = false;
}

void Screen::exit( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // Nothing needed
}
void Screen::sleep( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // Nothing needed
}

void Screen::wake( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // Nothing needed
}

void Screen::dispatch( AjaxScreenMgrEvent_T event, Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    m_color = (ColorState_T) (m_color + 1);
    if ( m_color > LAST )
    {
        m_screenMgr.pop();
    }
    else
    {
        m_stale = true;
    }
}

bool Screen::tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    return m_stale;
}

bool Screen::refresh( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // set the colour of the pen
    switch ( m_color )
    {
    case RED: SET_PEN_RED(); break;
    case GREEN: SET_PEN_GREEN(); break;
    case BLUE: SET_PEN_BLUE(); break;
    case WHITE: SET_PEN_WHITE(); break;
    default:
        SET_PEN_BLACK(); 
        break;
    }

    // fill the screen with the current pen colour
    m_graphics.clear();
    m_stale = false;
    return true;
}
