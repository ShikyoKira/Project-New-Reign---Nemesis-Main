/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONVEX_VERTICES_CONNECTIVITY_UTIL_H
#define HK_CONVEX_VERTICES_CONNECTIVITY_UTIL_H

#include <Physics/Collide/Shape/Convex/hkpConvexShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesConnectivity.h>
#include <Common/Base/Types/Geometry/hkGeometry.h>
#include <Common/Base/Types/Geometry/hkStridedVertices.h>

class hkpConvexVerticesShape;

/// This utility class is used to work out connectivity from a hkpConvexVerticesShape and store the results in
/// a hkpConvexVerticesConnectivity
/// Since it may not be possible to find valid connectivity the methods will return if they successfully found
/// connectivity.
/// The connectivity is used to find the inertia tensor and to cut the shape using a plane.
class hkpConvexVerticesConnectivityUtil
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpConvexVerticesConnectivityUtil);
            /// Will traverse the shape hierarchy and if any hkpConvexVerticesShape types are found it will set
            /// connectivity
        static hkResult HK_CALL ensureConnectivityAll(const hkpShape* shape);

            /// Checks if there is connectivity information. If not it will create a shape with connectivity
            /// May return the input shape reference counted
        static hkResult HK_CALL ensureConnectivity(const hkpConvexVerticesShape* shape);

            /// Find the connectivity from the convex shape
            /// Finding the connectivity may be a fairly slow process - as will need to reconstruct the convex
            /// hull.
        static hkpConvexVerticesConnectivity* HK_CALL findConnectivity(const hkpConvexVerticesShape* shape);

            /// Cuts the \a shape with the \a plane. The part of the shape that is on the positive side of the plane is kept.
            /// Returns HK_NULL if nothing was produced or if the volume of the produced shape is below \a minVolume.
            /// May return shape reference counted (and with connectivity set) if the plane does not cut the shape.
        static const hkpConvexVerticesShape* HK_CALL cut(const hkpConvexVerticesShape* shape, const hkVector4& plane, hkReal convexRadius, hkReal minVolume);

            /// Creates geometry from a hkpConvexVerticesShape and connectivity information
        static hkGeometry* HK_CALL createGeometry(const hkpConvexVerticesShape* shape,const hkpConvexVerticesConnectivity* conn);

			/// Creates geometry from a hkpConvexVerticesShape and connectivity information, and a transform for the vertices
		static hkGeometry* HK_CALL createGeometry(const hkpConvexVerticesShape* shape, const hkpConvexVerticesConnectivity* con, const hkTransform& transform);

            /// Quickly calculates the volume of the shape - shape requires the connectivity information
        static hkReal HK_CALL calculateVolume(const hkpConvexVerticesShape* shape,const hkpConvexVerticesConnectivity* conn);

            /// Quickly calculates the volume specified in the connectivity with the specified vertices
        static hkReal HK_CALL calculateVolume(const hkArrayBase<hkVector4>& vertices,const hkpConvexVerticesConnectivity* conn);


    public:


        struct Edge
        {
            hkInt32 m_start;        // If -1 means not set
            hkInt32 m_end;
        };
            /// For each face we create a linked list of face edges
            /// we then remove all of the edges which there is the opposite pair - this should leave
            /// only the outside (presumably convex) edges that make up the face
        struct FaceEdge
        {
        	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpConvexVerticesConnectivityUtil::FaceEdge);
            int m_startIndex;
            int m_endIndex;
            FaceEdge* m_next;
        };
        struct VertexInfo
        {
                /// Index to the new vertex
            hkUint32 m_newIndex;
                /// 1 if outside, 0 otherwise
            hkUint32 m_outside;
                /// The distance from the plane
            hkReal m_side;
        };


    private:
            // disable
        hkpConvexVerticesConnectivityUtil();
};

#endif // HK_CONVEX_VERTICES_CONNECTIVITY_UTIL_H

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
