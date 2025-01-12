#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Io/File/Littlefs/_bdfile/BdFile.h"
#include "Cpl/Io/File/Littlefs/Api.h"
#include "Cpl/Io/Serial/PhonyStdio/InputOutput.h"
#include <stdlib.h>


extern void run_readwrite_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut1, Cpl::Io::File::Littlefs::Api::Volume_T& uut2  );


#define ERASE_SIZE       4096
#define NUM_ERASE_BLOCKS 128

// Block driver instances
static Cpl::Io::File::Littlefs::BdFile driverInstance1_( "littlefs1.bin" );
static Cpl::Io::File::Littlefs::BdFile driverInstance2_( "littlefs2.bin" );

// Bypass the CPL IO File abstractions for stdout
Cpl::Io::Serial::PhonyStdio::InputOutput fd_;   

int main( int argc, char* argv[] )
{
    // Initialize Colony
    Cpl::System::Api::initialize();
    Cpl::System::Api::enableScheduling();

    // Create my volumes
    driverInstance1_.start();
    Cpl::Io::File::Littlefs::Api::Volume_T fs1Cfg( &driverInstance1_,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_readfn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_progfn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_erasefn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_syncfn,
                                                   ERASE_SIZE,
                                                   NUM_ERASE_BLOCKS,
                                                   500 );
    Cpl::Io::File::Littlefs::Api::initVolume( fs1Cfg, "vola", true );

    driverInstance2_.start();
    Cpl::Io::File::Littlefs::Api::Volume_T fs2Cfg( &driverInstance2_,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_readfn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_progfn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_erasefn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_syncfn,
                                                   ERASE_SIZE,
                                                   NUM_ERASE_BLOCKS,
                                                   500 );
    Cpl::Io::File::Littlefs::Api::initVolume( fs2Cfg, "volb", true );

    run_readwrite_tests( fs1Cfg, fs2Cfg );
    printf( "\nReadWrite tests passed" );

    Cpl::Io::File::Littlefs::Api::shutdownVolume( fs1Cfg );
    driverInstance1_.stop();
    Cpl::Io::File::Littlefs::Api::shutdownVolume( fs2Cfg );
    driverInstance2_.stop();
    return 0;
}
