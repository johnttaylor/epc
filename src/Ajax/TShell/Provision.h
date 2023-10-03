#ifndef Ajax_TShell_Provision_h
#define Ajax_TShell_Provision_h
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
#include "Cpl/Persistent/RecordServer.h"
#include "Ajax/Main/PersonalityRecord.h"
#include "Driver/Crypto/Hash.h"

///
namespace Ajax {
///
namespace TShell {



/** This class implements a TShell command that is used to provision a GM6000
    unit at production.

    This class assumes that the following MP exists:
        mp::serialNumber
        mp::modelNumber
        TODO: mp::XYZ - Algorithm stuffs....
 */
class Provision : public Cpl::TShell::Cmd::Command
{
public:
    /// The command verb/identifier
    static constexpr const char* verb = "prov";

    /// The command usage string
    static constexpr const char* usage = "prov <modelNumber> <serialNumber> <consolePwd> <h-outS> <h-maxY> <h-errS> <h-dErrS> <h-outK0> <h-outK1> <h-outK2> <h-outK3> <h-outK4> <fanLow> <fanMed> <fanHi>";

    /** The command detailed help string (recommended that lines do not exceed 80 chars)
                                                          1         2         3         4         5         6         7         8
                                                 12345678901234567890123456789012345678901234567890123456789012345678901234567890
     */
    static constexpr const char* detailedHelp = "  Provisions the GM6000 with its 'personality'";


public:
    /// See Cpl::TShell::Command                                                               `
    const char* getUsage() const noexcept { return usage; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return detailedHelp; }


public:
    /// Constructor
    Provision( Cpl::Container::Map<Cpl::TShell::Command>& commandList,
               Ajax::Main::PersonalityRecord&             personalityRecord,
               Cpl::Persistent::RecordServer&             recordServer,
               Driver::Crypto::Hash&                      sha512HashFunction,
               Cpl::TShell::Security::Permission_T        minPermLevel=OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL ) noexcept;


public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept;

protected:
    /// Helper function
    bool hashPassword( const char*        plaintext,
                       Cpl::Text::String& workBufferMem,
                       Cpl::Text::String& workDigestMem,
                       Cpl::Text::String& outputBufferMemory ) noexcept;

protected:
    /// Persistent Storage Server
    Cpl::Persistent::RecordServer& m_recordServer;

    /// Personality record that I write to
    Ajax::Main::PersonalityRecord& m_personalityRec;

    /// Hash function to use.  The expectation is this a SHA512 has function
    Driver::Crypto::Hash&  m_sha512;

};

}       // end namespaces
}
#endif  // end header latch
