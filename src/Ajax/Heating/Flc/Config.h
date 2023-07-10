#ifndef Ajax_Heating_Flc_Config_h_
#define Ajax_Heating_Flc_Config_h_
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

#include <stdint.h>
#include <string.h>


/// Number of member sets supported
#define AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS 5

/// 
namespace Ajax {
/// 
namespace Heating {
/// 
namespace Flc {


 
/** Struct defines the geometry of the input membership function
 */
struct Config_T
{
    int32_t outputScaler;       //!< Scaler value (applied to numerator) used when defuzzifying the output
    int32_t maxY;               //!< Maximum Y axis value
    int32_t errScaler;          //!< Scaler applied to the calculate error value (before fuzzifying)
    int32_t dErrScaler;         //!< Scaler applied to the calculate dError value (before fuzzifying).  Recommend that is value is larger than errScaler 
    int32_t outK[AJAX_HEATING_FLC_CONFIG_NUM_MEMBER_SETS]; //!< Set Strengths for the output membership function


    /// Constructor (to ensure any pad bytes get zero'd -->needed for brute force compare operation)
    Config_T()
    {
        memset( (void*) this, 0, sizeof( Config_T ) );
    }

    /// Comparison operator (for DM support)
    bool operator == ( Config_T const other ) const
    {
        return memcmp( this, &other, sizeof( Config_T ) ) == 0;
    }
};

};      // end namespaces
};
};
#endif  // end header latch

