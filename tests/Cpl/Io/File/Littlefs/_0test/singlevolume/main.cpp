#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Io/File/Littlefs/_bdfile/BdFile.h"
#include "Cpl/Io/File/Littlefs/Api.h"
#include "Cpl/Io/Serial/PhonyStdio/InputOutput.h"
#include <stdlib.h>

extern void run_read_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut );
extern void run_readwrite_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut );
extern void run_write_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut );
extern void run_objectapi_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut );
extern void run_api_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut );

#define ERASE_SIZE       4096
#define NUM_ERASE_BLOCKS 128

// Block driver instance
static Cpl::Io::File::Littlefs::BdFile driverInstance_( "littlefs.bin" );

// Bypass the CPL IO File abstractions for stdout
Cpl::Io::Serial::PhonyStdio::InputOutput fd_;   

int main( int argc, char* argv[] )
{
    // Initialize Colony
    Cpl::System::Api::initialize();
    Cpl::System::Api::enableScheduling();

    // Create my volume
    Cpl::Io::File::Littlefs::Api::Volume_T fs1Cfg( &driverInstance_,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_readfn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_progfn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_erasefn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_syncfn,
                                                   ERASE_SIZE,
                                                   NUM_ERASE_BLOCKS,
                                                   500 );
    driverInstance_.start();
    Cpl::Io::File::Littlefs::Api::initVolume( fs1Cfg, nullptr );

    run_read_tests( fs1Cfg );
    printf( "\nRead tests passed" );
    run_readwrite_tests( fs1Cfg );
    printf( "\nReadWrite tests passed" );
    run_write_tests( fs1Cfg );
    printf( "\nWrite tests passed" );
    run_objectapi_tests( fs1Cfg );
    printf( "\nObjectApi tests passed" );
    run_api_tests( fs1Cfg );
    printf( "\nApi tests passed" );

    Cpl::Io::File::Littlefs::Api::shutdownVolume( fs1Cfg );
    driverInstance_.stop();
}

Cpl::Io::Output* Cpl::System::Trace::getDefaultOutputStream_( void ) noexcept
{
    return &fd_;
}
