
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Trace.h"
#include "Driver/SPI/_0test/adxl345.h"
#include "Driver/SPI/Arduino/Master.h"
#include "Driver/DIO/Out.h"

#define SECT_ "_0test"


extern Cpl::Io::InputOutput& Bsp_Serial( void );

static Driver::DIO::Out                          cs_( PIN_LCD_CS, false );
static Driver::SPI::Arduino::Master::SPIConfig_T cfg_( 500000, MSBFIRST, SPI_MODE3 );
static Driver::SPI::Arduino::Master              uut_( SPI, cfg_ );

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
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Hello - SPI ADXL345 tests ....") );
    runtests( uut_, cs_ ); // This method never returns
}
