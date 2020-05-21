/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CAPSULE_TRIANGLE_AGENT3_H
#define HK_COLLIDE2_CAPSULE_TRIANGLE_AGENT3_H

#include <Physics/Collide/Agent3/hkpAgent3.h>

#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>
#include <Physics/Collide/Util/hkpCollideTriangleUtil.h>

struct hkpCapsuleTriangleCache3
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpCapsuleTriangleCache3 );

	hkContactPointId m_contactPointId[3];
	hkpCollideTriangleUtil::PointTriangleDistanceCache m_triangleCache;
};


class hkpCollisionDispatcher;

	/// a streamed agent implementing capsule triangle collisions.
	/// This agent is a discrete agent only
namespace hkCapsuleTriangleAgent3
{
	hkpAgentData* HK_CALL create  ( const hkpAgent3Input& input, hkpAgentEntry* entry, hkpAgentData* freeMemory );

	hkpAgentData* HK_CALL process ( const hkpAgent3ProcessInput& input, hkpAgentEntry* entry, hkpAgentData* agentData, hkVector4* separatingNormal, hkpProcessCollisionOutput& result);

	void HK_CALL sepNormal( const hkpAgent3Input& input, hkpAgentEntry* entry, hkpAgentData* agentData, hkVector4& separatingNormalOut );

	hkpAgentData* HK_CALL cleanup ( hkpAgentEntry* entry, hkpAgentData* agentData, hkpContactMgr* mgr, hkCollisionConstraintOwner& constraintOwner );

	void HK_CALL removePoint ( hkpAgentEntry* entry, hkpAgentData* agentData, hkContactPointId idToRemove );

	void HK_CALL commitPotential ( hkpAgentEntry* entry, hkpAgentData* agentData, hkContactPointId idToRemove );

	void HK_CALL createZombie( hkpAgentEntry* entry, hkpAgentData* agentData, hkContactPointId idToConvert );

	void HK_CALL destroy ( hkpAgentEntry* entry, hkpAgentData* agentData, hkpContactMgr* mgr, hkCollisionConstraintOwner& constraintOwner, hkpCollisionDispatcher* dispatcher );

	void HK_CALL registerAgent3( hkpCollisionDispatcher* dispatcher );

}

#endif // HK_COLLIDE2_CAPSULE_TRIANGLE_AGENT3_H

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
