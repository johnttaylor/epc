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

using namespace Ajax::Ui::Shutdown;

#define TEXT_HEIGHT                 18

#define COLUMN0_X0                  10
#define COLUMN1_X0                  (COLUMN0_X0+50+8)

#define ROW0_Y0                     (TEXT_HEIGHT*2)
#define ROW1_Y0                     (ROW0_Y0+ROW_HEIGHT)
#define ROW2_Y0                     (ROW1_Y0+ROW_HEIGHT)
#define ROW3_Y0                     (ROW2_Y0+ROW_HEIGHT)
#define ROW_HEIGHT                  (TEXT_HEIGHT+8)


#define SET_PEN_BACKGROUND()        m_graphics.set_pen( 0, 0, 0 );      // Black
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
    m_graphics.text( "Restarting. Please wait", pimoroni::Point( COLUMN0_X0, ROW0_Y0 ), 240 );
}
