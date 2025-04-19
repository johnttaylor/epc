/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2022  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "Command.h"


///
using namespace Cpl::TShell::Cmd;


////////////////////////////
Command::Command( Cpl::Container::SList<Cpl::TShell::Command>& commandList,
				  const char*                                verb,
				  Security::Permission_T                     minPermLevel ) noexcept
	: m_verb( verb )
	, m_minPermissionLevel( minPermLevel )
{
	commandList.put( *this );
}


////////////////////////////
const char* Command::getVerb() const noexcept
{
	return m_verb;
}

Cpl::TShell::Security::Permission_T Command::getMinPermissionRequired() const noexcept
{
	return m_minPermissionLevel;
}

