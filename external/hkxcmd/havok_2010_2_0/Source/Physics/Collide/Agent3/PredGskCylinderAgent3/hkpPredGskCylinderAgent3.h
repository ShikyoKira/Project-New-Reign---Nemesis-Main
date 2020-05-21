/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_PREDICTIVE_GSK_CYLINDER_AGENT3_H
#define HK_COLLIDE2_PREDICTIVE_GSK_CYLINDER_AGENT3_H

#include <Physics/Collide/Agent3/hkpAgent3.h>
#include <Physics/Collide/Shape/hkpShapeType.h>

class hkpCollisionDispatcher;
class hkpGskCache;

/// The pure purpose of hkPredGskCylinderAgent3 is to allow rolling of cylinders.
/// It only changes the functionality of hkPredGskAgent3 in its non-predictive, manifold-generating mode.
namespace hkPredGskCylinderAgent3
{
	hkpAgentData* HK_CALL create  ( const hkpAgent3Input& input, hkpAgentEntry* entry, hkpAgentData* freeMemory );

	hkpAgentData* HK_CALL process ( const hkpAgent3ProcessInput& input, hkpAgentEntry* entry, hkpAgentData* agentData, hkVector4* separatingNormal, hkpProcessCollisionOutput& result);

	void			HK_CALL registerAgent3( hkpCollisionDispatcher* dispatcher );
}



#endif // HK_COLLIDE2_PREDICTIVE_GSK_CYLINDER_AGENT3_H

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
