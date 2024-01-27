
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Trace.h"
#include "Driver/DIO/_0test/_out_pwm/out_pwm.h"

#define SECT_ "_0test"

extern Cpl::Io::InputOutput& Bsp_Serial( void );

static Driver::DIO::Out  uutOut_( PIN_LCD_DC, true );
static Driver::DIO::Pwm  uutPwm_( PIN_PWM_LCD_BACKLIGHT );

// the setup function runs once when you press reset or power the board
// NOTE: FreeRTOS is RUNNING at this point
void setup()
{
    // Initialize CPL
    Cpl::System::Api::initialize();

    // Make the current/main thread a CPL Thread
    Cpl::System::FreeRTOS::Thread::makeNativeMainThreadACplThread();

    // Initialize the board (for use with CPL)
    Bsp_Api_initialize();
    Bsp_beginArduinoSerialObject( 115200, SERIAL_8N1 );

    // Enable tracing
    CPL_SYSTEM_TRACE_ENABLE();
    CPL_SYSTEM_TRACE_ENABLE_SECTION( "_0test" ); 
}


// This function is called repeatedly forever
void loop()
{
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Hello - Starting DIO/PWM test(s)....") );
    Cpl::System::Api::sleep( 1000 );
    runtests( uutOut_, uutPwm_, 2 );   // This method returns - but the test is running in a seperate thread
    
    // Never exit the loop() function
    for ( ;;)
    {
        Cpl::System::Api::sleep( 1000 );
    }
}
