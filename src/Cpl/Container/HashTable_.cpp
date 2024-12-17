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

#include "HashTable_.h"


///
using namespace Cpl::Container;



///////////////////////////////
void HashTable_::insert( DictItem& node, DList<DictItem>* buckets, unsigned int numBuckets, HashFunc hashFunc, unsigned long& numItems ) noexcept
{
    // Plant the node in the indexed bucket.  Note: There is NO difference in
    // the handling of an empty-bucket vs. collision scenario since each
    // bucket IS a list.
    unsigned     keylength = 0;
    const void*  keyptr    = node.getKey().getRawKey( &keylength );
    unsigned int index     = hashFunc( keyptr, keylength, numBuckets );
    node.m_hashCode_       = index;
    buckets[index].put( node );
    numItems++;
}

DictItem* HashTable_::find( const Key& keyToFind, DList<DictItem>* buckets, unsigned int numBuckets, HashFunc hashFunc ) noexcept
{
    // Hash the key
    unsigned         keylength = 0;
    const void*      keyptr    = keyToFind.getRawKey( &keylength );
    unsigned int     index     = hashFunc( keyptr, keylength, numBuckets );
    DList<DictItem>& list      = buckets[index];
    DictItem*        itemPtr   = list.first();

    // Traverse the hash bucket's list looking for a match
    while ( itemPtr )
    {
        if ( itemPtr->getKey().compareKey( keyToFind ) == 0 )
        {
            break;
        }
        itemPtr = list.next( *itemPtr );
    }

    // Return the matching item (or 0 if not one found)
    return itemPtr;
}

DictItem* HashTable_::removeItem( DictItem& nodeToDelete, DList<DictItem>* buckets, unsigned int numBuckets, HashFunc hashFunc, unsigned long& numItems ) noexcept
{
    // Remove the node
    unsigned int index = nodeToDelete.m_hashCode_;
    if ( buckets[index].remove( nodeToDelete ) )
    {
        numItems--;
        return &nodeToDelete;
    }

    return 0;
}

DictItem* HashTable_::first( DList<DictItem>* buckets, unsigned int numBuckets, HashFunc hashFunc, unsigned long numItems ) noexcept
{
    // Trap empty table
    if ( numItems )
    {
        // Find the first non-empty bucket in the table
        unsigned int i;
        for ( i = 0; i < numBuckets; i++ )
        {
            DictItem* itemPtr = buckets[i].first();
            if ( itemPtr )
            {
                return itemPtr;
            }
        }
    }

    // Empty table -->return null
    return 0;
}

DictItem* HashTable_::next( DictItem& current, DList<DictItem>* buckets, unsigned int numBuckets, HashFunc hashFunc ) noexcept
{
    // Case: There are more item(s) in the current bucket
    unsigned int index   = current.m_hashCode_;
    DictItem*    nextPtr = buckets[index].next( current );
    if ( nextPtr )
    {
        return nextPtr;
    }

    // Case: Move to the next non-empty bucket
    while ( ++index < numBuckets )
    {
        if ( ( nextPtr = buckets[index].first() ) )
        {
            return nextPtr;
        }
    }

    // If I get here, then there are no more items in the table
    return 0;
}

//////////////////////////////
void HashTable_::tableStats( HashTableStats& info, DList<DictItem>* buckets, unsigned int numBuckets, HashFunc hashFunc, unsigned long numItems ) noexcept
{
    // Initialize stats structure
    info.m_numItems               = numItems;
    info.m_numBuckets             = numBuckets;
    info.m_numEmptyBuckets        = numBuckets;
    info.m_average_itemsPerBucket = 0.0;
    info.m_max_itemsPerBucket     = 0;
    if ( !numItems )
    {
        return;
    }

    // Walk all buckets
    unsigned int  i;
    unsigned int  filledBuckets = 0;
    unsigned long sumItemCount  = 0;
    for ( i = 0; i < numBuckets; i++ )
    {
        DictItem* itemPtr = buckets[i].first();
        if ( itemPtr )
        {
            info.m_numEmptyBuckets--;
            filledBuckets++;
            unsigned long itemCount = 0;
            while ( itemPtr )
            {
                itemCount++;
                itemPtr = buckets[i].next( *itemPtr );
            }
            sumItemCount += itemCount;
            if ( itemCount > info.m_max_itemsPerBucket )
            {
                info.m_max_itemsPerBucket = itemCount;
            }
        }
    }

    // Calc average
    info.m_average_itemsPerBucket = ( (double)sumItemCount ) / ( (double)filledBuckets );
}