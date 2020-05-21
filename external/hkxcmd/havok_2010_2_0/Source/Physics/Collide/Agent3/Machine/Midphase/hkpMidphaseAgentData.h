/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MIDPHASE_AGENT_DATA_H
#define HK_MIDPHASE_AGENT_DATA_H

#include <Physics/Collide/Agent3/Machine/1n/hkpAgent1nTrack.h>

struct hkpMidphaseAgentData
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpMidphaseAgentData );

		/// The number of shape keys in the shape key track for this agent.
		/// (This is the number of actual shape keys, regardless of whether the agent regards them
		/// as shape key pairs).
		/// A negative integer implies an error during midphase.
	hkInt32 m_numShapeKeys;

		/// The narrowphase agent data held by a midphase agent.
	hkpAgent1nTrack m_agent1nTrack;
};

#endif // HK_MIDPHASE_AGENT_DATA_H

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
