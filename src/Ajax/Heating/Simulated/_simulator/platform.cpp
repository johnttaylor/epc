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

#include "colony_config.h"
#include "Driver/NV/File/Cpl/Api.h"
#include <stdlib.h>

extern Driver::NV::Api& g_nvramDriver;


#ifndef DRIVER_NV_FILE_NAME     
#define DRIVER_NV_FILE_NAME         "eeprom.bin"
#endif
#ifndef DRIVER_NV_NUM_PAGES
#define DRIVER_NV_NUM_PAGES         512
#endif
#ifndef DRIVER_NV_BYTES_PER_PAGE
#define DRIVER_NV_BYTES_PER_PAGE    128
#endif

static Driver::NV::File::Cpl::Api nvDriver_( DRIVER_NV_NUM_PAGES, DRIVER_NV_BYTES_PER_PAGE, DRIVER_NV_FILE_NAME );
Driver::NV::Api&                  g_nvramDriver = nvDriver_;

