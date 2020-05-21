/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_hkSimpleMemorySystem_H
#define HKBASE_hkSimpleMemorySystem_H

#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Memory/Allocator/Solver/hkSolverAllocator.h>

	/// Sample memory system which forwards all requests to a single base allocator.
	/// This implementation won't perform well because of contention and has no
	/// provisions for dealing with fragmentation.
class hkSimpleMemorySystem : public hkMemorySystem
{
	//+hk.MemoryTracker(ignore=True)
public:
			/// Set of allocators to use
			/// Set m_solverAllocator to NULL to use the default hkSolverAllocator
			/// which allocates blocks from m_heapAllocator
		struct MemoryAllocators
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,MemoryAllocators);
			MemoryAllocators(hkMemoryAllocator *tempAllocator = HK_NULL, hkMemoryAllocator *heapAllocator = HK_NULL, hkMemoryAllocator *debugAllocator = HK_NULL,
											hkMemoryAllocator *solverAllocator = HK_NULL);
	
			hkMemoryAllocator* m_tempAllocator;
			hkMemoryAllocator* m_heapAllocator;
			hkMemoryAllocator* m_debugAllocator;
			hkMemoryAllocator* m_solverAllocator;
		};

			///
		hkSimpleMemorySystem();

		virtual hkMemoryRouter* mainInit(const FrameInfo& info, Flags f=FLAG_ALL);
		virtual hkResult mainQuit(Flags f=FLAG_ALL);
		virtual void threadInit(hkMemoryRouter& r, const char* name, Flags f=FLAG_ALL);
		virtual void threadQuit(hkMemoryRouter& r, Flags f=FLAG_ALL);

			/// Not implemented
		virtual hkResult walkMemory(MemoryWalkCallback callback, void* param) { return HK_FAILURE; }

			/// Print statistics for the allocators.
		virtual void printStatistics(hkOstream& ostr);
			/// Get statistics for the heap.
		virtual void getHeapStatistics(hkMemoryAllocator::MemoryStatistics& stats);

			/// Set the base allocator. The solver allocator will use the default hkSolverAllocator.
			/// Note that memory statistics will be wrong because all allocation types will forward
			/// to this single allocator.
		void setAllocator(hkMemoryAllocator* a);

			/// This allows settings different allocators for different allocation types
			/// If a solver allocator is set, the user is responsible for providing it with a buffer
			/// m_solverAllocator = NULL will use the default solver allocator with a buffer from the heap.
		void setAllocators(MemoryAllocators& allocators);
			/// Get all allocators
		MemoryAllocators& getAllocators();

		virtual hkMemoryAllocator* getUncachedLockedHeapAllocator();

	protected:

		FrameInfo m_frameInfo;
		hkMemoryRouter m_mainRouter;
		hkSolverAllocator m_solverAllocator;
		MemoryAllocators m_allocators;
};

#endif // HKBASE_hkSimpleMemorySystem_H

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
