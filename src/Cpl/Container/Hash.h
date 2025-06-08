#ifndef Cpl_Container_Hash_h_
#define Cpl_Container_Hash_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2025  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

///
namespace Cpl {
///
namespace Container {

/** This type defines the function signature for the hashing function that operates
    on a key stored in contiguous RAM.
 */
typedef unsigned int ( *HashFunc )( const void* keystart, unsigned keylen, unsigned int maxBuckets );


/** This struct defines what usage/stats can be retrieved from a Hash table
 */
struct HashTableStats
{
    ///
    unsigned long m_numItems;
    ///
    unsigned int m_numBuckets;
    ///
    unsigned int m_numEmptyBuckets;
    ///
    double m_average_itemsPerBucket;
    ///
    unsigned long m_max_itemsPerBucket;
};

};  // end namespaces
};
#endif  // end header latch
