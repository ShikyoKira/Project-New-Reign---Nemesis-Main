/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_TRIANGLE_COMPRESSOR_H
#define HK_TRIANGLE_COMPRESSOR_H

#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>

/// A class which compresses arrays of triangles
/// It assumes the triangles are spatially local.
class hkpTriangleCompressor
{
	public:
		static int HK_CALL getCompressedSize( const hkpTriangleShape* triangleShapes, int numTriangles, const hkUint8* materialIndices );

		static void HK_CALL compress( const hkpTriangleShape* triangleShapes, int numTriangles, const hkUint8* materialIndices, void* data );

		static HK_FORCE_INLINE void HK_CALL getTriangleShape( hkpTriangleShape& triangleShape, int index, const void* data );

		static HK_FORCE_INLINE hkUint8 getMaterialIndex(int triangleIndex, const void* data );

		static HK_FORCE_INLINE void setWeldingInfo(int triangleIndex, void* data, hkUint16 weldingInfo);

};

#include <Physics/Collide/Util/hkpTriangleCompressor.inl>

#endif // HK_TRIANGLE_COMPRESSOR_H

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
