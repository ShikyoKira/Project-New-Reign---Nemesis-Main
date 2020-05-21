/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_hkDelayedFreeAllocator_H
#define HKBASE_hkDelayedFreeAllocator_H

#include <Common/Base/System/StackTracer/hkStackTracer.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

	/// Scrubs free memory and delays real freeing.
	/// This class scrubs and puts freed memory into limbo to detect use-after-free errors.
	/// Not all allocations can be kept. There are limits on the number of blocks
	/// and the total size of all blocks.
class hkDelayedFreeAllocator : public hkMemoryAllocator
{
	public:
	//+hk.MemoryTracker(ignore=True)


			///
		struct Limits
		{
			//+hk.MemoryTracker(ignore=True)

			Limits()
				: m_randomSeed(0)
				, m_numAllocsKept(2048)
				, m_maxBytesKept(5*1024*1024)
				, m_blockTooBig(m_maxBytesKept/5)
			{
			}
				///
			int m_randomSeed;
				/// Max number of blocks kept
			int m_numAllocsKept;
				/// Max total size of all blocks
			int m_maxBytesKept;
				/// Immediately pass through very large blocks
			int m_blockTooBig;
		};

		HK_DECLARE_PLACEMENT_ALLOCATOR();

		hkDelayedFreeAllocator();

			///
		void init(hkMemoryAllocator* next, Limits* limits=HK_NULL);
			///
		void quit();

		virtual void* blockAlloc( int numBytes );
		virtual void blockFree( void* p, int numBytes );
		
			/// Immediately release cached free blocks.
		virtual void releaseDelayedFrees();

		hkBool32 isOk() const;

		virtual void getMemoryStatistics( MemoryStatistics& u );
		virtual int getAllocatedSize(const void* obj, int nbytes);

	protected:

		struct Alloc { void* p; int size; };

		hkMemoryAllocator* m_next;
		hkArrayBase<Alloc> m_kept;
		Limits m_limits;
		hkPseudoRandomGenerator m_prng;
		int m_curBytesKept;
		int m_inUse;
};

#endif // HKBASE_hkDelayedFreeAllocator_H

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
