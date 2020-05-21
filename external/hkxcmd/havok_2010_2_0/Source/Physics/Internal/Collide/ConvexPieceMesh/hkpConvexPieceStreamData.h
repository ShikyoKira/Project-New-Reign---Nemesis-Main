/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_INTERNAL_CONVEXPIECEMESH_STREAM_H
#define HK_INTERNAL_CONVEXPIECEMESH_STREAM_H

#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Physics/Collide/Shape/hkpShape.h>

extern const hkClass hkpConvexPieceStreamDataClass;

///
///	The output from the hkpConvexPieceMeshBuilder is constructed
/// by converting the arrays representing the convex pieces into a
/// data stream, which allows faster access and has a smaller storage
/// capacity.
///
/// This structure can be used as input to a hkpConvexPieceMeshShape.
/// 
class hkpConvexPieceStreamData : public hkReferencedObject
{
	public:
		//+vtable(1)
		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

		/// A bitstream representing the triangles in each
		/// convex piece.
		hkArray< hkUint32 > m_convexPieceStream;

		/// The offsets in the bitstream to the start of each convex piece.
		hkArray<hkUint32> m_convexPieceOffsets;

		/// An array of convex pieces that are only one triangle.
		hkArray<hkpShapeKey> m_convexPieceSingleTriangles;

		hkpConvexPieceStreamData() {}

		hkpConvexPieceStreamData( hkFinishLoadedObjectFlag flag )
			:	hkReferencedObject(flag),
				m_convexPieceStream(flag),
				m_convexPieceOffsets(flag),
				m_convexPieceSingleTriangles(flag)
		{
		}
};


#endif // HK_INTERNAL_CONVEXPIECEMESH_STREAM_H

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
