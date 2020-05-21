/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HK_SEMAPHORE_H
#define HKBASE_HK_SEMAPHORE_H

#include <Common/Base/Config/hkConfigThread.h>


#if (defined(HK_PLATFORM_MACPPC) || defined(HK_PLATFORM_MAC386) || defined(HK_PLATFORM_MACARM) || defined(HK_PLATFORM_UNIX) || defined(HK_PLATFORM_LRBXENSIM) ) && (HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED)
#	include <pthread.h>
#endif

	/// A wrapper class for a semaphore.
	/// Semaphores are about 10 times slower than critical sections, but
	/// they are more flexible.
	/// You can think of a Semaphore as a set of tokens.
	/// A call to acquire() grabs a token and release puts a token back.
	/// If acquire() can not get a token, it simply waits until another thread calls release.
	/// If the number of tokens is maxCount, release will do nothing.
class hkSemaphore
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkSemaphore);


			/// Create a semaphore with an initial count and a maximum count
		hkSemaphore( int initialCount = 0, int maxCount = 1000 );

		hkSemaphore( hkFinishLoadedObjectFlag f) {}

			/// Destruct the Semaphore
		~hkSemaphore();

			/// This call will block until the semaphore is released.
		void acquire();

			/// Release the semaphore. Releases a thread blocked by acquire().
		void release(int count = 1);


			// These static functions work on both simulated spu and cpu.
		static void HK_CALL acquire(hkSemaphore* semaphoreOnPpu);
		static void HK_CALL release(hkSemaphore* semaphoreOnPpu, int count = 1);
			
	protected:

#if (defined(HK_PLATFORM_MACPPC) || defined(HK_PLATFORM_MAC386) || defined(HK_PLATFORM_MACARM) || defined(HK_PLATFORM_UNIX) || defined(HK_PLATFORM_LRB)) && (HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED)

		//Mutex used to simulate the Semaphore
		struct hkSemaphorePosix
		{
			int				curCount;
			int				maxCount;
			pthread_mutex_t	mutex;
			pthread_cond_t	cond;
		} m_semaphore;

#elif defined(HK_PLATFORM_SIM) // spu simulator
	public:		void* m_semaphore;
#else
	protected:	void* m_semaphore;
#endif

};

#endif // HKBASE_HK_SEMAPHORE_H

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
