/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS_WORLD_MEMORY_UTIL_H
#define HK_DYNAMICS_WORLD_MEMORY_UTIL_H

#include <Physics/Dynamics/World/hkpWorld.h>

/// Class hkpWorldMemoryUtil
class hkpWorldMemoryUtil
{
	public:
			/// This checks the amount of free heap memory available in Havok memory system.
			/// If the memory is less than amount required by the hkWorldMemoryAvailableWatchDog,
			/// it calls the watchdog's freeHeapMemoryTillRequestedAmountIsAvailable() function to remove objects from the hkpWorld
			/// until the available memory requirement is met.
		static void HK_CALL watchHeapMemory( hkpWorld* world );

			/// This ensures enough memory is available for the integration step.
			/// The amount of memory needed for integration is know ahead, so we can remove
			/// objects as needed.
		static void HK_CALL checkMemoryForIntegration( hkpWorld* world );

			/// This attempts to remove objects from physics and run collision detection again
			/// to recover from failure in the last step.
		static void HK_CALL tryToRecoverFromMemoryErrors( hkpWorld* world );

	private:
		
		static void HK_CALL repeatCollideAndToiHandling( hkpWorld* world );

};



#endif // HK_DYNAMICS_WORLD_MEMORY_UTIL_H

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
