/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_WORLD_MEMORY_AVAILABLE_WATCH_DOG_H
#define HK_DYNAMICS2_WORLD_MEMORY_AVAILABLE_WATCH_DOG_H


extern const class hkClass hkWorldMemoryAvailableWatchDogClass;

class hkpWorld;
class hkStepInfo;
class hkpSimulationIsland;
class hkpEntity;
#include <Common/Base/Monitor/hkMonitorStream.h>

/// You should use this class to remove objects from your game, if memory is running low.
class hkWorldMemoryAvailableWatchDog : public hkReferencedObject
{
	public:

		// +version(1)
		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS );

			/// This struct is filled by hkpWorld::calcRequiredSolverBufferSize
		struct MemUsageInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,MemUsageInfo);
			int m_maxRuntimeBlockSize;							///< The biggest solver buffer the system needs. This size must be smaller than the buffer you give to hkSolverAllocator
			int m_sumRuntimeBlockSize;							///< The sum of all solver buffers. If this size is bigger than the buffer you give to hkSolverAllocator, you might get a small performance hit
			hkpSimulationIsland* m_largestSimulationIsland;		///< The island which requests the largest solver buffer
		};

		inline hkWorldMemoryAvailableWatchDog();
		inline hkWorldMemoryAvailableWatchDog( class hkFinishLoadedObjectFlag flag );

			/// Returns the minimum amount of free heap memory that is expected to be available before the start of a simulation step.
			/// (This is an extra memory buffer to hkFreeListAllocator::m_softLimit).
		virtual hkInt32 getAmountOfFreeHeapMemoryRequested() = 0;

			/// Free memory until: allocatedMemory < (hkpFreeListMemory::m_softLimit - m_freeHeapMemoryRequested)
			/// This function is called from many places in the engine to check the memory consumption.
			/// It is also called when the memory limit is exceeded. A typical implementation should remove
			/// unimportant rigid bodies from the world or apply more restrictive collision filtering
			/// in this function to reduce the memory consumption of the engine.
		virtual void freeHeapMemoryTillRequestedAmountIsAvailable( hkpWorld* world ) = 0;

			/// This is called from checkMemoryForIntegration. If there's not enough memory to perform solving and integration
			/// for a given island, the user is asked to lower their number e.g., by removing constraints or bodies.
			/// The MemUsageInfo contains the largest simulation island (which is the one requiring reduction) as well
			/// as the solver buffer size currently required. This can be used as a guide to how much memory must be reduced.
			/// numIterations counts the number of times this is called for a particular memory condition. The larger this
			/// number grows, the more aggressive reduction is required. A very large number probably indicates that the memory
			/// limitation has not been resolved
		virtual void reduceConstraintsInIsland(const MemUsageInfo& info, int numIterations ) = 0;
};

#include <Physics/Dynamics/World/Memory/hkpWorldMemoryAvailableWatchdog.inl>

#endif // HK_DYNAMICS2_WORLD_MEMORY_AVAILABLE_WATCH_DOG_H

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
