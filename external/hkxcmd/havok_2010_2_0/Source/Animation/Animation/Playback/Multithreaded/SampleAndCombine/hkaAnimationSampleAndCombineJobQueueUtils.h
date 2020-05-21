/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ANIMATION_SAMPLE_AND_COMBINE_JOB_QUEUE_UTILS
#define HK_ANIMATION_SAMPLE_AND_COMBINE_JOB_QUEUE_UTILS

#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

struct hkaAnimationSampleAndCombineJobQueueUtils
{
	static hkJobQueue::JobPopFuncResult  HK_CALL popAnimationJob   ( hkJobQueue& queue, hkJobQueue::DynamicData* data,       hkJobQueue::JobQueueEntry& jobIn, hkJobQueue::JobQueueEntry& jobOut  );
	static hkJobQueue::JobCreationStatus HK_CALL finishAnimationJob( hkJobQueue& queue, hkJobQueue::DynamicData* data, const hkJobQueue::JobQueueEntry& jobIn, hkJobQueue::JobQueueEntryInput& newJobCreated );

	static void HK_CALL registerWithJobQueue( hkJobQueue* jobQueue, hkBool useDeprecatedElf=false );
};


#endif // HK_ANIMATION_SAMPLE_AND_COMBINE_JOB_QUEUE_UTILS

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
