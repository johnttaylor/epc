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
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Thread.h"
#include "Cpl/System/Api.h"
#include "Driver/AIO/Ajax/Thermistor.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Cpl/Dm/Mp/Int32.h"

using namespace Driver::AIO::Ajax;

#define SECT_ "_0test"

////////////////////////////////////////////////////////////////////////////////

// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase    modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );
static Cpl::Dm::Mp::Int32        mp_idt_( modelDb_, "idt" );


///////////////////////////////////////////////////////////////////////////////
/// Mock the HAL method(s)

uint32_t mockAdcRawValue_;
bool     mockAdcSampleResult_ = true;
uint32_t mockSampleInc        = 0;
bool mockedHalSingleInput( int, uint32_t& dstADCBits )
{
    dstADCBits        = mockAdcRawValue_;
    mockAdcRawValue_ += mockSampleInc;

    return mockAdcSampleResult_;
}

uint8_t mockAdcBitsResolution_ = 10;
uint8_t mockAdcLastBitResolutionSet_;
uint8_t mockedHalSingleInput_setADCSize( int, uint8_t numADCBits )
{
    mockAdcLastBitResolutionSet_ = numADCBits;
    return mockAdcBitsResolution_;
}

////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "Thermistor" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Cpl::Dm::MailboxServer mbox;
    Thermistor             uut( mbox, 1, mp_idt_ );
    int32_t                value;
    bool                   valid;

    mockAdcSampleResult_ = true;

    Cpl::System::Thread* t1 = Cpl::System::Thread::create( mbox, "T1" );
    REQUIRE( t1 );

    SECTION( "wrong-adc-bits" )
    {
        mockAdcBitsResolution_ = OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION + 1;
        mockAdcRawValue_       = OPTION_DRIVER_AIO_AJAX_ADC_MIN_RAIL_BITS + 1;
        uut.open();
        REQUIRE( mp_idt_.isNotValid() );
        uut.close();

        mockAdcBitsResolution_ = OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION;
        uut.open();
        REQUIRE( mp_idt_.isNotValid() == false );
        uut.close();
    }

    SECTION( "failed ADC" )
    {
        mockAdcBitsResolution_ = OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION;
        mockAdcRawValue_       = OPTION_DRIVER_AIO_AJAX_ADC_MIN_RAIL_BITS + 1;
        uut.open();
        REQUIRE( mp_idt_.isNotValid() == false );
        uut.close();

        mockAdcSampleResult_ = false;
        uut.open();
        REQUIRE( mp_idt_.isNotValid() );
        uut.close();
    }

    SECTION( "table-look-up" )
    {
        mockAdcBitsResolution_ = OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION;

        mockAdcRawValue_ = 1108;
        uut.open();
        valid = mp_idt_.read( value );
        REQUIRE( valid );
        REQUIRE( value == 9504 );
        uut.close();

        // Not a multiple of 64
        mockAdcRawValue_ = 1108;
        mockSampleInc    = 7;
        uint32_t mockAvg = ((mockAdcRawValue_ * OPTION_DRIVER_AIO_AJAX_NUM_SAMPLES_TO_AVG) + (mockSampleInc * (OPTION_DRIVER_AIO_AJAX_NUM_SAMPLES_TO_AVG - 1))) / OPTION_DRIVER_AIO_AJAX_NUM_SAMPLES_TO_AVG;
        REQUIRE( mockAvg > mockAdcRawValue_ );
        uut.open();
        valid = mp_idt_.read( value );
        REQUIRE( valid );
        REQUIRE( value != 9504 );
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Average ADC bits=%u, temp 'F=%d", mockAvg, value) );

        mockAdcRawValue_ = mockAvg;
        mockSampleInc    = 0;
        int32_t tmpVal;
        uut.open();
        valid = mp_idt_.read( tmpVal );
        REQUIRE( valid );
        REQUIRE( tmpVal == value );
        uut.close();

        // Multiple of 64
        mockAdcRawValue_ = 1152;
        uut.open();
        valid = mp_idt_.read( value );
        REQUIRE( valid );
        REQUIRE( value == 9402 );
        uut.close();
    }

    SECTION( "min-max-values" )
    {
        mockAdcBitsResolution_ = OPTION_DRIVER_AIO_AJAX_ADC_BIT_RESOLUTION;

        mockAdcRawValue_ = OPTION_DRIVER_AIO_AJAX_ADC_MAX_RAIL_BITS;
        uut.open();
        REQUIRE( mp_idt_.isNotValid() == false );
        uut.close();

        mockAdcRawValue_ = OPTION_DRIVER_AIO_AJAX_ADC_MAX_RAIL_BITS + 1;
        uut.open();
        REQUIRE( mp_idt_.isNotValid() );
        uut.close();

        mockAdcRawValue_ = OPTION_DRIVER_AIO_AJAX_ADC_MIN_RAIL_BITS;
        uut.open();
        REQUIRE( mp_idt_.isNotValid() == false );
        uut.close();

        mockAdcRawValue_ = OPTION_DRIVER_AIO_AJAX_ADC_MIN_RAIL_BITS - 1;
        uut.open();
        REQUIRE( mp_idt_.isNotValid() );
        uut.close();
    }

    // Attempt to politely self-terminated the test thread
    mbox.pleaseStop();
    Cpl::System::Api::sleep( 100 );
    Cpl::System::Thread::destroy( *t1 );
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}