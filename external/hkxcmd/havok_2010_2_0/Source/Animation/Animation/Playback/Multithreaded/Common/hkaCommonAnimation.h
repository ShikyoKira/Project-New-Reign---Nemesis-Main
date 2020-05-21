/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COMMON_ANIMATION_H
#define HK_COMMON_ANIMATION_H

#include <Common/Base/hkBase.h>

class hkaQuantizedSampleAndCombineJob;

/// These enums are protected in a namespace for other files that want to use them
namespace hkaDmaGroups
{
	enum hkaDmaGroupsEnum
	{
		HK_SPU_DMA_GROUP_ANIMATION_DATA=  0, // reserve 2 groups for double-buffering
		HK_SPU_DMA_GROUP_ANIMATED_SKELETON_DATA =  2,
		HK_SPU_DMA_GROUP_OUTPUT_POSE=  3,
		HK_SPU_DMA_GROUP_ANIMATION_SAMPLE_DATA=  4, // reserve 2 groups for double-buffering
		HK_SPU_DMA_GROUP_HEADER                 =  7, // reserve 3 groups for triple-buffering
		HK_SPU_DMA_GROUP_FRAME                  =  10,// reserve 3 groups for triple-buffering
		HK_SPU_DMA_GROUP_SEMAPHORE=  13
	};
}

/// Executes a quantized sample and combine job
/// Uses the provided stack for all temp allocations
///
/// Execute a quantized sample and combine job.
/// This function may be called on both shared and non-shared memory architectures.
/// This function is called internally when using hkaAnimationJobs in a multithreaded environment.
/// Users may call this function directly in a single-threaded environment,
/// or it may be used in implementing a user-defined multithreading system.
void HK_CALL executeQuantizedSampleAndCombineJob( hkaQuantizedSampleAndCombineJob& job );


#endif // HK_COMMON_ANIMATION_H

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
