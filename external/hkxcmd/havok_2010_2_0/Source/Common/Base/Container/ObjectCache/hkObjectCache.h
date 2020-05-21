/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_OBJECT_CACHE_H
#define HK_OBJECT_CACHE_H

#include <Common/Base/Object/hkReferencedObject.h>

/// This class offers a simple database of objects which can be queried by key.
/// It is a cache in the sense that it preserves up to m_maxSize worth of removed objects.
template<typename KeyType>
class hkObjectCache : public hkReferencedObject
{
	public:
			/// Construct a cache with the given maximum size.
		hkObjectCache( hkUint32 maxSize );

			/// Destructor gives up references to any active or preserved objects.
		virtual ~hkObjectCache();

			/// Query the cache to see if it has an object corresponding to a key.
			/// If an object is found, it is returned and its usage count increased
			/// Otherwise, HK_NULL is returned and the method has no effect on the cache.
		hkReferencedObject* getObject( KeyType key );

			/// Add an object to the cache, and set its usage count accordingly.
			/// Asserts if there is an active object in the cache with that hash.
			/// If a preserved object has that hash, it is purged.
			/// \param size the size of the object.
			/// \param usage the number of times the object can be removed before it is preserved.
			///   If usage == 0, the object is immediately preserved.
		void addObject( KeyType key, hkReferencedObject* object, hkUint32 size, int usage = 1 );

			/// Reduces the usage count of an object. If the usage count hits 0 then the cache
			/// will take responsibility for the lifetime of the data.
		inline void removeObjectByKey( KeyType key );
		inline void removeObjectByReference( hkReferencedObject* object );

			/// Replace an active object by another of a possibly different size. It is an error
			/// to call this if there is no active object for the key. The equivalent effect for
			/// a preserved object is achieved using addObject.
			/// \param newSize the size of the new object, or -1 if it size should not be changed.
		void changeObject( KeyType key, hkReferencedObject* newObject, hkUint32 newSize = -1 );

			/// Change the size of an active object to the given size.
		void changeObjectSize( KeyType key, hkUint32 newSize );

			/// Release any references to preserved objects.
		void purge();

			/// Set the maximum total size of objects the cache can preserve.
			/// This can cause preserved objects to be purged from the cache.
		void setMaxSize( hkUint32 maxSize );

			/// Return the maximum total size of objects the cache can preserve.
		inline hkUint32 getMaxSize() const;

			/// Return the current total size of objects preserved by the cache.
		inline hkUint32 getCurrentSize() const;

	public:
			/// The information kept for each object in the database.
		struct CacheInfo 
		{
			KeyType m_key;
			hkReferencedObject* m_object;
			hkUint32 m_size;
		};

			/// Information about an object which is still in use.
		struct ActiveCacheInfo : CacheInfo
		{
			hkUint32 m_usageCount;
		};

			/// Information about an object which is being preserved by the cache.
		struct PreservedCacheInfo : CacheInfo
		{
		};

	public:
		// These methods support saving and loading cache data.

			/// Get up to maxSize (active and preserved) objects.
			/// When there isn't enough room for all of them, priority is given to active
			/// objects and then to newer preserved objects.
			/// No new references are added to the objects put into the array.
		void getAsPreserved( hkArray<PreservedCacheInfo>& arrayOut, hkUint32 maxSize );

			/// Get up to m_maxSize (active and preserved) objects.
		inline void getAsPreserved( hkArray<PreservedCacheInfo>& arrayOut );

			/// Add cache infos into this object cache, regarding them as preserved. The loaded
			/// objects are regarded as newer than any currently preserved objects, and so this
			/// operation may cause some preserved objects to be purged.
			/// \param arrayIn an array of those infos to be added in oldest-first order.
			///   (It is non-const, as the infos are added non-const to this object.)
		void addAsPreserved( hkArray<PreservedCacheInfo>& arrayIn );

	protected:

			/// The array of active objects.
		hkArray<struct ActiveCacheInfo> m_activeCacheInfo;

			/// The array of preserved objects.
		hkArray<struct PreservedCacheInfo> m_preservedCacheInfo;

			/// The cache will not preserve objects whose total size exceeds this amount.
		hkUint32 m_maxSize;

			/// The total size of the objects preserved by the cache.
		hkUint32 m_currentSize;

	protected:
			/// Returns 0 if not in either array,
			///	n >= 0 when its index in the active info array is ( n - 1 ),
			///	n <= 0 when its index in the preserved info array is ( -n - 1 ).
		virtual hkInt32 getIndexByKey( KeyType key ) const;
		virtual hkInt32 getIndexByReference( hkReferencedObject* object ) const;

			/// Remove the active object at the index.
			/// \param index the index which must be strictly positive.
		void removeActiveObjectByIndex( int index );

			/// Remove the preserved object at the index.
			/// \param index the index which must be strictly negative.
		void removePreservedObjectByIndex( int index );

			/// Purge enough objects to make room for the given size of objects.
		void purgeEnoughFor( hkUint32 size );	
};

#include <Common/Base/Container/ObjectCache/hkObjectCache.inl>

#endif // HK_OBJECT_CACHE_H

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
