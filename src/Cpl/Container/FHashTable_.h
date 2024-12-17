#ifndef Cpl_Container_FHashTable_x_h_
#define Cpl_Container_FHashTable_x_h_
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
    and/or Hash Table. All of the memory for the class is statically allocated

    The hash function is 'fixed' to the default hash function

    Template args:
        N:= The number of hash buckets
 */
template<int N>
class FHashTable_
{
protected:
    /// Struct to allocate Memory for array of hash buckets
    struct MemDList
    {
        void* m_ptrs[2];
    };

    /// Memory for an array of hash buckets
    MemDList m_memBuckets[N];

    /// Number of items in the table
    unsigned long m_numItems;

public:
    /// Constructor
    FHashTable_()
    : m_numItems( 0 )
    {
        // Brute force initialize the Bucket member
        memset( m_memBuckets, 0, sizeof( m_memBuckets ) );
    }

    /// Constructor when statically allocating the instance
    FHashTable_( const char* ignoreThisParameter_usedToCreateAUniqueConstructor)
    {
        // Do nothing since by definition I am in the BSS segment and everything is already zeroed by C runtime code
    }

public:
    /// See Cpl::Container::HashTable_
    inline void insert( DictItem& node ) { HashTable_::insert( node, (DList<DictItem>*)m_memBuckets, N, hashFuncDefault, m_numItems ); }

    /// See Cpl::Container::HashTable_
    inline DictItem* removeItem( DictItem& nodeToDelete ) { return HashTable_::removeItem( nodeToDelete, (DList<DictItem>*)m_memBuckets, N, hashFuncDefault, m_numItems ); }

    /// See Cpl::Container::HashTable_
    inline DictItem* find( const Key& keyToFind ) const { return HashTable_::find( keyToFind, (DList<DictItem>*)m_memBuckets, N, hashFuncDefault ); }

    /// See Cpl::Container::HashTable_
    inline DictItem* first() const { return HashTable_::first( (DList<DictItem>*)m_memBuckets, N, hashFuncDefault, m_numItems ); }

    /// See Cpl::Container::HashTable_
    inline DictItem* next( DictItem& current ) const { return HashTable_::next( current, (DList<DictItem>*)m_memBuckets, N, hashFuncDefault ); }


public:
    /// See Cpl::Container::HashTable_
    inline void tableStats( HashTableStats& tableInfo ) const { HashTable_::tableStats( tableInfo, (DList<DictItem>*)m_memBuckets, N, hashFuncDefault, m_numItems ); }
};


};  // end namespaces
};
#endif  // end header latch
