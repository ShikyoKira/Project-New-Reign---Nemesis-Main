/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_MAP_H
#define HKBASE_MAP_H

	/// Default operations for hkMaps
template <typename KEY>
struct hkMapOperations
{
	inline static unsigned hash( KEY key, unsigned mod )
	{
		// We ignore the lowest four bits on the address, since most addresses will be 16-byte aligned
		// knuths multiplicative golden hash
		return unsigned((hkUlong(key) >> 4) * 2654435761U) & mod;
	}
	inline static void invalidate( KEY& key )
	{
		key = KEY(-1);
	}
	inline static hkBool32 isValid( KEY key )
	{
		return key != KEY(-1);
	}
	inline static hkBool32 equal( KEY key0, KEY key1 )
	{
		return key0 == key1;
	}
};

// Specialized version without shifting away the bottom bits
template <>
struct hkMapOperations<int>
{
	inline static unsigned hash( int key, unsigned mod ) { return (unsigned(key) * 2654435761U) & mod; }
	inline static void invalidate( int& key ) { key = int(-1); }
	inline static hkBool32 isValid( int key ) { return key != int(-1); }
	inline static hkBool32 equal( int key0, int key1 ) { return key0 == key1; }
};

	/// A class to store key, value pairs..
	/// Note that the key must have an invalid or reserved state which is used to mark
	/// empty slots. For instance by default -1 is disallowed as a integer key. You can override
	/// this behavior by instantiating with a custom operations structure.
template <typename KEY, typename VAL=KEY, typename OPS=hkMapOperations<KEY> >
class hkMapBase
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MAP, hkMapBase );

			/// Iterator type.
			/// All iterators are invalidated after a mutating operation. i.e., insertion,removal
		typedef class Dummy* Iterator;
#		define HK_MAP_INDEX_TO_ITERATOR(i) reinterpret_cast<Iterator>( hkUlong(i) )
#		define HK_MAP_ITERATOR_TO_INDEX(it) static_cast<int>( reinterpret_cast<hkUlong>(it) )

			/// Create an empty pointer map.
		hkMapBase() : m_elem(HK_NULL), m_numElems(0), m_hashMod(-1) {}

			/// Create pointer map initially using preallocated memory block.
			/// Use the getSizeInBytesFor(int numKeys) method to find the buffer size
			/// required for a given number of keys.
		hkMapBase(void* ptr, int sizeInBytes);

	protected:

			/// For creating an uninitialized map.
		enum InternalInitializer
		{
			UNINITIALIZED,
		};

			/// Create an uninitialized map. For internal use.
		hkMapBase( InternalInitializer ii ) {}

	public:

			/// Destroy a pointer map.
		~hkMapBase();
		
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

			/// Return if the iterator has not reached the end.
		hkBool isValid( Iterator i ) const;

			/// Insert key with associated value val. Keys are unique and by default
			/// (if using hkPointerMapOperations) cannot be -1.
			/// If key already exists it is overwritten.
		hkBool32 insert( hkMemoryAllocator& alloc, KEY key, VAL val );

			/// Get an iterator at 'key'. Check if key was found with isValid().
		Iterator findKey( KEY key ) const;

			/// If key is present return its iterator, else insert (key,val) and return the new iterator.
			/// Thus the returned iterator is always valid.
		Iterator findOrInsertKey( hkMemoryAllocator& alloc, KEY key, VAL def );

			/// Shortcut for isValid(findKey(key)).
		hkBool hasKey( KEY key ) const { return isValid(findKey(key)); }

			/// Return the value associated with key or def if not present.
		VAL getWithDefault( KEY key, VAL def ) const;

			/// If key present, write value into out and return HK_SUCCESS. Otherwise return HK_FAILURE.
		hkResult get( KEY key, VAL* out ) const;

			/// Remove pair at iterator. The iterator is no longer valid after this call.
		void remove( Iterator it );

			/// If key present, remove it and return HK_SUCCESS. Otherwise return HK_FAILURE.
		hkResult remove( KEY key );

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

			/// Remove all keys from the map.
		void clearAndDeallocate( hkMemoryAllocator& a );

			// Estimates and sets the appropriate table size for a given number of elements.
		void reserve( hkMemoryAllocator& a, int numElements );

			/// Return true if the map was constructed with a buffer which was too small.
		inline hkBool wasReallocated() const { return ( (m_numElems & static_cast<int>(DONT_DEALLOCATE_FLAG)) == 0 ); }

			/// Advanced use only.
		void setOwnedData(void* ptr, int size, int capacity);

			/// Calculates buffer size required to store the specified number of keys.
		static int HK_CALL getSizeInBytesFor(int numOfKeys);

			/// Contains the implementation of hkMapBase(void* ptr, int sizeInBytes).
			/// For internal use only.
		void init(void* ptr, int sizeInBytes);

	protected:

		void resizeTable(hkMemoryAllocator& alloc, int capacity);

	protected:

		static inline hkBool32 isPower2(unsigned int v) { return (v & (v - 1)) == 0; }

		// Internal flags, set in constructor.
		enum
		{
			NUM_ELEMS_MASK = int(0x7FFFFFFF),
			DONT_DEALLOCATE_FLAG = int(0x80000000) // Indicates that the storage is not the array's to delete
		};

		struct Pair
		{
			KEY key;
			VAL val;
		};

		Pair* m_elem;
		int m_numElems; // high bits are flags
		int m_hashMod; // capacity - 1
};

	/// A hkMapBase with a builtin allocator.
