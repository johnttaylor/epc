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
#include "Eros/Main/screens.h"
#include "Driver/AIO/Eros/ModelPoints.h"
#include "Driver/AIO/Eros/Thermistor.h"

#define SECT_   "Eros::Ui::Home"

using namespace Eros::Ui::Home;

#define X0_TITLE                    55
#define Y0_TITLE                    4


#define TEXT_HEIGHT                 18

#define COLUMN0_X0                  10
#define COLUMN1_X0                  (COLUMN0_X0+60+8)

#define ROW0_Y0                     (Y0_TITLE+TEXT_HEIGHT+TEXT_HEIGHT/2)
#define ROW1_Y0                     (ROW0_Y0+ROW_HEIGHT)
#define ROW2_Y0                     (ROW1_Y0+ROW_HEIGHT)
#define ROW3_Y0                     (ROW2_Y0+ROW_HEIGHT)
#define ROW_HEIGHT                  (TEXT_HEIGHT+8)


#define SET_PEN_BACKGROUND()        m_graphics.set_pen( 0, 0, 0 );          // black
#define SET_PEN_STATIC_TEXT()       m_graphics.set_pen( 255, 255, 255  );   // white
#define SET_PEN_DYNAMIC_TEXT()      m_graphics.set_pen( 0, 0, 255  );       // blue

static void getDisplayIdt( int32_t srcTemp, int32_t& dstInteger, int32_t& dstFractional ) noexcept;

///////////////////////////
Screen::Screen( Ajax::ScreenMgr::Navigation&  screenMgr,
                pimoroni::PicoGraphics&       graphics )
    : m_screenMgr( screenMgr )
    , m_graphics( graphics )
{
}

///////////////////////////
void Screen::enter( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    m_timerMarker = currentElapsedTime.asMilliseconds();
    m_stale       = false;
    m_lastButton  = AJAX_UI_EVENT_NO_EVENT;
}

void Screen::exit( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // nothing needed
}

void Screen::sleep( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // nothing needed
    CPL_SYSTEM_TRACE_FUNC( SECT_ );
}

void Screen::wake( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // nothing needed
    CPL_SYSTEM_TRACE_FUNC( SECT_ );
}

void Screen::dispatch( AjaxScreenMgrEvent_T event, Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    m_lastButton = event;
    m_stale      = true;

    if ( event == AJAX_UI_EVENT_BUTTON_ESC )
    {
        m_screenMgr.push( Eros::Main::g_lcdTextScreen );
    }
}

bool Screen::tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // Poll the space temperature and update on change
    uint32_t now = currentElapsedTime.asMilliseconds();
    if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timerMarker, OPTION_EROS_UI_HOME_SCREEN_POLLING_MS, now ) )
    {
        m_timerMarker = now;
        m_stale       = true;
    }

    return m_stale;
}

bool Screen::refresh( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // set the colour of the pen
    SET_PEN_BACKGROUND();

    // fill the screen with the current pen colour
    m_graphics.clear();

    // Labels
    SET_PEN_STATIC_TEXT();
    m_graphics.set_font( &font8 );
    m_graphics.text( "Eros Test App", pimoroni::Point( X0_TITLE, Y0_TITLE ), 240 );
    m_graphics.text( "Temp:", pimoroni::Point( COLUMN0_X0, ROW0_Y0 ), 240 );
    m_graphics.text( "ADC:", pimoroni::Point( COLUMN0_X0, ROW1_Y0 ), 240 );
    m_graphics.text( "Button:", pimoroni::Point( COLUMN0_X0, ROW2_Y0 ), 240 );
    m_graphics.text( "<Press B+Y for LCD Test>", pimoroni::Point( COLUMN0_X0, ROW3_Y0 ), 240 );

    // Dynamic text: Heating mode
    SET_PEN_DYNAMIC_TEXT();
    const char* buttonText = "";
    switch ( m_lastButton )
    {
    case  AJAX_UI_EVENT_BUTTON_A:
        buttonText = "A";
        break;
    case  AJAX_UI_EVENT_BUTTON_B:
        buttonText = " B";
        break;
    case  AJAX_UI_EVENT_BUTTON_X:
        buttonText = "  X";
        break;
    case  AJAX_UI_EVENT_BUTTON_Y:
        buttonText = "   Y";
        break;
    default:
        break;
    }
    m_graphics.text( buttonText, pimoroni::Point( COLUMN1_X0, ROW2_Y0 ), 240 );

    // Dynamic text: Space Temperature and ADC bits
    int32_t vals[2];
    if ( !mp::erosThermistorSample.read( vals, 2 ) )
    {
        m_graphics.text( "--.-", pimoroni::Point( COLUMN1_X0, ROW0_Y0 ), 240 );
        m_graphics.text( "----", pimoroni::Point( COLUMN1_X0, ROW1_Y0 ), 240 );
    }
    else
    {
        int32_t idtInteger;
        int32_t idtTenths;
        getDisplayIdt( vals[Driver::AIO::Eros::Thermistor::IDX_TEMPERATURE], idtInteger, idtTenths );
        Cpl::Text::FString<5> tmp;
        tmp.format( "%02d.%d", idtInteger, idtTenths );
        m_graphics.text( tmp.getString(), pimoroni::Point( COLUMN1_X0, ROW0_Y0 ), 240 );
        tmp = vals[Driver::AIO::Eros::Thermistor::IDX_ADC_BITS];
        m_graphics.text( tmp.getString(), pimoroni::Point( COLUMN1_X0, ROW1_Y0 ), 240 );
    }

    m_stale = false;
    return true;
}

void getDisplayIdt( int32_t srcTemp, int32_t& dstInteger, int32_t& dstFractional ) noexcept
{
    srcTemp += 5;   // Round 'hundredths' of degree to 'tenths'
    dstFractional = (srcTemp % 100) / 10;
    dstInteger    = srcTemp / 100;
}
