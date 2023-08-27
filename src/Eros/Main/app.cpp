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

#include "Ajax/Main/appmain.h"      // FIXME: Must be first (because of the pimoroni graphic library stuffs)
#include "Ajax/Main/application.h"
#include "Eros/Main/screens.h"
#include "mp/ModelPoints.h"
#include <stdio.h>

using namespace Ajax::Main;

Eros::Ui::Home::Screen Eros::Main::g_homeScreen( Ajax::Main::g_screenNav, g_graphics );

/////////////////////////////
void Ajax::Main::appvariant_initialize0()
{
    printf("\nEROS: appvariant_initialize0()\n" );
}

void Ajax::Main::appvariant_initializeModelPoints0()
{
    // Populate 'static' Model points
    Cpl::Text::FString<OPTION_AJAX_MAX_VERSION_LENGTH> tmpVer;
    tmpVer.format( "%s-%07lu", EROS_SEMANTIC_VERSION_STR, BUILD_NUMBER );
    mp::fwVersion.write( tmpVer );
}

void Ajax::Main::appvariant_open0()
{
    // Nothing currently needed
}

void Ajax::Main::appvariant_launchHomeScreen()
{
    mp::homeScrPtr.write( &Eros::Main::g_homeScreen );
}

void Ajax::Main::appvariant_close0()
{
    // Nothing currently needed
}