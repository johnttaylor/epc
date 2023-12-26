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
    &mp::postFailedAlert,
    &mp::notProvisionedAlert
};

////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "Summary" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Cpl::Dm::MailboxServer          uutMbox;
    Cpl::System::Thread*            t1 = Cpl::System::Thread::create( uutMbox, "UUT" );
    Summary                         uut( uutMbox, alerts_ );
    Ajax::Dm::MpAlertSummary::Data  summaryInfo;
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
        bool valid = mp::alertSummary.read( summaryInfo );
        REQUIRE( valid );
        REQUIRE( summaryInfo.count == 1 );
        REQUIRE( summaryInfo.activeAlerts[0] == +Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE );

        uut.close();
    }

    SECTION( "priority-4" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: priority-4") );

        uut.open();
        mp::postFailedAlert.raiseAlert();
        mp::sensorFailAlert.raiseAlert();
        mp::failedSafeAlert.raiseAlert();
        uint16_t seqNum = mp::alertSummary.getSequenceNumber();
        mp::remoteSensorFailAlert.raiseAlert();
        minWaitOnModelPointSeqNumChange<Ajax::Dm::MpAlertSummary>( mp::alertSummary, 10, seqNum, &seqNum );
        bool valid = mp::alertSummary.read( summaryInfo );
        REQUIRE( valid );
        REQUIRE( summaryInfo.count == 4 );
        REQUIRE( summaryInfo.activeAlerts[0] == +Ajax::Type::Alert::ePOST_FAILURE );
        REQUIRE( summaryInfo.activeAlerts[1] == +Ajax::Type::Alert::eONBOARD_SENSOR_FAILED );
        REQUIRE( summaryInfo.activeAlerts[2] == +Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE );
        REQUIRE( summaryInfo.activeAlerts[3] == +Ajax::Type::Alert::eREMOTE_SENSOR_FAILED );

        uut.close();
    }

    SECTION( "priority-3" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: priority-3") );

        uut.open();
        mp::sensorFailAlert.raiseAlert();
        mp::failedSafeAlert.raiseAlert();
        uint16_t seqNum = mp::alertSummary.getSequenceNumber();
        mp::remoteSensorFailAlert.raiseAlert();
        minWaitOnModelPointSeqNumChange<Ajax::Dm::MpAlertSummary>( mp::alertSummary, 10, seqNum, &seqNum );
        bool valid = mp::alertSummary.read( summaryInfo );
        REQUIRE( valid );
        REQUIRE( summaryInfo.count == 3 );
        REQUIRE( summaryInfo.activeAlerts[0] == +Ajax::Type::Alert::eONBOARD_SENSOR_FAILED );
        REQUIRE( summaryInfo.activeAlerts[1] == +Ajax::Type::Alert::eHITEMP_HEATER_FAILSAFE );
        REQUIRE( summaryInfo.activeAlerts[2] == +Ajax::Type::Alert::eREMOTE_SENSOR_FAILED );

        uut.close();
    }

    SECTION( "priority-2" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: priority-2") );

        uut.open();
        mp::sensorFailAlert.raiseAlert();
        uint16_t seqNum = mp::alertSummary.getSequenceNumber();
        mp::remoteSensorFailAlert.raiseAlert();
        minWaitOnModelPointSeqNumChange<Ajax::Dm::MpAlertSummary>( mp::alertSummary, 10, seqNum, &seqNum );
        bool valid = mp::alertSummary.read( summaryInfo );
        REQUIRE( valid );
        REQUIRE( summaryInfo.count == 2 );
        REQUIRE( summaryInfo.activeAlerts[0] == +Ajax::Type::Alert::eONBOARD_SENSOR_FAILED );
        REQUIRE( summaryInfo.activeAlerts[1] == +Ajax::Type::Alert::eREMOTE_SENSOR_FAILED );

        uut.close();
    }

    // Shutdown threads
    uutMbox.pleaseStop();
    Cpl::System::Api::sleep( 300 );
    Cpl::System::Thread::destroy( *t1 );
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}