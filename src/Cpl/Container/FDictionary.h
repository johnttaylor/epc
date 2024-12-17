#ifndef Cpl_Container_FDictionary_h_
#define Cpl_Container_FDictionary_h_
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

#include "Cpl/Container/FHashTable_.h"


///
namespace Cpl {
///
namespace Container {


/** This template class implements a Dictionary and/or Hash Table with 
    a FIXED memory allocation (i.e. memory for the dictionary is not
    supplied via the constructor).

    Collisions are handled by a simple link list for each 'hash bucket'.

        o This is no checking for duplicate keys. You can insert multiple
          items with duplicates keys, but there is no guaranty on how
          those items are found in searches and/or removed from the table.

        o There is no limit to the number of items that can be
          stored in the table.  However, there is a performance
          penalty as the ratio of items vs. table buckets increases.
          Recommended size: Here is a list of some useful primes:
            67, 131, 257, 521, 1031, 2053, 4099, 8209, 16411

    Template ARGS:
        ITEM     - Data type of the object stored in the dictionary.  'ITEM'
                   must be a sub-class of the Cpl::Container::DictItem base
                   class.
        NBUCKETS - The number of hash buckets (see above for recommend values)

 */

template <class ITEM, int NBUCKETS>
class FDictionary
{
private:
    /// Delegate operations to the generic table implementation
    FHashTable_<NBUCKETS> m_table;

public:
    /// Constructor.
    FDictionary()
        : m_table()
    {
    }

    /// Constructor when statically allocating the instance
    FDictionary( const char* ignoreThisParameter_usedToCreateAUniqueConstructor )
        : m_table( ignoreThisParameter_usedToCreateAUniqueConstructor )
    {
    }

public:  ///@name Operations to manage items in the Dictionary
    ///@{
    /** Inserts an item into the table.
     */
    void insert( ITEM& node );

    /** Removes the node (if it exists) that has the matching key. Returns
        the node removed from the table or 0 if no key match was found.
     */
    ITEM* remove( const Key& keyOfObjectToDelete );

    /** Removes the specified item from the table.  Returns true
        when the node was found and removed; else false is
        returned (i.e. node not exists in the table).
     */
    bool removeItem( ITEM& node );

    /** Searches for a item with a matching key.  Returns the node that
        matches, else 0.
     */
    ITEM* find( const Key& keyToFind ) const;

    /** Returns true if the specified item is in the Dictionary; else false
        is returned.
     */
    bool isInDictionary( ITEM& node ) const;

    /// Returns the first item in the table. Returns 0 if table is empty
    ITEM* first() const;

    /** Returns the next item in the table.  Returns 0 if at the end-of-table
        Note: There is NO ORDER of the items stored in the Dictionary!
     */
    ITEM* next( ITEM& current ) const;

    /** Removes the first item in the list.  Returns 0 if the list
        is empty.
     */
    ITEM* getFirst();
    ///@}


public:  ///@name Operations on the Dictionary itself
    ///@{
    /** Moves the content of the this Dictionary to the specified Dictionary.
     */
    void move( FDictionary<ITEM,NBUCKETS>& dst );

    /** Empties the Dictionary.  All references to the item(s) in the
        dictionary are lost.
     */
    void clearTheDictionary();

    /** Returns table stats.  Caller provides the memory for the stats structure.

        Note: The stats are not calculate/gathered until this method is called.
              The duration of this call is directly related to the number of
              items in the dictionary.
     */
    void stats( HashTableStats& tableInfo ) const;
    ///@}


private:  ///@name Prevent the container from being copied
    ///@{
    /// Prevent access to the copy constructor -->Containers can not be copied!
    FDictionary( const FDictionary& m );

    /// Prevent access to the assignment operator -->Containers can not be copied!
    const FDictionary& operator=( const FDictionary& m );
    ///@}
};


/////////////////////////////////////////////////////////////////////////////
//                  INLINE IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////


template <class ITEM, int NBUCKETS>
void FDictionary<ITEM,NBUCKETS>::move( FDictionary<ITEM,NBUCKETS>& dst )
{
    // clear the destination dictionary
    dst.clearTheDictionary();

    // Copy each item (so the debug info is correct)
    ITEM* nextPtr;
    while ( ( nextPtr = getFirst() ) )
    {
        dst.insert( *nextPtr );
    }
}

template <class ITEM, int NBUCKETS>
void FDictionary<ITEM,NBUCKETS>::clearTheDictionary()
{
    // Drain dictionary so the debug traps work correctly
    while ( getFirst() )
        ;
}

template <class ITEM, int NBUCKETS>
void FDictionary<ITEM,NBUCKETS>::insert( ITEM& node )
{
    m_table.insert( node );
}

template <class ITEM, int NBUCKETS>
ITEM* FDictionary<ITEM,NBUCKETS>::remove( const Key& keyOfObjectToDelete )
{
    ITEM* nodePtr = find( keyOfObjectToDelete );
    if ( nodePtr )
    {
        return (ITEM*)m_table.removeItem( *nodePtr );
    }
    return 0;
}

template <class ITEM, int NBUCKETS>
bool FDictionary<ITEM,NBUCKETS>::removeItem( ITEM& node )
{
    return m_table.removeItem( node ) != 0;
}

template <class ITEM, int NBUCKETS>
ITEM* FDictionary<ITEM,NBUCKETS>::getFirst()
{
    ITEM* nodePtr = first();
    if ( nodePtr )
    {
        return (ITEM*)m_table.removeItem( *nodePtr );
    }
    return 0;
}


template <class ITEM, int NBUCKETS>
ITEM* FDictionary<ITEM,NBUCKETS>::find( const Key& keyToFind ) const
{
    return (ITEM*)m_table.find( keyToFind );
}

template <class ITEM, int NBUCKETS>
bool FDictionary<ITEM,NBUCKETS>::isInDictionary( ITEM& node ) const
{
    return node.isInContainer_( this );
}

template <class ITEM, int NBUCKETS>
ITEM* FDictionary<ITEM,NBUCKETS>::first() const
{
    return (ITEM*)m_table.first();
}

template <class ITEM, int NBUCKETS>
ITEM* FDictionary<ITEM,NBUCKETS>::next( ITEM& current ) const
{
    return (ITEM*)m_table.next( current );
}

template <class ITEM, int NBUCKETS>
void FDictionary<ITEM,NBUCKETS>::stats( HashTableStats& tableInfo ) const
{
    m_table.tableStats( tableInfo );
}


};  // end namespaces
};
#endif  // end header latch
