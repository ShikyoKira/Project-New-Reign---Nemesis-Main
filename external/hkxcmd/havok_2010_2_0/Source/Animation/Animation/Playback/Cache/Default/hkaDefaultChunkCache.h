/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULT_CHUNK_CACHE
#define HK_DEFAULT_CHUNK_CACHE

#include <Common/Base/hkBase.h>
#include <Animation/Animation/Playback/Cache/hkaChunkCache.h>

// allows cache usage statistics be generated
#if defined( HK_DEBUG )
#define HK_CACHE_STATS
#endif


/// Structure defining the layout of a pool.
struct hkaChunkPoolCinfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_CACHE, hkaChunkPoolCinfo );

	/// The number of buckets that a hashed key may end up being placed into ( prime numbers are preferred )
	hkUint32 m_buckets;

	/// The number of slots per bucket ( more slots => greater chance of finding data there but increased seek time )
	hkUint32 m_slots;

	/// The size ( in bytes ) of the data chunks pointed to by a bucket slot ( e.g. 64 byte chunks ). This MUST be a multiple of 16 for alignment purposes.
	hkUint32 m_chunkSize;

};


/// Construction info used to initialise the cache.
struct hkaDefaultChunkCacheCinfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_CACHE, hkaDefaultChunkCacheCinfo );

	/// The Number of cache pools to be managed.
	hkUint32 m_numberOfCachePools;

	/// The construction info for each cache pool ( pool chunk size MUST be in ascending order! ).
	hkArray<hkaChunkPoolCinfo> m_cachePools;

};

///
/// The default cache handles several pools, each of which represents data of differing chunk sizes.
/// Each pool is allocated as an n-way cache (where n is the number of buckets)
/// This implementation used a LRU scheme within the pool to discard and reallocate slots.
/// It also provide support for slot locking.
/// N.B. This cache is *not* thread-safe. A simple modification of this cache which is thread-safe is provided
/// in the class hkaMultithreadedChunkCache.
class hkaDefaultChunkCache : public hkaChunkCache
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Constructor.
		hkaDefaultChunkCache( struct hkaDefaultChunkCacheCinfo& cinfo );

			/// Destructor.
		~hkaDefaultChunkCache();

			/// Retrieves a read only pointer to a preallocated/cached chunk
			/// Returns HK_NULL if the key is not found in the cache
		virtual const hkUint8* retrieveChunk( hkUint32 key, hkUint32 chunkSize );

			/// Allocates a chunk from the cache and binds it to the given key
			/// Returns HK_NULL if it can't allocate.
			/// For this implementation this will happen if there is no pool big enough
			/// for the chunk or if the all the slots in the pool are locked.
			/// If it is not NULL
		virtual hkUint8* allocateChunk( hkUint32 key, hkUint32 chunkSize );

			/// Marks the chunk associated with the given key as available for reuse / reallocation
		virtual hkBool flushKey( hkUint32 key, hkUint32 chunkSize );

			/// Writes cache statistics to an output stream
		virtual hkBool printCacheStats( hkOstream* oStream ) const;

			/*
			 * Locking support
			 */

			/// (Deprecated) Locks a specific key ( will never be removed from the cache pool until it is unlocked )
		hkBool lockKeyNonMT( hkUint32 key, hkUint32 chunkSize );

			/// (Deprecated) Unlocks a specific key
		hkBool unlockKeyNonMT( hkUint32 key, hkUint32 chunkSize );


			/// Locks a specific key ( will never be removed from the cache pool until it is unlocked )
		virtual hkBool lockKeyForRead( hkUint32 key, hkUint32 chunkSize ) { return false; }

			/// Unlocks a specific key
		virtual hkBool unlockKeyForRead( hkUint32 key, hkUint32 chunkSize ) { return false; }

			/// Locks a specific key for writing (calls to retrieve are not allowed)
		virtual hkBool lockKeyForWrite( hkUint32 key, hkUint32 chunkSize ) { return false; }

			/// Unlocks a specific key for writing
		virtual hkBool unlockKeyForWrite( hkUint32 key, hkUint32 chunkSize ) { return false; }

			/// Query if key locked for write (use before calling retrieveChunk)
		virtual hkBool isKeyLockedForWrite( hkUint32 key, hkUint32 chunkSize ) { return false; }

			/// Critical section enter (no implementation for this class - use hkaMultithreadedDefaultChunkCache)
		virtual void enterCriticalSection( ) {}

			/// Critical section leave (no implement ion for this class - use hkaMultithreadedDefaultChunkCache)
		virtual void leaveCriticalSection( ) {}

			/// Flush the specified cache pool ( flushes ALL entries in the pool )
		hkBool flushCachePool( hkUint32 cachePool );

			// Results enum - UNUSED
		enum QueryResult
		{
			HK_ALL_SLOTS_LOCKED,
			HK_NO_POOL_AVAILABLE,
			HK_SLOT_AVAILABLE,
		};


	protected:

		struct slotUnit
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_CACHE, hkaDefaultChunkCache::slotUnit );

			// key
			hkUint32 m_key;

			// L.R.U. value and lock status ( < 0 )
			hkInt32 m_lru;

			slotUnit()
			{
				// initialise all
				m_key = HK_NULL;
			}

		};


		// small structure to fill in with hashed key info

		struct hashKeyInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_CACHE, hkaDefaultChunkCache::hashKeyInfo );

			// input

			// key to be hashed
			hkUint32 m_key;

			// chunk size we're dealing with
			hkUint32 m_chunkSize;


			// output

			// hashed key == bucket
			hkUint32 m_bucket;

			// the cache pool we're dealing with
			hkInt32 m_cachePool;

			hashKeyInfo()
			{
				// invalid cache pool
				m_cachePool = -1;
			}

		};

		// calculates the bucket into which the specified key will be hashed to and the correct cache pool
		inline hkBool hashKey( struct hashKeyInfo& info );


		// number of cache pools to be managed
		hkUint32 m_numberOfCachePools;

		// heap memory allocation for the cache pools data
		hkArray<hkUint8*> m_cacheData;

		// heap memory allocation for the cache pool tables
		hkArray<struct slotUnit*> m_cachePoolTables;

		// construction info for each cache pool ( buckets, slots and chunk size )
		hkArray<struct hkaChunkPoolCinfo> m_cachePools;

		//
		// the following arrays are only used when HK_CACHE_STATS is defined
		//


