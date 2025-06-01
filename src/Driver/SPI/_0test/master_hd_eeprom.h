#ifndef Driver_SPI_Master_0test_master_hd_eeprom_h_
#define Driver_SPI_Master_0test_master_hd_eeprom_h_
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

/** @file 

    This test assumes an SPI Based EEPROM device using a HALF-DUPLEX SPI interface.
*/

#include "Driver/SPI/MasterHalfDuplex.h"
#include "Driver/DIO/Out.h"

/// This method NEVER returns
void runtests( Driver::SPI::MasterHalfDuplex& uut, Driver::DIO::Out& cs );


/*--------------------------------------------------------------------------*/
#endif  // end header latch
