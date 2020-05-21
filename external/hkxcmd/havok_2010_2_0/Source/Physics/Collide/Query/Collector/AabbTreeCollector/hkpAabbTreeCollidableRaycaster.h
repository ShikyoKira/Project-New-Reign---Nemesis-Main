/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_AABBTREE_COLLIDABLE_RAYCASTER_H
#define HK_AABBTREE_COLLIDABLE_RAYCASTER_H

#include <Common/Internal/Collide/Tree/AabbTree/QueryUtils/hkAabbTreeQueryUtils.h>

struct hkpWorldRayCastInput;
struct hkpWorldRayCastOutput;
class hkpRayCollidableFilter;
class hkpRayShapeCollectionFilter;


class hkpAabbTreeCollidableRaycaster : public hkAabbTreeQueryUtils<hkAabbTreeData::hkAabbTreeAabb, hkUint16>::Collector,
	public hkAabbTreeQueryUtils<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>::Collector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_COLLIDE, hkpAabbTreeCollidableRaycaster);

		hkpAabbTreeCollidableRaycaster();

		virtual hkReal addLeaf( const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb, hkUint16>::Node* leaf );
		virtual hkReal addLeaf( const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>::Node* leaf );


	private:
		void testCollidable(const hkpCollidable* col);

	public:

		const hkpWorldRayCastInput* m_input;
		const hkpRayCollidableFilter* m_filter;
		const hkpRayShapeCollectionFilter*	m_collectionFilter;

		hkpWorldRayCastOutput* m_output;
		hkpRayHitCollector* m_collector;
		hkReal m_earlyOutHitFraction;
};



#endif

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
