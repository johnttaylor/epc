#ifndef Ajax_Main_PersonalityRecord_h_
#define Ajax_Main_PersonalityRecord_h_
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
#include "Cpl/System/Trace.h"

/// Major Schema index for my record
#ifndef OPTION_AJAX_MAIN_PERSONALITY_RECORD_MAJOR
#define OPTION_AJAX_MAIN_PERSONALITY_RECORD_MAJOR      0
#endif

/// Minor Schema index for my record
#ifndef OPTION_AJAX_MAIN_PERSONALITY_RECORD_MINOR
#define OPTION_AJAX_MAIN_PERSONALITY_RECORD_MINOR      0
#endif


///
namespace Ajax {
///
namespace Main {


/** This concrete class implements the "Record" class for storing the 'Personality' settings.
 */
class PersonalityRecord : public Cpl::Dm::Persistent::Record
{
public:
    /// Constructor
    PersonalityRecord( Cpl::Persistent::Chunk& chunkHandler )
        : Cpl::Dm::Persistent::Record( m_modelPoints, chunkHandler, OPTION_AJAX_MAIN_PERSONALITY_RECORD_MAJOR, OPTION_AJAX_MAIN_PERSONALITY_RECORD_MINOR )
    {
        m_modelPoints[0] ={ &mp::modelNumber, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[1] ={ &mp::serialNumber, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[2] ={ &mp::consolePwdHash, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[3] ={ &mp::consolePwdSalt, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[4] ={ &mp::flcConfig, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[5] ={ &mp::fanLowPercentage, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[6] ={ &mp::fanMedPercentage, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[7] ={ &mp::fanHighPercentage, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[8] ={ &mp::maxHeatingCapacity, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };

        // TODO: Add Algorithm config MPs here...
        m_modelPoints[9] ={ 0,0 };
    }

protected:
    /// List of Model Points for the Record
    Cpl::Dm::Persistent::Record::Item_T m_modelPoints[9 + 1];

public:
    /// See Cpl::Dm::Persistent::Record
    bool resetData() noexcept
    {
        // There are NO defaults -->throw an error and trigger the Error UI screen
        mp::notProvisionedAlert.raiseAlert();

        // Do NOT update/reset flash, i.e. trigger the error again on the next reboot until I get provisioned
        return false;
    }

};

}       // end namespace(s)
}

#endif  // end header latch
