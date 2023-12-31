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
#include "Ajax/Metrics/Api.h"

using namespace Ajax::Metrics;

#define SECT_   "_0test"




////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "Api" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Cpl::Dm::MailboxServer          uutMbox;
    Cpl::System::Thread*            t1 = Cpl::System::Thread::create( uutMbox, "UUT" );
    Api                             uut( uutMbox );
    Ajax::Dm::MpAlertSummary::Data  summaryInfo;
    bool                            valid;

    mp::failedSafeAlert.setInvalid();
    mp::cmdFanPWM.setInvalid();
    mp::cmdHeaterPWM.setInvalid();
    mp::metricFanOnTime.write(0);
    mp::metricHeaterOnTime.write( 0 );
    mp::metricRunningTime.write( 0 );
    mp::metricFaultHeaterSafety.write( 0 );

    SECTION( "failsafe" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: failsafe") );
        uut.open();
    
        uint16_t seqNum;
        uint32_t count;
        valid = mp::metricFaultHeaterSafety.read( count, &seqNum );
        REQUIRE( valid );
        REQUIRE( count == 0 );

        mp::failedSafeAlert.raiseAlert();
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint32>( mp::metricFaultHeaterSafety,
                                                              seqNum,
                                                              50,
                                                              &seqNum );

        valid = mp::metricFaultHeaterSafety.read( count, &seqNum );
        REQUIRE( valid );
        REQUIRE( count == 1 );

        mp::failedSafeAlert.lowerAlert();
        Cpl::System::Api::sleep( OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 3 );
        valid = mp::metricFaultHeaterSafety.read( count, &seqNum );
        REQUIRE( valid );
        REQUIRE( count == 1 );

        mp::failedSafeAlert.raiseAlert();
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint32>( mp::metricFaultHeaterSafety,
                                                              seqNum,
                                                              50,
                                                              &seqNum );

        valid = mp::metricFaultHeaterSafety.read( count, &seqNum );
        REQUIRE( valid );
        REQUIRE( count == 2 );

        uut.close();
    }


    SECTION( "runtime" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: runtime") );
        uut.open();

        // Wait for at least 2 update intervals (interval is set to 100ms)
        uint16_t seqNum = mp::metricRunningTime.getSequenceNumber();
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricRunningTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricRunningTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );

        uint64_t runtime;
        valid = mp::metricRunningTime.read( runtime, &seqNum );
        REQUIRE( valid );
        REQUIRE( runtime >= OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 2 );

        uut.close();
    }

    SECTION( "fantime" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: fantime") );
        uut.open();

        Cpl::System::Api::sleep( OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 5 );
        mp::cmdFanPWM.write( 60 );

        // Wait for at least 2 update intervals (interval is set to 100ms)
        uint16_t seqNum = mp::metricFanOnTime.getSequenceNumber();
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricFanOnTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricFanOnTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );

        uint64_t runtime;
        valid = mp::metricFanOnTime.read( runtime, &seqNum );
        mp::cmdFanPWM.write( 0 );
        REQUIRE( valid );
        REQUIRE( runtime >= OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 2 );
        REQUIRE( runtime <  OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 4 );

        Cpl::System::Api::sleep( OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 5 );
        mp::cmdFanPWM.write( 60 );
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricFanOnTime, // Wait 3 changes: two intervals and 1 PWM-change-notification
                                                              seqNum,
                                                              50,
                                                              &seqNum );
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricFanOnTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricFanOnTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );

        uint64_t runtime2;
        valid = mp::metricFanOnTime.read( runtime2, &seqNum );
        REQUIRE( valid );
        REQUIRE( runtime2 >= runtime + OPTION_AJAX_METRICS_INTERVAL_TIME_MS );
        REQUIRE( runtime2 <  runtime + (OPTION_AJAX_METRICS_INTERVAL_TIME_MS*4) );

        
        uut.close();
    }

    SECTION( "heatertime" )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("SECTION: heatertime") );
        uut.open();

        Cpl::System::Api::sleep( OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 5 );
        mp::cmdHeaterPWM.write( 60 );

        // Wait for at least 2 update intervals (interval is set to 100ms)
        uint16_t seqNum = mp::metricHeaterOnTime.getSequenceNumber();
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricHeaterOnTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricHeaterOnTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );

        uint64_t runtime;
        valid = mp::metricHeaterOnTime.read( runtime, &seqNum );
        mp::cmdHeaterPWM.write( 0 );
        REQUIRE( valid );
        REQUIRE( runtime >= OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 2 );
        REQUIRE( runtime < OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 4 );

        Cpl::System::Api::sleep( OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 5 );
        mp::cmdHeaterPWM.write( 60 );
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricHeaterOnTime,   // Wait 3 changes: two intervals and 1 PWM-change-notification
                                                              seqNum,
                                                              50,
                                                              &seqNum );
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricHeaterOnTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );
        minWaitOnModelPointSeqNumChange<Cpl::Dm::Mp::Uint64>( mp::metricHeaterOnTime,
                                                              seqNum,
                                                              50,
                                                              &seqNum );

        uint64_t runtime2;
        valid = mp::metricHeaterOnTime.read( runtime2, &seqNum );
        REQUIRE( valid );
        REQUIRE( runtime2 >= runtime + OPTION_AJAX_METRICS_INTERVAL_TIME_MS );
        REQUIRE( runtime2 < runtime + (OPTION_AJAX_METRICS_INTERVAL_TIME_MS * 4) );


        uut.close();
    }

    // Shutdown threads
    uutMbox.pleaseStop();
    Cpl::System::Api::sleep( 300 );
    Cpl::System::Thread::destroy( *t1 );
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}