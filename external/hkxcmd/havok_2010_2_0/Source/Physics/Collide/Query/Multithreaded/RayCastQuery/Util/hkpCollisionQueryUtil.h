/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE_COLLISION_QUERY_UTIL_H
#define HK_COLLIDE_COLLISION_QUERY_UTIL_H


#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>


class hkpShapeRayCastJobUtil
{
	public:
		// Returns a pointer to the next 'free' hkpShapeRayCastOutput.
		// This can either be:
		// - the next truly free entry (if there's room left in the array)
		// - the furthest hit in the array (if the array is already full and we need to drop entries)
		// - HK_NULL if the array is full and the current hit is further away than the furthest hit in the entry
		HK_FORCE_INLINE static hkpWorldRayCastOutputPpu* getNextFreeResult(hkpShapeRayCastCommand* command, hkpWorldRayCastOutputPpu* resultsArray, HK_PAD_ON_SPU(hkpWorldRayCastOutputPpu*)& nextFreeResult, hkReal hitFraction);
};


class hkpWorldRayCastJobUtil
{
	public:
		// Returns a pointer to the next 'free' hkpWorldRayCastOutput.
		// This can either be:
		// - the next truly free entry (if there's room left in the array)
		// - the furthest hit in the array (if the array is already full and we need to drop entries)
		// - HK_NULL if the array is full and the current hit is further away than the furthest hit in the entry
		HK_FORCE_INLINE static hkpWorldRayCastOutputPpu* getNextFreeResult(hkpWorldRayCastCommand* command, hkpWorldRayCastOutputPpu* resultsArray, HK_PAD_ON_SPU(hkpWorldRayCastOutputPpu*)& nextFreeResult, hkReal hitFraction);
};

template <typename COMMAND, typename OUTPUT>
class hkpRayCastJobUtil
{
	public:
		// Returns a pointer to the next 'free' OUTPUT.
		// This can either be:
		// - the next truly free entry (if there's room left in the array)
		// - the furthest hit in the array (if the array is already full and we need to drop entries)
		// - HK_NULL if the array is full and the current hit is further away than the furthest hit in the entry
		HK_FORCE_INLINE static OUTPUT* getNextFreeResult(COMMAND* command, OUTPUT* resultsArray, HK_PAD_ON_SPU(OUTPUT*)& nextFreeResult, hkReal hitFraction, hkReal& maxHitFractionOut);
};

#include <Physics/Collide/Query/Multithreaded/RayCastQuery/Util/hkpCollisionQueryUtil.inl>


#endif // HK_COLLIDE_COLLISION_QUERY_UTIL_H

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
