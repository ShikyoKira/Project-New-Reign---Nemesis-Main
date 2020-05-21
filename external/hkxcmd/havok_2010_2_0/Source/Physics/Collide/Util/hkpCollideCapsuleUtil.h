/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_COLLIDE_CAPSULE_UTIL_H
#define HK_COLLIDE2_COLLIDE_CAPSULE_UTIL_H

#include <Physics/Collide/Util/hkpCollideTriangleUtil.h>
#include <Common/Base/Algorithm/Collide/LineSegment/hkLineSegmentUtil.h>


/// This utility handles collisions between hkCapsules and hkCapsules.
extern "C"
{
	/// return the three closest points: point0: closest, point1: endPointA, point2 = endpointB
	/// expects the points.m_distance to be initialized with the collision.m_tolerance
	/// The triangle and capsule points are expected to be in the same space (e.g., world space)
	/// In all cases the points[0]. separating plane will be correctly set (not the position)
	int HK_CALL hkCollideCapsuleUtilManifoldCapsVsCaps( const  hkVector4* capsA, hkReal capsARadius, const hkVector4* capsB, hkReal capsBRadius,	hkContactPoint* pointsOut );

	hkResult HK_CALL hkCollideCapsuleUtilClostestPointCapsVsCaps( const  hkVector4* capsA, hkReal capsARadius, const hkVector4* capsB, hkReal capsBRadius,	hkReal collisionTolerance, hkContactPoint& pointOut);

	/// returns a maximum of three closest point (only if searchManifold is set to true)
	/// the shortest point is always one of the first two points.
	/// \param featureOutput if non-null, it should point to an array of three FeatureOutputs to which feature information about the collision will be written.
	void hkCollideCapsuleUtilCapsVsTri( const  hkVector4* capsAPoints, hkReal capsARadius, const hkVector4* triVertices, hkReal triBRadius,
		const hkpCollideTriangleUtil::PointTriangleDistanceCache& cache, hkReal collisionTolerance, int searchManifold, hkContactPoint* points , hkpFeatureOutput* featuresOut = HK_NULL );

}

#endif // HK_COLLIDE2_COLLIDE_CAPSULE_UTIL_H

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
