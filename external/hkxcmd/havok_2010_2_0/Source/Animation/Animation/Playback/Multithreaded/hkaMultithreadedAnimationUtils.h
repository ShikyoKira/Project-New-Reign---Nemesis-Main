/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MUTLITHREADED_ANIMATION_UTILS_H
#define HK_MUTLITHREADED_ANIMATION_UTILS_H

#include <Common/Base/hkBase.h>
#include <Animation/Animation/Playback/Multithreaded/SampleAndCombine/hkaAnimationSampleAndCombineJobs.h>
#include <Animation/Animation/Mapper/Multithreaded/hkaAnimationMappingJobs.h>

class hkaMultithreadedAnimationUtils
{
	public:
			/*
			 * Sample and Combine jobs
			 */

			/// Uses the memory manager to preallocate the animation control buffer
		static void HK_CALL allocateSampleAndCombineJob(const hkaAnimatedSkeleton* skeleton, hkaAnimationSampleAndCombineJob& jobOut);

			/// Frees the animation control buffer using the memory manager
		static void HK_CALL deallocateSampleAndCombineJob( hkaAnimationSampleAndCombineJob& jobOut);

			/// Uses the memory manager to preallocate the animation control buffer
		static void HK_CALL allocateQuantizedSampleAndCombineJob( const hkaAnimatedSkeleton* skeleton, hkaQuantizedSampleAndCombineJob& jobOut );

			/// Frees the animation control buffer using the memory manager
		static void HK_CALL deallocateQuantizedSampleAndCombineJob( hkaQuantizedSampleAndCombineJob& jobOut );



			/// Grab the current data from the animated skeleton and fill in the job structure
			/// Note: The hkaAnimatedSkeleton can contain only hkaQuantizedAnimation instances
			/// Note: the controlData pointer in the structure must be preallocated and capable of holding
			/// a number of entries equal to the number of active (non zero weight) controls.
			/// use allocateSampleAndCombineJob or allocate this manually
		static void HK_CALL createQuantizedSampleAndCombineJob( const hkaAnimatedSkeleton* animSkel, hkQsTransform* poseOut, hkReal* floatSlotsOut, hkReal* boneWeightsOut, hkReal* floatWeightsOut, const hkInt16* parentIndicesForLocalToModel, hkaQuantizedSampleAndCombineJob& jobOut, hkBool useSlerp, hkBool useNormalize, hkReal frameSteppingTolerance = 1.0e-3f );

			/// Grab the current data from the animated skeleton and fill in the job structure
			/// Note: the controlData pointer in the structure must be preallocated and capable of holding
			/// a number of entries equal to the number of active (non zero weight) controls.
			/// use allocateSampleAndCombineJob or allocate this manually
		static void HK_CALL createSampleAndCombineJob(const hkaAnimatedSkeleton* skeleton, hkUint32 maxBone, hkQsTransform* poseOut, hkUint32 maxFloat, hkReal* floatSlotsOut, hkInt16* parentIndicesForLocalToModel, hkaAnimationSampleAndCombineJob& jobOut);

			/// By default we use 70k max space (DEFAULT_MAX_COMPRESSED_ANIM_DATA) for animation buffer.
			/// which is very conservative. Use this method to get an upper bound on the size of the buffer required
			/// for a given hkaAnimationSampleAndCombineJob. Then set the job.m_bufferSize appropriately.
		static int HK_CALL getMaxSizeRequiredForSampleAndCombineJobBuffer(const hkaAnimatedSkeleton* skeleton);

			/// See getMaxSizeRequiredForSampleAndCombineJobBuffer(), but usable offline on a per-animation basis.
			/// At runtime, getMaxSizeRequiredForSampleAndCombineJobBuffer() called on an animated skeleton
			/// should return the same as the maximum of getConservativeMaxSizeRequiredForSampleAndCombineJobBuffer()
			/// over all animations used by the corresponding hkaAnimatedSkeleton.
		static int HK_CALL getConservativeMaxSizeRequiredForSampleAndCombineJobBuffer(const hkaAnimation* animation); 

			/// There is a requirement when using animation jobs for sampling on SPU that the float slot array
			/// be allocated to a size (number of bytes) which is a multiple of 16. The hkaPose class handles this automatically
			/// by setting the m_floatSlotValues array to have a *capacity* rounded up to a multiple of 4, but if you are not using
			/// this class you may wish to use this helper method.
			/// To deallocate, use deallocateFloatSlotsArray().
		static hkReal* HK_CALL allocateFloatSlotsArrayRoundedUpToMultipleOf16(int numFloatSlots);

			/// There is a requirement when using animation jobs for sampling on SPU that the float slot array
			/// be allocated to a size (number of bytes) which is a multiple of 16. If you have allocated this array using
			/// allocateFloatSlotsArrayRoundedUpToMultipleOf16(), then use this method to deallocate.
		static void HK_CALL deallocateFloatSlotsArray(hkReal* array);

};


#endif // HK_MUTLITHREADED_ANIMATION_UTILS_H

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
