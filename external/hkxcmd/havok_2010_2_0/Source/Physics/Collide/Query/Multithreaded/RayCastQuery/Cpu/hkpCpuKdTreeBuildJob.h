/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/hkBase.h>
#include <Common/Base/Spu/Util/hkSpuUtil.h>

#ifndef HK_CPU_RAYCAST_QUERY_KD_TREE_BUILD_JOB_H
#define HK_CPU_RAYCAST_QUERY_KD_TREE_BUILD_JOB_H

struct hkpProcessCollisionInput;


hkJobQueue::JobStatus HK_CALL hkCpuKdTreeBuildCoordinatorJob( hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut );

hkJobQueue::JobStatus HK_CALL hkCpuKdTreeBuildSetupJob( hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut );

hkJobQueue::JobStatus HK_CALL hkCpuKdTreeBuildJobFast(  hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut );

hkJobQueue::JobStatus HK_CALL hkCpuKdTreeBuildFastDistributedJob( hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut );


hkJobQueue::JobStatus HK_CALL hkCpuKdTreeBuildFastDistributedSubJob( hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut );

#endif //HK_CPU_RAYCAST_QUERY_KD_TREE_BUILD_JOB_H

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
