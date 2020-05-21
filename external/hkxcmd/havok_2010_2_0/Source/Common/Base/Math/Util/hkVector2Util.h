/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKMATH_VECTOR2UTIL_H
#define HKMATH_VECTOR2UTIL_H

class hkVector2;

namespace hkVector2Util
{
	hkBool HK_CALL edgesIntersect( const hkVector2& a1, const hkVector2& a2, const hkVector2& b1, const hkVector2& b2);
	void HK_CALL convexHullSimplePolyline( const hkArrayBase<hkVector2>& polyline, hkArray<int>& indicesOut );
	void HK_CALL convexHullVertices( const hkArrayBase<hkVector2>& points, hkArray<hkVector2>& hullOut );
	void HK_CALL convexHullIndices( const hkArrayBase<hkVector2>& points, hkArrayBase<int>& indicesOut );

	/// Helper to get the number of bytes allocated on stack for hull generation
	int  HK_CALL getStackSizeInBytesRequiredForConvexHullIndices(int inputPointsSize);
}

#endif //HKMATH_VECTOR2UTIL_H

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
