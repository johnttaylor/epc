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
#include "Ajax/Dm/MpAlert.h"
#include "common.h"
#include <string.h>


using namespace Ajax::Dm;

#define STRCMP(s1,s2)       (strcmp(s1,s2)==0)
#define MAX_STR_LENG        1024
#define SECT_               "_0test"

////////////////////////////////////////////////////////////////////////////////
// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase   modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );

#define APPLE_PRIORITY  10
#define ORANGE_PRIORITY 20

// Allocate my Model Points
static MpAlert          mp_apple_( modelDb_, "APPLE", Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE, APPLE_PRIORITY );
static MpAlert          mp_orange_( modelDb_, "ORANGE", Ajax::Type::Alert::eONBOARD_SENSOR_FAILED, ORANGE_PRIORITY );



////////////////////////////////////////////////////////////////////////////////

//
// Note: The bare minimum I need to test code that is 'new' to concrete MP type
//
TEST_CASE( "MpAlert" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    Cpl::Text::FString<MAX_STR_LENG> errorMsg = "noerror";
    char string[MAX_STR_LENG + 1];
    bool truncated;
    bool valid;
    MpAlert::Data value;
    MpAlert::Data expectedValue;

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
        REQUIRE( strcmp( mpType, "Ajax::Dm::MpAlert" ) == 0 );
    }


    SECTION( "read/writes" )
    {
        expectedValue ={ Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE, APPLE_PRIORITY };
        valid = mp_apple_.read( value );
        REQUIRE( !valid );
        mp_apple_.raiseAlert();
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( value.name == expectedValue.name );
        REQUIRE( value.priority == expectedValue.priority );

        mp_apple_.lowerAlert();
        valid = mp_apple_.read( value );
        REQUIRE( !valid );

        mp_apple_.raiseAlert();
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( value.name == expectedValue.name );
        REQUIRE( value.priority == expectedValue.priority );

        expectedValue ={ Ajax::Type::Alert::eONBOARD_SENSOR_FAILED, ORANGE_PRIORITY };
        mp_orange_.raiseAlert();
        valid = mp_orange_.read( value );
        REQUIRE( valid );
        REQUIRE( value.name == expectedValue.name );
        REQUIRE( value.priority == expectedValue.priority );

        mp_orange_.setInvalid();
        mp_orange_.raiseAlert();
        valid = mp_orange_.read( value );
        REQUIRE( valid );
        REQUIRE( value.name == expectedValue.name );
        REQUIRE( value.priority == expectedValue.priority );
    }

    SECTION( "copy" )
    {
        expectedValue ={ Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE, APPLE_PRIORITY };
        mp_apple_.write( expectedValue );
        mp_orange_.copyFrom( mp_apple_ );
        valid = mp_orange_.read( value );
        REQUIRE( valid );
        REQUIRE( value.name == expectedValue.name );
        REQUIRE( value.priority == expectedValue.priority );

        mp_orange_.setInvalid();
        REQUIRE( mp_apple_.isNotValid() == false );
        mp_apple_.copyFrom( mp_orange_ );
        REQUIRE( mp_apple_.isNotValid() );
    }

    SECTION( "toJSON-pretty" )
    {
        mp_apple_.raiseAlert();
        mp_apple_.toJSON( string, MAX_STR_LENG, truncated, true, true );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("toJSON: [%s]", string) );

        StaticJsonDocument<1024> doc;
        DeserializationError err = deserializeJson( doc, string );
        REQUIRE( err == DeserializationError::Ok );
        REQUIRE( doc["locked"] == false );
        REQUIRE( doc["valid"] == true );
        REQUIRE( strcmp( doc["val"]["name"], "eHITEMP_HEATER_FAILSAFE") == 0 );
        REQUIRE( doc["val"]["priority"] == APPLE_PRIORITY );
    }

    SECTION( "fromJSON" )
    {
        const char* json = "{name:\"APPLE\", val:true}}";
        bool result = modelDb_.fromJSON( json, &errorMsg );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("errorMsg: [%s]", errorMsg.getString()) );
        REQUIRE( result == true );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( value.name == +Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE );
        REQUIRE( value.priority == APPLE_PRIORITY );

        json = "{name:\"APPLE\", val:{ack:abc}}";
        result = modelDb_.fromJSON( json, &errorMsg );
        REQUIRE( result == false );
        REQUIRE( errorMsg != "noerror" );

        result = modelDb_.fromJSON( json );
        REQUIRE( result == false );

        json = "{name:\"APPLE\", val:\"abc\"}}";
        result = modelDb_.fromJSON( json, &errorMsg );
        REQUIRE( result == false );
        REQUIRE( errorMsg != "noerror" );

        result = modelDb_.fromJSON( json );
        REQUIRE( result == false );
    }

    SECTION( "observer" )
    {
        expectedValue = { Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE, APPLE_PRIORITY };
        Cpl::Dm::MailboxServer          t1Mbox;
        Viewer<MpAlert, MpAlert::Data>  viewer_apple1( t1Mbox, Cpl::System::Thread::getCurrent(), mp_apple_, expectedValue );
        Cpl::System::Thread* t1 = Cpl::System::Thread::create( t1Mbox, "T1" );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Created Viewer thread (%p)", t1) );

        // NOTE: The MP MUST be in the INVALID state at the start of this test
        mp_apple_.setInvalid();
        viewer_apple1.open();
        mp_apple_.raiseAlert();
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Waiting for viewer signal...") );
        Cpl::System::Thread::wait();
        viewer_apple1.close();
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Viewer closed.") );

        // Shutdown threads
        t1Mbox.pleaseStop();
        Cpl::System::Api::sleep( 100 ); // allow time for threads to stop
        REQUIRE( t1->isRunning() == false );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Destroying Viewer thread (%p)...", t1) );
        Cpl::System::Thread::destroy( *t1 );
        Cpl::System::Api::sleep( 100 ); // allow time for threads to stop BEFORE the runnable object goes out of scope
    }

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}
