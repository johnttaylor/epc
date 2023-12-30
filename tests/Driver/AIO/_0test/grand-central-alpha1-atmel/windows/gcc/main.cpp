
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Trace.h"
#include "Driver/AIO/_0test/singleinput.h"
#include "Driver/AIO/Arduino/HalSingleInput.h"

#define SECT_ "_0test"

extern Cpl::Io::InputOutput& Bsp_Serial( void );


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
    driverAIOHalSingleInputArduino_initialize( PIN_ONBOARD_IDT, AR_DEFAULT ); // use the internal 3.3V reference voltage
    driverAIOHalSingleInputArduino_setADCSize( PIN_ONBOARD_IDT, 12 );

    Cpl::System::Api::sleep( 5000 ); // Allow time to connect a serial terminal
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Hello - Starting AIO test(s)....") );
    
    runtests( PIN_ONBOARD_IDT );   // This method never returns
}
