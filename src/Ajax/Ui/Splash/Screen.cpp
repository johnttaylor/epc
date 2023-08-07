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
#include "mp/ModelPoints.h"
#include "Cpl/Text/FString.h"

using namespace Ajax::Ui::Splash;

#define X0_TITLE                    90
#define Y0_TITLE                    4


#define TEXT_HEIGHT                 18

#define COLUMN0_X0                  10
#define COLUMN1_X0                  (COLUMN0_X0+50+8)

#define ROW0_Y0                     (Y0_TITLE+TEXT_HEIGHT+TEXT_HEIGHT/2)
#define ROW1_Y0                     (ROW0_Y0+ROW_HEIGHT)
#define ROW2_Y0                     (ROW1_Y0+ROW_HEIGHT)
#define ROW3_Y0                     (ROW2_Y0+ROW_HEIGHT)
#define ROW_HEIGHT                  (TEXT_HEIGHT+8)


#define SET_PEN_BACKGROUND()        m_graphics.set_pen( 0, 0, 255 );    // blue
#define SET_PEN_TEXT()              m_graphics.set_pen( 0, 255, 0  );   // green

///////////////////////////
Screen::Screen( pimoroni::PicoGraphics& graphics )
    :m_graphics( graphics )
{
}

///////////////////////////
void Screen::paint( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // set the colour of the pen
    SET_PEN_BACKGROUND();

    // fill the screen with the current pen colour
    m_graphics.clear();

    // Title 
    SET_PEN_TEXT();
    m_graphics.set_font( &font8 );
    m_graphics.text( "GM6000", pimoroni::Point( X0_TITLE, Y0_TITLE ), 240 );

    // Model Number
    Cpl::Text::FString<OPTION_AJAX_MAX_MODEL_LENGTH> tmpModel="<unknown>";
    mp::modelNumber.read( tmpModel );
    m_graphics.set_font( &font8 );
    m_graphics.text( "model:", pimoroni::Point( COLUMN0_X0, ROW0_Y0 ), 240 );
    m_graphics.text( tmpModel.getString(), pimoroni::Point( COLUMN1_X0, ROW0_Y0 ), 240 );

    // Version Number
    Cpl::Text::FString<OPTION_AJAX_MAX_VERSION_LENGTH> tmpVer="<unknown>";
    mp::fwVersion.read( tmpVer );
    m_graphics.text( "ver:", pimoroni::Point( COLUMN0_X0, ROW1_Y0 ), 240 );
    m_graphics.text( tmpVer.getString(), pimoroni::Point( COLUMN1_X0, ROW1_Y0 ), 240 );

    // Company name
    m_graphics.text( "Initech, LLC.", pimoroni::Point( COLUMN0_X0, ROW3_Y0 ), 240 );
}
