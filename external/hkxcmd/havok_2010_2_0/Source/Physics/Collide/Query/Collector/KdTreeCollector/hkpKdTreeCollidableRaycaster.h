/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KDTREE_GEOMETRY_RAYCASTER_H
#define HK_KDTREE_GEOMETRY_RAYCASTER_H

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Common/Internal/KdTree/QueryUtils/hkKdTreeCastCollector.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>


struct hkpWorldRayCastInput;
struct hkpWorldRayCastOutput;
class hkpRayCollidableFilter;

class hkpKdTreeCollidableRaycaster : public hkKdTreeCastCollector
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_COLLIDE, hkpKdTreeCollidableRaycaster);

	hkpKdTreeCollidableRaycaster() : m_input(HK_NULL), m_filter(HK_NULL), m_collectionFilter(HK_NULL), m_output(HK_NULL), m_collector(HK_NULL) {}

	hkBool castRayIterative( const hkKdTree* tree, const hkpWorldRayCastInput& input, hkpWorldRayCastOutput& output);

	virtual	hkBool addMidPhaseHit( hkPrimitiveId primId );
	virtual hkBool hasHit();
	virtual hkReal getHitFraction();

	const hkpWorldRayCastInput* m_input;
	const hkpRayCollidableFilter* m_filter;
	const hkpRayShapeCollectionFilter*	m_collectionFilter;

	hkpWorldRayCastOutput* m_output;
	hkpRayHitCollector* m_collector;
};

class hkKdTreeCollidableRayBundleCaster : public hkKdTreeBundleCastCollector
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_COLLIDE, hkKdTreeCollidableRayBundleCaster);

	hkKdTreeCollidableRayBundleCaster();
	virtual	void addMidPhaseHit( hkPrimitiveId primId, hkVector4ComparisonParameter mask  );
	virtual void getHitFractions( hkVector4& hitFractions) const;

	//virtual hkBool hasHit();

	hkpWorldRayCastInput* m_input;
	const hkpRayCollidableFilter* m_filter;
	const hkpRayShapeCollectionFilter*	m_collectionFilter;
	hkpWorldRayCastOutput* m_output;
};

#endif // HK_KDTREE_GEOMETRY_RAYCASTER_H

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
