/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_STRINGMAP_H
#define HKBASE_STRINGMAP_H

#include <Common/Base/Container/StringMap/hkCachedHashMap.h>

struct hkStringMapOperations
{
	HK_FORCE_INLINE hkBool32 equal(hkUlong k1, hkUlong k2) const
	{
		return hkString::strCmp( (const char*)k1, (const char*)k2 ) == 0;
	}
	HK_FORCE_INLINE hkUlong hash(hkUlong key) const
	{
		const char* p = (const char*)key;
		hkUlong h = 0;
		for( int i = 0; p[i] != HK_NULL; ++i )
		{
			h = 31 * h + p[i];
		}
		return h & (hkUlong(-1)>>1); // reserve -1 for empty
	}
};

/// Map strings to integers or pointers.
/// Note that neither the keys nor values are copied so the values
/// must exist for the lifetime of this object.
template <typename V, typename Allocator=hkContainerHeapAllocator>
class hkStringMap
{
	HK_COMPILE_TIME_ASSERT(hkSizeOf(V) <= hkSizeOf(hkUlong));

	public:
		friend class hkTrackerStringMapLayoutHandler;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MAP, hkStringMap );

			/// Iterator class
			/// All iterators are invalidated after a mutating operation. i.e., insertion,removal
		typedef typename hkCachedHashMap<hkStringMapOperations, Allocator>::Iterator Iterator;

		hkStringMap()
		{
		}

			///	Insert key with associated value val.
			/// If key already exists it is overwritten. The key string is not
			/// copied and must exist for the lifetime of the entry.
		HK_FORCE_INLINE void insert( const char* key, V val )
		{
			m_map.insert( hkUlong(key), hkUlong(val) );
		}

			/// Return the iterator associated with key. Check with isValid().
		HK_FORCE_INLINE Iterator findKey( const char* key ) const
		{
			return m_map.findKey( hkUlong(key) );
		}

			/// If key is present return its iterator, else insert (key,val) and return the new iterator.
			/// Thus the returned iterator is always valid.
		HK_FORCE_INLINE Iterator findOrInsertKey( const char* key, V val )
		{
			return m_map.findOrInsertKey( hkUlong(key), hkUlong(val) );
		}

			/// Return if this map contains the given key.
		HK_FORCE_INLINE hkBool hasKey( const char* key ) const
		{
			return m_map.hasKey( hkUlong(key) );
		}

			/// Return the value associated with key or if not present, insert and return 'ifNotFound'.
		HK_FORCE_INLINE V getOrInsert( const char* key, V ifNotFound )
		{
			return (V)m_map.getOrInsert( hkUlong(key), hkUlong(ifNotFound) );
		}

			/// Return the value associated with key or def if not present.
		HK_FORCE_INLINE V getWithDefault( const char* key, V def ) const
		{
			return (V)m_map.getWithDefault( hkUlong(key), hkUlong(def) );
		}

			/// If key present, write value into out and return HK_SUCCESS. Else return HK_FAILURE.
		hkResult get( const char* key, V* out ) const
		{
			hkUlong tmp;
			if( m_map.get( hkUlong(key), &tmp ) == HK_SUCCESS )
			{
				*out = V(tmp);
				return HK_SUCCESS;
			}
			return HK_FAILURE;

		}

			/// Remove pair at "it".
		void remove( Iterator it )
		{
			m_map.remove( it );
		}

			/// If key present, remove it and return HK_SUCCESS. Otherwise return HK_FAILURE.
		hkResult remove( const char* key )
		{
			return m_map.remove( hkUlong(key) );
		}

			/// Return the number of elements in this map.
		int getSize() const
		{
			return m_map.getSize();
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
			/// Do not modify this key directly. If you must change a key, remove and re-add it.
		const char* getKey( Iterator i ) const
		{
			return reinterpret_cast<const char*>(m_map.getKey(i));
		}

			/// Get the value at iterator i.
		V getValue( Iterator i ) const
		{
			return (V)m_map.getValue(i);
		}

			/// Overwrite the value at iterator i.
		void setValue( Iterator i, V v )
		{
			m_map.setValue( i, hkUlong(v) );
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

			/// Remove all keys from the map.
		void clear()
		{
			m_map.clear();
		}

			/// Swap all data with another map.
		void swap( hkStringMap& other )
		{
			m_map.swap(other.m_map);
		}

			/// Insert keys from other into this, overwriting duplicates.
		void merge( const hkStringMap& other )
		{
			m_map.merge(other.m_map);
		}

		int getCapacity() const
		{
			return m_map.getCapacity();
		}

			/// Reserve space for at least numElements;
		void reserve( int numElements )
		{
			m_map.reserve(numElements);
		}

	protected:

		hkCachedHashMap<hkStringMapOperations, Allocator> m_map;
};

#endif // HKBASE_STRINGMAP_H

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
