/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_CPU_CACHE_H
#define HKBASE_CPU_CACHE_H

//#define HK_CPU_CACHE_SIMULATION_MODE




/// This class serves several modes:
///   - In a final build, this class will give access to the low level cache control functions
///   - In a performance checking build, this class will allow for timing cache misses
///   - In a 'checking build' it will simulate the cache behavior internally
class hkCpuCache
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkCpuCache);
	struct Cinfo
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,Cinfo);
		void* m_memoryStart;
		int   m_memorySize;
		void* m_workingBuffer;

		int m_level1Size;
		int m_levle1LineSize;
		int m_level1Associativity;
		int m_level2Size;
		int m_levle2LineSize;
		int m_level2Associativity;
	};

	hkCpuCache();

	enum{ MAX_CHECKS = 32 };

	/// initializes itself
	void init( const Cinfo& info  );

	/// Activates the CPU cache simulator. Copies the whole memory into the working buffer and wipes the memory with '0xce'
	void activate(){}

	/// flushes the cache and copies the working buffer into the memory buffer
	void deactivate(){}

	/// prefetches 128 bytes.
	/// In check mode this will monitor the cache memory footprint
	void prefetch128(void* p ){ hkMath::prefetch128(p); }

	/// prefetches 128 bytes and leaves the data uninitialized (depending on the platform)
	/// In check mode this will monitor the cache memory footprint
	void blankCache128( void *p ){ 
		HK_ASSERT( 0xf043dfed, ( hkUlong(p) & 0x7f)==0);
		hkMath::prefetch128(p);
	}

	/// Releases a cache line
	/// In check mode this will monitor the cache memory footprint
	void release( void* p ){ 
	}

	/// Time how long it will take to bring \a data in.
	/// Note that for performance reason, only stochastic sampling will be used.
	/// My id must be unique in this frame and between [0..MAX_CHECKS[
	HK_FORCE_INLINE void check( const char* name, int myId, const void* data );

	/// print the statistics
	void printStats();

	void flushCache();

	void startFrame();

	volatile int m_dummy;
	int m_checkTicks[MAX_CHECKS];
	int m_checkCount[MAX_CHECKS];
	const char* m_names[MAX_CHECKS];

	int m_frameIterations;
	int m_currentId;
	int m_checkId;
};



inline void hkCpuCache::check( const char* name, int myId, const void* data )
{
	if ( m_currentId == m_checkId )
	{

#if defined(HK_ARCH_IA32) && !defined(HK_COMPILER_GCC)
/*		// prefetch does not work on intel architectures
				hkUint32 time0;
				hkUint32 time1;
				__asm {
					rdtsc
					mov dword ptr[time0], eax
				}
				register int x = *(int*)data;
				x = x + x;
				__asm {
					push ebx
					cpuid
					pop ebx
					rdtsc
					mov dword ptr[time1], eax
				}
				m_checkTicks[myId] += int(time1-time0);
				m_checkCount[myId] ++;
				m_names[myId] = name;
				m_dummy = x;*/
		

#elif defined(HK_PLATFORM_XBOX360)
		unsigned __int64 time0 = __mftb();
		register int x = *(int*)data;
		x = x + x;
		unsigned __int64 time1 = __mftb();

		m_checkTicks[myId] += int(time1-time0);
		m_checkCount[myId] ++;
		m_names[myId] = name;
		m_dummy = x;
#endif
	}
	m_currentId = (m_currentId+1)& 63;
}


#endif // HKBASE_CPU_CACHE_H

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
