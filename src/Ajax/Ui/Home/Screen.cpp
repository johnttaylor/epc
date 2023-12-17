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

#define SECT_   "Ajax::Ui::Home"

using namespace Ajax::Ui::Home;


#define TEXT_HEIGHT                 18

#define COLUMN0_X0                  10
#define COLUMN1_X0                  (COLUMN0_X0+50+8)
#define COLUMN2_X0                  (COLUMN1_X0+50+8)
#define COLUMN3_X0                  (COLUMN2_X0+50+8)


#define ROW0_Y0                     (TEXT_HEIGHT/2)
#define ROW1_Y0                     (ROW0_Y0+ROW_HEIGHT)
#define ROW2_Y0                     (ROW1_Y0+(2*ROW_HEIGHT))
#define ROW3_Y0                     (ROW2_Y0+ROW_HEIGHT)
#define ROW_HEIGHT                  (TEXT_HEIGHT+8)


#define SET_PEN_BACKGROUND()        m_graphics.set_pen( 0, 0, 0 );          // black
#define SET_PEN_STATIC_TEXT()       m_graphics.set_pen( 255, 255, 255  );   // white
#define SET_PEN_DYNAMIC_TEXT()      m_graphics.set_pen( 255, 255, 0  );     // Yellow

///////////////////////////
Screen::Screen( Ajax::ScreenMgr::Navigation&  screenMgr,
                pimoroni::PicoGraphics&       graphics,
                Cpl::Dm::MailboxServer&       myMbox,
                Cpl::Dm::Mp::Int32&           mpSpaceTemperature )
    : m_screenMgr( screenMgr )
    , m_graphics( graphics )
    , m_mpIdt( mpSpaceTemperature )
    , m_obHeatMode( *((Cpl::Dm::EventLoop*) &myMbox), *this, &Screen::heatingModeChanged )
{
}

///////////////////////////
void Screen::heatingModeChanged( Cpl::Dm::Mp::Bool& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    if ( !mp.isNotValidAndSync( clientObserver ) )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("hmode changed") );
        m_stale = true;
    }
}

///////////////////////////
void Screen::enter( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    m_timerMarker = currentElapsedTime.asMilliseconds();
    m_stale       = false;
    getDisplayIdt( m_idtInteger, m_idtTenths );
    mp::heatingMode.attach( m_obHeatMode, mp::heatingMode.getSequenceNumber() );    // Subscribe at the current 'value'
}

void Screen::exit( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    mp::heatingMode.detach( m_obHeatMode );
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
    // TODO - real implementation

    if ( event == AJAX_UI_EVENT_BUTTON_ESC )
    {
        m_screenMgr.push( Ajax::Main::g_aboutScreen_ );
    }
}

bool Screen::tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
{
    bool prev = m_stale;

    // Poll the space temperature and update on change
    uint32_t now = currentElapsedTime.asMilliseconds();
    if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timerMarker, OPTION_AJAX_UI_HOME_SCREEN_POLLING_MS, now ) )
    {
        m_timerMarker = now;
        int32_t idtInteger;
        int32_t idtTenths;
        if ( getDisplayIdt( idtInteger, idtTenths ) &&
             (idtInteger != m_idtInteger ||
               idtTenths != m_idtTenths) )
        {
            m_idtInteger = idtInteger;
            m_idtTenths  = idtTenths;
            m_stale      = true;
        }
    }

    // Check if my content is stale/out-of-date
    if ( !prev && m_stale )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("tick. m_stale transitioned to TRUE") );
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
    m_graphics.text( "mode:", pimoroni::Point( COLUMN0_X0, ROW0_Y0 ), 240 );
    m_graphics.text( "fan:", pimoroni::Point( COLUMN2_X0, ROW0_Y0 ), 240 );
    m_graphics.text( "temp:", pimoroni::Point( COLUMN0_X0, ROW1_Y0 ), 240 );
    m_graphics.text( "temp:", pimoroni::Point( COLUMN2_X0, ROW1_Y0 ), 240 );
    m_graphics.text( "heater:", pimoroni::Point( COLUMN0_X0, ROW2_Y0 ), 240 );
    m_graphics.text( "fan:", pimoroni::Point( COLUMN2_X0, ROW2_Y0 ), 240 );

    // Dynamic text: Heating mode
    SET_PEN_DYNAMIC_TEXT();
    bool heating = false;
    mp::heatingMode.read( heating );
    const char* heatingModeString = heating ? "ON" : "off";
    m_graphics.text( heatingModeString, pimoroni::Point( COLUMN1_X0, ROW0_Y0 ), 240 );

    // Dynamic text: Fan mode
    const char* fmodeString = "";
    Ajax::Type::FanMode fmode;
    if ( mp::fanMode.read( fmode ) )
    {
        switch ( fmode )
        {
        case Ajax::Type::FanMode::eHIGH: fmodeString   = "high"; break;
        case Ajax::Type::FanMode::eLOW: fmodeString    = "low"; break;
        case Ajax::Type::FanMode::eMEDIUM: fmodeString = "med"; break;
        default:
            fmodeString = "???";    // This SHOULDN'T every happen
            break;
        }
    }
    m_graphics.text( fmodeString, pimoroni::Point( COLUMN3_X0, ROW0_Y0 ), 240 );

    // Dynamic text: Space Temperature
    if ( !m_idtValid )
    {
        m_graphics.text( "--.-", pimoroni::Point( COLUMN1_X0, ROW1_Y0 ), 240 );
    }
    else
    {
        Cpl::Text::FString<5> tmp;
        tmp.format( "%02d.%d", m_idtInteger, m_idtTenths );
        m_graphics.text( tmp.getString(), pimoroni::Point( COLUMN1_X0, ROW1_Y0 ), 240 );
    }


    m_stale = false;
    return true;
}

bool Screen::getDisplayIdt( int32_t& dstInteger, int32_t& dstFractional ) noexcept
{
    // Get 'display' value for the space temperature
    int32_t idt;
    if ( m_mpIdt.read( idt ) )
    {
        idt += 5;   // Round 'hundredths' of degree to 'tenths'
        dstFractional = (idt % 100) / 10;
        dstInteger    = idt / 100;
        m_idtValid    = true;
    }
    else
    {
        m_idtValid = false;
    }
    return m_idtValid;
}
