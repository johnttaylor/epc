#ifndef Cpl_Container_DHashTable_x_h_
#define Cpl_Container_DHashTable_x_h_
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

#include "Cpl/Container/DictItem.h"
#include "Cpl/Container/Hash.h"
#include "Cpl/Container/HashTable_.h"
#include "Cpl/Container/HashFuncDefault.h"
#include "Cpl/Container/DList.h"


///
namespace Cpl {
///
namespace Container {


/** This concrete class implements the core functionality for a Dictionary
    and/or Hash Table. The class requires the application to supply the memory
    and hash function (a default function is available) for the hash table.
 */
class DHashTable_
{
private:
    /// Array of hash buckets
    DList<DictItem>* m_buckets;

    /// Number of hash buckets
    unsigned int m_numBuckets;

    /// Pointer to the hashing function
    HashFunc m_hashFunc;

    /// Number of items in the table
    unsigned long m_numItems;

public:
    /// Constructor
    DHashTable_( DList<DictItem> buckets[], unsigned int numBuckets, HashFunc func = Cpl::Container::hashFuncDefault )
    : m_buckets( buckets )
    , m_numBuckets( numBuckets )
    , m_hashFunc( func )
    , m_numItems( 0 )
    {
    }


public:
    /// See Cpl::Container::HashTable_
    inline void insert( DictItem& node ) { HashTable_::insert( node, m_buckets, m_numBuckets, m_hashFunc, m_numItems ); }

    /// See Cpl::Container::HashTable_
    inline DictItem* removeItem( DictItem& nodeToDelete ) { return HashTable_::removeItem( nodeToDelete, m_buckets, m_numBuckets, m_hashFunc, m_numItems ); }

    /// See Cpl::Container::HashTable_
    inline DictItem* find( const Key& keyToFind ) const { return HashTable_::find( keyToFind, m_buckets, m_numBuckets, m_hashFunc ); }

    /// See Cpl::Container::HashTable_
    inline DictItem* first() const { return HashTable_::first( m_buckets, m_numBuckets, m_hashFunc, m_numItems ); }

    /// See Cpl::Container::HashTable_
    inline DictItem* next( DictItem& current ) const { return HashTable_::next( current, m_buckets, m_numBuckets, m_hashFunc ); }


public:
    /// See Cpl::Container::HashTable_
    inline void tableStats( HashTableStats& tableInfo ) const { HashTable_::tableStats( tableInfo, m_buckets, m_numBuckets, m_hashFunc, m_numItems ); }
};


};  // end namespaces
};
#endif  // end header latch
