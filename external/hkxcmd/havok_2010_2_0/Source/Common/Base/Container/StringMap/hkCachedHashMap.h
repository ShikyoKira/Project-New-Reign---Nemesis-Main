/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_CACHEDHASHMAP_H
#define HKBASE_CACHEDHASHMAP_H

	/// A hash map which class to map strings to pointers/pointer size integers.
template <typename Operations, typename Allocator>
class hkCachedHashMap
{
	public:
		friend class hkTrackerCachedHashMapLayoutHandler;
		typedef hkCachedHashMap<Operations, Allocator> ThisType;
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MAP, ThisType );

			/// Iterator class
			/// All iterators are invalidated after a mutating operation. i.e., insertion,removal
		typedef class Dummy* Iterator;

			/// Create an empty String map.
		hkCachedHashMap(Operations ops = Operations());

			/// Destroy a String map.
		~hkCachedHashMap();

			/// Get an iterator over the keys of this map.
		Iterator getIterator() const;

			/// Get the key at iterator i.
		hkUlong getKey( Iterator i ) const;

			/// Get the value at iterator i.
		hkUlong getValue( Iterator i ) const;

			/// Overwrite the value at iterator i.
		void setValue( Iterator i, hkUlong v );

			/// Get the next iterator after i.
		Iterator getNext( Iterator i ) const;

			/// Return if the iterator has reached the end.
		hkBool isValid( Iterator i ) const;

			/// Insert key with associated value val.
			/// If key already exists it is overwritten. The string storage is not
			/// copied and must exist for the lifetime of the key.
		void insert( hkUlong key, hkUlong val );

			/// Get an iterator at 'key'. Check if key was found with isValid().
		Iterator findKey( hkUlong key ) const;

			/// Find the given key or insert it with the supplied value if not found.
		Iterator findOrInsertKey( hkUlong key, hkUlong value );

			/// Shortcut for isValid(findKey(key)).
		hkBool hasKey( hkUlong key ) const { return isValid(findKey(key)); }

			/// Return the value associated with key or if not present, insert and return 'ifNotFound'.
		hkUlong getOrInsert( hkUlong key, hkUlong notFound );

			/// Return the value associated with key or def if not present.
		hkUlong getWithDefault( hkUlong key, hkUlong def ) const;

			/// If key present, write value into out and return HK_SUCCESS. Otherwise return HK_FAILURE.
		hkResult get( hkUlong key, hkUlong* out ) const;
		
			/// Remove pair at iterator.
		void remove( Iterator it );

			/// If key present, remove it and return HK_SUCCESS. Otherwise return HK_FAILURE.
		hkResult remove( hkUlong key );

			/// Return the number of keys.
		int getSize() const { return m_numElems; }	
	
			/// Perform an internal consistency check.
		hkBool isOk() const;

			/// Assignment operator. Will copy the memory chunk.
		hkCachedHashMap& operator=(const hkCachedHashMap* other);

			/// Remove all keys from the map.
		void clear();

			/// Swap all data with another map.
		void swap( hkCachedHashMap& other );

			/// Insert keys from other into this, overwriting duplicates.
		void merge( const hkCachedHashMap& other );

			///
		int getCapacity() const;

			/// Reserve space for at least numElements;
		void reserve( int numElements );

	protected:
			
		void resizeTable(int capacity);

	protected:

		struct Elem
		{
			hkUlong hash;
			hkUlong key;
			hkUlong value;
		};

		Elem* m_elem;
		int m_numElems;
		int m_hashMod; // capacity - 1
		Operations m_ops;
};

#endif // HKBASE_CACHEDHASHMAP_H

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
