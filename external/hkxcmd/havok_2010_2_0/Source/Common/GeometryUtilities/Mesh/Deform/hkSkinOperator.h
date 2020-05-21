/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SKIN_OPERATOR_H
#define HK_SKIN_OPERATOR_H

/// A skin operator - it updates an output buffer based on an input reference buffer, a transform set, and the weight information stored in the operator.
class hkSkinOperator
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkSkinOperator );

	public:

		///	Encodes information about a skinned buffer
		struct Buffer
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkSkinOperator::Buffer );

			void* m_start;	// Buffer start pointer
			int m_stride;	// Buffer elements stride (vertex size)
		};

		
		/// An individual bone influence.
		struct BoneInfluence
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkSkinOperator::BoneInfluence );

			/// The index of the bone.
			hkUint8 m_boneIndex;

			/// The quantized weight of the bone influence.
			hkUint8 m_weight;
		};

		
		//	Encodes a component buffer, such as position, normal, etc
		struct Component
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkSkinOperator::Component );

			int m_bufferIndex;	///< Index of the buffer holding this component
			int m_offset;		///< Offset from the buffer start
		};

		//	Encodes a skinning data stream (input / output) as an array of buffers, each buffer having one or more strided components
		struct SkinStream
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkSkinOperator::SkinStream );

			Buffer m_buffers[4];				///< The buffers (at most 4)
			int m_numBuffers;					///< The number of valid buffers

			Component m_positions;				///< Positions
			Component m_normals;				///< Normals
			Component m_tangents;				///< Tangents
			Component m_biTangents;				///< BiTangents

			hkUint32 m_skinExecutionFlags;		///< Specifies which components will be skinned
			hkUint32 m_simdExecutionFlags;		///< Specifies which components are stored in SIMD friendly buffers
		};

		///	Encodes the necessary data for skinning
		struct Parameters
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkSkinOperator::Parameters );

			const hkMatrix4* m_compositeMatrices;
			const BoneInfluence* m_boneInfluences;
			const hkUint16* m_startInfluencePerVertex;


			SkinStream m_input;
			SkinStream m_output;

			int m_numVertices;			///< Size of m_startInfluencePerVertex will be m_numVertices + 1 due to optimizations
			int m_numBones;				///< Size of m_compositeMatrices
			int m_numBoneInfluences;	///< Number of bone influences in the m_boneInfluences array
			hkBool m_partialSkinning;

		};

		
		///	Possible values for execution / SIMD skinning flags
		enum ExecutionFlags
		{
			HK_SKIN_NONE			= 0x00,

			HK_TYPE_POSITIONS		= 0,
			HK_TYPE_NORMALS			= 1,
			HK_TYPE_TANGENTS		= 2,
			HK_TYPE_BITANGENTS		= 3,

			HK_SKIN_POSITIONS		= (1 << HK_TYPE_POSITIONS),
			HK_SKIN_NORMALS			= (1 << HK_TYPE_NORMALS),
			HK_SKIN_TANGENTS		= (1 << HK_TYPE_TANGENTS),
			HK_SKIN_BITANGENTS		= (1 << HK_TYPE_BITANGENTS),
			HK_SKIN_ALL				= (HK_SKIN_POSITIONS | HK_SKIN_NORMALS | HK_SKIN_TANGENTS | HK_SKIN_BITANGENTS),

			HK_USE_SIMD_FOR_POSITIONS_SHIFT		= 4,
			HK_USE_SIMD_FOR_NORMALS_SHIFT		= 5,
			HK_USE_SIMD_FOR_TANGENTS_SHIFT		= 6,
			HK_USE_SIMD_FOR_BITANGENTS_SHIFT	= 7,

			HK_USE_SIMD_FOR_POSITIONS	= (1 << HK_USE_SIMD_FOR_POSITIONS_SHIFT),
			HK_USE_SIMD_FOR_NORMALS		= (1 << HK_USE_SIMD_FOR_NORMALS_SHIFT),
			HK_USE_SIMD_FOR_TANGENTS	= (1 << HK_USE_SIMD_FOR_TANGENTS_SHIFT),
			HK_USE_SIMD_FOR_BITANGENTS	= (1 << HK_USE_SIMD_FOR_BITANGENTS_SHIFT),
			HK_USE_SIMD_FOR_ALL			= (HK_USE_SIMD_FOR_POSITIONS | HK_USE_SIMD_FOR_NORMALS | HK_USE_SIMD_FOR_TANGENTS | HK_USE_SIMD_FOR_BITANGENTS),
			HK_USE_SIMD_FOR_POSITIONS_AND_NORMALS	= (HK_USE_SIMD_FOR_POSITIONS | HK_USE_SIMD_FOR_NORMALS),
		};

	public:

		static void HK_CALL executeCpu (const Parameters& parameters);

		//
		// SPU Implementations
};

#endif	//	HK_SKIN_OPERATOR_H

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
