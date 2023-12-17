#ifndef Ajax_ScreenMgr_NullScreen_h_
#define Ajax_ScreenMgr_NullScreen_h_
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

#include "Ajax/ScreenMgr/ScreenApi.h"
#include "Ajax/ScreenMgr/StaticScreenApi.h"

/// 
namespace Ajax {
/// 
namespace ScreenMgr {


/** This class implements a 'Mock' ScreenApi instances.  The intended usage
    is for unit tests
 */
class MockScreen : public ScreenApi
{
public:
    /// Constructor
    MockScreen( const char* name )
        : m_name( name )
        , m_enterCount( 0 )
        , m_exitCount( 0 )
        , m_sleepCount( 0 )
        , m_wakeCount( 0 )
        , m_dispatchCount( 0 )
        , m_tickCount( 0 )
        , m_refreshCount( 0 )
        , m_lastTimeStamp( { 0,0 } )
        , m_refreshResult( true )
        , m_tickResult( false )
    {
    }

    /// Screen name
    const char* m_name;
    /// Call counter
    unsigned m_enterCount;
    /// Call counter
    unsigned m_exitCount;
    /// Call counter
    unsigned m_sleepCount;
    /// Call counter
    unsigned m_wakeCount;
    /// Call counter
    unsigned m_dispatchCount;
    /// Call counter
    unsigned m_tickCount;
    /// Call counter
    unsigned m_refreshCount;
    /// Last Event
    AjaxScreenMgrEvent_T m_lastEvent;
    /// Last Timestamp
    Cpl::System::ElapsedTime::Precision_T m_lastTimeStamp;
    /// Return value for refresh
    bool m_refreshResult;
    /// Return value for tick
    bool m_tickResult;

public:
    /// See Ajax::ScreenMgr::ScreenApi
    void enter( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
    {
        m_enterCount++;
        m_lastTimeStamp = currentElapsedTime;
    }

    /// See Ajax::ScreenMgr::ScreenApi
    void exit( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
    {
        m_exitCount++;
        m_lastTimeStamp = currentElapsedTime;
    }

    /// See Ajax::ScreenMgr::ScreenApi
    void sleep( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
    {
        m_sleepCount++;
        m_lastTimeStamp = currentElapsedTime;
    }

    /// See Ajax::ScreenMgr::ScreenApi
    void wake( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
    {
        m_wakeCount++;
        m_lastTimeStamp = currentElapsedTime;
    }

    /// See Ajax::ScreenMgr::ScreenApi
    void dispatch( AjaxScreenMgrEvent_T event, Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
    {
        m_dispatchCount++;
        m_lastEvent     = event;
        m_lastTimeStamp = currentElapsedTime;
    }

    /// See Ajax::ScreenMgr::ScreenApi
    bool tick( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
    {
        m_tickCount++;
        m_lastTimeStamp = currentElapsedTime;
        return m_tickResult;
    }

    /// See Ajax::ScreenMgr::ScreenApi
    bool refresh( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
    {
        m_refreshCount++;
        m_lastTimeStamp = currentElapsedTime;
        return m_refreshResult;
    }
};

/** This class implements a 'Mock' StaticScreenApi instances.  The intended usage
    is for unit tests
 */
class MockStaticScreen : public StaticScreenApi
{
public:
    /// Constructor
    MockStaticScreen( const char* name )
        : m_name( name )
        , m_paintCount( 0 )
        , m_lastTimeStamp( { 0,0 } )
    {
    }

    /// Screen name
    const char* m_name;
    /// Call counter
    unsigned m_paintCount;
    /// Last Timestamp
    Cpl::System::ElapsedTime::Precision_T m_lastTimeStamp;

public:
    /// See Ajax::ScreenMgr::ScreenApi
    void paint( Cpl::System::ElapsedTime::Precision_T currentElapsedTime ) noexcept
    {
        m_paintCount++;
        m_lastTimeStamp = currentElapsedTime;
    }
};

}       // end namespaces
}
#endif  // end header latch

