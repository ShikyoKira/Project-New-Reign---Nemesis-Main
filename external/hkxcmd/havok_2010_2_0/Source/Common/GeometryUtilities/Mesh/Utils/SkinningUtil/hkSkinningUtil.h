/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SKINNING_UTIL_H
#define HK_SKINNING_UTIL_H

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>
#include <Common/GeometryUtilities/Mesh/hkMeshSystem.h>

/// Utility functions for skin meshes
class hkSkinningUtil
{
    public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkSkinningUtil);

        struct Entry
        {
            hkReal m_distanceSquared;   ///< The distance squared
            int m_index;                ///< If -1 then not used
        };

			/// Works out the range of matrices indexed in a vertex buffer
        static hkResult HK_CALL findMatrixIndexRange(hkMeshVertexBuffer* buffer, int& minIndexOut, int& maxIndexOut);

            /// Find the matrix index range for a shape
        static hkResult HK_CALL findMatrixIndexRange(hkMeshShape* meshShape, int& minIndexOut, int& maxIndexOut);

			/// Works out the amount of bone indices are needed for a vertex format
		static int HK_CALL findNumBoneIndices(const hkVertexFormat& vertexFormat);

			/// Work out the distance based on spheres from the bones centers
        static void HK_CALL findDistances(const hkArray<hkVector4>& vertices, const hkArray<hkVector4>& boneCenter, int numDistances, hkReal maxDistance, hkArray<Entry>& entriesOut);

            /// Uses the position data in the vertex buffer as well as the bones info, to set up weights and bone indices
        static hkResult HK_CALL calculateBoneIndicesAndWeights(const hkArray<hkVector4>& boneCenters, hkMeshVertexBuffer* vertexBuffer, hkReal maxDistance);

			/// Remove the center of each of the bones (the pos part of the bones matrix)
		static void HK_CALL extractBoneCenters(const hkArray<hkMatrix4>& bones, hkArray<hkVector4>& boneCenters);

			/// Create a vertex buffer, which has the skinning information (still empty, use calculateBoneIndicesAndWeights() to set weights)
        static hkMeshVertexBuffer* HK_CALL createSkinnedVertexBuffer(hkMeshSystem* meshSystem, hkMeshVertexBuffer* srcVertexBuffer, int numWeights);

			/// If the srcVertexBuffer is already skinned it is returned with the ref count increased, else a new vertex buffer will be constructed with the srcVertexBuffers data
			/// and skinning elements (NOTE this elements will hold garbage, and need to be filled in for example using setSkinningValues)
		static hkMeshVertexBuffer* HK_CALL ensureSkinnedVertexBuffer(hkMeshSystem* meshSystem, hkMeshVertexBuffer* srcVertexBuffer, int numWeights);

            /// Creates a shape with skinned vertex buffers. If they are already skinned does nothing to that buffer. If all are skinned, will return shape, ref counted.
        static hkMeshShape* HK_CALL createEmptySkinnedShape(hkMeshSystem* meshSystem, const hkMeshShape* shape,  int numWeights);

            /// Create a skinned shape
        static hkResult HK_CALL setSkinningValues(hkMeshShape* shape, const hkArray<hkVector4>& boneCenters, hkReal maxDistances);

			/// Set the skinning values
		static hkResult HK_CALL setSkinningValues(const hkArray<Entry>& entries, hkMeshVertexBuffer* vertexBuffer, hkReal maxDistance);

			/// Returns true if the format is a skinning format - i.e., has bones indices and weights of sub usage 0
		static hkBool HK_CALL isSkinnedVertexFormat(const hkVertexFormat& vertexFormat);


			/// Returns true if all of the vertex buffers associated with the shape have skinning information
		static hkBool HK_CALL isSkinnedShape(hkMeshShape* meshShape);
};

#endif // HK_SKINNING_UTIL_H

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
