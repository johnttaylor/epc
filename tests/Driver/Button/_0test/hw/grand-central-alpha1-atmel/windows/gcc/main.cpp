
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Trace.h"
#include "Driver/Button/_0test/_hw/test.h"
#include "Driver/Button//Arduino/Hal.h"

#define SECT_ "_0test"

extern Cpl::Io::InputOutput& Bsp_Serial( void );

static DriverButtonPinHalArduino_T button1_( 12, INPUT_PULLUP, true );
static DriverButtonPinHalArduino_T button2_( 11, INPUT_PULLUP, true );


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
    Cpl::System::Api::sleep( 5000 ); // Allow time to connect a serial terminal
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Hello - Starting BUTTON test(s)....") );
    runtests( button1_, 2, button2_, 100 );   // This method never returns
}
