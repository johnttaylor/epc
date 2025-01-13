#include "colony_config.h"
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Driver/SPI/STM32/MasterHalfDuplex.h"
#include "Driver/DIO/Out.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/Io/File/Littlefs/_bdspi/BdSpi.h"


extern void runtest( const lfs_config& cfg );

#define CACHE_SIZE       16
#define READ_SIZE        CACHE_SIZE
#define PROG_SIZE        CACHE_SIZE
#define ERASE_SIZE       4096
#define NUM_ERASE_BLOCKS 4096

static Driver::SPI::STM32::MasterHalfDuplex spiDriver_( &hspi3 );
static DriverDioOutSTM32PinConfig_T         chipSelectConfig_( CS_SPI_Flash_GPIO_Port, CS_SPI_Flash_Pin );
static Driver::DIO::Out                     csDriver_( chipSelectConfig_, false );
static Cpl::Io::File::Littlefs::BdSpi       uut_( spiDriver_, csDriver_ );

// buffers
static uint8_t readBuffer_[CACHE_SIZE];
static uint8_t progBuffer_[CACHE_SIZE];
static uint8_t lookaheadBuffer_[CACHE_SIZE];

// configuration of the filesystem is provided by this struct
static const struct lfs_config fsCfg_ = {
    &uut_,  // filesystem context, aka the block driver

    // block device operations
    Cpl::Io::File::Littlefs::BlockDriverApi::callback_readfn,
    Cpl::Io::File::Littlefs::BlockDriverApi::callback_progfn,
    Cpl::Io::File::Littlefs::BlockDriverApi::callback_erasefn,
    Cpl::Io::File::Littlefs::BlockDriverApi::callback_syncfn,

    // block device configuration
    READ_SIZE,
    PROG_SIZE,
    ERASE_SIZE,
    NUM_ERASE_BLOCKS,
    500,  // Block cycles
    CACHE_SIZE,
    16,                // Lookahead buffer size
    0,                 // compact_threshold
    readBuffer_,       // read_buffer
    progBuffer_,       // program_buffer
    lookaheadBuffer_,  // lookahead_buffer
};


#define SECT_ "_0test"

/// Thread for the test (note: the console driver only works when called from a CPL thread)
class ThreadMain : public Cpl::System::Runnable
{
public:
    ThreadMain(){};

protected:
    void appRun()
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "**** DRIVER TEST APPLICATION STARTED ****" ) );
        //Cpl::System::Api::sleep( 1000 );
        uut_.start();
        runtest( fsCfg_ );
        uut_.stop();
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
    // CPL_SYSTEM_TRACE_ENABLE_SECTION( "Cpl::Io::File::Littlefs::BdSpi" );

    //// Create the main thread
    Cpl::System::Thread* t1 = Cpl::System::FreeRTOS::Thread::create( runnable_, "main", CPL_SYSTEM_THREAD_PRIORITY_NORMAL );
    if ( t1 == nullptr )
    {
        printf( "Failed to create the main thread\n" );
        while ( 1 )
            ;
    }

    // Start the scheduler
    Cpl::System::Api::enableScheduling();  // Enable scheduling NEVER return!
    return 0;
}
