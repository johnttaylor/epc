/*-----------------------------------------------------------------------------
 * This file is part of the Colony.Core Project.  The Colony.Core Project is an
 * open source project with a BSD type of licensing agreement.  See the license
 * agreement (license.txt) in the top/ directory or on the Internet at
 * http://integerfox.com/colony.core/license.txt
 *
 * Copyright (c) 2014-2025  John T. Taylor
 *
 * Redistributions of the source code must retain the above copyright notice.
 *----------------------------------------------------------------------------*/

#include "Fs.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"
#include "Cpl/Io/File/Api.h"
#include "Cpl/Io/File/Input.h"
#include "Cpl/System/Assert.h"
#include "Cpl/Text/format.h"
#include "Cpl/Text/atob.h"
#include <string.h>

///
using namespace Cpl::Io::File::Littlefs::TShell;


namespace {

class Walker : public Cpl::Io::File::Api::DirectoryWalker
{
public:
    Cpl::TShell::Context_& m_context;
    Cpl::Text::String&     m_outtext;
    unsigned               m_count;
public:
    ///
    Walker( Cpl::TShell::Context_& context, Cpl::Text::String& outtext )
        : m_context( context )
        , m_outtext( outtext )
        , m_count( 0 )
    {
    }

public:
    Cpl::Type::Traverser::Status_T item( const char* currentParent, const char* fsEntryName, Cpl::Io::File::Api::Info& entryInfo )
    {
        m_outtext.format( "%s%s %8lu  %s",
                          entryInfo.m_isFile ? "f" : "-",
                          entryInfo.m_isDir ? "d" : "-",
                          entryInfo.m_size,
                          fsEntryName );
        bool io = m_context.writeFrame( m_outtext );
        m_count++;
        return io ? Cpl::Type::Traverser::eCONTINUE : Cpl::Type::Traverser::eABORT;
    }
};

}  // end anonymous namespace

///////////////////////////
Fs::Fs( Cpl::Container::SList<Cpl::TShell::Command>& commandList,
        unsigned                                   numVolumes,
        const char*                                volumeRootPaths[],
        Cpl::TShell::Security::Permission_T        minPermLevel ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, verb, minPermLevel )
    , m_volPaths( volumeRootPaths )
    , m_numVols( numVolumes )
{
}

bool dumpFile( const char*            filename,
               Cpl::Io::File::Input&  fd,
               Cpl::TShell::Context_& context,
               Cpl::Text::String&     outtext,
               Cpl::Text::String&     tempBuf,
               int                    startOffset = 0,
               int                    length      = -1 ) noexcept
{
    int  tempBufSize;
    auto bufPtr = (uint8_t*)tempBuf.getBuffer( tempBufSize );
    CPL_SYSTEM_ASSERT( tempBufSize >= OPTION_CPL_IO_FILE_LITTLEFS_TSHELL_BYTES_PER_LINE );

    // Set the initial file offset
    if ( startOffset > 0 )
    {
        if ( !fd.setAbsolutePos( startOffset ) )
        {
            outtext.format( "Error seeking to offset: %d", startOffset );
            context.writeFrame( outtext );
            return false;
        }
    }

    // Read till EOF or the specified length
    bool noError = true;

    while ( noError )
    {
        // Determine how many bytes to read
        int numBytesToRead = OPTION_CPL_IO_FILE_LITTLEFS_TSHELL_BYTES_PER_LINE;
        if ( length > 0 && length < numBytesToRead )
        {
            numBytesToRead = length;
        }

        // Read the data
        int bytesRead;
        noError = fd.read( bufPtr, numBytesToRead, bytesRead );
        if ( !noError && !fd.isEof() )
        {
            // Error reading file
            outtext.format( "Error reading file: %s", filename );
            context.writeFrame( outtext );
            return false;
        }

        // Display the data
        if ( noError )
        {
            outtext.format( "%08X: ", startOffset );
            if ( Cpl::Text::bufferToViewer( bufPtr, bytesRead, outtext, OPTION_CPL_IO_FILE_LITTLEFS_TSHELL_BYTES_PER_LINE, " ", true, true ) )
            {
                startOffset += OPTION_CPL_IO_FILE_LITTLEFS_TSHELL_BYTES_PER_LINE;
                context.writeFrame( outtext );
            }
            else
            {
                context.writeFrame( "Failed to format the input data" );
                return false;
            }
        }

        // Update the length (when specified)
        if ( length > 0 )
        {
            length -= bytesRead;
            if ( length <= 0 )
            {
                break;
            }
        }
    }

    return true;
}

