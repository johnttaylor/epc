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

#include "Provision.h"
#include "mp/ModelPoints.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"
#include "Driver/Crypto/Random.h"
#include "Driver/Crypto/PasswordHash/Api.h"
#include "Cpl/System/Trace.h"
#include <string.h>

#define SECT_   "Ajax::TShell"

///
using namespace Ajax::TShell;


///////////////////////////
Provision::Provision( Cpl::Container::Map<Cpl::TShell::Command>& commandList,
                      Ajax::Main::PersonalityRecord&             personalityRecord,
                      Cpl::Persistent::RecordServer&             recordServer,
                      Driver::Crypto::Hash&                      sha512HashFunction,
                      Cpl::TShell::Security::Permission_T        minPermLevel ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, verb, minPermLevel )
    , m_recordServer( recordServer )
    , m_personalityRec( personalityRecord )
    , m_sha512( sha512HashFunction )
{
}

static bool parseInt32( const char* argName, const char* token, int32_t& val, Cpl::TShell::Context_& context, Cpl::Text::String& outtext  );

#define PARSE_INT32( n, t, v, c, buf )      if ( !parseInt32( n,t,v,c,buf ) ) { return Command::eERROR_BAD_SYNTAX; }
 
///////////////////////////
Cpl::TShell::Command::Result_T Provision::execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( cmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              outtext  = context.getOutputBuffer();

    // Initial check for not-enough-tokens
    if ( tokens.numParameters() != 4+9+3+1 )
    {
        return Command::eERROR_MISSING_ARGS;
    }

    // Hash the password (and updates the Model points when successful)
    if ( !hashPassword( tokens.getParameter( 3 ), context.getOutputBuffer(), context.getTokenBuffer(), context.getTokenBuffer2() ) )
    {
        context.writeFrame( "Failed to hash the console password" );
        return Command::eERROR_FAILED;
    }

    // Model/Serial
    mp::modelNumber.write( tokens.getParameter( 1 ) );
    mp::serialNumber.write( tokens.getParameter( 2 ) );

    // Heating Algo Config
    Ajax::Heating::Flc::Config_T config;
    PARSE_INT32( "<h-outS>", tokens.getParameter( 4 ), config.outputScalar, context, outtext );
    PARSE_INT32( "<h-maxY>", tokens.getParameter( 5 ), config.maxY, context, outtext );
    PARSE_INT32( "<h-errS>", tokens.getParameter( 6 ), config.errScalar, context, outtext );
    PARSE_INT32( "<h-dErrS>", tokens.getParameter( 7 ), config.dErrScalar, context, outtext );
    PARSE_INT32( "<h-outK0>", tokens.getParameter( 8 ), config.outK[0], context, outtext );
    PARSE_INT32( "<h-outK1>", tokens.getParameter( 9 ), config.outK[1], context, outtext );
    PARSE_INT32( "<h-outK2>", tokens.getParameter( 10 ), config.outK[2], context, outtext );
    PARSE_INT32( "<h-outK3>", tokens.getParameter( 11 ), config.outK[3], context, outtext );
    PARSE_INT32( "<h-outK4>", tokens.getParameter( 12 ), config.outK[4], context, outtext );
    int32_t fanLow, fanMed, fanHi;
    PARSE_INT32( "<fanLow>", tokens.getParameter( 13 ), fanLow, context, outtext );
    PARSE_INT32( "<fanMed>", tokens.getParameter( 14 ), fanMed, context, outtext );
    PARSE_INT32( "<fanHi>", tokens.getParameter( 15 ), fanHi, context, outtext );
    int32_t maxCap;
    PARSE_INT32( "<maxCap>", tokens.getParameter( 16 ), maxCap, context, outtext );
    if ( maxCap < 0 )
    {
        context.writeFrame( "ERROR: <maxCap> is negative" );
        return Command::eERROR_FAILED;
    }
    mp::flcConfig.write( config );
    mp::fanLowPercentage.write( fanLow );
    mp::fanMedPercentage.write( fanMed );
    mp::fanHighPercentage.write( fanHi );
    mp::maxHeatingCapacity.write( maxCap );

    // Update NVRAM
    m_personalityRec.flush( m_recordServer );

    // Feedback
    bool io = true;
    outtext.format( "Model Number:     %s", tokens.getParameter( 1 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "Serial Number:    %s", tokens.getParameter( 2 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "Console Pwd:      %s", tokens.getParameter( 3 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<h-outS>:         %s", tokens.getParameter( 4) );
    io &= context.writeFrame( outtext );
    outtext.format( "<h-maxY>:         %s", tokens.getParameter( 5 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<h-errS>:         %s", tokens.getParameter( 6 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<h-dErrS>:        %s", tokens.getParameter( 7 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<h-outK0>:        %s", tokens.getParameter( 8 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<h-outK1>:        %s", tokens.getParameter( 9 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<h-outK2>:        %s", tokens.getParameter( 10 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<h-outK3>:        %s", tokens.getParameter( 11 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<h-outK4>:        %s", tokens.getParameter( 12 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<fanLow>:         %s", tokens.getParameter( 13 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<fanMed>:         %s", tokens.getParameter( 14 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<fanHi>:          %s", tokens.getParameter( 15 ) );
    io &= context.writeFrame( outtext );
    outtext.format( "<maxCap>:         %s", tokens.getParameter( 16 ) );
    io &= context.writeFrame( outtext );

    // Return the command result
    return io ? Command::eSUCCESS : Command::eERROR_IO;
}


bool Provision::hashPassword( const char*        plaintext,
                              Cpl::Text::String& workBufferMem,
                              Cpl::Text::String& workDigestMem,
                              Cpl::Text::String& outputBufferMemory ) noexcept
{
    // Basic password validation. TODO: There is more validation required here...
    size_t pwdLen = strlen( plaintext );
    if ( pwdLen < OPTION_AJAX_PASSWORD_MIN_LENGTH || pwdLen > OPTION_AJAX_PASSWORD_MAX_LENGTH )
    {
        return false;
    }

    // Work space memory
    int workBufferSize;
    uint8_t* workBuffer = (uint8_t*) workBufferMem.getBuffer( workBufferSize );
    CPL_SYSTEM_ASSERT( workBufferSize >= OPTION_AJAX_HASHED_PASSWORD_SIZE + OPTION_AJAX_PASSWORD_MAX_LENGTH );
    int workDigestSize;
    uint8_t* workDigest = (uint8_t*) workDigestMem.getBuffer( workDigestSize );
    CPL_SYSTEM_ASSERT( workDigestSize >= OPTION_AJAX_HASHED_PASSWORD_SIZE );
    int outputBufferSize;
    uint8_t* outputBuffer = (uint8_t*) outputBufferMemory.getBuffer( outputBufferSize );
    CPL_SYSTEM_ASSERT( outputBufferSize >= OPTION_AJAX_HASHED_PASSWORD_SIZE );

    // Create a random salt
    uint8_t salt[16] ={ 0, };
    Driver::Crypto::generateRandom( salt, sizeof( salt ) );

    // Hash the password
    CPL_SYSTEM_TRACE_ALLOCATE( unsigned long, startTime, Cpl::System::ElapsedTime::milliseconds() );
    DriverCryptoStatus_T result = Driver::Crypto::PasswordHash::hash( plaintext,
                                                                      strlen( plaintext ),
                                                                      salt,
                                                                      sizeof( salt ),
                                                                      workBuffer,
                                                                      workBufferSize,
                                                                      workDigest,
                                                                      workDigestSize,
                                                                      m_sha512,
                                                                      OPTION_AJAX_HASHED_PASSWORD_ITERATIONS,
                                                                      outputBuffer,
                                                                      outputBufferSize );
    CPL_SYSTEM_TRACE_ALLOCATE( unsigned long, endTime, Cpl::System::ElapsedTime::milliseconds() );
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Hash time for %lu iterations = %lu ms", OPTION_AJAX_HASHED_PASSWORD_ITERATIONS, endTime - startTime) );

    // Update the model points
    if ( result == DRIVER_CRYPTO_SUCCESS )
    {
        mp::consolePwdHash.write( outputBuffer, OPTION_AJAX_HASHED_PASSWORD_SIZE );
        mp::consolePwdSalt.write( salt, sizeof( salt ) );
        return true;
    }
    return false;
}

bool parseInt32( const char* argName, const char* token, int32_t& val, Cpl::TShell::Context_& context, Cpl::Text::String& outtext )
{
    long tmpVal;
    if ( Cpl::Text::a2l( tmpVal, token ) )
    {
        val = tmpVal;
        return true;
    }
    outtext.format( "Failed to parse %s (%s)", argName, token );
    context.writeFrame( outtext );
    return false;
}
