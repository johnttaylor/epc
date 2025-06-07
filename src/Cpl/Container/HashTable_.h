#ifndef Cpl_Container_HashTable_x_h_
#define Cpl_Container_HashTable_x_h_
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
#include "Cpl/Container/DList.h"


///
namespace Cpl {
///
namespace Container {


/** This concrete provides a collection of functions used by the 'Hash Table'
    classes.  This class is not intended to be used directly by clients.

    Key collisions are handled by a simple link list for each 'hash bucket'. 
    
    NOTE: This is no checking for duplicate keys. You can insert multiple
          items with duplicates keys, but there is no guaranty on how
          those items are found in searches or removed from the table.
 */
class HashTable_
{
public:
    /** Inserts an item into the table.
     */
    static void insert( DictItem&        node,
                        DList<DictItem>* buckets,
                        unsigned int     numBuckets,
                        HashFunc         hashFunc,
                        unsigned long&   numItems ) noexcept;


    /** Removes the specified item from the table.  Returns the
        node removed.  If the remove fails (i.e. node does
        not exist in the table), then 0 is returned.
     */
    static DictItem* removeItem( DictItem&        nodeToDelete,
                                 DList<DictItem>* buckets,
                                 unsigned int     numBuckets,
                                 HashFunc         hashFunc,
                                 unsigned long&   numItems ) noexcept;

    /** Searches for a item with a matching key.  Returns the node that
        matches, else 0.
     */
    static DictItem* find( const Key&       keyToFind,
                           DList<DictItem>* buckets,
                           unsigned int     numBuckets,
                           HashFunc         hashFunc ) noexcept;

    /// Returns the first item in the table. Returns 0 if table is empty
    static DictItem* first( DList<DictItem>* buckets,
                            unsigned int     numBuckets,
                            HashFunc         hashFunc,
                            unsigned long    numItems ) noexcept;

    /// Returns the next item in the table.  Returns 0 if at the end-of-table
    static DictItem* next( DictItem&        current,
                           DList<DictItem>* buckets,
                           unsigned int     numBuckets,
                           HashFunc         hashFunc ) noexcept;


public:
    /** Returns table stats.  Caller provides the memory for the stats structure.

        Note: The stats are not calculate/gathered until this method is called.
              The duration of this call is directly related to the number of
              items in the hash table.
     */
    static void tableStats( HashTableStats&  tableInfo,
                            DList<DictItem>* buckets,
                            unsigned int     numBuckets,
                            HashFunc         hashFunc,
                            unsigned long    numItems ) noexcept;
};


};  // end namespaces
};
#endif  // end header latch
