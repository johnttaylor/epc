#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Io/Serial/PhonyStdio/InputOutput.h"
#include "Cpl/TShell/Cmd/Win32/Threads.h"




// External references
extern void shell_test( Cpl::Io::Input& infd, Cpl::Io::Output& outfd );

Cpl::Io::Serial::PhonyStdio::InputOutput fd_;

// Note: this is global variable for test purposes
Cpl::Container::SList<Cpl::TShell::Command>  cmdlist( "ignore_this_parameter-used to invoke the static constructor" );

// HACK: to reuse a common test file
#ifndef BUILD_VARIANT_CPP11
static Cpl::TShell::Cmd::Win32::Threads    threadsCmd_( cmdlist );
#else
static Cpl::TShell::Cmd::Threads           threadsCmd_( cmdlist );
#endif


int main( int argc, char* const argv[] )
{
	// Initialize Colony
	Cpl::System::Api::initialize();

	CPL_SYSTEM_TRACE_ENABLE();
	CPL_SYSTEM_TRACE_ENABLE_SECTION( "_0test" );
	CPL_SYSTEM_TRACE_SET_INFO_LEVEL( Cpl::System::Trace::eINFO );

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

