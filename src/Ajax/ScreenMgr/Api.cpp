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

#include "Api.h"

using namespace Ajax::ScreenMgr;



/////////////////////////////////////////////////////////////////////////////
Api::Api( Cpl::Dm::MailboxServer& uiMbox,
          MpScreenApiPtr&         homeScreenMP,
          MpStaticScreenApiPtr&   haltUiMP,
          Cpl::Dm::Mp::Bool&      displaySleepRequestMP )
    : Cpl::Itc::CloseSync( uiMbox )
    , m_mpHomeScreen( homeScreenMP )
    , m_mpHaltUi( haltUiMP )
    , m_mpDisplaySleepRequest( displaySleepRequestMP )
    , m_splashScreenHdl( nullptr )
    , m_haltScreenHdl( nullptr )
    , m_homeScreenHdl( nullptr )
    , m_opened( false )
{
}

void Api::request( OpenMsg& msg )
{
}

void Api::request( CloseMsg& msg )
{
}
