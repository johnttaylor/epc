/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2022  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "colony_config.h"
#include "api.h"
#include "Ajax/Heating/Io/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Thread.h"
#include "Cpl/Container/Map.h"
#include "mp/ModelPoints.h"
#include "Cpl/TShell/Cmd/Help.h"
#include "Cpl/TShell/Cmd/Bye.h"
#include "Cpl/TShell/Cmd/Trace.h"
#include "Cpl/TShell/Cmd/TPrint.h"
#include "Cpl/TShell/Cmd/Threads.h"
#include "Cpl/Dm/TShell/Dm.h"
#include "Cpl/TShell/Stdio.h"
#include "Cpl/TShell/Maker.h"

///
using namespace Ajax::Heating::Io;

#define SECT_ "_0test"


static Cpl::Dm::MailboxServer mbox_;

static Cpl::Container::SList<Cpl::TShell::Command>   cmdlist;
static Cpl::TShell::Maker cmdProcessor_( cmdlist );
static Cpl::TShell::Stdio shell_( cmdProcessor_ );

static Cpl::TShell::Cmd::Help	    helpCmd_( cmdlist );
static Cpl::TShell::Cmd::Trace	    traceCmd_( cmdlist );
static Cpl::TShell::Cmd::TPrint	    tprintCmd_( cmdlist );
static Cpl::TShell::Cmd::Threads    threadsCmd_( cmdlist );
static Cpl::Dm::TShell::Dm          dmCmd_( cmdlist, mp::g_modelDatabase );

///////////////////////////////////////////////////////////////////////
void runtests( Driver::DIO::In&  hwSafetyInDriver,
               Driver::DIO::Pwm& heaterPWMDriver,
               Driver::DIO::Pwm& fanPWMDriver,
               Cpl::Io::Input&    infd,
               Cpl::Io::Output&   outfd )
{
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Starting test for the Ajax::Heating::Io component\nUse 'dm' command to read/write the model points") );
    hwSafetyInDriver.start();
    heaterPWMDriver.start( 0 );
    fanPWMDriver.start( 0 );

    Ajax::Heating::Io::Api* uut = new(std::nothrow) Ajax::Heating::Io::Api( mbox_, heaterPWMDriver, fanPWMDriver, hwSafetyInDriver );
    if ( uut == nullptr )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Failed to create UUT") );
        for ( ;;);
    }
    Cpl::System::Thread::create( mbox_, "TEST" );

    shell_.launch( infd, outfd );

    uut->open();
    for ( ;;)
    {
        Cpl::System::Api::sleep( 1000 );
    }
}