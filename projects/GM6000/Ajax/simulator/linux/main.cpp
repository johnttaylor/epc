#include "Ajax/Main/appmain.h"
#include "Ajax/Main/_plat_simulator/platform.h"
#include "Cpl/System/Api.h"
#include "Cpl/TShell/Cmd/Threads.h"
#include "Cpl/Io/Stdio/StdIn.h"
#include "Cpl/Io/Stdio/StdOut.h"


static const char USAGE[] =
R"(Ajax Simulation.

    Usage:
      ajax-sim [options]

    Options:
      -v            Be verbose
      --help        Show this screen.

)"; 

using namespace Ajax::Main;

static Cpl::TShell::Cmd::Threads cmdThreads_( g_cmdlist );

std::map<std::string, docopt::value> Ajax::Main::g_args;

Cpl::Io::Stdio::StdIn   infd_;
Cpl::Io::Stdio::StdOut  outfd_;


int main( int argc, char* const argv[] )
{
    // Parse the command line arguments
    g_args = docopt::docopt( USAGE, { argv + 1, argv + argc }, true );

    // Initialize Colony
    Cpl::System::Api::initialize();

    // Run the application
    return runTheApplication( infd_, outfd_ );
}