template <typename KEY, typename VAL=KEY, typename OPS=hkMapOperations<KEY>, typename Allocator=hkContainerHeapAllocator>
class hkMap : public hkMapBase<KEY,VAL,OPS>
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MAP, hkMap );

		typedef hkMap<KEY,VAL,OPS,hkContainerTempAllocator> Temp;
		typedef hkMap<KEY,VAL,OPS,hkContainerDebugAllocator> Debug;
		typedef hkMap<KEY,VAL,OPS,hkContainerHeapAllocator> Heap;

			///
		hkMap(int nelem=0) { if (nelem){ reserve(nelem); } }

			/// Create pointer map initially using preallocated memory block.
			/// Use the getSizeInBytesFor(int numKeys) method to find the buffer size
			/// required for a given number of keys.
		hkMap(void* ptr, int sizeInBytes) : hkMapBase<KEY,VAL,OPS>(ptr, sizeInBytes) {}

			/// For creating an uninitialized map.
		enum InternalInitializer
		{
			UNINITIALIZED,
		};

			/// Create an uninitialized map. For internal use.
		hkMap( InternalInitializer ii ) : hkMapBase<KEY,VAL,OPS>( hkMapBase<KEY,VAL,OPS>::UNINITIALIZED )  {}

			/// Destroy a pointer map.
		~hkMap() { hkMapBase<KEY, VAL, OPS>::clearAndDeallocate(Allocator().get(this)); }
		
			/// Insert key with associated value val. Keys are unique and by default
			/// (if using hkPointerMapOperations) cannot be -1.
			/// If key already exists it is overwritten.
		HK_FORCE_INLINE hkBool32 insert( KEY key, VAL val ) { return hkMapBase<KEY,VAL,OPS>::insert(Allocator().get(this), key, val); }

			/// If key is present return its iterator, else insert (key,val) and return the new iterator.
			/// Thus the returned iterator is always valid.
		HK_FORCE_INLINE typename hkMapBase<KEY,VAL,OPS>::Iterator findOrInsertKey( KEY key, VAL def ) { return hkMapBase<KEY,VAL,OPS>::findOrInsertKey(Allocator().get(this), key, def); }

			/// Swap all data with another map.
		void swap(hkMap& other);

			// Estimates and sets the appropriate table size for a given number of elements.
		HK_FORCE_INLINE void reserve( int numElements ) { return hkMapBase<KEY,VAL,OPS>::reserve(Allocator().get(this), numElements); }
};

template < typename KEY, typename VAL, typename OPS >
inline typename hkMapBase<KEY,VAL,OPS>::Iterator hkMapBase<KEY,VAL,OPS>::getIterator() const
{
	int i;
	for( i = 0; i <= m_hashMod; ++i )
	{
		if( OPS::isValid( m_elem[i].key ) )
		{
			break;
		}
	}
	
	return HK_MAP_INDEX_TO_ITERATOR(i);
}

template < typename KEY, typename VAL, typename OPS >
inline KEY hkMapBase<KEY,VAL,OPS>::getKey(Iterator it) const
{
	int i = HK_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	return m_elem[i].key;
}

template < typename KEY, typename VAL, typename OPS >
inline VAL hkMapBase<KEY,VAL,OPS>::getValue(Iterator it) const
{
	int i = HK_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	return m_elem[i].val;
}

template < typename KEY, typename VAL, typename OPS >
inline void hkMapBase<KEY,VAL,OPS>::setValue(Iterator it, VAL val)
{
	int i = HK_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	m_elem[i].val = val;
}

template < typename KEY, typename VAL, typename OPS >
inline typename hkMapBase<KEY,VAL,OPS>::Iterator hkMapBase<KEY,VAL,OPS>::getNext( Iterator it ) const
{
	int i = HK_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	for( i += 1; i <= m_hashMod; ++i )
	{
		if( OPS::isValid( m_elem[i].key ) )
		{
			break;
		}
	}
	return HK_MAP_INDEX_TO_ITERATOR(i);
}

template < typename KEY, typename VAL, typename OPS >
inline hkBool hkMapBase<KEY,VAL,OPS>::isValid( Iterator it ) const
{
	// range [0, hashMod] is valid
	// hashMod+1 invalid
	// anything else is bad input
	int i = HK_MAP_ITERATOR_TO_INDEX(it);
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod+1);
	return i <= m_hashMod;
}

#endif // HKBASE_MAP_H

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
