/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Common/Base/Math/Vector/hkIntVector.h>

inline void loadUCharToVector4(const hkUint8* c, hkVector4& v)
{ 
	v.setAll( hkReal(*c) ); 
}

template <	class PositionInput,	class PositionOutput,
			class NormalInput,		class NormalOutput,
			class TangentInput,		class TangentOutput,
			class BiTangentInput,	class BiTangentOutput	>
void _skinGeneric (const hkSkinOperator::Parameters& parameters)
{
	const hkMatrix4* compositeMatrices = parameters.m_compositeMatrices;
	const hkSkinOperator::BoneInfluence* boneInfluences = parameters.m_boneInfluences;
	const hkUint16* startInfluencePerVertex = parameters.m_startInfluencePerVertex;
	const hkUint32 numVertices = parameters.m_numVertices;

	hkMath::forcePrefetch<512>(compositeMatrices);

	const hkUint16* currentStartInfluence = startInfluencePerVertex;

	const hkUint16 firstInfluence = startInfluencePerVertex [0];
	const hkSkinOperator::BoneInfluence* currentInfluence = boneInfluences + firstInfluence;
	hkMath::forcePrefetch<512>(currentInfluence);

	hkVector4 oneOver255Vec; oneOver255Vec.setAll(3.921568627e-03f);

	const hkSkinOperator::SkinStream& inputStream = parameters.m_input;
	const hkSkinOperator::SkinStream& outputStream = parameters.m_output;

	

	const void* HK_RESTRICT positionsIn		= hkAddByteOffsetConst(inputStream.m_buffers[inputStream.m_positions.m_bufferIndex].m_start, inputStream.m_positions.m_offset);
	void*		HK_RESTRICT positionsOut	= hkAddByteOffset(outputStream.m_buffers[outputStream.m_positions.m_bufferIndex].m_start, outputStream.m_positions.m_offset);
	const hkUint32 positionsInStride		= inputStream.m_buffers[inputStream.m_positions.m_bufferIndex].m_stride;
	const hkUint32 positionsOutStride		= outputStream.m_buffers[outputStream.m_positions.m_bufferIndex].m_stride;
	
	const void* HK_RESTRICT normalsIn	= hkAddByteOffsetConst(inputStream.m_buffers[inputStream.m_normals.m_bufferIndex].m_start, inputStream.m_normals.m_offset);
	void*		HK_RESTRICT normalsOut	= hkAddByteOffset(outputStream.m_buffers[outputStream.m_normals.m_bufferIndex].m_start, outputStream.m_normals.m_offset);
	const hkUint32 normalsInStride		= inputStream.m_buffers[inputStream.m_normals.m_bufferIndex].m_stride;
	const hkUint32 normalsOutStride		= outputStream.m_buffers[outputStream.m_normals.m_bufferIndex].m_stride;

	const void* HK_RESTRICT tangentsIn	= hkAddByteOffsetConst(inputStream.m_buffers[inputStream.m_tangents.m_bufferIndex].m_start, inputStream.m_tangents.m_offset);
	void*		HK_RESTRICT tangentsOut = hkAddByteOffset(outputStream.m_buffers[outputStream.m_tangents.m_bufferIndex].m_start, outputStream.m_tangents.m_offset);
	const hkUint32 tangentsInStride		= inputStream.m_buffers[inputStream.m_tangents.m_bufferIndex].m_stride;
	const hkUint32 tangentsOutStride	= outputStream.m_buffers[outputStream.m_tangents.m_bufferIndex].m_stride;

	const void* HK_RESTRICT biTangentsIn	= hkAddByteOffsetConst(inputStream.m_buffers[inputStream.m_biTangents.m_bufferIndex].m_start, inputStream.m_biTangents.m_offset);
	void*		HK_RESTRICT biTangentsOut	= hkAddByteOffset(outputStream.m_buffers[outputStream.m_biTangents.m_bufferIndex].m_start, outputStream.m_biTangents.m_offset);
	const hkUint32 biTangentsInStride		= inputStream.m_buffers[inputStream.m_biTangents.m_bufferIndex].m_stride;
	const hkUint32 biTangentsOutStride		= outputStream.m_buffers[outputStream.m_biTangents.m_bufferIndex].m_stride;

	HK_ASSERT2(0x37e0e7cd, (!positionsIn) || (positionsIn && (positionsIn != positionsOut)), "Input and output positions buffers of skin operator cannot be identical!");
	HK_ASSERT2(0x37e0e7cd, (!normalsIn) || (normalsIn && (normalsIn != normalsOut)), "Input and output normals buffers of skin operator cannot be identical!");
	HK_ASSERT2(0x37e0e7cd, (!tangentsIn) || (tangentsIn && (tangentsIn != tangentsOut)), "Input and output tangents buffers of skin operator cannot be identical!");
	HK_ASSERT2(0x37e0e7cd, (!biTangentsIn) || (biTangentsIn && (biTangentsIn != biTangentsOut)), "Input and output bitangents buffers of skin operator cannot be identical!");


	for (hkUint32 i=0; i<numVertices; i++)
	{
		PositionInput::prefetchNext (positionsIn, positionsInStride * 4);
		NormalInput::prefetchNext (normalsIn, normalsInStride * 4);
		TangentInput::prefetchNext (tangentsIn, tangentsInStride * 4);
		BiTangentInput::prefetchNext (biTangentsIn, biTangentsInStride * 4);
		PositionOutput::prefetchNext (positionsOut, positionsOutStride * 4);
		NormalOutput::prefetchNext (normalsOut, normalsOutStride * 4);
		TangentOutput::prefetchNext (tangentsOut, tangentsOutStride * 4);
		BiTangentOutput::prefetchNext (biTangentsOut, biTangentsOutStride * 4);

		const int startEntry = *currentStartInfluence;
		const int endEntry = *(currentStartInfluence+1);
		const int numEntries = endEntry-startEntry;

		// If no triangles influence this vertex, ignore it
		if (numEntries>0)
		{
			hkVector4 originalPosition, originalNormal, originalTangent, originalBiTangent;
			PositionInput::getCurrentVector (positionsIn, originalPosition);
			NormalInput::getCurrentVector (normalsIn, originalNormal);
			TangentInput::getCurrentVector (tangentsIn, originalTangent);
			BiTangentInput::getCurrentVector (biTangentsIn, originalBiTangent);

			hkVector4 blendedPosition, blendedNormal, blendedTangent, blendedBiTangent;
			PositionOutput::initBlend (blendedPosition);
			NormalOutput::initBlend (blendedNormal);
			TangentOutput::initBlend (blendedTangent);
			BiTangentOutput::initBlend (blendedBiTangent);

			int inf=0;
			do
			{
				hkVector4 transformedPosition, transformedNormal, transformedTangent, transformedBiTangent;

				const hkUint8& weightInt = currentInfluence->m_weight;

				const hkMatrix4 boneTransform = compositeMatrices[currentInfluence->m_boneIndex];

				hkVector4 weightVec;
				loadUCharToVector4(&weightInt, weightVec);

				PositionOutput::transformPosition (originalPosition, boneTransform, transformedPosition);
				NormalOutput::transformDirection (originalNormal, boneTransform, transformedNormal);

				weightVec.mul4(oneOver255Vec);

				TangentOutput::transformDirection (originalTangent, boneTransform, transformedTangent);
				BiTangentOutput::transformDirection (originalBiTangent, boneTransform, transformedBiTangent);

				PositionOutput::blendTransformed (blendedPosition, weightVec, transformedPosition);
				NormalOutput::blendTransformed (blendedNormal, weightVec, transformedNormal);

				TangentOutput::blendTransformed (blendedTangent, weightVec, transformedTangent);
				BiTangentOutput::blendTransformed (blendedBiTangent, weightVec, transformedBiTangent);

				currentInfluence++;
			}
			while ( ++inf < numEntries );

			currentStartInfluence++;

			// Next!
			PositionInput::next (positionsIn, positionsInStride);
			NormalInput::next (normalsIn, normalsInStride);
			TangentInput::next (tangentsIn, tangentsInStride);
			BiTangentInput::next (biTangentsIn, biTangentsInStride);

			PositionOutput::storeBlend (positionsOut, blendedPosition);
			NormalOutput::storeBlend (normalsOut, blendedNormal);
			TangentOutput::storeBlend (tangentsOut, blendedTangent);
			BiTangentOutput::storeBlend (biTangentsOut, blendedBiTangent);

			PositionOutput::next (positionsOut, positionsOutStride);
			NormalOutput::next (normalsOut, normalsOutStride);
			TangentOutput::next (tangentsOut, tangentsOutStride);
			BiTangentOutput::next (biTangentsOut, biTangentsOutStride);
		}
		else
		{
			currentStartInfluence++;

			// Next!
			PositionInput::next (positionsIn, positionsInStride);
			NormalInput::next (normalsIn, normalsInStride);
			TangentInput::next (tangentsIn, tangentsInStride);
			BiTangentInput::next (biTangentsIn, biTangentsInStride);

			PositionOutput::next (positionsOut, positionsOutStride);
			NormalOutput::next (normalsOut, normalsOutStride);
			TangentOutput::next (tangentsOut, tangentsOutStride);
			BiTangentOutput::next (biTangentsOut, biTangentsOutStride);
		}
	}
}

