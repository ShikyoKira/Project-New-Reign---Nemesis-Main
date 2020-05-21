/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_MULTI_MAP_H
#define HKBASE_MULTI_MAP_H

template <typename KEY>
struct hkMultiMapOperations
{
	inline static unsigned hash( KEY key, unsigned mod )
	{
		// We ignore the lowest four bits on the address, since most addresses will be 16-byte aligned
		// knuths multiplicative golden hash
		return unsigned((hkUlong(key) >> 4) * 2654435761U) & mod;
	}
	inline static void invalidate( KEY& key ) { key = KEY(-1); }
	inline static hkBool32 isValid( KEY key ) { return key != KEY(-1); }
	inline static hkBool32 equal( KEY key0, KEY key1 ) { return key0 == key1; }
};

// Operations for integral types - the difference is the hash, doesn't throw away the bottom bits
// which are significant for most uses of integral types.
struct hkMultiMapIntegralOperations
{
	inline static unsigned hash( hkUlong key, unsigned mod ) { return (unsigned(key) * 2654435761U) & mod; }
	inline static void invalidate( hkUlong& key ) { key = hkUlong(-1); }
	inline static hkBool32 isValid( hkUlong key ) { return key != hkUlong(-1); }
	inline static hkBool32 equal( hkUlong key0, hkUlong key1 ) { return key0 == key1; }
};

/// A class to map between POD type keys and values.
/// Note that the key must have an invalid or reserved state which is used to mark
/// empty slots. For instance by default -1 is disallowed as a integer key. You can override
/// this behavior by instantiating with a custom operations structure.
template <typename KEY, typename VAL=KEY, typename OPS=hkMultiMapOperations<KEY>, typename ALLOC=hkContainerHeapAllocator >
class hkMultiMap //+reflected(false)
{
	public:

		typedef hkMultiMap<KEY, VAL, OPS, ALLOC> ThisType;
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MAP, ThisType );

			/// Iterator type.
			/// All iterators are invalidated after a mutating operation. i.e., insertion,removal
		typedef class Dummy* Iterator;
#       define HK_MULTI_MAP_INDEX_TO_ITERATOR(i) reinterpret_cast<Iterator>( hkUlong(i) )
#       define HK_MULTI_MAP_ITERATOR_TO_INDEX(it) static_cast<int>( reinterpret_cast<hkUlong>(it) )

			/// Create an empty pointer map.
        hkMultiMap();

			/// Create an empty pointer map with a pre-allocated amount of space.
        hkMultiMap(int numElements);

			/// Create pointer map initially using preallocated memory block.
			/// Use the getSizeInBytesFor(int numKeys) method to find the buffer size
			/// required for a given number of keys.
        hkMultiMap(void* ptr, int sizeInBytes);

			/// Destroy a pointer map.
        ~hkMultiMap();

            /// Determine the number of entries with the key
        int findNumEntries(KEY key) const;
            /// Find the number of entries with the same key and value
        int findNumEntries(KEY key, VAL val) const;

            /// Remove all of the entries with the key. Returns the amount removed.
        int removeAll( KEY key);

			/// Get an iterator over the keys of this map.
		Iterator getIterator() const;


			/// Get the key at iterator i.
		KEY getKey( Iterator i ) const;

			/// Get the value at iterator i.
		VAL getValue( Iterator i ) const;

			/// Overwrite the value at iterator i.
		void setValue( Iterator i, VAL val );

			/// Get the next iterator after i.
		Iterator getNext( Iterator i ) const;

			/// Get the next iterator after i - with the key value
			/// Only meaningful when the iterator was created with findKey
		Iterator getNext( Iterator i, KEY key) const;

			/// Return if the iterator has not reached the end.
		hkBool isValid( Iterator i ) const;

			/// Insert key with associated value val. Keys are unique and by default
			/// (if using hkPointerMultiMapOperations) cannot be -1.
		void insert( KEY key, VAL val );

			/// Get an iterator at 'key'. Check if key was found with isValid().
		Iterator findKey( KEY key ) const;

			/// Get an iterator at 'key'. Check if key was found with isValid().
		Iterator findKeyWithValue( KEY key, VAL val ) const;

			/// If key is present return its iterator, else insert (key,val) and return the new iterator.
			/// Thus the returned iterator is always valid.
		Iterator findOrInsertKey( KEY key, VAL def );

			/// Shortcut for isValid(findKey(key)).
		hkBool hasKey( KEY key ) const { return isValid(findKey(key)); }

			/// Return the value associated with key or def if not present.
		VAL getWithDefault( KEY key, VAL def ) const;

			/// If key present, write value into out and return HK_SUCCESS. Otherwise return HK_FAILURE.
		hkResult get( KEY key, VAL* out ) const;

			/// Remove pair at iterator.
		void remove( Iterator it );

			/// If key present, remove it and return HK_SUCCESS. Otherwise return HK_FAILURE.
		hkResult remove( KEY key );

			/// Remove an entry (if found) that has both key, and value. Returns HK_SUCCESS.
		hkResult remove( KEY key, VAL val);

			/// Return the number of keys.
		int getSize() const { return m_numElems & static_cast<int>(NUM_ELEMS_MASK); }

			/// Return the number of keys possible to store without reallocation.
		int getCapacity() const { return (m_hashMod + 1) & static_cast<int>(NUM_ELEMS_MASK); }

			/// Return the amount of allocated memory in bytes. Use for statistics.
		int getMemSize() const;

			/// Return the start address in memory of the hashmap. Use for statistics.
		void* getMemStart() const { return m_elem; }

			/// Perform an internal consistency check.
		hkBool isOk() const;

			/// Remove all keys from the map.
		void clear();

			/// Swap all data with another map.
		void swap(hkMultiMap& other);

			// Estimates and sets the appropriate table size for a given number of elements.
		void reserve( int numElements );

			/// Return true if the map was constructed with a buffer which was too small.
		inline hkBool wasReallocated() const { return ( (m_numElems & static_cast<int>(DONT_DEALLOCATE_FLAG)) == 0 ); }

			/// Calculates buffer size required to store the specified number of keys.
		static int HK_CALL getSizeInBytesFor(int numOfKeys);

	protected:

		void resizeTable(int capacity);

	protected:
		static inline bool isPower2(unsigned int v) { return (v & (v - 1)) == 0; }

		// Internal flags, set in constructor.
		enum
		{
			NUM_ELEMS_MASK = int(0x7FFFFFFF),
			DONT_DEALLOCATE_FLAG = int(0x80000000) // Indicates that the storage is not the array's to delete
		};

		struct Pair //+reflected(false)
		{
			KEY key;
			VAL val;
		};

		Pair* m_elem;	
		int m_numElems; // high bits are flags
		int m_hashMod; // capacity - 1
};

