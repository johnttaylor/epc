/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2023  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "common.h"
#include "Catch/catch.hpp"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Thread.h"
#include "Cpl/Text/FString.h"
#include "Cpl/Text/DString.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Ajax/Dm/MpFanMode.h"
#include "Cpl/Type/enum.h"
#include "Cpl/Itc/CloseSync.h"
#include <string.h>

#define STRCMP(s1,s2)       (strcmp(s1,s2)==0)
#define MAX_STR_LENG        1024
#define SECT_               "_0test"


#define INITIAL_VALUE       Ajax::Type::FanMode::eLOW

using namespace Ajax::Dm;

////////////////////////////////////////////////////////////////////////////////

// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase    modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );

// Allocate my Model Points
static MpFanMode       mp_apple_( modelDb_, "APPLE" );
static MpFanMode       mp_orange_( modelDb_, "ORANGE", INITIAL_VALUE );


////////////////////////////////////////////////////////////////////////////////

//
// Note: The bare minimum I need to test code that is 'new' to concrete MP type
//
TEST_CASE( "MpFanMode" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    Cpl::Text::FString<MAX_STR_LENG> errorMsg = "noerror";
    char string[MAX_STR_LENG + 1];
    bool truncated;
    bool valid;
    Ajax::Type::FanMode value = Ajax::Type::FanMode::eHIGH;
    mp_apple_.setInvalid();

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
        REQUIRE( strcmp( mpType, "Ajax::Dm::MpFanMode" ) == 0 );
    }


    SECTION( "read/writes" )
    {
        mp_apple_.write( Ajax::Type::FanMode::eHIGH );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( value == +Ajax::Type::FanMode::eHIGH );
        mp_apple_.write( Ajax::Type::FanMode::eLOW );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( value == +Ajax::Type::FanMode::eLOW );

        valid = mp_orange_.read( value );
        REQUIRE( valid );
        REQUIRE( value == +INITIAL_VALUE );
    }

    SECTION( "copy" )
    {
        mp_apple_.write( Ajax::Type::FanMode::eHIGH );
        mp_orange_.copyFrom( mp_apple_ );
        valid = mp_apple_.read( value );
        REQUIRE( value == +Ajax::Type::FanMode::eHIGH );
        REQUIRE( valid );

        mp_apple_.setInvalid();
        mp_orange_.copyFrom( mp_apple_ );
        REQUIRE( mp_orange_.isNotValid() );
    }


    SECTION( "toJSON-pretty" )
    {
        mp_apple_.write( Ajax::Type::FanMode::eLOW );
        mp_apple_.toJSON( string, MAX_STR_LENG, truncated, true, true );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("toJSON: [%s]", string) );

        StaticJsonDocument<1024> doc;
        DeserializationError err = deserializeJson( doc, string );
        REQUIRE( err == DeserializationError::Ok );
        REQUIRE( doc["locked"] == false );
        REQUIRE( doc["valid"] == true );
        REQUIRE( STRCMP( doc["val"], (+Ajax::Type::FanMode::eLOW)._to_string() ) );
    }

    SECTION( "fromJSON" )
    {
        const char* json = "{name:\"APPLE\", val:\"eLOW\"}";
        bool result = modelDb_.fromJSON( json );
        REQUIRE( result == true );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( value == +Ajax::Type::FanMode::eLOW );

        json = "{name:\"APPLE\", val:\"true\"}";
        result = modelDb_.fromJSON( json, &errorMsg );
        REQUIRE( result == false );
        REQUIRE( errorMsg != "noerror" );

        result = modelDb_.fromJSON( json );
        REQUIRE( result == false );

        json = "{name:\"APPLE\", val:true}";
        result = modelDb_.fromJSON( json, &errorMsg );
        REQUIRE( result == false );
        REQUIRE( errorMsg != "noerror" );

        result = modelDb_.fromJSON( json );
        REQUIRE( result == false );
    }

    SECTION( "observer" )
    {
        Cpl::Dm::MailboxServer   t1Mbox;
        Viewer<MpFanMode, Ajax::Type::FanMode>        viewer_apple1( t1Mbox, Cpl::System::Thread::getCurrent(), mp_apple_, Ajax::Type::FanMode::eHIGH );
        Cpl::System::Thread* t1 = Cpl::System::Thread::create( t1Mbox, "T1" );

        // NOTE: The MP MUST be in the INVALID state at the start of this test
        viewer_apple1.open();
        mp_apple_.write( Ajax::Type::FanMode::eHIGH );
        Cpl::System::Thread::wait();
        viewer_apple1.close();

        // Shutdown threads
        t1Mbox.pleaseStop();
        Cpl::System::Api::sleep( 100 ); // allow time for threads to stop
        REQUIRE( t1->isRunning() == false );
        Cpl::System::Thread::destroy( *t1 );
        Cpl::System::Api::sleep( 100 ); // allow time for threads to stop BEFORE the runnable object goes out of scope
    }

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}
