/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ANIMATION_MAPPING_JOBS_H
#define HK_ANIMATION_MAPPING_JOBS_H

#include <Common/Base/hkBase.h>

#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Animation/Animation/Playback/Multithreaded/hkaAnimationJobs.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>


class hkaAnimationMappingJob : public hkJob
{
	public:

		/// Types enum.
		enum JobSubType
		{
			/// Mapping (ragdoll and animation retargeting)
			ANIMATION_MAPPING_JOB_MAP_POSE,
			//
			ANIMATION_MAPPING_JOB_END		// just a marker marking the end of the animation job id range
		};

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaAnimationMappingJob );

	protected:

		HK_FORCE_INLINE hkaAnimationMappingJob( JobSubType type, hkUint16 size );
};


/// A job which maps poses either for ragdolls or for animation retargeting
class hkaAnimationMapPoseJob : public hkaAnimationMappingJob
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaAnimationMapPoseJob);

		HK_FORCE_INLINE hkaAnimationMapPoseJob();

		HK_FORCE_INLINE ~hkaAnimationMapPoseJob() {}

	public:

		// Sample inputs
		hkaSkeletonMapper*				m_mapper;		///< Point to skeleton mapper

		// These inputs are in MODEL space for RAGDOLL MAPPING
		// and LOCAL space for ANIMATION RETARGETING
		const hkQsTransform*			m_poseAIn;		///< Point to the input pose in model space
		hkQsTransform*					m_poseBInOut;		///< Point to the output pose in local space

		/// Used only for ragdoll mapping
		const hkQsTransform*				m_poseBLocalSpaceIn;
		hkaSkeletonMapper::ConstraintSource	m_source;

		/// Used only for animation retargeting
		hkBool m_isAdditive;

		/// This semaphore is released when the work is finished.
		/// Can be set to HK_NULL if you don't need to wait on a specific job
		hkSemaphoreBusyWait*			m_jobDoneSemaphore;

		/// This flag is incremented when the work is finished.
		/// Can be set to HK_NULL if you don't need to wait on a specific job
		hkUint32*						m_jobDoneFlag;
};

#include <Animation/Animation/Mapper/Multithreaded/hkaAnimationMappingJobs.inl>

#endif // HK_ANIMATION_MAPPING_JOBS_H

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
