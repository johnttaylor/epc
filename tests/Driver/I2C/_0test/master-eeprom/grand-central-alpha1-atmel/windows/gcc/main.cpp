
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Trace.h"
#include "Driver/I2C/_0test/master_eeprom.h"
#include "Driver/I2C/Arduino/Master.h"

#define SECT_ "_0test"

#ifndef OPTION_I2C_DEV_ADDRESS
#define OPTION_I2C_DEV_ADDRESS              (0x51)
#endif

extern Cpl::Io::InputOutput& Bsp_Serial( void );

static Driver::I2C::Arduino::Master uut_( Wire );

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
    Cpl::System::Api::sleep( 5000 );  // Allow time to spin up a terminal session
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Hello - I2C EEPROM tests ....") );
    runtests( uut_, OPTION_I2C_DEV_ADDRESS ); // This method never returns
}
