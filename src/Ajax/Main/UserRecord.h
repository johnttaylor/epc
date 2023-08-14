#ifndef Ajax_Main_UserRecord_h_
#define Ajax_Main_UserRecord_h_
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
#include "Cpl/Dm/Persistent/Record.h"
#include "mp/ModelPoints.h"


/// Major Schema index for my record
#ifndef OPTION_AJAX_MAIN_USER_RECORD_MAJOR
#define OPTION_AJAX_MAIN_USER_RECORD_MAJOR      0
#endif

/// Minor Schema index for my record
#ifndef OPTION_AJAX_MAIN_USER_RECORD_MINOR
#define OPTION_AJAX_MAIN_USER_RECORD_MINOR      0
#endif

/// Default value for the Heating setpoint (in hundredths of degrees Fahrenheit)
#ifndef OPTION_AJAX_DEFAULT_HEATING_SETPOINT_F
#define OPTION_AJAX_DEFAULT_HEATING_SETPOINT_F      (70*100)
#endif

/// Default heating mode
#ifndef OPTION_AJAX_DEFAULT_HEATING_MODE
#define OPTION_AJAX_DEFAULT_HEATING_MODE            false
#endif

/// Default fan mode
#ifndef OPTION_AJAX_DEFAULT_FAN_MODE
#define OPTION_AJAX_DEFAULT_FAN_MODE                Ajax::Type::FanMode::eMEDIUM
#endif


///
namespace Ajax {
///
namespace Main {


/** This concrete class implements the "Record" class for storing user settings.
 */
class UserRecord : public Cpl::Dm::Persistent::Record
{
public:
    /// Constructor
    UserRecord( Cpl::Persistent::Chunk& chunkHandler )
        : Cpl::Dm::Persistent::Record( m_modelPoints, chunkHandler, OPTION_AJAX_MAIN_USER_RECORD_MAJOR, OPTION_AJAX_MAIN_USER_RECORD_MINOR )
    {
        m_modelPoints[0] ={ &mp::heatSetpoint, CPL_DM_PERISTENCE_RECORD_USE_SUBSCRIBER };
        m_modelPoints[1] ={ &mp::heatingMode, CPL_DM_PERISTENCE_RECORD_USE_SUBSCRIBER };
        m_modelPoints[2] ={ &mp::fanMode, CPL_DM_PERISTENCE_RECORD_USE_SUBSCRIBER };
        m_modelPoints[3] ={ 0,0 };
    }

protected:
    /// List of Model Points for the Record
    Cpl::Dm::Persistent::Record::Item_T m_modelPoints[3 + 1];

public:
    /// See Cpl::Dm::Persistent::Record
    bool resetData() noexcept
    {
        mp::heatSetpoint.write( OPTION_AJAX_DEFAULT_HEATING_SETPOINT_F );
        mp::heatingMode.write( OPTION_AJAX_DEFAULT_HEATING_MODE );
        mp::fanMode.write( OPTION_AJAX_DEFAULT_FAN_MODE );

        return true;
    }

};

}       // end namespace(s)
}

#endif  // end header latch
