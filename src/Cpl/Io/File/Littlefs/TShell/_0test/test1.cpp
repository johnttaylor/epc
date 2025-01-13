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

#include "colony_config.h"
#include "Cpl/TShell/Maker.h"
#include "Cpl/TShell/Stdio.h"
#include "Cpl/TShell/Cmd/Help.h"
#include "Cpl/TShell/Cmd/Bye.h"
#include "Cpl/TShell/Cmd/Trace.h"
#include "Cpl/Io/File/Api.h"
#include "Cpl/Io/File/Output.h"
#include "Cpl/Io/File/Littlefs/TShell/Fs.h"
#include "Cpl/TShell/Stdio.h"

#define ROOT_VOL1_PATH "/apple"
#define ROOT_VOL2_PATH "/orange"

///
extern void shell_test( Cpl::Io::Input& infd, Cpl::Io::Output& outfd );

extern Cpl::Container::Map<Cpl::TShell::Command> cmdlist;

static Cpl::TShell::Maker cmdProcessor_( cmdlist );

static Cpl::TShell::Cmd::Help  helpCmd_( cmdlist );
static Cpl::TShell::Cmd::Bye   byeCmd_( cmdlist );
static Cpl::TShell::Cmd::Trace traceCmd_( cmdlist );

static const char* volPaths[] = { ROOT_VOL1_PATH, ROOT_VOL2_PATH };
static Cpl::Io::File::Littlefs::TShell::Fs fsCmd_( cmdlist, 2, volPaths );

////////////////////////////////////////////////////////////////////////////////
static Cpl::TShell::Stdio shell_( cmdProcessor_ );

static void createEntries();

void shell_test( Cpl::Io::Input& infd, Cpl::Io::Output& outfd )
{
    // Create some entries
    createEntries();

    // Start the shell
    shell_.launch( infd, outfd );

    // Start the scheduler
    Cpl::System::Api::enableScheduling();
}


void createEntries()
{
    Cpl::Io::File::Api::createDirectory( ROOT_VOL1_PATH "/bob" );
    Cpl::Io::File::Api::createDirectory( ROOT_VOL2_PATH "/charlie" );

    {
        Cpl::Io::File::Output outfd( ROOT_VOL1_PATH "/text1", true, true );
        outfd.write( "Hello World\nThis is test of text file\nSo long and thanks for all the fish" );
        outfd.close();
    }
    {
        Cpl::Io::File::Output outfd( ROOT_VOL1_PATH "/text2", true, true );
        outfd.write( "A second\ntext file\for some\n variety... (trailing newline)\n" );
        outfd.close();
    }
    {
        Cpl::Io::File::Output outfd( ROOT_VOL1_PATH "/bob/text11", true, true );
        outfd.write( "text...\n11" );
        outfd.close();
    }
    {
        Cpl::Io::File::Output outfd( ROOT_VOL1_PATH "/bob/text111", true, true );
        outfd.write( "text...\n11 ... 1" );
        outfd.close();
    }
    {
        Cpl::Io::File::Output outfd( ROOT_VOL2_PATH "/charlie/text22", true, true );
        outfd.write( "text...\n22\n not very original (trailing newline)\n" );
        outfd.close();
    }
}

