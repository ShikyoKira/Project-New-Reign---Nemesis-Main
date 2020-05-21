/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_PREDICTIVE_GSK_AGENT3_H
#define HK_COLLIDE2_PREDICTIVE_GSK_AGENT3_H

#include <Physics/Collide/Agent3/hkpAgent3.h>
#include <Physics/Collide/Shape/hkpShapeType.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>

#include <Physics/Internal/Collide/Gjk/hkpGskCache.h>

class hkpCollisionDispatcher;
class hkpGskCache;

namespace hkPredGskAgent3
{
	hkpAgentData*	HK_CALL create			( const hkpAgent3Input& input, hkpAgentEntry* entry, hkpAgentData* freeMemory );

	hkpAgentData*	HK_CALL process			( const hkpAgent3ProcessInput& input, hkpAgentEntry* entry, hkpAgentData* agentData, hkVector4* separatingNormal, hkpProcessCollisionOutput& result);

	void            HK_CALL sepNormal		( const hkpAgent3Input& input, hkpAgentEntry* entry, hkpAgentData* agentData, hkVector4& separatingNormalOut );

	hkpAgentData*	HK_CALL cleanup			( hkpAgentEntry* entry, hkpAgentData* agentData, hkpContactMgr* mgr, hkCollisionConstraintOwner& constraintOwner );

	void            HK_CALL removePoint		( hkpAgentEntry* entry, hkpAgentData* agentData, hkContactPointId idToRemove );

	void            HK_CALL commitPotential	( hkpAgentEntry* entry, hkpAgentData* agentData, hkContactPointId idToRemove );

	void			HK_CALL createZombie	( hkpAgentEntry* entry, hkpAgentData* agentData, hkContactPointId idToConvert );

	void            HK_CALL destroy			( hkpAgentEntry* entry, hkpAgentData* agentData, hkpContactMgr* mgr, hkCollisionConstraintOwner& constraintOwner, hkpCollisionDispatcher* dispatcher );

	void			HK_CALL registerAgent3	( hkpCollisionDispatcher* dispatcher, hkpShapeType typeA = HK_SHAPE_CONVEX, hkpShapeType typeB = HK_SHAPE_CONVEX );

	void			HK_CALL HK_INIT_FUNCTION( initAgentFunc )( hkpCollisionDispatcher::Agent3Funcs& f );

	//
	// Helper functions
	//

	HK_FORCE_INLINE int HK_CALL getGskFlag(const hkpAgentData* agentData, hkpGskCache::GskFlagValues mask) { return reinterpret_cast<const hkpGskCache*>(agentData)->m_gskFlags & mask; }
	HK_FORCE_INLINE void HK_CALL setGskFlagToTrue(hkpAgentData* agentData, hkpGskCache::GskFlagValues mask) { reinterpret_cast<hkpGskCache*>(agentData)->m_gskFlags |= mask; }
	HK_FORCE_INLINE void HK_CALL setGskFlagToFalse(hkpAgentData* agentData, hkpGskCache::GskFlagValues mask) { reinterpret_cast<hkpGskCache*>(agentData)->m_gskFlags &= ~mask; }

}



#endif // HK_COLLIDE2_PREDICTIVE_GSK_AGENT3_H

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
