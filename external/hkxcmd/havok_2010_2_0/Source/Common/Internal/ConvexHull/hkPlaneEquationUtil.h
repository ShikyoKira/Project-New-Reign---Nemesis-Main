/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PLANEEQUATION_UTIL
#define HK_PLANEEQUATION_UTIL

#include <Common/Base/hkBase.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>


///
/// The function getConvexVerticesFromPlaneEquations calculates the set of vertices at the
/// intersection of a set of plane equations.
///
class hkPlaneEquationUtil
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkPlaneEquationUtil);
		struct IntersectionPoint
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_GEOMETRY, hkPlaneEquationUtil::IntersectionPoint );

			void sort()
			{
				if ( m_planeA > m_planeB ) hkAlgorithm::swap( m_planeA, m_planeB );
				//if ( m_planeB > m_planeC ) hkAlgorithm::swap( m_planeB, m_planeC );
				//if ( m_planeA > m_planeB ) hkAlgorithm::swap( m_planeA, m_planeB );
			}

			hkVector4 m_location;

			int m_planeA;
			int m_planeB;
			int m_planeC;
		};

		struct VisitedEdge
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_GEOMETRY, hkPlaneEquationUtil::VisitedEdge );

			hkBool operator==( const VisitedEdge& other )
			{
				return ( (( m_planeA == other.m_planeA ) &&
						 ( m_planeB == other.m_planeB ) ) ||
						 (( m_planeB == other.m_planeA  ) &&
						 ( m_planeA == other.m_planeB )) );
			}
			int m_planeA;
			int m_planeB;
			IntersectionPoint* m_fromPoint;
			hkVector4 m_intersectionLineDir;
		};

		struct hkpPlaneEqnIndexPair
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_GEOMETRY, hkPlaneEquationUtil::hkpPlaneEqnIndexPair );

			hkBool operator==( const hkpPlaneEqnIndexPair& other)
			{
				return ( (m_first == other.m_first &&
					m_second == other.m_second ) );
			}
			int m_first;
			int m_second;
		};

	public:
			/// The entry function to the algorithm.
			/// If the planeEquations wrap a convex object, then this function finds the vertices of that convex object, and
			/// returns success. The algorithm used is one that walks around the intersection lines of the plane equations.
		static hkResult HK_CALL getConvexVerticesFromPlaneEquations( const hkArray<hkVector4>& planeEquations, hkArray<hkVector4>& vertexCloudOut );

	public:

			/// Finds a point on the inner space (on the actual convex hull)
			/// to start the walk through the planar intersections.
		static hkResult HK_CALL getStartingPoint( const hkArray<hkVector4>& planeEquations, 
										int& nearlyParallelPlaneLower,
										int& nearlyParallelPlaneUpper,
										IntersectionPoint& lowerPoint, 
										IntersectionPoint& upperPoint );

			/// Given three plane equations, this function calculates the point at their intersection.
		static hkResult HK_CALL findTriPlaneIntersectionPoint( const hkArray<hkVector4>& planeEquations, int m_planeAIndex, int planeBIndex, int planeCIndex, IntersectionPoint& intersectionPointOut);

			/// Returns two points on the convex hull that are on the line of intersection between the two input planes.
		static hkResult HK_CALL findPlanarIntersections( const hkVector4& intersectionLineDir, const hkArray<hkVector4>& planeEquations, int m_planeAIndex, int planeBIndex, IntersectionPoint& upperPointOut, IntersectionPoint& lowerPointOut );

			/// Determines if there is another plane that can be intersected with, when travelling along the intersectionLineDirection (which is the intersection of the two input planes).
			/// There may be several planes that intersect at the same point - these are filled into the array.
		static hkBool HK_CALL isNextIntersectionPoint( const hkVector4& intersectionLineDir, const hkArray<hkVector4>& planeEquations, int planeAIndex, int planeBIndex, hkVector4& vertex, hkArray< IntersectionPoint >& pointsOut );

			/// Calculates the intersection line between plane A and plane B, pointing in the direction away from plane C.
		static void HK_CALL calculateIntersectionLine(const hkArray<hkVector4>& planeEquations, int m_planeAIndex, int planeBIndex, int planeCIndex, hkVector4& intersectionLineDir);

			/// The guts of the algorithm - this performs a flood-fill over the intersection lines of the planeEquations.
			/// This function is called recursively and finishes when it cannot find any more new visitedEdges.
		static void HK_CALL walkAlongEdge( VisitedEdge& v, const hkArray<hkVector4>& planeEquations, hkArray<VisitedEdge>& visitedEdges, hkArray<hkVector4>& vertexCloudOut );
	private:
		
		/// Former hkGeomConvexHullBuilder member, duplicated here to remove coupling
		static hkBool HK_FORCE_INLINE vectorLessAndMergeCoordinates( hkVector4& v1, hkVector4& v2 )
		{
			if ( v1(0) < v2(0) )												return true;
			else if ( (v1(0) == v2(0)) && (v1(1) < v2(1)) )						return true;
			else if ( (v1(0) == v2(0)) && (v1(1) == v2(1)) && (v1(2) < v2(2)) )	return true;
			else																return false;
		}
		/// Former hkGeomConvexHullBuilder member, duplicated here to remove coupling
		static void HK_CALL weldXsortedVertices( hkReal weldTolerance, hkArray<hkVector4>& verts, int& numVertices );
};


#endif	// HK_PLANEEQUATION_UTIL

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
