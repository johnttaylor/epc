#ifndef Storm_Thermostat_Main_UserRecord_h_
#define Storm_Thermostat_Main_UserRecord_h_
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
/** @file 

    This file contains the magic values for how data is laid out in the
    off-board EEPROM
*/

#include "colony_config.h"

/// Starting offset for Application Records
#define AJAX_MAIN_APPLICATION_RECORDS_START_OFFSET      0

/// Metadata length for a Mirrored Chunck
#define AJAX_MAIN_MIRRORED_CHUNK_META_LEN               16

/// Metadata length for a CRC Chunck
#define AJAX_MAIN_CRC_CHUNK_META_LEN                    8

/// Maximum number of log entries
#ifndef OPTION_AJAX_MAIN_MAX_LOG_ENTRIES
#define OPTION_AJAX_MAIN_MAX_LOG_ENTRIES                256
#endif

/// USER Record/Region
#define AJAX_MAIN_USER_REGION_DATA_LEN                  64
/// USER Record/Region
#define AJAX_MAIN_USER_REGION_RESERVED                  64
/// USER Record/Region
#define AJAX_MAIN_USER_REGION_LENGTH                    (AJAX_MAIN_USER_REGION_DATA_LEN+AJAX_MAIN_USER_REGION_RESERVED+AJAX_MAIN_MIRRORED_CHUNK_META_LEN)
/// USER Record/Region
#define AJAX_MAIN_USER_REGION_A_START_ADDRESS           (AJAX_MAIN_APPLICATION_RECORDS_START_OFFSET)
/// USER Record/Region
#define AJAX_MAIN_USER_REGION_B_START_ADDRESS           (AJAX_MAIN_USER_REGION_A_START_ADDRESS+AJAX_MAIN_USER_REGION_LENGTH)
/// USER Record/Region
#define AJAX_MAIN_USER_REGION_NEXT_OFFSET               (AJAX_MAIN_USER_REGION_B_START_ADDRESS+AJAX_MAIN_USER_REGION_LENGTH)


/// PERSONALITY Record/Region
#define AJAX_MAIN_PERSONALITY_REGION_DATA_LEN           193
/// PERSONALITY Record/Region
#define AJAX_MAIN_PERSONALITY_REGION_RESERVED           64
/// PERSONALITY Record/Region
#define AJAX_MAIN_PERSONALITY_REGION_LENGTH             (AJAX_MAIN_PERSONALITY_REGION_RESERVED+AJAX_MAIN_PERSONALITY_REGION_DATA_LEN+AJAX_MAIN_MIRRORED_CHUNK_META_LEN)
/// PERSONALITY Record/Region
#define AJAX_MAIN_PERSONALITY_REGION_A_START_ADDRESS    (AJAX_MAIN_USER_REGION_NEXT_OFFSET)
/// PERSONALITY Record/Region
#define AJAX_MAIN_PERSONALITY_REGION_B_START_ADDRESS    (AJAX_MAIN_PERSONALITY_REGION_A_START_ADDRESS+AJAX_MAIN_PERSONALITY_REGION_LENGTH)
/// PERSONALITY Record/Region
#define AJAX_MAIN_PERSONALITY_REGION_NEXT_OFFSET        (AJAX_MAIN_PERSONALITY_REGION_B_START_ADDRESS+AJAX_MAIN_PERSONALITY_REGION_LENGTH)

/// METRICS Record/Region
#define AJAX_MAIN_METRICS_REGION_DATA_LEN               64
/// METRICS Record/Region
#define AJAX_MAIN_METRICS_REGION_RESERVED               64
/// METRICS Record/Region
#define AJAX_MAIN_METRICS_REGION_LENGTH                 (AJAX_MAIN_METRICS_REGION_RESERVED+AJAX_MAIN_METRICS_REGION_DATA_LEN+AJAX_MAIN_MIRRORED_CHUNK_META_LEN)
/// METRICS Record/Region
#define AJAX_MAIN_METRICS_REGION_A_START_ADDRESS        (AJAX_MAIN_PERSONALITY_REGION_NEXT_OFFSET)
/// METRICS Record/Region
#define AJAX_MAIN_METRICS_REGION_B_START_ADDRESS        (AJAX_MAIN_METRICS_REGION_A_START_ADDRESS+AJAX_MAIN_METRICS_REGION_LENGTH)
/// METRICS Record/Region
#define AJAX_MAIN_METRICS_REGION_NEXT_OFFSET            (AJAX_MAIN_METRICS_REGION_B_START_ADDRESS+AJAX_MAIN_METRICS_REGION_LENGTH)

/// LOGGING - Index Record/Region
#define AJAX_MAIN_LOGINDEX_REGION_DATA_LEN              12
/// METRICS Record/Region
#define AJAX_MAIN_LOGINDEX_REGION_RESERVED              32
/// METRICS Record/Region
#define AJAX_MAIN_LOGINDEX_REGION_LENGTH                (AJAX_MAIN_LOGINDEX_REGION_RESERVED+AJAX_MAIN_LOGINDEX_REGION_DATA_LEN+AJAX_MAIN_MIRRORED_CHUNK_META_LEN)
/// METRICS Record/Region
#define AJAX_MAIN_LOGINDEX_REGION_A_START_ADDRESS       (AJAX_MAIN_METRICS_REGION_NEXT_OFFSET)
/// METRICS Record/Region
#define AJAX_MAIN_LOGINDEX_REGION_B_START_ADDRESS       (AJAX_MAIN_LOGINDEX_REGION_A_START_ADDRESS+AJAX_MAIN_LOGINDEX_REGION_LENGTH)
/// METRICS Record/Region
#define AJAX_MAIN_LOGINDEX_REGION_NEXT_OFFSET           (AJAX_MAIN_LOGINDEX_REGION_B_START_ADDRESS+AJAX_MAIN_LOGINDEX_REGION_LENGTH)


/// LOGGING - Entries Record/Region
#define AJAX_MAIN_LOGENTRIES_REGION_DATA_LEN            (159* OPTION_AJAX_MAIN_MAX_LOG_ENTRIES) // Length include: log entry + metadata + chunk-metadata
/// METRICS Record/Region
#define AJAX_MAIN_LOGENTRIES_REGION_RESERVED            0
/// METRICS Record/Region
#define AJAX_MAIN_LOGENTRIES_REGION_LENGTH              (AJAX_MAIN_LOGENTRIES_REGION_RESERVED+AJAX_MAIN_LOGENTRIES_REGION_DATA_LEN)
/// METRICS Record/Region
#define AJAX_MAIN_LOGENTRIES_REGION_START_ADDRESS       (AJAX_MAIN_LOGINDEX_REGION_NEXT_OFFSET)
/// METRICS Record/Region
#define AJAX_MAIN_LOGENTRIES_REGION_NEXT_OFFSET         (AJAX_MAIN_LOGENTRIES_REGION_START_ADDRESS+AJAX_MAIN_LOGENTRIES_REGION_LENGTH)

#endif  // end header latch