//#define HK_MULTI_MAP_EMPTY_KEY -1

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
inline typename hkMultiMap<KEY,VAL,OPS,ALLOC>::Iterator hkMultiMap<KEY,VAL,OPS,ALLOC>::getIterator() const
{
	int i;
	for( i = 0; i <= m_hashMod; ++i )
	{
		if( OPS::isValid( m_elem[i].key ) )
		{
			break;
		}
	}

    return HK_MULTI_MAP_INDEX_TO_ITERATOR(i);
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
inline KEY hkMultiMap<KEY,VAL,OPS,ALLOC>::getKey(Iterator it) const
{
    int i = HK_MULTI_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	return m_elem[i].key;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
inline VAL hkMultiMap<KEY,VAL,OPS,ALLOC>::getValue(Iterator it) const
{
    int i = HK_MULTI_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	return m_elem[i].val;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
inline void hkMultiMap<KEY,VAL,OPS,ALLOC>::setValue(Iterator it, VAL val)
{
    int i = HK_MULTI_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	m_elem[i].val = val;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
inline typename hkMultiMap<KEY,VAL,OPS,ALLOC>::Iterator hkMultiMap<KEY,VAL,OPS,ALLOC>::getNext( Iterator it ) const
{
    int i = HK_MULTI_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	for( i += 1; i <= m_hashMod; ++i )
	{
		if( OPS::isValid( m_elem[i].key ) )
		{
			break;
		}
	}
    return HK_MULTI_MAP_INDEX_TO_ITERATOR(i);
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
inline typename hkMultiMap<KEY,VAL,OPS,ALLOC>::Iterator hkMultiMap<KEY,VAL,OPS,ALLOC>::getNext( Iterator it, KEY key ) const
{
    int i = HK_MULTI_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);

	i++;	// Next
	while (true)
	{		
		for(; i <= m_hashMod; ++i )
		{
			if ( !OPS::isValid(m_elem[i].key))
			{
				// not found
				return HK_MULTI_MAP_INDEX_TO_ITERATOR(m_hashMod+1); // not found
			}
			if( OPS::equal(m_elem[i].key, key) )
			{
				// Found
				return HK_MULTI_MAP_INDEX_TO_ITERATOR(i); 
			}
		}
		// Start from the top
		i = 0;
	}
}


template < typename KEY, typename VAL, typename OPS, typename ALLOC >
inline hkBool hkMultiMap<KEY,VAL,OPS,ALLOC>::isValid( Iterator it ) const
{
	// range [0, hashMod] is valid
	// hashMod+1 invalid
	// anything else is bad input
    int i = HK_MULTI_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod+1);
	return i <= m_hashMod;
}

#endif // HKBASE_HKPOINTERMAPBASE_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
