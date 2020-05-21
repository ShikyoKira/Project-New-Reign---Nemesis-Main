/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ANIMATION_JOBS_H
#define HK_ANIMATION_JOBS_H

#include <Common/Base/hkBase.h>

#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Animation/hkaAnimation.h>

class hkaAnimatedSkeleton;

// The base class for all animation jobs

// CONSERVATIVE 70k max space for animation buffer. 
// Use either:
// hkaAnimation::getMaxSizeOfCombinedDataChunks() for sample-only jobs , or
// hkaMultithreadedAnimationUtils::getMaxSizeRequiredForSampleAndCombineJobBuffer() and
// getConservativeMaxSizeRequiredForSampleAndCombineJobBuffer() for sample-and-combine jobs
// to get a better estimate of how large this actually needs to be.
const int DEFAULT_MAX_COMPRESSED_ANIM_DATA = 70 * 1024 ; 

const int HK_MAX_NUM_DATA_CHUNKS = 10;



#include <Animation/Animation/Playback/Multithreaded/hkaAnimationJobs.inl>

#endif // HK_ANIMATION_JOBS_H

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
