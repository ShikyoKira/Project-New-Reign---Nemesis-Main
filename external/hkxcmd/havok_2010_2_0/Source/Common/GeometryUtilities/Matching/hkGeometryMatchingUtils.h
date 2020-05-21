/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GEOMETRY_MATCHING_UTILS_H
#define HK_GEOMETRY_MATCHING_UTILS_H

#include <Common/Base/Types/Geometry/hkGeometry.h>

/// Utility class, contains methods that operate with hkGeometry objects.
class hkGeometryMatchingUtils
{
	public:		

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES, hkGeometryMatchingUtils);

		/// A simple geometry made of triangles
		struct Geometry
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES, hkGeometryMatchingUtils::Geometry);

			HK_FORCE_INLINE Geometry();

			HK_FORCE_INLINE Geometry( const hkGeometry* geometry );

			/// The vertices
			const hkVector4* m_vertices;

			// The vertex stride
			int m_vertexStride;

			/// The number of vertices
			int m_numVertices;

			/// All vertex indices of the triangles (3 per triangle)
			const int* m_triangleIndices;

			// The index buffer stride between triples of indices.
			int m_triangleIndexStride;

			/// The number of triangles
			int m_numTriangles;
		};


		//
		// Triangle Matching
		//

		/// structure holding the results of matchTriangles()
		/// Map between Search -> Reference
		struct TriangleMap
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES, hkGeometryMatchingUtils::TriangleMap);

			/// The best matched triangle identifier
			struct Hit
			{
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES, hkGeometryMatchingUtils::TriangleMap::Hit);

				hkUint16 m_geometryIndex; 	
				hkUint8 m_indexOffset;		
				int     m_triangleIndex;	
				bool m_flippedWinding;
			};

			/// Returns the best matched triangle for each search triangle
			HK_FORCE_INLINE const Hit& getBestMatchingTriangle( int searchGeometryIndex, int searchTriangleIndex) const;

			/// A start index into the m_foundReferenceTriangle array per geometry
			hkInplaceArray<hkUint32,16>   m_startIndexPerGeometry;

			/// An array of hits, one for each input triangle references the closest match
			hkInplaceArray<Hit,128> m_foundReferenceTriangle;
		};

		/// Try to match two set of triangles:
		/// for every search triangle you get an (m_geometryIndex,m_triangleIndex) pointing to the closest matched reference triangle
		/// (only if the vertex distance squared sum is <maxDistance^2, else m_triangleIndex = -1 is returned).
		static void HK_CALL matchTriangles( const hkArray<Geometry>& referenceTriangles, const hkArray<Geometry>& searchTriangles,  hkReal maxDistance, TriangleMap& triangleMapOut );


		//
		// Vertex Matching
		//

		/// Structure holding the results of matchVertices()
		struct FullMap
		{

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES, hkGeometryMatchingUtils::FullMap);

			TriangleMap m_triangleMap;

				/// The information returned by getBestMatchingVertex
			struct VertexHit
			{
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES, hkGeometryMatchingUtils::FullMap::VertexHit);

				VertexHit() : m_gotHit(false), m_geometryIndex(0xffff), m_triangleIndex(0xffffffff), m_trianglePos(0xff) {}

				bool m_gotHit;

					/// The index of the output (reference) geometry containing the associated vertex
				hkUint16 m_geometryIndex;
					
					/// The index of the output (reference) triangle containing the associated vertex
				hkUint32 m_triangleIndex;

					/// Either 0, 1 or 2 - the position in the triangle containing the associated vertex.
				hkUint8 m_trianglePos; 
			};

			HK_FORCE_INLINE void getBestMatchingVertex (int searchGeometryIndex, int searchVertexIndex, VertexHit& hitOut) const;


			// 
			// Internal use
			//

			struct VertexTriangleEntry
			{
				VertexTriangleEntry() : m_triangleIndex(-1) {}
				hkInt32 m_triangleIndex;
				hkUint8 m_trianglePos; // either 0,1 or 2
			};

			HK_FORCE_INLINE const VertexTriangleEntry& _getSearchTriangleForSearchVertex (int searchGeometryIndex, int searchVertexIndex) const;

			hkInplaceArray<hkUint32,16> m_startEntryPerGeometry;
			hkArray<VertexTriangleEntry> m_searchTrianglePerSearchVertex;
		};

		static void HK_CALL matchGeometries ( const hkArray<Geometry>& referenceGeometries, const hkArray<Geometry>& searchGeometries, hkReal maxDistance, FullMap& fullMapOut);
};

#include <Common/GeometryUtilities/Matching/hkGeometryMatchingUtils.inl>

#endif //HK_GEOMETRY_MATCHING_UTILS_H

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
