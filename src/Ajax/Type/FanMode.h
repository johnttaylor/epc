#ifndef Ajax_Type_FanMode_h_
#define Ajax_Type_FanMode_h_
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

#include "Cpl/Type/enum.h"


///
namespace Ajax {
///
namespace Type {

/** This enumeration defines the supported Fan modes

    @param FanMode                          Enum

    @param eLOW                             Minimum Fan speed
    @param eMEDIUM                          Medium Fan speed
    @param eHIGH                            Maximum Fan speed
    @param eLAST_MODE                       Marker for end-of-enum-list
 */
BETTER_ENUM( FanMode, unsigned
             , eLOW = 0
             , eMEDIUM
             , eHIGH
             , eLAST_MODE  
);

};      // end namespaces
};
#endif  // end header latch

