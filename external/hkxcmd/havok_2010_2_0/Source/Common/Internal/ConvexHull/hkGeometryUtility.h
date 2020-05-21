/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_GEOMETRYUTILITY_H
#define HK_GEOMETRYUTILITY_H

#include <Common/Base/Types/Geometry/hkStridedVertices.h>
#include <Common/Base/Types/Geometry/hkGeometry.h>

class hkVector4;
class hkSphere;
class hkGeomHull;


/// Used to specify any modifications to the behaviour of the convex hull builder.
enum hkGeomConvexHullMode
{
	///
	HK_GEOM_CONVEXHULL_MODE_INVALID,
	/// Default: fast mode.
	HK_GEOM_CONVEXHULL_MODE_FAST,
	/// Should not be used when the algorithm is being called at runtime,
	/// and really only should be called offline by the hkpConvexPieceMeshBuilder
	/// utility, because it can be very slow and only offers accuracy improvements
	/// in a limited number of cases, such as when the input is nearly completely
	/// a 2d object with a large number of vertices spread over a large distance.
	HK_GEOM_CONVEXHULL_MODE_ACCURATE_BUT_SLOW,
	///
	HK_GEOM_CONVEXHULL_MODE_MAX_ID
};

/// Used to specify any modifications to the behaviour of the OBB builder.
enum hkGeomObbMode
{
	///
	HK_GEOM_OBB_MODE_INVALID,
	/// Default: normal mode. The mimics the calculations used prior to Havok 3.3, and
	/// is used in the filter pipeline.
	HK_GEOM_OBB_MODE_DEFAULT,
	/// Produces tighter-fitting bounding boxes, but may be considerably slower.
	/// The algorithm uses the normal result as a starting guess, and then iteratively
	/// improves the box to reduce the total volume.
	HK_GEOM_OBB_MODE_BETTER_FIT_BUT_SLOWER,
	///
	HK_GEOM_OBB_MODE_MAX_ID
};

///
/// Convex hull generator: Calculated the convex hull of a set of vertices in 3-space.
///
/// The convex hull algorithm is used in several places in our current product:
///  - Generation of plane equations for convex vertices shapes. These are used for ray-casting.
///  - Generation of visualisation code for display of convex vertices shapes (in the visual debugger)
///  - Generation of a hull for calculating an inertia tensor for convex vertices shapes.
///
class hkGeometryUtility
{
public:
		//
		// GEOMETRY CONVEXITY AND CREATION FUNCTIONS
		//

		/// Calculates the convex hull of the points in stridedVertsIn.
	static void HK_CALL createConvexGeometry( const hkStridedVertices& stridedVertsIn, hkGeometry& geometryOut);

		/// Calculates the convex hull of the points in stridedVertsIn and returns the vertices used, the plane equations generated and the geometry of hull.
	static void HK_CALL createConvexGeometry( const hkStridedVertices& stridedVertsIn, hkGeometry& geometryOut, hkArray<hkVector4>& planeEquationsOut);

		/// Calculates the vertices of the convex hull at the inner intersection of the specified plane equations. Only one hull is found, and
		/// more plane equations than the minimum necessary can be specified.
	static void HK_CALL createVerticesFromPlaneEquations(  const hkArray<hkVector4>& planeEquationsIn, hkArray<hkVector4>& vertsOut );

		/// Calculates the geometry of the convex hull whose vertices are the intersections of the planeEquationsIn.
	static void HK_CALL createGeometryFromPlaneEquations(  const hkArray<hkVector4>& planeEquationsIn, hkGeometry& geometryOut );


        // Calculate axis from covariance matrix
    static hkBool HK_CALL getAxesFromCovariance(hkMatrix3 &covArray, hkVector4 &axis0, hkVector4 &axis1, hkVector4 &axis2);

		//
		// OTHER UTILITY FUNCTIONS
		//

		/// A utility class for creating oriented bounding boxes from a point cloud.
		/// This is useful if you want to create primitives from arbitrary geometry
		/// The OBB utility returns a position for the centre of the box, a set of
		/// 3 axes representing the orientation of the box, and a set of half extents
		/// representing the scale of the box.
	static void HK_CALL calcObb( const hkStridedVertices& stridedVertsIn, hkVector4& halfExtents, hkTransform& transform, hkGeomObbMode mode = HK_GEOM_OBB_MODE_DEFAULT);

		/// This method duplicates the functionality of the deprecated hkGeometryUtil::minSphere() method.
		/// To avoid numerical roundoff error, a radiusEpsilon parameter must be passed. The default should
		/// be sufficient to prevent error.
		/// In certain (very rare) extreme cases this code may fail.
		///
		/// \param vertexArray A block of memory containing a number of hkVector4s, this is the cloud of points to bound
		/// \param vertexCount The number of hkVector4s in the block of memory pointed to by the parameter vertices
		/// \param radiusEpsilon To avoid numerical rounding error, the sphere calculated is increased in size by this value.
		///                      which should not need to be changed.
		///
	static hkSphere HK_CALL createMinBoundingSphere( const hkStridedVertices& stridedVertsIn, const hkReal radiusEpsilon = 1e-5f);

		/// Simple function to push out given planes by the desired distance. It just alters
		/// the w component (D) of each of the plane equations by the given amount (can be negative to shrink)
	static void HK_CALL expandPlanes(  hkArray<hkVector4>& planeEquations, hkReal byDistance );

};

#endif // HK_GEOMETRYUTILITY_H

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
