/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SIMD_SKINNING_DEFORMER_H
#define HK_SIMD_SKINNING_DEFORMER_H

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>

/// Mesh deformation
///
/// The derived reference counted class for a SIMD based implementation of weighted vertex deformation.
/// Applies to both indexed and non indexed skinning.
/// This deformer requires that the input buffers' deformable
/// members (position, normals) be aligned as a hkVector4 (16byte aligned as it is 128bit)
/// and for faster operation make sure that the outputs are aligned as well.
/// N.B. It is important to note that these deformers are here to be used by Havok's demos but are not production quality.
/// It is assumed that deforming will be done most commonly by your graphics engine, usually in hardware on GPUs or VUs.
/// That hardware deformation is usually performed at the same time as per vertex lighting operations, so Havok cannot
/// provide optimized deformers for all such game specific usage.
class hkSimdSkinningDeformer : public hkReferencedObject
{
	public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

        struct Binding
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkSimdSkinningDeformer::Binding );

            hkMeshVertexBuffer::LockedVertices m_input;
            hkMeshVertexBuffer::LockedVertices m_output;

            const hkUint8* m_weights;
            int m_weightsStride;

            const hkUint8* m_transformIndices;
            int m_transformIndicesStride;

            int m_numWeights;

			hkUint32 m_numVerts;
			hkBool m_normalizeDirections;			
		};

			/// Only Input is 16 byte aligned, requires internal loop copy to unaligned float output buffers so slower
        static void HK_CALL deform( const hkMatrix4* worldCompositeMatrices, const Binding& binding );
};

#endif // HKA_SIMD_SKINNING_DEFORMER_H

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
