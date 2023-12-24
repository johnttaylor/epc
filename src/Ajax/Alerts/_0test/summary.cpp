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

#include "Catch/catch.hpp"
#include "Catch/helpers.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "mp/ModelPoints.h"
#include "Ajax/Alerts/Summary.h"

using namespace Ajax::Alerts;

#define SECT_   "_0test"



static Ajax::Dm::MpAlert* alerts_[Ajax::Type::Alert::NUM_ALERTS] ={
    &mp::failedSafeAlert ,
    &mp::sensorFailAlert,
    &mp::remoteSensorFailAlert,
    &mp::postFailedAlert
};

////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "Summary" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Cpl::Dm::MailboxServer  uutMbox;
    Cpl::System::Thread*    t1 = Cpl::System::Thread::create( uutMbox, "UUT" );
    Summary uut( uutMbox, alerts_ );
    mp::alertSummary.setInvalid();
    mp::failedSafeAlert.setInvalid();
    mp::sensorFailAlert.setInvalid();
    mp::remoteSensorFailAlert.setInvalid();
    mp::postFailedAlert.setInvalid();

    SECTION( "one-alert" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: one-alert") );
        uint16_t seqNum = mp::alertSummary.getSequenceNumber();
        
        uut.open();
        minWaitOnModelPointSeqNumChange<Ajax::Dm::MpAlertSummary>( mp::alertSummary, 10, seqNum, &seqNum );
        mp::failedSafeAlert.raiseAlert();
        minWaitOnModelPointSeqNumChange<Ajax::Dm::MpAlertSummary>( mp::alertSummary, 10, seqNum, &seqNum );


    }

    // Shutdown threads
    uutMbox.pleaseStop();
    Cpl::System::Api::sleep( 300 );
    Cpl::System::Thread::destroy( *t1 );
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}