/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SOLVER_ALLOCATOR_H
#define HK_SOLVER_ALLOCATOR_H

#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

	/// Allocator to handle large allocations required by the Havok Physics Solver.
	/// It manages a single input block of memory, and as required by the
	/// Havok simulation brakes this block into smaller blocks as required.
	/// Havok physics can calculate the required block size before
	/// calling hkpWorld::stepDeltaTime() by calling hkpWorld::getMemUsageForIntegration().
	/// Based on this there are two different out of memory situations:
	///     - The largest island needs a solver buffer which is bigger than the supplied one.
	///       In this case either the solver buffer has to be increased or rigid bodies
	///       removed from the simulation island. This can be done automatically by
	///       using an implementation of the hkpMemoryWatchDog.
	///     - If the largest island requires a buffer smaller than
	///       the supplied buffer, but another simulation island has already
	///       allocated away big part of the full buffer, the physics scheduler will
	///       reschedule this big island to the end of the simulation step when
	///       all the other islands have freed their solver buffer. In this case
	///       you will experience a small extra CPU hit.
	///     - If you are stepping 2 Havok world simultaneously it could happen that on world
	///       allocates a big buffer so that the other world cannot continue simulating.
	///       In this the stalled world will spin on the job queue until the big buffer becomes
	///       available. In this case you could experience a pretty decent CPU spike (several 100%)
class hkSolverAllocator : public hkMemoryAllocator
{
	public:

		HK_DECLARE_PLACEMENT_ALLOCATOR();

            /// Ctor
		hkSolverAllocator();

		void setBuffer( void* buffer, int bufferSize );

            /// Dtor
        ~hkSolverAllocator();

		virtual void* blockAlloc( int numBytes );
		virtual void blockFree( void* p, int numBytes );
		virtual void* bufAlloc( int& reqNumInOut );
		virtual void bufFree( void* p, int num );

		bool canAllocSingleBlock( int numBytes );

		struct Element
		{
			HK_DECLARE_POD_TYPE();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkSolverAllocator::Element);
			char* getEnd(){ return m_start+m_size; }
			char* m_start;
			int m_size;
		};

		int getBufferSize() { return int(m_bufferEnd-m_bufferStart); }

		virtual void getMemoryStatistics( hkMemoryAllocator::MemoryStatistics& u );
		virtual int getAllocatedSize(const void* obj, int nbytes){ return nbytes; }

		virtual void resetPeakMemoryStatistics();

	public:

		char* m_bufferStart;
		char* m_bufferEnd;
		char* m_currentEnd;
		int   m_numAllocatedBlocks;
		hk_size_t m_peakUse;

		hkArrayBase<Element> m_freeElems;
		Element m_elemsBuf[64]; // max 64 simultaneous allocations and threads
		hkCriticalSection m_criticalSection;

	private:

		void* allocate(int& numBytesIn, bool useExactSize);
};

#endif // HK_SOLVER_ALLOCATOR_H

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
