/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MULTITHREADED_CHUNK_CACHE
#define HK_MULTITHREADED_CHUNK_CACHE

#include <Common/Base/hkBase.h>
#include <Animation/Animation/Playback/Cache/Default/hkaDefaultChunkCache.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>


/// This version of hkaDefaultChunkCache uses critical sections and locking to ensure MT-safety.
class hkaMultithreadedChunkCache : public hkaDefaultChunkCache
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Constructor.
		hkaMultithreadedChunkCache( struct hkaDefaultChunkCacheCinfo& cinfo );

			/// Destructor.
		~hkaMultithreadedChunkCache();

			/// Retrieves a read only pointer to a preallocated/cached chunk
			/// Returns HK_NULL if the key is not found in the cache
		virtual const hkUint8* retrieveChunk( hkUint32 key, hkUint32 chunkSize );

			/*
			 * Locking support
			 */

			/// Locks a specific key ( will never be removed from the cache pool until it is unlocked )
		virtual hkBool lockKeyForRead( hkUint32 key, hkUint32 chunkSize );

			/// Unlocks a specific key
		virtual hkBool unlockKeyForRead( hkUint32 key, hkUint32 chunkSize );

				/// Locks a specific key for writing (calls to retrieve are not allowed)
		virtual hkBool lockKeyForWrite( hkUint32 key, hkUint32 chunkSize );

			/// Unlocks a specific key for writing
		virtual hkBool unlockKeyForWrite( hkUint32 key, hkUint32 chunkSize );

			/// Query if key locked for write (use before calling retrieveChunk)
		virtual hkBool isKeyLockedForWrite( hkUint32 key, hkUint32 chunkSize );

			/// Critical section enter.
		virtual void enterCriticalSection( );

			/// Critical section leave.
		virtual void leaveCriticalSection( );


	private:

		// internal method used by (un)lock / retrieve / flushKey to manipulate locked status
		hkBool manipulateKeyMT( hkUint32 key, hkUint32 chunkSize, hkUint8 operation, hkUint8** chunkPointer );

		hkCriticalSection m_criticalSection;


};

#endif // HK_MULTITHREADED_CHUNK_CACHE

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
