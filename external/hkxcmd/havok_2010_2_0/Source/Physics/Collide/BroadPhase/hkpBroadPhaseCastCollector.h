/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_BROAD_PHASE_CAST_COLLECTOR_H
#define HK_COLLIDE2_BROAD_PHASE_CAST_COLLECTOR_H

class hkpBroadPhaseHandle;

/// This class is a callback, which is called by the hkpBroadPhase::castRay() and hkpBroadPhase::castAabb(),
/// Typical implementations are hkpWorldLinearCaster or hkpWorldRayCaster
class hkpBroadPhaseCastCollector
{
	//+hk.MemoryTracker(ignore=True)
	public:

		virtual ~hkpBroadPhaseCastCollector() { }

			/// Return fraction (meaning the primitive hist the collidable)
			/// also update the fraction between 0 and 1 where it hits
			/// The castIndex is the index of the object. Typically if you have done a single
			/// raycast, castIndex is 0, if you do a multiple raycast, castIndex is the ray index
		virtual	hkReal addBroadPhaseHandle( const hkpBroadPhaseHandle* broadphaseHandle, int castIndex ) = 0;
};

#endif // HK_COLLIDE2_BROAD_PHASE_CAST_COLLECTOR_H

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
