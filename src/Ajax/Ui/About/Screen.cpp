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
#include "Cpl/System/Trace.h"
#include "mp/ModelPoints.h"

#define SECT_ "Ajax::Ui::About"

using namespace Ajax::Ui::About;

#define X0_TITLE                    90
#define Y0_TITLE                    0   // No title


#define TEXT_HEIGHT                 18

#define COLUMN0_X0                  10
#define COLUMN1_X0                  (COLUMN0_X0+60+8)

#define ROW0_Y0                     (TEXT_HEIGHT/2)
#define ROW1_Y0                     (ROW0_Y0+ROW_HEIGHT)
#define ROW2_Y0                     (ROW1_Y0+ROW_HEIGHT)
#define ROW3_Y0                     (ROW2_Y0+ROW_HEIGHT)
#define ROW_HEIGHT                  (TEXT_HEIGHT+8)

#define CALL_JENNY                  "1-888-867-5309"

#define SET_PEN_BACKGROUND()        m_graphics.set_pen( 255, 255, 255 );    // white
#define SET_PEN_TEXT()              m_graphics.set_pen( 0, 0, 0  );         // black

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
    // TODO
}

void Screen::wake( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // TODO
}

void Screen::dispatch( AjaxScreenMgrEvent_T event, Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    if ( event == AJAX_UI_EVENT_BUTTON_A || event == AJAX_UI_EVENT_BUTTON_X )
    {
        m_screenMgr.pop();
    }
}

void Screen::tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // TODO
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

    // Serial Number
    Cpl::Text::FString<OPTION_AJAX_MAX_SERIAL_NUM_LENGTH> tmpSerial="<unknown>";
    mp::serialNumber.read( tmpSerial );
    m_graphics.set_font( &font8 );
    m_graphics.text( "serial:", pimoroni::Point( COLUMN0_X0, ROW1_Y0 ), 240 );
    m_graphics.text( tmpSerial.getString(), pimoroni::Point( COLUMN1_X0, ROW1_Y0 ), 240 );

    // Version Number
    Cpl::Text::FString<OPTION_AJAX_MAX_VERSION_LENGTH> tmpVer="<unknown>";
    mp::fwVersion.read( tmpVer );
    m_graphics.text( "ver:", pimoroni::Point( COLUMN0_X0, ROW2_Y0 ), 240 );
    m_graphics.text( tmpVer.getString(), pimoroni::Point( COLUMN1_X0, ROW2_Y0 ), 240 );

    // Tech support phone number
    m_graphics.text( "tel:", pimoroni::Point( COLUMN0_X0, ROW3_Y0 ), 240 );
    m_graphics.text( CALL_JENNY, pimoroni::Point( COLUMN1_X0, ROW3_Y0 ), 240 );
    
    return true;
}
 
