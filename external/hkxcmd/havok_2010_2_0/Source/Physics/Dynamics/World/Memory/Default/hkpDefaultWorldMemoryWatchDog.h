/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



#ifndef HK_DYNAMICS2_DEFAULT_WORLD_MEMORY_WATCH_DOG_H
#define HK_DYNAMICS2_DEFAULT_WORLD_MEMORY_WATCH_DOG_H

#include <Physics/Dynamics/World/Memory/hkpWorldMemoryAvailableWatchDog.h>

extern const class hkClass hkpDefaultWorldMemoryWatchDogClass;

class hkpWorld;
class hkStepInfo;
class hkpSimulationIsland;

/// The purpose of this class is to remove objects from the physics to
/// free memory. To do this class simply iterates over all islands, finds the objects
/// with the highest autoRemoveLevel and removes those objects until
/// the memory system has 'm_freeHeapMemoryRequested' available.
class hkpDefaultWorldMemoryWatchDog : public hkWorldMemoryAvailableWatchDog 
{
	//+version(1)
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Constructor
			/// \param m_freeHeapMemoryRequested	The minimum amount of memory that has to be available.
			///										Entities will be released until the memory system claims to have that amount available.
		hkpDefaultWorldMemoryWatchDog( hkInt32 m_freeHeapMemoryRequested = 20000);

		hkpDefaultWorldMemoryWatchDog( class hkFinishLoadedObjectFlag flag ) : hkWorldMemoryAvailableWatchDog(flag) {}

			/// Returns the minimum amount of free memory expected (relative to hkpFreeListMemory::m_softLimit)
		virtual hkInt32 getAmountOfFreeHeapMemoryRequested();

		virtual void freeHeapMemoryTillRequestedAmountIsAvailable( hkpWorld* world );

		virtual void reduceConstraintsInIsland(const hkWorldMemoryAvailableWatchDog::MemUsageInfo& info, int numIterations);

	protected:
			/// Remove some entities from one island
		static void removeObjectsFromIsland( hkpSimulationIsland* island, int minAutoRemoveLevelToProcess, hkInt32 freeHeapMemoryRequested, int & maxFoundAutoRemoveLevelOut );

			// Once the used memory breaches this limit, the watchdog should start to delete hkWorldObjects
		hkInt32 m_freeHeapMemoryRequested;

};


#endif // HK_DYNAMICS2_DEFAULT_WORLD_MEMORY_WATCH_DOG_H

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