///////////////////////////
Cpl::TShell::Command::Result_T Fs::execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( cmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              outtext  = context.getOutputBuffer();
    Cpl::Text::String&              tempBuf  = context.getTokenBuffer();
    unsigned                        numParms = tokens.numParameters();
    bool                            io       = true;

    // Remove file sub-command
    if ( numParms == 3 && strcmp( tokens.getParameter( 1 ), "rm") == 0 )
    {
        if ( Cpl::Io::File::Api::remove( tokens.getParameter( 2 ) ) )
        {
            outtext.format( "Removed entry: %s", tokens.getParameter( 2 ) );
            return context.writeFrame( outtext ) ? Command::eSUCCESS : Command::eERROR_IO;
        }
        else
        {
            outtext.format( "Error removing entry: %s", tokens.getParameter( 2 ) );
            context.writeFrame( outtext );
            return Command::eERROR_FAILED;
        }
    }

    // Move entry sub-command
    else if ( numParms == 4 && strcmp( tokens.getParameter( 1 ), "mv") == 0 )
    {
        if ( Cpl::Io::File::Api::renameInPlace( tokens.getParameter( 2 ), tokens.getParameter( 3 ) ) )
        {
            outtext.format( "Renamed entry: %s to: %s", tokens.getParameter( 2 ), tokens.getParameter( 3 )  );
            return context.writeFrame( outtext ) ? Command::eSUCCESS : Command::eERROR_IO;
        }
        else
        {
            outtext.format( "Error renaming entry: %s to: %s", tokens.getParameter( 2 ), tokens.getParameter( 3 ) );
            context.writeFrame( outtext );
            return Command::eERROR_FAILED;
        }
    }

    // Make file entry sub-command
    else if ( numParms == 3 && strncmp( tokens.getParameter( 1 ), "mkf", 3) == 0 )
    {
        if ( Cpl::Io::File::Api::createFile( tokens.getParameter( 2 ) ) )
        {
            outtext.format( "Created file: %s", tokens.getParameter( 2 ) );
            return context.writeFrame( outtext ) ? Command::eSUCCESS : Command::eERROR_IO;
        }
        else
        {
            outtext.format( "Failed to create file: %s", tokens.getParameter( 2 ) );
            context.writeFrame( outtext );
            return Command::eERROR_FAILED;
        }
    }

    // Make directory entry sub-command
    else if ( numParms == 3 && strncmp( tokens.getParameter( 1 ), "mkd", 3) == 0 )
    {
        if ( Cpl::Io::File::Api::createDirectory( tokens.getParameter( 2 ) ) )
        {
            outtext.format( "Created directory: %s", tokens.getParameter( 2 ) );
            return context.writeFrame( outtext ) ? Command::eSUCCESS : Command::eERROR_IO;
        }
        else
        {
            outtext.format( "Failed to create directory: %s", tokens.getParameter( 2 ) );
            context.writeFrame( outtext );
            return Command::eERROR_FAILED;
        }
    }

    // Dump a file sub-command
    else if ( numParms >= 3 && tokens.getParameter( 1 )[0] == 'd' )
    {
        Cpl::Io::File::Input fd( tokens.getParameter( 2 ) );
        if ( fd.isOpened() )
        {
            // Display entire file
            if ( numParms == 3 )
            {
                return dumpFile( tokens.getParameter( 2 ), fd, context, outtext, tempBuf ) ? Command::eSUCCESS : Command::eERROR_FAILED;
            }

            // Display a sub-set of the file
            else if ( numParms >= 4 && numParms <= 5 )
            {
                // Parse the starting offset and length arguments
                int startOffset = 0;
                if ( !Cpl::Text::a2i( startOffset, tokens.getParameter( 3 ), 10, " " ) )
                {
                    outtext.format( "Invalid starting offset: %s", tokens.getParameter( 3 ) );
                    context.writeFrame( outtext );
                    return Command::eERROR_FAILED;
                }
                int length = -1;
                if ( numParms == 5 && !Cpl::Text::a2i( length, tokens.getParameter( 4 ), 10, " " ) )
                {
                    outtext.format( "Invalid length: %s", tokens.getParameter( 4 ) );
                    context.writeFrame( outtext );
                    return Command::eERROR_FAILED;
                }

                // Display the file
                return dumpFile( tokens.getParameter( 2 ), fd, context, outtext, tempBuf, startOffset, length ) ? Command::eSUCCESS : Command::eERROR_FAILED;
            }
            else
            {
                return context.writeFrame( "Invalid number of parameters for 'dump' command" ) ? Command::eSUCCESS : Command::eERROR_IO;
            }
        }
        else
        {
            outtext.format( "Unable to open file: %s", tokens.getParameter( 2 ) );
            return context.writeFrame( outtext ) ? Command::eSUCCESS : Command::eERROR_IO;
        }
        return Command::eERROR_FAILED;
    }

    // Show volumes sub-command
    if ( numParms == 2 && tokens.getParameter( 1 )[0] == 'v' )
    {
        if ( m_numVols == 1 )
        {
            return context.writeFrame( "vol1 root:  /" ) ? Command::eSUCCESS : Command::eERROR_IO;
        }
        else
        {
            for ( unsigned i = 0; i < m_numVols; i++ )
            {
                outtext.format( "vol%d root:  %s", i + 1, m_volPaths[i] );
                io &= context.writeFrame( outtext );
            }

            return io ? Command::eSUCCESS : Command::eERROR_IO;
        }
    }

    // List directory sub-command
    else if ( numParms == 3 && tokens.getParameter( 1 )[0] == 'l' )
    {
        outtext.format( "Listing: %s", tokens.getParameter( 2 ) );
        io &= context.writeFrame( outtext );

        // Trap the use case of the root directory when there are multiple volumes
        unsigned count = 0;
        if ( m_numVols > 1 && ( strcmp( tokens.getParameter( 2 ), "." ) == 0 || strcmp( tokens.getParameter( 2 ), "/" ) == 0 ) )
        {
            count = m_numVols;
            for ( unsigned i = 0; i < m_numVols; i++ )
            {
                outtext.format( "-d           %s", m_volPaths[i] );
                io &= context.writeFrame( outtext );
            }
        }

        // List the directory
        else
        {
            Walker walker( context, outtext );
            io    &= Cpl::Io::File::Api::walkDirectory( tokens.getParameter( 2 ), walker );
            count  = walker.m_count;
        }

        // Display the totals
        if ( io )
        {
            outtext.format( "Total: %u items", count );
            io &= context.writeFrame( outtext );
        }
        return io ? Command::eSUCCESS : Command::eERROR_FAILED;
    }

    // If I get here the command failed!
    return Command::eERROR_FAILED;
}