class SimdSkinInput
{
	public:

		// Sequential access
		HK_FORCE_INLINE static void prefetchNext(const void* HK_RESTRICT current, hkUint32 stride)
		{
			hkMath::prefetch128( reinterpret_cast<const char*> (current) + stride );
		}

		HK_FORCE_INLINE static void getCurrentVector (const void* HK_RESTRICT current, hkVector4& vectorOut)
		{
			vectorOut = * reinterpret_cast<const hkVector4*> (current);
		}

		HK_FORCE_INLINE static void next (const void* HK_RESTRICT &current, hkUint32 stride)
		{
			current = hkAddByteOffsetConst (current, stride);
		}
};

class SimdSkinOutput
{
	public:

		HK_FORCE_INLINE static void prefetchNext(const void* HK_RESTRICT current, hkUint32 stride)
		{
			hkMath::prefetch128( reinterpret_cast<const char*> (current) + stride );
		}

		HK_FORCE_INLINE static void initBlend (hkVector4& blended)
		{
			blended.setZero4();
		}

		HK_FORCE_INLINE static void transformPosition (const hkVector4& original, const hkMatrix4& boneTransform, hkVector4& transformedOut)
		{
			boneTransform.transformPosition (original, transformedOut);
		}

		HK_FORCE_INLINE static void transformDirection (const hkVector4& original, const hkMatrix4& boneTransform, hkVector4& transformedOut)
		{
			boneTransform.transformDirection(original, transformedOut);
		}

