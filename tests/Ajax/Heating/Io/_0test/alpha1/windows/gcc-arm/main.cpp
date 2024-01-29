#include "colony_config.h"
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Driver/DIO/Pwm.h"
#include "Driver/DIO/In.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Bsp/Initech/alpha1/console/Output.h"
#include "Ajax/Heating/Io/_0test/api.h"

static DriverDioPwmSTM32Config_T            heaterPWMCfg_( PWM_HEATER_BLOCK_PTR, PWM_HEATER_CHANNEL );
static Driver::DIO::Pwm                     heaterPWMDriver_( heaterPWMCfg_ );
static DriverDioPwmSTM32Config_T            fanPWMCfg_( PWM_FAN_BLOCK_PTR, PWM_FAN_CHANNEL );
static Driver::DIO::Pwm                     fanPWMDriver_( fanPWMCfg_ );
static DriverDioInSTM32PinConfig_T          hwSafetyCfg_( HW_SAFETY_GPIO_Port, HW_SAFETY_Pin );
static Driver::DIO::In                      hwSafetyDriver_( hwSafetyCfg_, false );

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
        runtests( hwSafetyDriver_, heaterPWMDriver_, fanPWMDriver_, g_bspConsoleStream, g_bspConsoleStream );
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
