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
#include "mp/ModelPoints.h"
#include "Cpl/System/Trace.h"

#define SECT_   "Ajax::Ui::EditSetpt"


using namespace Ajax::Ui::EditSetpt;

#define TEXT_HEIGHT                 18

#define COLUMN0_X0                  10
#define COLUMN1_X0                  (COLUMN0_X0+50+8)
#define COLUMN2_X0                  (COLUMN1_X0+50+8)
#define COLUMN3_X0                  (COLUMN2_X0+50+8)

#define ROW0_Y0                     (TEXT_HEIGHT/2)
#define ROW1_Y0                     (ROW0_Y0+ROW_HEIGHT)
#define ROW2_Y0                     (ROW1_Y0+ROW_HEIGHT)
#define ROW3_Y0                     (ROW2_Y0+ROW_HEIGHT)
#define ROW4_Y0                     (ROW3_Y0+ROW_HEIGHT)
#define ROW_HEIGHT                  (TEXT_HEIGHT+8)
#define OUTLINE_ROW_OFFSET          (8/2+2)
#define OUTLINE_COL_OFFSET          (4)

#define SET_PEN_BACKGROUND()        m_graphics.set_pen( 0, 0, 0 );          // black
#define SET_PEN_EDIT_BACKGROUND()   m_graphics.set_pen( 0, 0, 255 );        // blue
#define SET_PEN_STATIC_TEXT()       m_graphics.set_pen( 255, 255, 255  );   // white
#define SET_PEN_EDIT_TEXT()         m_graphics.set_pen( 255, 255, 255  );   // white

///////////////////////////
Screen::Screen( Ajax::ScreenMgr::Navigation&  screenMgr,
                pimoroni::PicoGraphics&       graphics)
    : m_screenMgr( screenMgr )
    , m_graphics( graphics )
{
}

///////////////////////////
void Screen::enter( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    m_stale = false;
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
    switch ( event )
    {
    case AJAX_UI_EVENT_BUTTON_A:
    break;

    case AJAX_UI_EVENT_BUTTON_B:
        m_screenMgr.pop();
        break;

    case AJAX_UI_EVENT_BUTTON_X:
    {
        int32_t setpt;
        if ( mp::heatSetpoint.read( setpt ) )
        {
            setpt += 100;
            if ( setpt > OPTION_AJAX_MAX_HEATING_SETPOINT_F )
            {
                setpt = OPTION_AJAX_MAX_HEATING_SETPOINT_F;
            }
            mp::heatSetpoint.write( setpt );
            m_stale = true;
        }
    }
    break;

    case AJAX_UI_EVENT_BUTTON_Y:
    {
        int32_t setpt;
        if ( mp::heatSetpoint.read( setpt ) )
        {
            setpt -= 100;
            if ( setpt < OPTION_AJAX_MIN_HEATING_SETPOINT_F )
            {
                setpt = OPTION_AJAX_MIN_HEATING_SETPOINT_F;
            }
            mp::heatSetpoint.write( setpt );
            m_stale = true;
        }
    }

    case AJAX_UI_EVENT_BUTTON_ESC:
        break;

    default:
        break;
    }
}

bool Screen::tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    // Force a repaint when the setpoint has been changed
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
    m_graphics.text( "setpt:", pimoroni::Point( COLUMN2_X0, ROW1_Y0 ), 240 );
    m_graphics.text( "Press X to increase", pimoroni::Point( COLUMN0_X0, ROW2_Y0 ), 240 );
    m_graphics.text( "Press Y to decrease", pimoroni::Point( COLUMN0_X0, ROW3_Y0 ), 240 );
    m_graphics.text( "Press B to exit", pimoroni::Point( COLUMN0_X0, ROW4_Y0 ), 240 );

    //// Edit text: Heating mode
    SET_PEN_EDIT_BACKGROUND();
    pimoroni::Rect editBackground( COLUMN3_X0 - OUTLINE_COL_OFFSET, ROW1_Y0 - OUTLINE_ROW_OFFSET, 28, ROW_HEIGHT );
    m_graphics.rectangle( editBackground );
    SET_PEN_EDIT_TEXT();
    int32_t setpt;
    if ( !mp::heatSetpoint.read( setpt ) )
    {
        m_graphics.text( "--", pimoroni::Point( COLUMN3_X0, ROW1_Y0 ), 240 );
    }
    else
    {
        Cpl::Text::FString<3> tmp;
        tmp.format( "%02d", setpt / 100 );
        m_graphics.text( tmp.getString(), pimoroni::Point( COLUMN3_X0, ROW1_Y0 ), 240 );
    }

    //// Dynamic text: Heater PWM
    //uint32_t pwm;
    //if ( !mp::cmdHeaterPWM.read( pwm ) || pwm == 0 )
    //{
    //    m_graphics.text( "off", pimoroni::Point( COLUMN1_X0, ROW2_Y0 ), 240 );
    //}
    //else
    //{
    //    Cpl::Text::FString<4> tmp;
    //    pwm += ONE_PERCENT_PWM / 2; // Round to the nearest 1%
    //    tmp.format( "%d%%", pwm / ONE_PERCENT_PWM );
    //    m_graphics.text( tmp.getString(), pimoroni::Point( COLUMN1_X0, ROW2_Y0 ), 240 );
    //}

    //// Dynamic text: Fan PWM
    //if ( !mp::cmdFanPWM.read( pwm ) || pwm == 0 )
    //{
    //    m_graphics.text( "off", pimoroni::Point( COLUMN3_X0, ROW2_Y0 ), 240 );
    //}
    //else
    //{
    //    Cpl::Text::FString<4> tmp;
    //    pwm += ONE_PERCENT_PWM / 2; // Round to the nearest 1%
    //    tmp.format( "%d%%", pwm / ONE_PERCENT_PWM );
    //    m_graphics.text( tmp.getString(), pimoroni::Point( COLUMN3_X0, ROW2_Y0 ), 240 );
    //}

    //m_stale = false;
    return true;
}
