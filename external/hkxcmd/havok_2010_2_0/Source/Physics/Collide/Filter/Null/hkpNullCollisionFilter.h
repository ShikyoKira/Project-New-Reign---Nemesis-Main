/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_NULL_COLLISION_FILTER_H
#define HK_COLLIDE2_NULL_COLLISION_FILTER_H

#include <Physics/Collide/Filter/hkpCollisionFilter.h>

extern const hkClass hkpNullCollisionFilterClass;

	/// This class disables all collision filtering.
class hkpNullCollisionFilter : public hkpCollisionFilter
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Construct an hkpNullCollisionFilter.
		hkpNullCollisionFilter() { m_type = HK_FILTER_NULL; }

			///
		~hkpNullCollisionFilter();

			/// Checks two collidables
		virtual hkBool isCollisionEnabled( const hkpCollidable& a, const hkpCollidable& b ) const { return true; }

			// hkpShapeCollectionFilter interface forwarding
		virtual	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const HK_SHAPE_CONTAINER& bContainer, hkpShapeKey bKey  ) const { return true; }

			// hkpShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const { return true; }

			// hkpRayShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& bShape, const HK_SHAPE_CONTAINER& bContainer, hkpShapeKey bKey ) const { return true; }

			// hkpRayCollidableFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& a, const hkpCollidable& collidableB ) const { return true; }

	public:

		hkpNullCollisionFilter( class hkFinishLoadedObjectFlag flag ) : hkpCollisionFilter(flag)
		{
			if( flag.m_finishing )
			{
				m_type = HK_FILTER_NULL;
			}
		}
};

#endif // HK_COLLIDE2_COLLISION_FILTER_H

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
