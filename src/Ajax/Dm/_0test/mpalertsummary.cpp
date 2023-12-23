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
#include "Ajax/Dm/MpAlertSummary.h"
#include "Ajax/Dm/MpAlertSummary.h"
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
static MpAlertSummary   mp_apple_( modelDb_, "APPLE" );
static MpAlertSummary   mp_orange_( modelDb_, "ORANGE" );
static MpAlert          mp_alert1_( modelDb_, "alert1", Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE, 20 );
static MpAlert          mp_alert2_( modelDb_, "alert2", Ajax::Type::Alert::eONBOARD_SENSOR_FAILED, 22 );



////////////////////////////////////////////////////////////////////////////////

//
// Note: The bare minimum I need to test code that is 'new' to concrete MP type
//
TEST_CASE( "MpAlertSummary" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    Cpl::Text::FString<MAX_STR_LENG> errorMsg = "noerror";
    char string[MAX_STR_LENG + 1];
    bool truncated;
    bool valid;
    MpAlertSummary::Data value;
    MpAlertSummary::Data expectedValue;
    mp_apple_.setInvalid();
    mp_orange_.setInvalid();
    mp_alert1_.setInvalid();
    mp_alert2_.setInvalid();

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
        REQUIRE( strcmp( mpType, "Ajax::Dm::MpAlertSummary" ) == 0 );
    }


    SECTION( "read/writes" )
    {
        expectedValue.count = 0;
        valid = mp_apple_.read( value );
        REQUIRE( !valid );
        mp_apple_.write( expectedValue );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( value.count == expectedValue.count );

        expectedValue.count           = 2;
        expectedValue.activeAlerts[0] = &mp_alert1_;
        expectedValue.activeAlerts[1] = &mp_alert2_;
        mp_apple_.write( expectedValue );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( value.count == expectedValue.count );
        REQUIRE( expectedValue.activeAlerts[0] == &mp_alert1_ );
        REQUIRE( expectedValue.activeAlerts[1] == &mp_alert2_ );

        expectedValue.count           = 1;
        expectedValue.activeAlerts[0] = nullptr;
        mp_orange_.write( expectedValue );
        valid = mp_orange_.read( value );
        REQUIRE( valid );
        REQUIRE( value.count == expectedValue.count );
        REQUIRE( expectedValue.activeAlerts[0] == nullptr );
    }

    SECTION( "copy" )
    {
        expectedValue.count = 1;
        expectedValue.activeAlerts[0] = &mp_alert2_;
        mp_apple_.write( expectedValue );
        mp_orange_.copyFrom( mp_apple_ );
        valid = mp_orange_.read( value );
        REQUIRE( valid );
        REQUIRE( value.count == expectedValue.count );
        REQUIRE( expectedValue.activeAlerts[0] == &mp_alert2_ );


        mp_orange_.setInvalid();
        REQUIRE( mp_apple_.isNotValid() == false );
        mp_apple_.copyFrom( mp_orange_ );
        REQUIRE( mp_apple_.isNotValid() );
    }

    SECTION( "toJSON-pretty" )
    {
        expectedValue.count           = 2;
        expectedValue.activeAlerts[0] = &mp_alert1_;
        expectedValue.activeAlerts[1] = &mp_alert2_;
        mp_alert1_.raiseAlert();
        mp_alert2_.raiseAlert();

        mp_apple_.write( expectedValue );
        mp_apple_.toJSON( string, MAX_STR_LENG, truncated, true, true );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("toJSON: [%s]", string) );

        StaticJsonDocument<1024> doc;
        DeserializationError err = deserializeJson( doc, string );
        REQUIRE( err == DeserializationError::Ok );
        REQUIRE( doc["locked"] == false );
        REQUIRE( doc["valid"] == true );
        REQUIRE( doc["val"]["count"] == 2 );
        REQUIRE( strcmp( doc["val"]["active"][0], "eHITEMP_HEATER_FAILSAFE" ) == 0 );
    }

    SECTION( "fromJSON" )
    {
        const char* json = "{name:\"APPLE\", val:[\"alert2\",\"alert1\"]}}";
        bool result = modelDb_.fromJSON( json, &errorMsg );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("errorMsg: [%s]", errorMsg.getString()) );
        REQUIRE( result == true );
        valid = mp_apple_.read( value );
        REQUIRE( valid );
        REQUIRE( value.count == 2 );
        REQUIRE( value.activeAlerts[0] == &mp_alert2_ );
        REQUIRE( value.activeAlerts[1] == &mp_alert1_ );


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

#if 0
    SECTION( "observer" )
    {
        expectedValue = { Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE, APPLE_PRIORITY };
        Cpl::Dm::MailboxServer          t1Mbox;
        Viewer<MpAlertSummary, MpAlertSummary::Data>  viewer_apple1( t1Mbox, Cpl::System::Thread::getCurrent(), mp_apple_, expectedValue );
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
#endif
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}