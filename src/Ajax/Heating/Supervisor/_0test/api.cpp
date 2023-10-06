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
#include "Ajax/Heating/Flc/Api.h"    
#include "Ajax/Heating/Supervisor/Api.h"    
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/SimTick.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "mp/ModelPoints.h"

using namespace Ajax::Heating::Supervisor;

#define SECT_   "_0test"

////////////////////////////////////////////////////////////////////////////////
// MOCK FLC
Ajax::Heating::Flc::Api::Api( Ajax::Dm::MpFlcConfig& mpCfg )
    :m_mpCfg( mpCfg )
{
}

static bool flcStartResult_ = true;
static int  flcStartCount_;
bool Ajax::Heating::Flc::Api::start() noexcept
{
    flcStartCount_++;
    return flcStartResult_;
}

static int32_t flcCalcChangeResult_;
static int     flcCalcChangeCount_;
int32_t Ajax::Heating::Flc::Api::calcChange( int32_t currentTemp, int32_t setpoint ) noexcept
{
    flcCalcChangeCount_++;
    return flcCalcChangeResult_;
}

static int  flcStopCount_;
void Ajax::Heating::Flc::Api::stop() noexcept
{
    flcStopCount_++;
}

void Ajax::Heating::Flc::Api::fuzzify( int32_t inValue, int32_t fuzzyOut[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ) noexcept
{
}

void Ajax::Heating::Flc::Api::runInference( const int32_t m1Vector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS],
                                            const int32_t m2Vector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS],
                                            int32_t       outVector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ) noexcept
{
}

int32_t Ajax::Heating::Flc::Api::defuzz( const int32_t outVector[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS] ) noexcept
{
    return 0;
}

static void resetState( int32_t calcResult )
{
    flcStartCount_       = 0;
    flcStopCount_        = 0;
    flcCalcChangeCount_  = 0;
    flcCalcChangeResult_ = calcResult;
    flcStartResult_      = true;

    mp::heatingMode.setInvalid();
    mp::hwSafetyLimit.setInvalid();
    mp::onBoardIdt.setInvalid();
    mp::remoteIdt.setInvalid();
    mp::heatSetpoint.setInvalid();
    mp::fanMode.setInvalid();
    mp::cmdHeaterPWM.setInvalid();
    mp::cmdFanPWM.setInvalid();
    mp::fanHighPercentage.setInvalid();
    mp::fanLowPercentage.setInvalid();
    mp::fanMedPercentage.setInvalid();
}


////////////////////////////////////////////////////////////////////////////////
// NOTE: Simulated time and CATCH2 do NOT play well together, i.e simulated
//       time only works with a single TEST_CASE with zero or one SECTIONS.

TEST_CASE( "api" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Cpl::Dm::MailboxServer mbox;
    Api                    uut( mbox, 100, 100 );
    AsyncOpenClose         openerCloser( uut );
    uint32_t               heaterPWM;
    uint32_t               fanPWM;

    Cpl::System::Thread* t1 = Cpl::System::Thread::create( mbox, "T1" );
    REQUIRE( t1 );
    Cpl::System::Thread* t2 = Cpl::System::Thread::create( openerCloser, "T2" );
    REQUIRE( t2 );

    //
    // Happy path
    //
    resetState( 10 );
    mp::onBoardIdt.write( 7500 );
    mp::heatSetpoint.write( 7600 );
    mp::fanMode.write( Ajax::Type::FanMode::eMEDIUM );
    mp::fanMedPercentage.write( 666 ); // 66.6%

    openerCloser.openSubject();
    simAdvanceTillOpened( openerCloser );
    REQUIRE( uut.isInOff() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 0 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    mp::heatingMode.write( true );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS * 2);
    REQUIRE( uut.isInHeating() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 2 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 20 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 66 );

    mp::heatingMode.write( false );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS );
    REQUIRE( uut.isInOff() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 3 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    openerCloser.closeSubject();
    simAdvanceTillClosed( openerCloser );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 1 );
    REQUIRE( flcCalcChangeCount_ == 3 );

    //
    // Bad sensor path
    //
    resetState( 10 );
    //mp::onBoardIdt.write( 7500 );
    mp::heatSetpoint.write( 7600 );
    mp::fanMode.write( Ajax::Type::FanMode::eLOW );
    mp::fanLowPercentage.write( 333 ); // 33.3%

    openerCloser.openSubject();
    simAdvanceTillOpened( openerCloser );
    REQUIRE( uut.isInOff() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 0 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    mp::heatingMode.write( true );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS  );
    REQUIRE( uut.isInWaitingForSensor() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 0 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS );
    REQUIRE( uut.isInWaitingForSensor() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 0 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    mp::onBoardIdt.write( 7500 );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS*2 );
    REQUIRE( uut.isInHeating() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 1 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 10 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 33 );

    mp::onBoardIdt.setInvalid();
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS );
    REQUIRE( uut.isInWaitingForSensor() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 1 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    openerCloser.closeSubject();
    simAdvanceTillClosed( openerCloser );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 1 );
    REQUIRE( flcCalcChangeCount_ == 1 );

    //
    // Safety limit paths
    //
    resetState( 10 );
    mp::onBoardIdt.write( 7500 );
    mp::heatSetpoint.write( 7600 );
    mp::fanMode.write( Ajax::Type::FanMode::eHIGH );
    mp::fanHighPercentage.write( 1000 ); // 100%
    mp::hwSafetyLimit.write( true );

    openerCloser.openSubject();
    simAdvanceTillOpened( openerCloser );
    REQUIRE( uut.isInFailedSafeOff() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 0 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    mp::hwSafetyLimit.write( false );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS  );
    REQUIRE( uut.isInOff() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 0 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    mp::heatingMode.write( true );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS * 2 );
    REQUIRE( uut.isInHeating() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 2 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 20 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 100 );

    mp::hwSafetyLimit.write( true );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS );
    REQUIRE( uut.isInFailedSafeOn() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 3 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    mp::heatingMode.write( false );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS );
    REQUIRE( uut.isInFailedSafeOff() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 3 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    mp::heatingMode.write( true );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS );
    REQUIRE( uut.isInFailedSafeOn() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 3 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 0 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 0 );

    mp::hwSafetyLimit.write( false );
    Cpl::System::SimTick::advance( OPTION_AJAX_HEATING_SUPERVISOR_ALGO_INTERVAL_MS );
    REQUIRE( uut.isInHeating() );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 0 );
    REQUIRE( flcCalcChangeCount_ == 4 );
    REQUIRE( mp::cmdHeaterPWM.read( heaterPWM ) );
    REQUIRE( heaterPWM == 10 );
    REQUIRE( mp::cmdFanPWM.read( fanPWM ) );
    REQUIRE( fanPWM == 100 );

    openerCloser.closeSubject( true );  // Exit the Async helper to that its thread self terminates
    simAdvanceTillClosed( openerCloser );
    REQUIRE( flcStartCount_ == 1 );
    REQUIRE( flcStopCount_ == 1 );
    REQUIRE( flcCalcChangeCount_ == 4 );


    // 
    // Shutdown the test threads
    //
    
    // Attempt to politely self-terminated the test thread
    mbox.pleaseStop();
    for ( int i=0; i < 5; i++ )
    {
        Cpl::System::SimTick::advance( 2 );
        Cpl::System::Api::sleep( 100 );
    }
    
    // Delete the thread object (and brute force kill the threads if they are still running)
    Cpl::System::Thread::destroy( *t1 );
    Cpl::System::Thread::destroy( *t2 );
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}
