#include "colony_config.h"
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Driver/SPI/_0test/master_eeprom.h"
#include "Driver/SPI/STM32/Master.h"
#include "Cpl/System/FreeRTOS/Thread.h"


static Driver::SPI::STM32::Master       uut_( &hspi3 );
static DriverDioOutSTM32PinConfig_T     chipSelectConfig_( CS_SPI_Flash_GPIO_Port, CS_SPI_Flash_Pin );
static Driver::DIO::Out                 csDriver_( chipSelectConfig_, false );


#define SECT_   "_0test"

/// Thread for the test (note: the console driver only works when called from a CPL thread)
class ThreadMain : public Cpl::System::Runnable
{
public:
    ThreadMain() {};

protected:
    void appRun()
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("**** DRIVER TEST APPLICATION STARTED ****") );
        runtests( uut_, csDriver_ );
    }
};

/* NOTE: The way FreeRTOS start the 'first' task - corrupts the raw main
         stack -->this means we cannot allocate anything on the raw main
         stack and expect it to stay in scope for duration of the applicatoin
*/
static ThreadMain runnable_;

/*-----------------------------------------------------------*/
int main( void )
{
    // Initialize CPL
    Cpl::System::Api::initialize();

    // Initialize the board
    Bsp_Api_initialize();

    CPL_SYSTEM_TRACE_ENABLE();
    CPL_SYSTEM_TRACE_ENABLE_SECTION( SECT_ );

    //// Create the main thread
    Cpl::System::Thread* t1 = Cpl::System::FreeRTOS::Thread::create( runnable_, "main", CPL_SYSTEM_THREAD_PRIORITY_NORMAL );
    if ( t1 == nullptr )
    {
        printf( "Failed to create the main thread\n" );
        while ( 1 );
    }

    // Start the scheduler
    Cpl::System::Api::enableScheduling(); // Enable scheduling NEVER return!
    return 0;
}
