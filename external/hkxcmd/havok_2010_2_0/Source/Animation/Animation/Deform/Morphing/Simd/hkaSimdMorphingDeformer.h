/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SIMD_MORPHING_DEFORMER_H
#define HK_SIMD_MORPHING_DEFORMER_H

#include <Animation/Animation/Deform/Morphing/hkaMorphingDeformer.h>

/// The derived reference counted class for a SIMD based implementation of weighted vertex deformation.
/// This deformer requires that the input buffers' deformable
/// members (pos, normals) be aligned as a hkVector4 (16byte aligned as it is 128bit)
/// and for faster operation make sure that the outputs are aligned as well.
/// N.B. It is important to note that these deformers are here to be used by Havok's demos but are not production quality.
/// It is assumed that deforming will be done most commonly by your graphics engine, usually in hardware on GPUs or VUs.
/// That hardware deformation is usually performed at the same time as per vertex lighting operations, so Havok cannot
/// provide optimized deformers for all such game specific usage.
class hkaSimdMorphingDeformer : public hkReferencedObject, public hkaMorphingDeformer
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Initializes to an unbound deformer
		hkaSimdMorphingDeformer();

			/// Bind the buffers
			/// The output buffer should be preallocated.
			/// Returns false if the deformer does not support the input or output buffer format.
			/// The input and output buffers must be appropriately aligned (16 byte)
		hkBool bind( const hkaVertexDeformerInput& input, const hkxVertexBuffer* inputBuffer1, const hkxVertexBuffer* inputBuffer2,  hkxVertexBuffer* outputBuffer );

			/// Interpolate the input buffers into the output buffer.
			/// The deformer must first be bound and the output buffer locked before deforming.
		virtual void deform ( hkReal delta );

	private:

		void deformAligned( hkReal delta );
		void deformAlignedInput( hkReal delta );

		struct hkaSimdBinding
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaSimdMorphingDeformer::hkaSimdBinding );

			// Input buffer 1
			const hkVector4* m_i1PosBase;
			const hkVector4* m_i1NormBase;
			const hkVector4* m_i1BinormBase;
			const hkVector4* m_i1TangentBase;
			hkUint8 m_i1Stride;

			// Input Buffer 2
			const hkVector4* m_i2PosBase;
			const hkVector4* m_i2NormBase;
			const hkVector4* m_i2BinormBase;
			const hkVector4* m_i2TangentBase;
			hkUint8 m_i2Stride;

			// Output Buffer
			hkReal* m_oPosBase;
			hkReal* m_oNormBase;
			hkReal* m_oBinormBase;
			hkReal* m_oTangentBase;
			hkUint8 m_oStride;

			hkUint32 m_numVerts;
		};

		struct hkaSimdBinding m_binding;
		hkBool m_outputAligned;
};

#endif // HK_SIMD_MORPHING_DEFORMER_H

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