		HK_FORCE_INLINE static void blendTransformed (hkVector4& blended, const hkVector4& weightVec, const hkVector4& transformed)
		{
			blended.addMul4(weightVec, transformed);
		}

		HK_FORCE_INLINE static void storeBlend(void* HK_RESTRICT current, const hkVector4& blended)
		{
			* (reinterpret_cast<hkVector4*> (current)) = blended;
		}

		HK_FORCE_INLINE static void next (void* HK_RESTRICT &current, hkUint32 stride)
		{
			current = hkAddByteOffset (current, stride);
		}
};

class EmptySkinInputOutput
{
	public:

		HK_FORCE_INLINE static void prefetchNext(const void* HK_RESTRICT current, hkUint32 stride) { }
		HK_FORCE_INLINE static void getCurrentVector (const void* HK_RESTRICT current, hkVector4& vectorOut) { } 
		HK_FORCE_INLINE static void next(const void* HK_RESTRICT &current, hkUint32 stride) { } 

		HK_FORCE_INLINE static void initBlend (hkVector4& blended) { } 
		HK_FORCE_INLINE static void transformPosition (const hkVector4& original, const hkMatrix4& boneTransform, hkVector4& transformedOut) { } 
		HK_FORCE_INLINE static void transformDirection (const hkVector4& original, const hkMatrix4& boneTransform, hkVector4& transformedOut) { } 
		HK_FORCE_INLINE static void blendTransformed (hkVector4& blended, const hkVector4& weightVec, const hkVector4& transformed) { } 
		HK_FORCE_INLINE static void storeBlend(void* HK_RESTRICT current, const hkVector4& blended) { } 
		HK_FORCE_INLINE static void next(void* HK_RESTRICT &current, hkUint32 stride) { } 
};

class FloatSkinInput
{
	public:

		HK_FORCE_INLINE static void prefetchNext(const void* HK_RESTRICT current, hkUint32 stride)
		{
			hkMath::prefetch128( reinterpret_cast<const char*> (current) + stride );
		}

		HK_FORCE_INLINE static void getCurrentVector (const void* HK_RESTRICT current, hkVector4& vectorOut)
		{
			const hkReal* vsReal = reinterpret_cast<const hkReal*> (current);
			vectorOut.set(vsReal[0], vsReal[1], vsReal[2]);
		}

		HK_FORCE_INLINE static void next(const void* HK_RESTRICT &current, hkUint32 stride)
		{
			current = hkAddByteOffsetConst (current, stride);
		}

};

class FloatSkinOutput
{
	public:

		HK_FORCE_INLINE static void prefetchNext(const void* HK_RESTRICT current, hkUint32 stride)
		{
			hkMath::prefetch128( reinterpret_cast<const char*> (current) + stride );
		}

		HK_FORCE_INLINE static void initBlend( hkVector4& blended)
		{
			blended.setZero4();
		}

		HK_FORCE_INLINE static void transformPosition (const hkVector4& original, const hkMatrix4& boneTransform, hkVector4& transformedOut)
		{
			boneTransform.transformPosition(original, transformedOut);
		}

		HK_FORCE_INLINE static void transformDirection (const hkVector4& original, const hkMatrix4& boneTransform, hkVector4& transformedOut)
		{
			boneTransform.transformDirection(original, transformedOut);
		}

		HK_FORCE_INLINE static void blendTransformed (hkVector4& blended, const hkVector4& weightVec, const hkVector4& transformed)
		{
			blended.addMul4(weightVec, transformed);
		}


		HK_FORCE_INLINE static void storeBlend(void* HK_RESTRICT current, const hkVector4& blended)
		{
			hkReal* v = reinterpret_cast<hkReal*> (current);

			blended.store3(v);

		}

		HK_FORCE_INLINE static void next (void* HK_RESTRICT &current, hkUint32 stride)
		{
			current = hkAddByteOffset (current, stride);
		}
};

typedef SimdSkinInput SKIN_SIMD_IN;
typedef SimdSkinOutput SKIN_SIMD_OUT;
typedef FloatSkinInput SKIN_FLOAT_IN;
typedef FloatSkinOutput SKIN_FLOAT_OUT;
typedef EmptySkinInputOutput SKIN_IGNORE;

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
