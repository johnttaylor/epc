#ifndef Cpl_Io_File_Littlefs_TShell_Fs_h
#define Cpl_Io_File_Littlefs_TShell_Fs_h
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
#include "Cpl/TShell/Cmd/Command.h"
#include "Cpl/Dm/ModelDatabaseApi.h"


/** Number of bytes to display per line when using the 'dump' command */
#ifndef OPTION_CPL_IO_FILE_LITTLEFS_TSHELL_BYTES_PER_LINE
#define OPTION_CPL_IO_FILE_LITTLEFS_TSHELL_BYTES_PER_LINE 16
#endif

///
namespace Cpl {
///
namespace Io {
///
namespace File {
///
namespace Littlefs {
///
namespace TShell {


/** This class implements a TShell command for various file system commands for
    a system that is using a Littlefs file system.
 */
class Fs : public Cpl::TShell::Cmd::Command
{
public:
    /// The command usage string
    static constexpr const char* verb = "fs";

    /// The command usage string
    static constexpr const char* usage = "fs ls <dir>\n" 
                                         "fs dump <file> [<offset> [<len>]]\n" 
                                         "fs ren <oldentry> <newentry>\n"
                                         "fs rm <entry>\n"
                                         "fs mkfile <file>\n"
                                         "fs mkdir  <dir>\n"
                                         "fs vols";

    /** The command detailed help string (recommended that lines do not exceed 80 chars)
                                                          1         2         3         4         5         6         7         8
                                                 12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* detailedHelp = "  Performs several file system actions.\n"
                                                "    ls     - Lists the content of directory. For a single volume system, use '/'\n"
                                                "             for the root directory.\n"
                                                "    dump   - Displays the content of the specified file. Optional starting file\n" 
                                                "             offset and length can be specified.\n"
                                                "    mv     - Moves/renames the specified entry. The new entry name MUST be on\n"
                                                "             the same volume.\n"
                                                "    rm     - Removes the specified entry. If 'entry' is directory, it must be\n"
                                                "             empty.\n"
                                                "    mkfile - Creates a new file. The file will be empty.\n"
                                                "    mkdir  - Creates a new directory.\n"
                                                "    vols   - Lists the volumes in the file system\n"
                                                "\n"
                                                "  NOTE: You can NOT rename, delete, etc. the volume root directory(s)";


public:
    /// See Cpl::TShell::Command                                                               `
    const char* getUsage() const noexcept { return usage; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return detailedHelp; }


public:
    /// Constructor
    Fs( Cpl::Container::SList<Cpl::TShell::Command>&  commandList, 
        unsigned                                    numVolumes,
        const char*                                 volumeRootPaths[],
        Cpl::TShell::Security::Permission_T         minPermLevel=OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL ) noexcept;


public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept;

protected:
    /// Root paths for the volumes
    const char** m_volPaths;    

    /// Number of volumes
    unsigned m_numVols;

};

}       // end namespaces
}
}
}
}
#endif  // end header latch
