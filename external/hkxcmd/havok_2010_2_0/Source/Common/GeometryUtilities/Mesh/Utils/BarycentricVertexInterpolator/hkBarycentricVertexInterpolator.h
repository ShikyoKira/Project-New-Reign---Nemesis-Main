/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BARYCENTRIC_VERTEX_INTERPOLATOR_H
#define HK_BARYCENTRIC_VERTEX_INTERPOLATOR_H

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>

class hkBarycentricVertexInterpolator
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DESTRUCTION, hkBarycentricVertexInterpolator );

            /// Ctor
        hkBarycentricVertexInterpolator();

            /// Call before anything is calculated. Returns
        void start(const hkMeshVertexBuffer::LockedVertices& srcLockedVertices, const hkMatrix4& objectToWorld, hkBool isInverted);

            /// Call when processing has finished
        void end();

            /// Set up a support - must be called before any vertex interpolations are calculated with calculateVertex
        void setSupport(int v0, int v1, int v2);

			/// Calculate a destination vertex from the vertex position
        void calculateVertex(const hkVector4& vertexPosition, const hkMeshVertexBuffer::LockedVertices& dstVertex);

            /// Calculate the vertex data from the barycentric coordinates. If vertexPosition is HK_NULL it will interpolated for the output
            /// else it will just be copied
        void calculateVertex(const hkVector4* vertexPosition, hkPadSpu<hkReal> lambda[3], const hkMeshVertexBuffer::LockedVertices& dstVertex);

            /// Copy a vertex taking into account transform, inversion etc
        void copyVertex(int vertexIndex, const hkMeshVertexBuffer::LockedVertices& dstVertex);

            /// Calculate barycentric coordinates
        static void HK_CALL calcBarycentricCoordinates(hkVector4Parameter pos, hkVector4Parameter t0, hkVector4Parameter t1, hkVector4Parameter t2, hkPadSpu<hkReal> result[3]);

    protected:
        hkBool m_isStarted;                                             ///< Set if calculator is started
		hkMeshVertexBuffer::LockedVertices m_srcLockedVertices;
        hkMeshVertexBuffer::LockedVertices::Buffer m_srcPositionBuffer;

        hkVector4 m_supportPositions[3];
        int m_supportIndices[3];

		hkMatrix4 m_objectToWorld;
        hkMatrix4 m_worldToObject;
		hkBool m_isInverted;
};

#endif // HK_BARYCENTRIC_VERTEX_INTERPOLATOR_H

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
