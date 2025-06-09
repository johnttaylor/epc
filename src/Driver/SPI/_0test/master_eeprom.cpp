/*-----------------------------------------------------------------------------
 * This file is part of the Colony.Core Project.  The Colony.Core Project is an
 * open source project with a BSD type of licensing agreement.  See the license
 * agreement (license.txt) in the top/ directory or on the Internet at
 * http://integerfox.com/colony.core/license.txt
 *
 * Copyright (c) 2014-2025  John T. Taylor
 *
 * Redistributions of the source code must retain the above copyright notice.
 *----------------------------------------------------------------------------*/
/** @file */

#include "master_eeprom.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Api.h"

#define SECT_ "_0test"

#define STOP_TEST() for ( ;; )
using namespace Driver::SPI;

static uint8_t srcBuffer_[10];
static uint8_t dstBuffer_[10];
static uint8_t magicValues_[10];


static bool waitForWriteToComplete( Driver::SPI::Master& uut, Driver::DIO::Out& cs )
{
    bool inProgress = true;
    while ( inProgress )
    {
        // Read the Status register
        srcBuffer_[0] = 5;  // Read Status Reg Command
        memset( dstBuffer_, 0xA5, sizeof( dstBuffer_ ) );
        cs.assertOutput();
        if ( !uut.transfer( 2, srcBuffer_, dstBuffer_ ) )
        {
            break;
        }
        cs.deassertOutput();
        if ( !( dstBuffer_[1] & 0x01 ) )
        {
            inProgress = false;
            return true;
        }
        Cpl::System::Api::sleep( 1 );
    }
    return false;
}

void runtests( Driver::SPI::Master& uut, Driver::DIO::Out& cs )
{
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "SPI (Full Duplex) Driver Test - expecting a M95M04-DR (or compatible) EEPROM peripheral device." ) );

    //
    // Initialize ChipSelect
    //
    bool result = cs.start( false );
    if ( !result )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "FAILED to start the CS driver" ) );
        STOP_TEST();
    }

    //
    // Verify driver not-started is bad thing
    //
    srcBuffer_[0] = 5;  // Read Status Reg Command
    cs.assertOutput();
    result = uut.transfer( 3, srcBuffer_, dstBuffer_ );
    cs.deassertOutput();
    if ( !result )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "EXPECTED: driver not started" ) );
    }
    else
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "ERROR:[%x] unexpected result code" ) );
        STOP_TEST();
    }

    //
    // Start the driver
    //
    bool started = uut.start();
    if ( !started )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "ERROR: uut.start() failed" ) );
        STOP_TEST();
    }

    //
    // Get the current values
    //
    waitForWriteToComplete( uut, cs );
    memset( srcBuffer_, 0, sizeof( srcBuffer_ ) );
    memset( magicValues_, 0xA5, sizeof( magicValues_ ) );
    srcBuffer_[0] = 3;  // Read command
    srcBuffer_[1] = 0;  // Start reading at offset zero (24 bit address)
    srcBuffer_[2] = 0;
    srcBuffer_[3] = 0;
    cs.assertOutput();
    result = uut.transfer( 1 + 3 + 3, srcBuffer_, magicValues_ );
    cs.deassertOutput();
    if ( result )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "EXPECTED: Read 3 bytes to offset 0. vals=%02x %02x %02x", magicValues_[4], magicValues_[5], magicValues_[6] ) );
    }
    else
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "ERROR: Failed to read 3 bytes at offset 0" ) );
        STOP_TEST();
    }
    magicValues_[4] += 1;
    magicValues_[5] += 2;
    magicValues_[6] += 3;
    CPL_SYSTEM_TRACE_MSG( SECT_,
                          ( "INFO: Preparation to write 3 bytes to offset 0 (vals=%02x %02x %02x)",
                            magicValues_[4],
                            magicValues_[5],
                            magicValues_[6] ) );

    //
    // Enable writing
    //
    srcBuffer_[0] = 6;
    cs.assertOutput();
    result = uut.transfer( 1, srcBuffer_ );
    cs.deassertOutput();
    if ( result )
    {
        // CPL_SYSTEM_TRACE_MSG( SECT_, ( "EXPECTED: Write Enable" ) );
    }
    else
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "ERROR: Write Enable failed" ) );
        STOP_TEST();
    }

    //
    // Write 3 (new) bytes to the eeprom
    //
    srcBuffer_[0] = 2;  // Write command
    srcBuffer_[1] = 0;  // Start writing at offset zero (24 bit address)
    srcBuffer_[2] = 0;
    srcBuffer_[3] = 0;
    srcBuffer_[4] = magicValues_[4];
    srcBuffer_[5] = magicValues_[5];
    srcBuffer_[6] = magicValues_[6];
    cs.assertOutput();
    result = uut.transfer( 1 + 3 + 3, srcBuffer_ );
    cs.deassertOutput();
    if ( !result )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_,
                              ( "ERROR: Failed to write 3 bytes to offset 0 (vals=%02x %02x %02x)",
                                srcBuffer_[4],
                                srcBuffer_[5],
                                srcBuffer_[6] ) );
        STOP_TEST();
    }

    //
    // Wait for the write operation to complete
    //
    waitForWriteToComplete( uut, cs );

    //
    // Read back the bytes
    //
    memset( srcBuffer_, 0, sizeof( srcBuffer_ ) );
    memset( dstBuffer_, 0, sizeof( dstBuffer_ ) );
    srcBuffer_[0] = 3;  // Read command
    srcBuffer_[1] = 0;  // Start reading at offset zero (24 bit address)
    srcBuffer_[2] = 0;
    srcBuffer_[3] = 0;
    cs.assertOutput();
    result = uut.transfer( 1 + 3 + 3, srcBuffer_, dstBuffer_ );
    cs.deassertOutput();
    if ( result && dstBuffer_[4] == magicValues_[4] && dstBuffer_[5] == magicValues_[5] && dstBuffer_[6] == magicValues_[6] )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "EXPECTED: Read 3 bytes to offset 0 (vals=%02x %02x %02x)", dstBuffer_[4], dstBuffer_[5], dstBuffer_[6] ) );
    }
    else
    {
        CPL_SYSTEM_TRACE_MSG( SECT_,
                              ( "ERROR: Failed to read 3 bytes at offset 0 (act=%02x %02x %02x | ex=%02x %02x %02x)",
                                dstBuffer_[4],
                                dstBuffer_[5],
                                dstBuffer_[6],
                                magicValues_[4],
                                magicValues_[5],
                                magicValues_[6] ) );
        STOP_TEST();
    }

    //
    // Do not return once the test has completed
    //
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "ALL TESTS COMPLETED SUCCESSFULLY" ) );
    STOP_TEST();
}
