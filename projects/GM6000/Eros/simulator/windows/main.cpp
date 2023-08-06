#include "Ajax/Main/appmain.h"
#include "Ajax/Main/_plat_simulator/platform.h"
#include "Cpl/System/Api.h"
#include "Cpl/TShell/Cmd/Win32/Threads.h"
#include "Cpl/Io/Stdio/StdIn.h"
#include "Cpl/Io/Stdio/StdOut.h"
#include "Driver/PicoDisplay/TPipe/Api.h"
#include "Cpl/Io/Socket/Win32/Connector.h"
#include "Cpl/Io/Socket/InputOutput.h"

#define SIMULATION_TITLE        "GM6000 - Eros Test App"
#define TITLE_COMMAND           "00 00:00:00.000 title " SIMULATION_TITLE

static const char USAGE[] =
R"(Ajax Simulation.

    Usage:
      ajax-sim [options]

    Options:
      -s HOST       Hostname for the Display Simulation. [Default: 127.0.0.1]
      -p PORT       The Display Simulation's Port number [Default: 5010]
      -v            Be verbose
      --help        Show this screen.

)"; 

using namespace Ajax::Main;

static Cpl::TShell::Cmd::Win32::Threads cmdThreads_( g_cmdlist );

std::map<std::string, docopt::value> Ajax::Main::g_args;

Cpl::Io::Stdio::StdIn   infd_;
Cpl::Io::Stdio::StdOut  outfd_;


int main( int argc, char* const argv[] )
{
    // Parse the command line arguments
    g_args = docopt::docopt( USAGE, { argv + 1, argv + argc }, true );

    // Initialize Colony
    Cpl::System::Api::initialize();

    // Platform specific sockets: Connect to the Display simulation
    Cpl::Io::Descriptor                 socketFd;
    Cpl::Io::Socket::Win32::Connector   connector;
    if ( connector.establish( g_args["-s"].asString().c_str(), g_args["-p"].asLong(), socketFd ) != Cpl::Io::Socket::Connector::eSUCCESS )
    {
        printf( "\nERROR. Failed to connected to the Display Simulation. (host=%s, port=%ld).\n",
                g_args["-s"].asString().c_str(),
                g_args["-p"].asLong() );
        return 1;
    }
    Cpl::Io::Socket::InputOutput socketStream( socketFd );

    // Initializes the simulator's socket connect to the GUI application
    Driver::PicoDisplay::TPipe::initialize( socketStream, socketStream );

    // Set the title for the Simulation window
    Cpl::System::Api::sleep( 100 ); // Allow time for the TPipe thread to spin up
    Driver::PicoDisplay::TPipe::tpipe().sendCommand( TITLE_COMMAND, strlen( TITLE_COMMAND ) );

    // Run the application
    return runTheApplication( infd_, outfd_ );
}
