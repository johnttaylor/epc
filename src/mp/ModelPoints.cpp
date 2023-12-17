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
/** @file */

#include "ModelPoints.h"


// Helper macros to simplify the instantiation of the MP instances
#define ALLOC_INVALID( t, n )               t mp::n(  mp::g_modelDatabase, (#n) )
#define ALLOC_INVALID_ALERT( t, n, pri )    t mp::n(  mp::g_modelDatabase, (#n), (uint8_t) pri )

/// Static allocate the MP database
Cpl::Dm::ModelDatabase   mp::g_modelDatabase( "ignoreThisParameter_usedToCreateAUniqueConstructor" );



/*---------------------------------------------------------------------------*/

ALLOC_INVALID( Ajax::ScreenMgr::MpStaticScreenApiPtr, shutdownScrPtr );
ALLOC_INVALID( Ajax::ScreenMgr::MpStaticScreenApiPtr, errorScrPtr );
ALLOC_INVALID( Ajax::ScreenMgr::MpScreenApiPtr, homeScrPtr );
ALLOC_INVALID( Cpl::Dm::Mp::Bool, displaySleepTrigger );
ALLOC_INVALID( Cpl::Dm::Mp::Uint32, uiEventQueueCount );

ALLOC_INVALID( Cpl::Dm::Mp::String<OPTION_AJAX_MAX_VERSION_LENGTH>, fwVersion );
ALLOC_INVALID( Cpl::Dm::Mp::String<OPTION_AJAX_MAX_MODEL_LENGTH>, modelNumber );
ALLOC_INVALID( Cpl::Dm::Mp::String<OPTION_AJAX_MAX_SERIAL_NUM_LENGTH>, serialNumber );

ALLOC_INVALID( Ajax::Dm::MpFanMode, fanMode );
ALLOC_INVALID( Cpl::Dm::Mp::Int32, heatSetpoint );
ALLOC_INVALID( Cpl::Dm::Mp::Bool, heatingMode );

ALLOC_INVALID( Cpl::Dm::Mp::Uint32, metricBootCounter );
ALLOC_INVALID( Cpl::Dm::Mp::Uint64, metricRunningTime );
ALLOC_INVALID( Cpl::Dm::Mp::Uint64, metricHeaterOnTime );
ALLOC_INVALID( Cpl::Dm::Mp::Uint64, metricFanOnTime );
ALLOC_INVALID( Cpl::Dm::Mp::Uint32, metricFaultTempSensor );
ALLOC_INVALID( Cpl::Dm::Mp::Uint32, metricFaultHeaterSafety );

ALLOC_INVALID( Cpl::Dm::Mp::Uint32, loggingQueCount );
ALLOC_INVALID( Cpl::Dm::Mp::Uint64, latestLoggingEntryKey );

ALLOC_INVALID( Cpl::Dm::Mp::ArrayUint8<OPTION_AJAX_HASHED_PASSWORD_SIZE>, consolePwdHash );
ALLOC_INVALID( Cpl::Dm::Mp::ArrayUint8<OPTION_AJAX_HASHED_PASSWORD_SALT_SIZE>, consolePwdSalt );

ALLOC_INVALID( Cpl::Dm::Mp::Int32, onBoardIdt );
ALLOC_INVALID( Cpl::Dm::Mp::Int32, remoteIdt );
ALLOC_INVALID( Cpl::Dm::Mp::Uint32, cmdHeaterPWM );
ALLOC_INVALID( Cpl::Dm::Mp::Uint32, cmdFanPWM );
ALLOC_INVALID_ALERT( Ajax::Dm::MpAlert, sensorFailAlert, AJAX_ALERT_PRIORITY_SENSOR_FAILED );
ALLOC_INVALID_ALERT( Ajax::Dm::MpAlert, failedSafeAlert, AJAX_ALERT_PRIORITY_HI_TEMP );
ALLOC_INVALID( Ajax::Dm::MpFlcConfig, flcConfig );
ALLOC_INVALID( Cpl::Dm::Mp::Uint32, maxHeatingCapacity );
ALLOC_INVALID( Cpl::Dm::Mp::Uint32, fanLowPercentage );
ALLOC_INVALID( Cpl::Dm::Mp::Uint32, fanMedPercentage );
ALLOC_INVALID( Cpl::Dm::Mp::Uint32, fanHighPercentage );
ALLOC_INVALID( Cpl::Dm::Mp::Bool, hwSafetyLimit );
