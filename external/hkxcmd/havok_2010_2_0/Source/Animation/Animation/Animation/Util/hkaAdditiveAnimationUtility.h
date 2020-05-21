/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ADDITIVE_ANIMATION_UTILITY_H
#define HK_ADDITIVE_ANIMATION_UTILITY_H

/// This utility creates an additive animation for an input animation and a base animation
/// The additive animation is created by 'subtracting' the base from the original to produce delta values.
/// There are two common forms supplied. The first subtracts a single pose from the original data.
/// The second subtracts an entire animation.
/// A third less common form is also supplied which allows an animation to be subtracted from the reference
/// pose of a character.
/// NOTE: When using the results you must still set the blend hint flag in the corresponding
/// animation binding to hkaAnimationBinding::ADDITIVE.
class hkaAdditiveAnimationUtility
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaAdditiveAnimationUtility);
			/// Input structure
		struct Input
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaAdditiveAnimationUtility::Input );

				/// The number of tracks in the animation
			int m_numberOfTransformTracks;					
				/// The number of poses (usually equates to number of frames)
			int m_numberOfPoses;
				/// The base data to remove.
				/// This data should be either m_numberOfTransformTracks or m_numberOfTransformTracks * m_numberOfAnimationPoses elements in length depending on which create call is used
			const hkQsTransform* m_baseData;
				/// The original data
			const hkQsTransform* m_originalData;
		};
				
			/// This creates an additive animation by subtracting the single pose stored in 'baseData'
			/// from each of the poses passed in the original data
			/// The result is stored in 'deltaOut'.
			/// This function is alias safe so you can reuse the same buffer for input and output if desired
		static void HK_CALL createAdditiveFromPose( const Input& input, hkQsTransform* deltaOut);

			/// This creates an additive animation by subtracting the animation passed in 'baseData'
			/// from each of the poses passed in the original data.
			/// The result is stored in 'deltaOut'.
			/// This function is alias safe so you can reuse the same buffer for input and output if desired
		static void HK_CALL createAdditiveFromAnimation( const Input& input, hkQsTransform* deltaOut);

			/// Input structure
		struct ReferencePoseInput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaAdditiveAnimationUtility::ReferencePoseInput );

				/// The number of tracks in the animation
			int m_numberOfTransformTracks;					
				/// The number of poses (usually equates to number of frames)
			int m_numberOfPoses;
				/// The original data
			const hkQsTransform* m_originalData;

				/// The reference pose for the character
			hkQsTransform* m_referencePose;
				/// Size of m_referencePose array.
			int m_numReferencePose;

				/// A mapping from track numbers to bone indices
			hkInt16* m_transformTrackToBoneIndices;
				/// Size of m_transformTrackToBoneIndices array.
			hkInt32 m_numTransformTrackToBoneIndices;
		};

			/// This creates an additive animation by subtracting the animation passed in 'baseData'
			/// from each of the poses passed in the original data.
			/// The result is stored in 'deltaOut'.
			/// This function is alias safe so you can reuse the same buffer for input and output if desired
		static void HK_CALL createAdditiveFromReferencePose( const ReferencePoseInput& input, hkQsTransform* deltaOut);

};


#endif // HK_ADDITIVE_ANIMATION_UTILITY_H

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
