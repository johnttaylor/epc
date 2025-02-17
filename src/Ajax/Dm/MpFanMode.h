#ifndef Storm_Dm_MpFanMode_h_
#define Storm_Dm_MpFanMode_h_
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


#include "Ajax/Type/FanMode.h"
#include "Cpl/Dm/Mp/Enum_.h"

///
namespace Ajax {
///
namespace Dm {

/** Concrete Model Point the FanMode Enum
 */
class MpFanMode : public Cpl::Dm::Mp::Enum_<Ajax::Type::FanMode, MpFanMode>
{
public:
    /// Constructor. 
    MpFanMode( Cpl::Dm::ModelDatabase& myModelBase, const char* symbolicName )
        :Cpl::Dm::Mp::Enum_<Ajax::Type::FanMode, MpFanMode>( myModelBase, symbolicName )
    {
    }

    /// Constructor. 
    MpFanMode( Cpl::Dm::ModelDatabase& myModelBase, const char* symbolicName, Ajax::Type::FanMode initialValue )
        : Cpl::Dm::Mp::Enum_<Ajax::Type::FanMode, MpFanMode>( myModelBase, symbolicName, initialValue )
    {
    }

    /// Type
    const char* getTypeAsText() const noexcept
    {
        return "Ajax::Dm::MpFanMode";
    }

public:
    /// Type safe subscriber
    typedef Cpl::Dm::Subscriber<MpFanMode> Observer;
};


};      // end namespaces
};
#endif  // end header latch
