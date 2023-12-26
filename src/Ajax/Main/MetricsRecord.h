#ifndef Ajax_Main_MetricsRecord_h_
#define Ajax_Main_MetricsRecord_h_
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
#ifndef OPTION_AJAX_MAIN_METRICS_RECORD_MAJOR
#define OPTION_AJAX_MAIN_METRICS_RECORD_MAJOR      0
#endif

/// Minor Schema index for my record
#ifndef OPTION_AJAX_MAIN_METRICS_RECORD_MINOR
#define OPTION_AJAX_MAIN_METRICS_RECORD_MINOR      0
#endif



///
namespace Ajax {
///
namespace Main {


/** This concrete class implements the "Record" class for storing user settings.
 */
class MetricsRecord : public Cpl::Dm::Persistent::Record
{
public:
    /// Constructor
    MetricsRecord( Cpl::Persistent::Chunk& chunkHandler )
        : Cpl::Dm::Persistent::Record( m_modelPoints, chunkHandler, OPTION_AJAX_MAIN_METRICS_RECORD_MAJOR, OPTION_AJAX_MAIN_METRICS_RECORD_MINOR )
    {
        m_modelPoints[0] = { &mp::metricBootCounter, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[1] = { &mp::metricRunningTime, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[2] = { &mp::metricHeaterOnTime, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[3] = { &mp::metricFanOnTime, CPL_DM_PERISTENCE_RECORD_NO_SUBSCRIBER };
        m_modelPoints[4] = { &mp::metricFaultHeaterSafety, CPL_DM_PERISTENCE_RECORD_USE_SUBSCRIBER };
        m_modelPoints[5] = { 0,0 };
    }                      

protected:
    /// List of Model Points for the Record
    Cpl::Dm::Persistent::Record::Item_T m_modelPoints[5 + 1];

public:
    /// See Cpl::Dm::Persistent::Record
    bool resetData() noexcept
    {
        mp::metricBootCounter.write( 1 );
        mp::metricRunningTime.write( 0 );
        mp::metricHeaterOnTime.write( 0 );
        mp::metricFanOnTime.write( 0 );
        mp::metricFaultHeaterSafety.write( 0 );
        return true;
    }

    /// See Cpl::Dm::Persistent::Record
    void hookProcessPostRecordLoaded() noexcept
    {
        mp::metricBootCounter.increment();
    }
};

}       // end namespace(s)
}

#endif  // end header latch
