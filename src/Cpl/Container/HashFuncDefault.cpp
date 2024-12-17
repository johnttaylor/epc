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

#include "HashFuncDefault.h"


///
using namespace Cpl::Container;

unsigned int Cpl::Container::hashFuncDefault( const void* keystart, unsigned keylen, unsigned int maxBuckets ) noexcept
{
    static const unsigned int bits_in_unsigned     = sizeof( unsigned int ) * 8;
    static const unsigned int seventy_five_percent = (unsigned int)( bits_in_unsigned * .75 );
    static const unsigned int twelve_percent       = (unsigned int)( bits_in_unsigned * .125 );
    static const unsigned int high_bits            = ~( (unsigned int)( ~0 ) >> twelve_percent );

    unsigned char* ptr = (unsigned char*)keystart;
    unsigned       i   = 0;
    unsigned int   key = 0;
    unsigned int   g;
    while ( i++ < keylen )
    {
        key = ( key << twelve_percent ) + (unsigned int)( *ptr++ );
        if ( ( g = key & high_bits ) )
        {
            key = ( key ^ ( g >> seventy_five_percent ) ) & ~high_bits;
        }
    }
    return key % maxBuckets;
}