#if defined( HK_CACHE_STATS )

		// a small convience structure to help with analysis

		struct slotStatisticsInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_CACHE, hkaDefaultChunkCache::slotStatisticsInfo );

			// number of times this slot has been successfully read
			hkUint32 m_hits;

			// number of time this slot has been trashed
			hkUint32 m_misses;

			slotStatisticsInfo()
			{
				m_hits = 0;
				m_misses = 0;
			}

		};

		// heap memory allocation for the cache pool statistics tables ( mirrors actual slotUnit tables )
		hkArray<struct slotStatisticsInfo*> m_cachePoolStatsTables;

		// another small convience structure to help with analysis
		
		struct unavailableChunk
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_CACHE, hkaDefaultChunkCache::unavailableChunk );

			// requested chunk size
			hkUint32 m_chunkSize;

			// frequency this chunk has been requested
			hkUint32 m_frequency;

			unavailableChunk()
			{
				m_chunkSize = 0;
				m_frequency = 0;
			}

		};

		// a simple array to keep track of requests for unavailable chunk sizes ( pools )
		hkArray<struct unavailableChunk> m_unavailableChunks;

#endif

	private:

			// internal method used by (un)lock / retrieve / flushKey to manipulate locked status
		hkBool manipulateKey( hkUint32 key, hkUint32 chunkSize, hkUint8 operation, hkUint8** chunkPointer );


};

#include <Animation/Animation/Playback/Cache/Default/hkaDefaultChunkCache.inl>

/*

Cache pool layout : ( example uses a pool with 5 buckets each containing 3 slots )

................ cache pool ................
.  bucket 1 : | slot 1 | slot 2 | slot 3 | .
.  bucket 2 : | slot 1 | slot 2 | slot 3 | .
.  bucket 3 : | slot 1 | slot 2 | slot 3 | .
.  bucket 4 : | slot 1 | slot 2 | slot 3 | .
.  bucket 5 : | slot 1 | slot 2 | slot 3 | .
............................................

Each slot is a slotUnit structure which contains the key itself, information on the L.R.U. value of 
use of this key and locking information. A one-to-one mapping between this table and the cache data
exists and so a pointer to a memory location which is 'chunk' bytes in size where data is cached may be
calculated.

Each key that the cache receives is passed into a hashing function which determines which bucket in
the cache pool the key maps to. The cache may then search that bucket for the key and can retrieve
a pointer to the cached data.


Hashing function : ( very simple for now )

    h(k) = k mod m   ( m = bucket size )

*/

#endif // HK_DEFAULT_CHUNK_CACHE

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
