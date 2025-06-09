/*-----------------------------------------------------------------------------
* COPYRIGHT_HEADER_TO_BE_FILLED_LATER
*----------------------------------------------------------------------------*/
/** @file */

#include "MApp_.h"

using namespace Cpl::MApp;

MApp_::MApp_( Cpl::Container::SList<MAppApi>& mappList,
              const char*                     mappName,
              const char*                     description,
              const char*                     usage )
    : m_name( mappName )
    , m_description( description )
    , m_usage( usage )
    , m_started( false )
{
    mappList.put( *this );
}

const char* MApp_::getName() const noexcept
{
    return m_name;
}

const char* MApp_::getDescription() const noexcept
{
    return m_description;
}

const char* MApp_::getUsage() const noexcept
{
    return m_usage;
}
