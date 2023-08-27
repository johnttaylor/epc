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



/// Starting offset for Application Records
#define AJAX_MAIN_APPLICATION_RECORDS_START_OFFSET      0

/// Metadata length for a Mirrored Chunck
#define AJAX_MAIN_MIRRORED_CHUNK_META_LEN               16

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
#define AJAX_MAIN_PERSONALITY_REGION_DATA_LEN           64
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



#endif  // end header latch
