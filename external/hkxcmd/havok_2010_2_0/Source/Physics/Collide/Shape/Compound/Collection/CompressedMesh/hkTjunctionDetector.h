/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TJUNCTION_DETECTOR_H
#define HK_TJUNCTION_DETECTOR_H

#include <Physics/Collide/Shape/Compound/Collection/hkpShapeCollection.h>
#include <Physics/Collide/Shape/Compound/Tree/hkpBvTreeShape.h>
#include <Common/Base/Types/Geometry/hkGeometry.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>


class hkTjunctionDetector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkTjunctionDetector);

			/// Proximity info enum
		enum ProximityInfoEnum
		{
			NEAR_NONE = 0,
			NEAR_EDGE0,
			NEAR_EDGE1,
			NEAR_EDGE2,
			NEAR_FACE
		};

			/// Proximity info struct
		struct ProximityInfo 
		{
				/// The neighboring vertex
			hkVector4 m_vertex;
			
				/// The index of the vertex
			int m_index;

				/// The key of the face (index in the triangle list)
			hkpShapeKey m_key;

				/// The type of proximity
			ProximityInfoEnum m_type;

				/// The corners of the triangle
			hkVector4 m_v0, m_v1, m_v2;
		};

			/// Detects the T-junctions and welded vertices for a given geometry
		static void HK_CALL detect( const hkGeometry& geometry, hkArray<ProximityInfo>& Tjunctions, hkArray<hkVector4>& weldedVertices, 
			hkReal junctionTolerance = 1e-4f, hkReal weldTolerance = 1e-3f );

			/// Decides whether a vertex is close to a triangle or not (within a given tolerance)
		static ProximityInfoEnum HK_CALL vertexCloseToTriangle( const hkVector4& vertex, const hkVector4& triA, 
			const hkVector4& triB, const hkVector4& triC, hkReal tolerance );

			/// Detects the T-junctions and welded vertices for a given simple mesh shape and a bounding volume tree wrapping it
		static void HK_CALL detect( hkpSimpleMeshShape* mesh, const hkpBvTreeShape* bvTree, hkArray<ProximityInfo>& Tjunctions, 
			hkArray<hkVector4>& weldedVertices, hkReal junctionTolerance = 1e-4f, hkReal weldTolerance = 1e-3f );

		static hkpSimpleMeshShape* HK_CALL createSimpleMeshFromGeometry( const hkGeometry& geometry );
};

#endif //HK_TJUNCTION_DETECTOR_H

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
