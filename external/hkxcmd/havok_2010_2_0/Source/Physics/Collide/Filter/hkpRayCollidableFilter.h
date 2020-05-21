/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_RAY_COLLIDABLE_FILTER_H
#define HK_COLLIDE2_RAY_COLLIDABLE_FILTER_H

struct hkpWorldRayCastInput;
class hkpCollidable;

extern const hkClass hkpRayCollidableFilterClass;

	/// This filter filters between ray casts and collidables.
	/// It is called when ray-casting through the broadphase, or ray casting against a number of hkpCollidable objects.
	/// For ray against hkpShapeCollection child shape the hkpRayShapeCollectionFilter is used instead.
class hkpRayCollidableFilter
{
	//+hk.MemoryTracker(ignore=True)
	public:
		HK_DECLARE_REFLECTION();

		virtual ~hkpRayCollidableFilter() { }

			/// Return true if the ray should hit the collidable, false if it should miss.
		virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& a, const hkpCollidable& collidableB ) const = 0;

};

#endif //HK_COLLIDE2_RAY_COLLIDABLE_FILTER_H

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
