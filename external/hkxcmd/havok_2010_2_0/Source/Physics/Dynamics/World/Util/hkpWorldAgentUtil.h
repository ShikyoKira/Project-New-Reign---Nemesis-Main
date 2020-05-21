/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_WORLD_AGENT_UTIL_H
#define HK_DYNAMICS2_WORLD_AGENT_UTIL_H

#include <Physics/Dynamics/World/hkpWorldCinfo.h>

class hkCollidablePair;
class hkpCollidable;
struct hkpCollisionInput;
struct hkpAgentNnTrack;
struct hkpAgent1nTrack;
struct hkpAgentNnEntry;
class hkpLinkedCollidable;
struct hkpProcessCollisionInput;

/// Class hkpWorldAgentUtil
class hkpWorldAgentUtil
{
	public:
			/// ###ACCESS_CHECKS###( [theIsland->m_world,HK_ACCESS_IGNORE] [theIsland,HK_ACCESS_RW] );
		static hkpAgentNnEntry* HK_CALL addAgent( hkpLinkedCollidable* collA, hkpLinkedCollidable* collB, const hkpProcessCollisionInput& input );

			/// ###ACCESS_CHECKS###( [theIsland->m_world,HK_ACCESS_IGNORE] [theIsland,HK_ACCESS_RW] );
		static void HK_CALL removeAgent ( hkpAgentNnEntry* agent );

		static void HK_CALL removeAgentAndItsToiEvents ( hkpAgentNnEntry* agent );

		static hkpSimulationIsland* HK_CALL getIslandFromAgentEntry( hkpAgentNnEntry* entry, hkpSimulationIsland* candidateA, hkpSimulationIsland* candidateB);

			/// ###ACCESS_CHECKS###( [entity->getWorld(),HK_ACCESS_RW] );
		static void HK_CALL updateEntityShapeCollectionFilter( hkpEntity* entity, hkpCollisionInput& collisionInput );

		static void HK_CALL invalidateTim( hkpEntity* entity, const hkpCollisionInput& collisionInput );

			/// ###ACCESS_CHECKS###( [island->m_world,HK_ACCESS_RO] [island,HK_ACCESS_RW] );
		static void HK_CALL warpTime( hkpSimulationIsland* island, hkTime oldTime, hkTime newTime, const hkpCollisionInput& collisionInput );
};


#endif // HK_DYNAMICS2_WORLD_AGENT_UTIL_H

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
