/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BLEND_UTILS_H
#define HK_BLEND_UTILS_H

#include <Common/Base/hkBase.h>

class hkaSkeleton;

	/// Utility class for hkaAnimatedSkeleton
class hkaSampleAndCombineUtils
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaSampleAndCombineUtils);
	
			/// Utility structure for blendNormal, blendAdditive below.
	struct TransformBlendParameters
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaSampleAndCombineUtils::TransformBlendParameters );

			/// Transforms in
		const hkQsTransform* m_animationTransformsIn;
			/// Max track to blend
		int m_maxTransformTrackIndex;
			/// Total number of bones
		int m_numBones;
			/// Master weight (control weight) of all tracks to be blended in (combined with per-track weights if they exist).
		hkReal m_masterWeight;
			/// Per track weights
		const hkUint8* m_perBoneWeights;
			/// Track-to-bone mapping (usually hkaBinding::m_transformTrackToBoneIndices)
		const hkInt16* m_trackToBoneMapping;

			/// Constructor
		TransformBlendParameters() : m_animationTransformsIn(HK_NULL),
							m_maxTransformTrackIndex(-1),
							m_numBones(0),
							m_masterWeight(1.0f),
							m_perBoneWeights(HK_NULL),
							m_trackToBoneMapping(HK_NULL)
		{}
	};

		/// Utility structure for blendNormal, blendAdditive below.
	struct FloatBlendParameters
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaSampleAndCombineUtils::FloatBlendParameters );

			/// Floats in
		const hkReal* m_floatTracksIn;
			/// Max track to blend
		int m_maxFloatTrackIndex;
			/// Total number of float slots
		int m_numFloatSlots;
			/// Master weight (control weight) of all tracks to be blended in (combined with per-track weights if they exist).
		hkReal m_masterWeight;
			/// Per track weights
		const hkUint8* m_perFloatTrackWeights;
			/// Track-to-slot mapping (usually hkaBinding::m_floatTrackToFloatSlotIndices)
		const hkInt16* m_floatTrackToFloatSlotMapping;

			/// Constructor
		FloatBlendParameters() : m_floatTracksIn(HK_NULL),
							m_maxFloatTrackIndex(-1),
							m_numFloatSlots(0),
							m_masterWeight(1.0f),
							m_perFloatTrackWeights(HK_NULL),
							m_floatTrackToFloatSlotMapping(HK_NULL)
		{}
	};

		/// Blends in a multiple of a new pose to an existing pose using perBoneWeights: P' = P + t*Q, W' = W + t
		/// where, for each bone 'i' in the pose:
		///     - P = accumulationPoseInOut[i]
		///     - Q = paramsIn.m_animationTransformsIn[i]
		///     - W = accumulatedPerBoneWeightsInOut[i]
		///     - t = paramsIn.m_perBoneWeights[i]/255.0f (or 1.0 if there are no per bone weights in the control)
		///     - P' = accumulationPoseInOut[i] after above modification
		///     - W' = accumulatedPerBoneWeightsInOut[i] after above modification
		///
		/// The final result is not a normalized pose, and should be scaled by the weights array W' before use.
	static void HK_CALL blendNormal(TransformBlendParameters& paramsIn, hkQsTransform* accumulationPoseInOut, hkReal* accumulatedPerBoneWeightsInOut );
	
		/// Blends a new pose to an existing pose: P' = (1-t)*P + t*Q*P
		/// where:
		///     - P = accumulationPoseInOut[i]
		///     - Q = paramsIn.m_animationTransformsIn[i]
		///     - t = paramsIn.m_perBoneWeights[i]/255.0f (or 1.0 if there are no per bone weights in the control)
		///     - P' = accumulationPoseInOut[i] after above modification
		///
		/// The final pose is a normalized pose.
	static void HK_CALL blendAdditive(TransformBlendParameters& paramsIn, hkQsTransform* accumulationPoseInOut);

		/// Blends in a multiple of the float tracks to an existing set of slots using perFloatTrackWeights: P' = P + t*Q, W' = W + t
		/// where, for each bone 'i' in the pose:
		///     - P = accumulationPoseInOut[i]
		///     - Q = paramsIn.m_animationTransformsIn[i]
		///     - W = accumulatedPerBoneWeightsInOut[i]
		///     - t = paramsIn.m_perBoneWeights[i]/255.0f (or 1.0 if there are no per bone weights in the control)
		///     - P' = accumulationPoseInOut[i] after above modification
		///     - W' = accumulatedPerBoneWeightsInOut[i] after above modification
		///
		/// The final result is not a 'normalized' pose, and should be scaled by the weights array W' before use.
	static void HK_CALL blendNormalFloat(FloatBlendParameters& paramsIn, hkReal* accumulationInOut, hkReal* accumulatedWeightsInOut);

		/// Finds largest track index with non-zero (> HK_REAL_EPSILON) weighting.
		/// \return The index of the index of the largest track with non-trivial weighting
		/// \param trackToBoneMapping Array of track to bone (or slot) indices
		/// \param perTrackWeights Array of weights for each track
		/// \param maxBone Can ignore all tracks mapping to bone >= maxBone
		/// \param numTracks Total number of tracks in the animation
	static hkUint32 HK_CALL getMaxTrackIndex(const hkInt16* trackToBoneMapping, const hkUint8* perTrackWeights, hkUint32 maxBone, hkUint32 numTracks);

		/// Scales the m_translation component of each given hkQsTransform.
		/// Useful when changing the size of a character dynamically
	static void HK_CALL scaleTranslations( hkReal scale, hkQsTransform* poseInOut, int numTransforms );

		/// Utilities to initialize an output pose to the reference pose
	static void HK_CALL copyReferencePose( hkQsTransform* bonesOut, hkReal* floatsOut, const hkaSkeleton* );
	static void HK_CALL copyReferencePose( hkQsTransform* bonesOut, hkReal* floatsOut, const hkQsTransform* referencePose, hkUint32 numBones, const hkReal* referenceFloats, hkUint32 numFloats );
};


#endif // HK_BLEND_UTILS_H

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
