#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Io/File/Littlefs/_bdfile/BdFile.h"
#include "Cpl/Io/File/Littlefs/Api.h"
#include "Cpl/Io/Serial/PhonyStdio/InputOutput.h"
#include "Cpl/TShell/Command.h"
#include "Cpl/Container/Map.h"
#include "Cpl/System/Shutdown.h"
//#include <signal.h>

extern void runtest( lfs_t* cfg );

#define SECT_   "_0test"


// Bypass the CPL IO File abstractions for stdout
Cpl::Io::Serial::PhonyStdio::InputOutput fd_;   

// External references
extern void shell_test( Cpl::Io::Input& infd, Cpl::Io::Output& outfd );

// Note: this is global variable for test purposes
Cpl::Container::Map<Cpl::TShell::Command>  cmdlist( "ignore_this_parameter-used to invoke the static constructor" );

#define ERASE_SIZE       4096
#define NUM_ERASE_BLOCKS 128

// Block driver instances
static Cpl::Io::File::Littlefs::BdFile driverInstance1_( "littlefs1.bin" );
static Cpl::Io::File::Littlefs::BdFile driverInstance2_( "littlefs2.bin" );


int main( int argc, char* argv[] )
{
	CPL_SYSTEM_TRACE_ENABLE();
	CPL_SYSTEM_TRACE_ENABLE_SECTION( "_0test" );
	CPL_SYSTEM_TRACE_SET_INFO_LEVEL( Cpl::System::Trace::eINFO );

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
    Cpl::Io::File::Littlefs::Api::initVolume( fs1Cfg, "apple", true );

    driverInstance2_.start();
    Cpl::Io::File::Littlefs::Api::Volume_T fs2Cfg( &driverInstance2_,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_readfn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_progfn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_erasefn,
                                                   Cpl::Io::File::Littlefs::BlockDriverApi::callback_syncfn,
                                                   ERASE_SIZE,
                                                   NUM_ERASE_BLOCKS,
                                                   500 );
    Cpl::Io::File::Littlefs::Api::initVolume( fs2Cfg, "orange", true );

    // Run the test
    shell_test( fd_, fd_ );
    
	// do not exit
	for ( ;;)
	{
		Cpl::System::Api::sleep( 1000 );
	}

	return 0;
}

Cpl::Io::Output* Cpl::System::Trace::getDefaultOutputStream_( void ) noexcept
{
    return &fd_;
}

////////////////////////////////////////////////////////////////////////////////

static int shutdown_application_( int exit_code )
{
    exit( exit_code); // raise( SIGTERM );
    return exit_code;
}

int Cpl::System::Shutdown::success( void )
{
    return shutdown_application_( notifyShutdownHandlers_( OPTION_CPL_SYSTEM_SHUTDOWN_SUCCESS_ERROR_CODE ) );
}

int Cpl::System::Shutdown::failure( int exit_code )
{
    return shutdown_application_( notifyShutdownHandlers_( exit_code ) );
}
