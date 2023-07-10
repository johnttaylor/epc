/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2020  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "Catch/catch.hpp"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Text/FString.h"
#include "Cpl/Text/DString.h"
#include "Cpl/Math/real.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Ajax/Dm/MpFlcConfig.h"
#include "common.h"
#include <string.h>


using namespace Ajax::Dm;

#define STRCMP(s1,s2)       (strcmp(s1,s2)==0)
#define MAX_STR_LENG        1024
#define SECT_               "_0test"


////////////////////////////////////////////////////////////////////////////////
// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase   modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );

// Allocate my Model Points
static MpFlcConfig          mp_apple_( modelDb_, "APPLE" );
static MpFlcConfig          mp_orange_( modelDb_, "ORANGE" );



////////////////////////////////////////////////////////////////////////////////

//
// Note: The bare minimum I need to test code that is 'new' to concrete MP type
//
TEST_CASE( "MpFlcConfig" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    Cpl::Text::FString<MAX_STR_LENG> errorMsg = "noerror";
    char string[MAX_STR_LENG + 1];
    bool truncated;
    bool valid;
    Ajax::Heating::Flc::Config_T value;
    Ajax::Heating::Flc::Config_T expectedVal;
    mp_apple_.setInvalid();
    mp_orange_.setInvalid();

    SECTION( "gets" )
    {
        // Gets...
        const char* name = mp_apple_.getName();
        REQUIRE( strcmp( name, "APPLE" ) == 0 );

        size_t s = mp_apple_.getSize();
        REQUIRE( s == sizeof( value ) );

        s = mp_apple_.getExternalSize();
        REQUIRE( s == sizeof( value ) + sizeof( bool ) );

        const char* mpType = mp_apple_.getTypeAsText();
        CPL_SYSTEM_TRACE_MSG( SECT_, ("typeText: [%s]", mpType) );
        REQUIRE( strcmp( mpType, "Ajax::Dm::MpFlcConfig" ) == 0 );
    }

    SECTION( "read/writes" )
    {
        int32_t outk[5] ={ 10, 20, 30, -10, -5 };
        expectedVal.maxY = 1;
        expectedVal.outputScalar = 2;
        expectedVal.errScalar = 3;
        expectedVal.dErrScalar = 4;
        memcpy( expectedVal.outK, outk, sizeof( expectedVal.outK) );
        mp_apple_.write( expectedVal );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( expectedVal == value );

        expectedVal.outputScalar = 22;
        mp_apple_.write( expectedVal );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( expectedVal == value );

        expectedVal.outK[0] = 666;
        mp_apple_.write( expectedVal );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( expectedVal == value );
    }

    SECTION( "copy" )
    {
        int32_t outk[5] ={ 110, 20, 30, -10, -5 };
        expectedVal.maxY = 11;
        expectedVal.outputScalar = 21;
        expectedVal.errScalar = 31;
        expectedVal.dErrScalar = 41;
        memcpy( expectedVal.outK, outk, sizeof( expectedVal.outK ) );
        mp_apple_.write( expectedVal );
        mp_orange_.copyFrom( mp_apple_ );
        valid = mp_orange_.read( value );
        REQUIRE( valid );
        REQUIRE( expectedVal == value );

        mp_orange_.setInvalid();
        REQUIRE( mp_apple_.isNotValid() == false );
        mp_apple_.copyFrom( mp_orange_ );
        REQUIRE( mp_apple_.isNotValid() );
    }
    
    SECTION( "toJSON-pretty" )
    {
        int32_t outk[5] ={ 110, 20, 30, -10, -5 };
        expectedVal.maxY = 11;
        expectedVal.outputScalar = 21;
        expectedVal.errScalar = 31;
        expectedVal.dErrScalar = 41;
        memcpy( expectedVal.outK, outk, sizeof( expectedVal.outK ) );
        mp_apple_.write( expectedVal );
        mp_apple_.toJSON( string, MAX_STR_LENG, truncated, true, true );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("toJSON: [%s]", string) );

        StaticJsonDocument<1024> doc;
        DeserializationError err = deserializeJson( doc, string );
        REQUIRE( err == DeserializationError::Ok );
        REQUIRE( doc["val"]["maxY"] ==  11 );
        REQUIRE( doc["val"]["outScalar"] == 21 );
        REQUIRE( doc["val"]["errScalar"] == 31 );
        REQUIRE( doc["val"]["dErrScalar"] == 41 );
        REQUIRE( doc["val"]["outK"][0] == 110 );
        REQUIRE( doc["val"]["outK"][1] == 20 );
        REQUIRE( doc["val"]["outK"][2] == 30 );
        REQUIRE( doc["val"]["outK"][3] == -10 );
        REQUIRE( doc["val"]["outK"][4] == -5 );
    }
 
    SECTION( "fromJSON" )
    {
        const char* json = R"({
              "name": "APPLE",
              "val": {
                "maxY": 121,
                "outScalar": 221,
                "dErrScalar": 421,
                "errScalar": 321,
                "outK": [
                  111,
                  20,
                  30,
                  -10,
                  -6
                ]
              }
            })";
        bool result = modelDb_.fromJSON( json, &errorMsg );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("errorMsg: [%s]", errorMsg.getString()) );
        REQUIRE( result == true );
        valid = mp_apple_.read( value );
        REQUIRE( valid );

        int32_t outk[5] ={ 111, 20, 30, -10, -6 };
        expectedVal.maxY = 121;
        expectedVal.outputScalar = 221;
        expectedVal.errScalar = 321;
        expectedVal.dErrScalar = 421;
        memcpy( expectedVal.outK, outk, sizeof( expectedVal.outK ) );
        REQUIRE( expectedVal == value );

        json = "{name:\"APPLE\", val:{maxY:\"abc\"}}";
        result = modelDb_.fromJSON( json, &errorMsg );
        REQUIRE( result == true );
        REQUIRE( errorMsg == "noerror" );
        REQUIRE( expectedVal == value );

        json = "{name:\"APPLE\", val:{outK:\"abc\"}}";
        result = modelDb_.fromJSON( json, &errorMsg );
        REQUIRE( result == true );
        REQUIRE( errorMsg == "noerror" );
        REQUIRE( expectedVal == value );
    }

    SECTION( "observer" )
    {
        Cpl::Dm::MailboxServer     t1Mbox;
        int32_t outk[5] ={ 110, 20, 30, -10, -5 };
        expectedVal.maxY = 11;
        expectedVal.outputScalar = 21;
        expectedVal.errScalar = 31;
        expectedVal.dErrScalar = 41;
        memcpy( expectedVal.outK, outk, sizeof( expectedVal.outK ) );
        mp_apple_.write( expectedVal );
        Viewer<MpFlcConfig, Ajax::Heating::Flc::Config_T>    viewer_apple1( t1Mbox, Cpl::System::Thread::getCurrent(), mp_apple_, expectedVal );
        Cpl::System::Thread* t1 = Cpl::System::Thread::create( t1Mbox, "T1" );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Created Viewer thread (%p)", t1) );

        // NOTE: The MP MUST be in the INVALID state at the start of this test
        viewer_apple1.open();
        mp_apple_.write( expectedVal );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Waiting for viewer signal...") );
        Cpl::System::Thread::wait();
        viewer_apple1.close();
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Viewer closed.") );

        // Shutdown threads
        t1Mbox.pleaseStop();
        Cpl::System::Api::sleep( 100 ); // allow time for threads to stop
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Destroying Viewer thread (%p)...", t1) );
        Cpl::System::Thread::destroy( *t1 );
        Cpl::System::Api::sleep( 100 ); // allow time for threads to stop BEFORE the runnable object goes out of scope
    }


    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}
