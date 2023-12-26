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

#include "Summary.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Assert.h"
#include "Ajax/Logging/Api.h"
#include "mp/ModelPoints.h"

#define SECT_ "Ajax::Alerts::Summary"

using namespace Ajax::Alerts;

static void shiftDown( Ajax::Dm::MpAlert::Data array[], unsigned elemIdxToShift, unsigned numElems ) noexcept;



///////////////////////////
Summary::Summary( Cpl::Dm::MailboxServer& myMbox,
                  Ajax::Dm::MpAlert*      alertList[])
    : Cpl::Itc::CloseSync( myMbox )
    , m_alertList( alertList )
    , m_eventLoop( myMbox )
    , m_opened( false )
{
}

void Summary::request( OpenMsg& msg )
{
    if ( m_opened )
    {
        Ajax::Logging::logf( Ajax::Logging::WarningMsg::OPEN_CLOSE, "open: " SECT_ );
        msg.returnToSender();
        return;
    }

    // Housekeeping
    m_opened = true;

    // Construct (and attach) Alert Observers
    for ( unsigned i=0; i < Ajax::Type::Alert::NUM_ALERTS; i++ )
    {
        void*          memPtr = &(m_observers[i]);
        AlertObserver* obPtr  = new(memPtr) AlertObserver( m_eventLoop, *this, &Summary::alertChanged );
        m_alertList[i]->attach( *obPtr );
    }

    msg.returnToSender();
}

/// Shutdowns the component
void Summary::request( CloseMsg& msg )
{
    if ( !m_opened )
    {
        Ajax::Logging::logf( Ajax::Logging::WarningMsg::OPEN_CLOSE, "close: " SECT_ );
        msg.returnToSender();
        return;
    }

    // Housekeeping
    m_opened = false;

    // Detach (and destroy) Alert Observers
    for ( unsigned i=0; i < Ajax::Type::Alert::NUM_ALERTS; i++ )
    {
        AlertObserver* obPtr = (AlertObserver *) (&(m_observers[i]));
        m_alertList[i]->detach( *obPtr );
        obPtr->~SubscriberComposer();
    }

    msg.returnToSender();
}

///////////////////////////
void Summary::alertChanged( Ajax::Dm::MpAlert& mp, Cpl::Dm::SubscriberApi& clientObserver ) noexcept
{
    // Note: Since the all of alerts are 'polled' when building the Alert summary
    //       it is a don't-care if we get extra change notifications, i.e. justification
    //       for NOT using the xxxAndSync() method in the callback.
    
    // Allocate a temporary array of active alert info. Note: It is ASSUMED that 
    // the total number of alert is 'small enough' to allocate this array on 
    // the stack
    Ajax::Dm::MpAlert::Data activeAlertsInfo[Ajax::Type::Alert::NUM_ALERTS];

    // Loop through all possible alerts
    Ajax::Dm::MpAlertSummary::Data summaryInfo;
    for ( unsigned i=0; i < Ajax::Type::Alert::NUM_ALERTS; i++ )
    {
        Ajax::Dm::MpAlert::Data alertInfo;
        if ( m_alertList[i]->read( alertInfo ) )    // Note: Alert is active it is in the VALID state
        {
            // Insert into the temporary list by priority (0=highest priority)
            bool inserted = false;
            for ( unsigned j=0; j < summaryInfo.count; j++ )
            {
                if ( alertInfo.priority < activeAlertsInfo[j].priority )
                {
                    shiftDown( activeAlertsInfo, j, Ajax::Type::Alert::NUM_ALERTS );
                    activeAlertsInfo[j] = alertInfo;
                    inserted            = true;
                    break;
                }
            }

            // Insert at the tail (i.e. the lowest priority)
            if ( !inserted )
            {
                activeAlertsInfo[summaryInfo.count] = alertInfo;
            }
            summaryInfo.count++;
        }
    }

    // Copy the sorted list of active alerts into the Alert Summary
    for ( unsigned i=0; i < summaryInfo.count; i++ )
    {
        summaryInfo.activeAlerts[i] = activeAlertsInfo[i].name;
    }
    mp::alertSummary.write( summaryInfo );
}

void shiftDown( Ajax::Dm::MpAlert::Data array[], unsigned elemIdxToShift, unsigned numElems ) noexcept
{
    // Start at bottom
    for ( unsigned i=numElems - 1; i > elemIdxToShift && i > 0; i-- )
    {
        array[i] = array[i - 1];
    }
}