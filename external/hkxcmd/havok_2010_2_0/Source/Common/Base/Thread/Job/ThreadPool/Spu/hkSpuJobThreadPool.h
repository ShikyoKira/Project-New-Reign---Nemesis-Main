/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_MULTITHREADING_UTIL_NEW_H
#define HK_UTILITIES2_MULTITHREADING_UTIL_NEW_H

#include <Common/Base/Thread/Job/ThreadPool/hkJobThreadPool.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>

class hkSpuUtil;
class hkJobQueue;


/// This structure defines the parameters and configuration of the
/// SPU job thread pool. In it, we can set the number of SPUs used,
/// taskset priorities and monitor buffer size.
struct hkSpuJobThreadPoolCinfo
{
	hkSpuJobThreadPoolCinfo();

	hkSpuUtil* m_spuUtil;

		/// The number of SPUs used can not be larger than 6.
	int m_maxNumSpus;

		/// Defaults to 1024*100 (100K).
	int m_perSpuMonitorBufferSize;

		/// Defaults to "HavokSpursTaskset".
	const char* m_tasksetName;

		/// Specifies the priority of the taskset for each SPU.
		/// The priorities range from 1 to 15. In terms of execution on an SPU,
		/// tasksets with lower priority values will be prioritized over those
		/// with larger ones. Priority values do not need to be uniform across the different SPUs.
		/// To disable execution of the taskset on a specific SPU, set its priority value to 0.
	hkUint8 m_tasksetPriority[8];
};


class hkSpuJobThreadPool : public hkJobThreadPool
{
	public:

		hkSpuJobThreadPool( hkSpuJobThreadPoolCinfo& cinfo );
		~hkSpuJobThreadPool();

		virtual void processAllJobs( hkJobQueue* queue, hkJobType firstJobType = HK_JOB_TYPE_MAX );
		virtual void waitForCompletion( );

		virtual bool isProcessing();

		virtual void appendTimerData( hkArrayBase<hkTimerData>& timerData, hkMemoryAllocator& alloc );
		virtual void clearTimerData();

		virtual int getNumThreads();
		virtual void setNumThreads( int numThreads );

		hkSpuUtil* getSpuUtil() { return m_spuUtil; }
		class hkSpuMonitorCache* getMonitorCache(int i) { return m_monitorCaches[i]; }

		enum { MAX_NUM_SPUS = 5 };

		int startSpursTask( hkJobType jobType, hkJobQueue* queue );

	protected:

		friend class hkJobQueue;
		hkSpuUtil* m_spuUtil;
		int m_maxNumSpus;

		bool m_isRunning;

		const char* m_tasksetName;

		hkSemaphoreBusyWait* m_taskCompletionSemaphore;

		hkJobQueue* m_jobQueue;
		hkSpuMonitorCache* m_monitorCaches[MAX_NUM_SPUS];

};





#endif // HK_UTILITIES2_MULTITHREADING_UTIL_H

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
