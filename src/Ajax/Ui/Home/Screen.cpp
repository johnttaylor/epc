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
#include "Ajax/Main/screens.h"

using namespace Ajax::Ui::Home;


#define TEXT_HEIGHT                 18

#define COLUMN0_X0                  10
#define COLUMN1_X0                  (COLUMN0_X0+60+8)
#define COLUMN2_X0                  (COLUMN0_X1+40+8)
#define COLUMN3_X0                  (COLUMN0_X1+40+8)


#define ROW0_Y0                     (TEXT_HEIGHT/2)
#define ROW1_Y0                     (ROW0_Y0+ROW_HEIGHT)
#define ROW2_Y0                     (ROW1_Y0+(2*ROW_HEIGHT))
#define ROW3_Y0                     (ROW2_Y0+ROW_HEIGHT)
#define ROW_HEIGHT                  (TEXT_HEIGHT+8)


#define SET_PEN_BACKGROUND()        m_graphics.set_pen( 0, 0, 0 );          // black
#define SET_PEN_STATIC_TEXT()       m_graphics.set_pen( 255, 255, 255  );   // white
#define SET_PEN_DYNAMIC_TEXT()      m_graphics.set_pen( 255, 255, 0  );     // Yellow

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
    // TODO
}
void Screen::exit( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // TODO
}
void Screen::sleep( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // nothing needed
}

void Screen::wake( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // nothing needed
}

void Screen::dispatch( AjaxScreenMgrEvent_T event, Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // TODO - real implementation

    if ( event == AJAX_UI_EVENT_BUTTON_ESC )
    {
        m_screenMgr.push( Ajax::Main::g_aboutScreen_ );
    }
}

void Screen::tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // nothing needed
}

bool Screen::refresh( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // set the colour of the pen
    SET_PEN_BACKGROUND();

    // fill the screen with the current pen colour
    m_graphics.clear();

    // Model Number
    SET_PEN_TEXT();
    Cpl::Text::FString<OPTION_AJAX_MAX_MODEL_LENGTH> tmpModel="<unknown>";
    mp::modelNumber.read( tmpModel );
    m_graphics.set_font( &font8 );
    m_graphics.text( "model:", pimoroni::Point( COLUMN0_X0, ROW0_Y0 ), 240 );
    m_graphics.text( tmpModel.getString(), pimoroni::Point( COLUMN1_X0, ROW0_Y0 ), 240 );

    
    return true;
}
