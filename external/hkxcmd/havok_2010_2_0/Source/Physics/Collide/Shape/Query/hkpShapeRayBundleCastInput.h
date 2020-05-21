/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SHAPE_RAY_BUNDLE_CAST_INPUT
#define HK_SHAPE_RAY_BUNDLE_CAST_INPUT

#include <Common/Base/hkBase.h>

class hkpRayShapeCollectionFilter;


	/// This is the input structure you need to fill out in order to call any hkpShape::castRay() function
struct hkpShapeRayBundleCastInput
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpShapeRayBundleCastInput );

		/// The constructor initializes the filter pointer to HK_NULL, which means that no shape collection filtering
		/// will be performed by default.
	inline hkpShapeRayBundleCastInput();

		/// The start positions of 4 rays transposed
	hkVector4 m_from[3];

		/// The end positions of 4 rays transposed
	hkVector4 m_to[3];

		/// Filter information associated with this ray.
		/// You only need to set this if you supply a filter (by setting the m_rayShapeCollectionFilter) which
		/// accesses this value. For example, the supplied filter, hkpGroupFilter, uses this value, so if you use
		/// this the hkpGroupFilter as the m_rayShapeCollectionFilter you must set this value.
	hkUint32 m_filterInfo;

		/// This is the filter which will be used to decide whether to hit or not hit.
		/// Note: this variable will normally be set automatically by the engine if you
		/// call hkpWorld::castRay / hkpAabbPhantom::castRay
	const hkpRayShapeCollectionFilter*	m_rayShapeCollectionFilter;

};

void HK_CALL hkBundleRays( const hkpShapeRayCastInput rays[4], hkpShapeRayBundleCastInput& bundleOut );
void HK_CALL hkUnBundleRays(const hkpShapeRayBundleCastInput& bundleIn, hkpShapeRayCastInput rays[4]);

#include <Physics/Collide/Shape/Query/hkpShapeRayBundleCastInput.inl>

#endif // HK_SHAPE_RAY_BUNDLE_CAST_INPUT

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
