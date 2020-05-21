/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_POINTER_MAP_H
#define HKBASE_POINTER_MAP_H

#include <Common/Base/Container/PointerMap/hkMap.h>

// Helpers to get the hkMap implementation for a given key size.
template<int N>
struct hkPointerMapStorage
{
	typedef hkUlong Type;
};

template<>
struct hkPointerMapStorage<8>
{
	typedef hkUint64 Type;
};

/// A class to map between pointer or integer keys and arbitrary pointer/integer values.
/// The key is not allowed to be -1.
template <typename K, typename V, typename Allocator=hkContainerHeapAllocator>
class hkPointerMap
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MAP, hkPointerMap );

		typedef typename hkPointerMapStorage<sizeof(K)>::Type Storage;
		typedef typename hkPointerMapStorage<sizeof(V)>::Type ValueCast;

			/// Iterator class
			/// All iterators are invalidated after a mutating operation. i.e., insertion,removal
		typedef typename hkMap<Storage, Storage>::Iterator Iterator;

		typedef hkPointerMap<K,V,hkContainerTempAllocator> Temp;
		typedef hkPointerMap<K,V,hkContainerDebugAllocator> Debug;
		typedef hkPointerMap<K,V,hkContainerHeapAllocator> Heap;

			/// Create an empty pointermap.
		hkPointerMap()
		{
		}

			/// Create pointer map initially using preallocated memory block.
			/// Use the getSizeInBytesFor(int numKeys) method to find the buffer size
			/// required for a given number of keys.
		hkPointerMap(void* ptr, int sizeInBytes) : m_map(ptr, sizeInBytes)
		{
		}

	protected:
		
			/// For creating an uninitialized map.
		enum InternalInitializer
		{
			UNINITIALIZED,
		};

			/// Creates an uninitialized hkPointerMap. For internal use.
		hkPointerMap( InternalInitializer ii ) : m_map( hkMap<Storage,Storage, hkMapOperations<Storage>, Allocator >::UNINITIALIZED ) {}

	public:

			///	Insert key with associated value val. Keys are unique and cannot be -1.
			/// If key already exists it is overwritten. Returns true if the key is new
			/// and false if an existing key was overwritten.
		HK_FORCE_INLINE hkBool32 insert( K key, V val )
		{
			return m_map.insert( Storage(key), Storage(val) );
		}

			/// Return the iterator associated with key or the end iterator if not present.
		HK_FORCE_INLINE Iterator findKey( K key ) const
		{
			return m_map.findKey( Storage(key) );
		}

			/// If key is present return its iterator, else insert (key,val) and return the new iterator.
			/// Thus the returned iterator is always valid.
		HK_FORCE_INLINE Iterator findOrInsertKey( K key, V val )
		{
			return m_map.findOrInsertKey( Storage(key), Storage(val) );
		}

			/// Return if this map contains the given key.
		HK_FORCE_INLINE hkBool hasKey( K key ) const
		{
			return m_map.hasKey( Storage(key) );
		}

			/// Return the value associated with key or def if not present.
		HK_FORCE_INLINE V getWithDefault( K key, V def ) const
		{
			return (V)(ValueCast)m_map.getWithDefault( Storage(key), Storage(def) );
		}

			/// If key present, write value into out and return HK_SUCCESS. Else return HK_FAILURE.
		hkResult get( K key, V* out ) const
		{
			Storage tmp;
			if( m_map.get( Storage(key), &tmp ) == HK_SUCCESS )
			{
				*out = V(tmp);
				return HK_SUCCESS;
			}
			return HK_FAILURE;
		}

			/// Remove pair at it. The iterator is no longer valid after this call.
		void remove( Iterator it )
		{
			m_map.remove( it );
		}

			/// If key present, remove it and return HK_SUCCESS. Otherwise return HK_FAILURE.
		hkResult remove( K key )
		{
			return m_map.remove( Storage(key) );
		}

			/// Return the number of elements in this map.
		int getSize() const
		{
			return m_map.getSize();
		}

			/// Return the number of elements that can be stored in this map without reallocation.
		int getCapacity() const
		{
			return m_map.getCapacity();
		}

			/// Return the amount of allocated memory in bytes. Use for statistics.
		int getMemSize() const
		{
			return m_map.getMemSize();
		}

			/// Return the start address in memory of the hashmap. Use for statistics.
		void* getMemStart() const
		{
			return m_map.getMemStart();
		}

			/// Perform internal consistency check.
		hkBool isOk() const
		{
			return m_map.isOk();
		}

			/// Get an iterator over the keys of this map.
		Iterator getIterator() const
		{
			return m_map.getIterator();
		}

			/// Get the key at iterator i.
		K getKey( Iterator i ) const
		{
			return (K)m_map.getKey(i);
		}

			/// Get the value at iterator i.
		V getValue( Iterator i ) const
		{
			return (V)(ValueCast)m_map.getValue(i);
		}

			/// Overwrite the value at iterator i.
		void setValue( Iterator i, V val )
		{
			m_map.setValue(i, Storage(val) );
		}

			/// Get the next iterator after i.
		Iterator getNext( Iterator i ) const
		{
			return m_map.getNext(i);
		}

			/// Return if the iterator has reached the end.
		hkBool isValid( Iterator i ) const
		{
			return m_map.isValid(i);
		}

			/// clear the table
		void clear()
		{
			m_map.clear();
		}

			/// Reserve space for at least numElements;
		void reserve( int numElements )
		{
			m_map.reserve(numElements);
		}

			/// Swap the internal representation with another map.
		void swap( hkPointerMap& other )
		{
			m_map.swap( other.m_map );
		}

			/// Return true if the map was constructed with a buffer which was too small.
		inline hkBool wasReallocated() const { return m_map.wasReallocated(); }

			/// Advanced use only.
		void setOwnedData(void* ptr, int size, int capacity)
		{
			m_map.setOwnedData(ptr, size, capacity);
		}

			/// Calculates buffer size required to store the specified number of keys.
		static int HK_CALL getSizeInBytesFor(int numOfKeys) { return hkMap<Storage,Storage>::getSizeInBytesFor(numOfKeys); }

	private:

		void operator = (const hkPointerMap<K,V>& map) {}
		hkPointerMap ( const hkPointerMap<K,V>& map ) {}

	public:

	protected:

		hkMap<Storage,Storage, hkMapOperations<Storage>, Allocator > m_map;
};

#endif // HKBASE_HKPOINTERMAP_H

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
