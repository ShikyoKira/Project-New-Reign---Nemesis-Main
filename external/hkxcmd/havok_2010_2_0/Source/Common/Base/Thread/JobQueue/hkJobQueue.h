/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BASE_THREAD_JOB_QUEUE_H
#define HK_BASE_THREAD_JOB_QUEUE_H

#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>

#include <Common/Base/Container/Queue/hkQueue.h>

extern HK_THREAD_LOCAL( int ) hkThreadNumber;

#define HK_INVALID_JOB 0xff
#define HK_BROAD_PHASE_THREAD_AFFINITY 1

typedef hkUint8 hkJobSubType;

enum hkJobType
{
	HK_JOB_TYPE_DYNAMICS,
	HK_JOB_TYPE_COLLIDE,
	HK_JOB_TYPE_COLLISION_QUERY,
	HK_JOB_TYPE_RAYCAST_QUERY,
	HK_JOB_TYPE_ANIMATION_SAMPLE_AND_COMBINE,
	HK_JOB_TYPE_ANIMATION_DEPRECATED,
	HK_JOB_TYPE_ANIMATION_MAPPING,
	HK_JOB_TYPE_BEHAVIOR,
	HK_JOB_TYPE_CLOTH,
	HK_JOB_TYPE_AI_PATHFINDING,
	HK_JOB_TYPE_AI_DYNAMIC,
	HK_JOB_TYPE_AI_LOCAL_STEERING,
	HK_JOB_TYPE_DESTRUCTION,
	HK_JOB_TYPE_PHYSICS_TESTS,
	HK_JOB_TYPE_UNIT_TEST,
	HK_JOB_TYPE_CHARACTER_PROXY,
	HK_JOB_TYPE_VEHICLE,
	HK_JOB_TYPE_OTHER,
	HK_JOB_TYPE_COLLIDE_MIDPHASE,
	HK_JOB_TYPE_COLLIDE_NARROWPHASE,
	HK_JOB_TYPE_HAVOK_MAX,
	HK_JOB_TYPE_USER_0 = HK_JOB_TYPE_HAVOK_MAX,
	//HK_JOB_TYPE_USER_1,
	HK_JOB_TYPE_MAX
};


enum hkJobSpuType
{
	HK_JOB_SPU_TYPE_INVALID,
	HK_JOB_SPU_TYPE_ENABLED,
	HK_JOB_SPU_TYPE_DISABLED
};

	/// Base type for all jobs.
struct hkJob
{
	HK_FORCE_INLINE hkJob( hkJobType jobType, hkUint32 jobSubType, hkUint16 size, hkJobSpuType jobProcessorTypes = HK_JOB_SPU_TYPE_ENABLED );
	hkJob() {}

	HK_ALIGN16(hkJobSubType m_jobSubType);
	hkEnum<hkJobType, hkUint8> m_jobType;
	hkEnum<hkJobSpuType, hkUint8> m_jobSpuType;
	hkUint16 m_size;
	hkInt16 m_threadAffinity; // -1 by default indicates no thread affinity

};

/// A structure used for specifying the number of threads, shared caches, and additionally for the
/// PlayStation(R)3, whether the PPU can take SPU tasks
struct hkJobQueueHwSetup
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkJobQueueHwSetup);
	hkJobQueueHwSetup();


		/// PlayStation(R)3 only. A flag used to determine if the PPU can take SPU tasks.
	enum CellRules
	{
			/// If there are only SPU jobs on the queue and no SPU is free to take one, the
			/// PPU is allowed to take the job.
		PPU_CAN_TAKE_SPU_TASKS,

			/// The PPU will never take SPU jobs, even if there are only SPU jobs on the queue.
			/// This is the default setting.
		PPU_CANNOT_TAKE_SPU_TASKS
	};
	CellRules m_cellRules;

	hkBool m_noSpu;

	enum SpuSchedulePolicy
	{
		SEMAPHORE_WAIT_OR_SWITCH_ELF,
		RULE_BASED_QUIT_OR_TIMEOUT
	};

	SpuSchedulePolicy m_spuSchedulePolicy;

	int m_numCpuThreads;
	hkArray< hkArray<int> > m_threadIdsSharingCaches;
};

struct hkJobQueueCinfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkJobQueueCinfo );
	hkJobQueueCinfo() { m_maxNumJobTypes = HK_JOB_TYPE_HAVOK_MAX; }
	hkJobQueueHwSetup m_jobQueueHwSetup;
	int m_maxNumJobTypes;

};

class hkSpuJobThreadPool;

	/// This class implements a job queue with all necessary locking and waiting
class hkJobQueue
{
	public:

		// NOTE: Using these fixed values instead of hkArrays for easy SPU
		#define	CUSTOM_JOB_TYPE_MAX 2

	#define MAX_NUM_THREAD_TYPES CUSTOM_JOB_TYPE_MAX + 2 + 1// Max shared caches + 1 for master thread
	#define MAX_NUM_CPU_THREADS HK_MAX_NUM_THREADS

		#define MAX_NUM_QUEUES MAX_NUM_THREAD_TYPES + HK_JOB_TYPE_MAX

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, hkJobQueue );

		typedef int QueueIndex;

			/// The basic struct stored in the job queue
#if (HK_POINTER_SIZE == 4)
		enum { JOB_QUEUE_ENTRY_SIZE = 128 };
#else
		enum { JOB_QUEUE_ENTRY_SIZE = 256 };
#endif

		struct JobQueueEntry : public hkJob
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, JobQueueEntry );
			JobQueueEntry() {}
			hkUchar m_data[ JOB_QUEUE_ENTRY_SIZE - sizeof(hkJob) ];
		};

		// 12 bytes
		struct Queue: public hkQueue<JobQueueEntry>
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, Queue );
		};

		//
		// Enums to specify job types
		//

		/// See comments for setWaitPolicy for details.
		enum WaitPolicy
		{
			WAIT_UNTIL_ALL_WORK_COMPLETE,
			WAIT_INDEFINITELY
		};

		struct JobTypeSpuStats
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, JobTypeSpuStats );

			JobTypeSpuStats(): m_runningTasks(0),
				m_waitingTasks(0),
				m_timeSinceLastTask(0)
			{ 
			}

			hkPadSpu<int> m_runningTasks;

			hkPadSpu<int> m_waitingTasks;

			hkPadSpu<int> m_timeSinceLastTask;
		};

		struct DynamicData
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, DynamicData );

				// The number of jobs which are currently processed (this is both spu and ppu jobs)
			HK_ALIGN16( hkInt16 m_numActiveJobs[HK_JOB_TYPE_MAX] );

			int m_masterThreadFinishingFlags;

			// If this is true, the SPUs won't leave their current physics (collide or integrate) .ELF
			WaitPolicy m_waitPolicy;

			hkBool m_outOfMemory;


				// one entry per semaphore
			hkUint16 m_numThreadsWaiting[MAX_NUM_THREAD_TYPES];

			Queue m_jobQueue[MAX_NUM_QUEUES];
		};


			/// Whether a job is placed at the front or the end of its appropriate queue
		enum JobPriority
		{
			JOB_HIGH_PRIORITY,
			JOB_LOW_PRIORITY,
		};


		//
		// Callback functions used from within critical sections
		//

			/// The result from JobPopFunc
		enum JobPopFuncResult
		{
				/// Set to this if the function is taking the element off the queue
			POP_QUEUE_ENTRY,
				/// Set to this if the function has modified the element, and left it on the queue
			DO_NOT_POP_QUEUE_ENTRY 
		};

		typedef JobPopFuncResult (HK_CALL *JobPopFunc)( hkJobQueue& queue, hkJobQueue::DynamicData* data, JobQueueEntry& jobIn, JobQueueEntry& jobOut );

		enum JobCreationStatus
		{
			JOB_CREATED,
			NO_JOB_CREATED
		};

			/// A flag to tell getNextJob what to do if no job is immediately available
		enum WaitStatus
		{
			WAIT_FOR_NEXT_JOB,
			DO_NOT_WAIT_FOR_NEXT_JOB
		};

		struct JobQueueEntryInput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, JobQueueEntryInput );
			hkPadSpu<hkUint32>	m_jobPriority;		// JobPriority
			JobQueueEntry		m_job;
		};

		typedef JobCreationStatus (HK_CALL *FinishJobFunc)( hkJobQueue& queue, DynamicData* data, const JobQueueEntry& jobIn, JobQueueEntryInput& newJobCreatedOut );


		/// Whether a getNextJob call got another job or not
		enum JobStatus
		{
			JOB_INVALID = -1,
			GOT_NEXT_JOB,
			NO_JOBS_AVAILABLE,
			ALL_JOBS_FINISHED,
			JOBS_AVAILABLE_BUT_NOT_FOR_CURRENT_ELF,
			SPU_QUIT_ELF
		};


		typedef JobStatus (HK_CALL *ProcessJobFunc)( hkJobQueue& jobQueue, JobQueueEntry& jobInOut );

		struct hkJobHandlerFuncs
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkJobHandlerFuncs );
			ProcessJobFunc* m_processJobFuncs;
			int	m_numProcessJobFuncs;
			JobPopFunc m_popJobFunc;
			FinishJobFunc m_finishJobFunc;

			HK_FORCE_INLINE void initProcessJobFuncs( ProcessJobFunc* processJobFuncs, int numProcessJobFuncs)
			{
				m_processJobFuncs = processJobFuncs;
				m_numProcessJobFuncs = numProcessJobFuncs;
			}

			HK_FORCE_INLINE void registerProcessJobFunc( hkJobSubType jobId, ProcessJobFunc func )
			{
				HK_ASSERT2(0x543e432e, (jobId & 0xff) < m_numProcessJobFuncs, " Job ID out of range for registerd function table"); 
				m_processJobFuncs[ (jobId & 0xff) ] = func;
			}
		};

		inline JobStatus findJobInternal( QueueIndex queueIndexOfNewJob, DynamicData* data, WaitStatus waitStatus, JobQueueEntry& jobOut );

		void getFinishingFlags(DynamicData* data, bool& masterThreadShouldFinish, bool& allWorkComplete);

	public:

		//
		// CPU only functions
		//


			/// Note: this class should only ever be constructed on a CPU. If running on an SPU it must be DMAd into an
			/// appropriately sized 128 byte aligned local store buffer.
		 hkJobQueue( const hkJobQueueCinfo& cinfo );
		~hkJobQueue();


		void registerJobWithCpuThread( hkJobType jobType, hkJobSubType subType, int threadId );


			/// Batch add 'n' jobs of the same priority and job thread type - without entering critical section for every job.
		void addJobBatch( const hkArrayBase<hkJob*>& jobs, JobPriority priority );

			// Called automatically
		void updateJobQueryRules();


			/// Set the wait policy on the job queue. By default it is WAIT_UNTIL_ALL_WORK_COMPLETE, which means
			/// worker threads return when all jobs placed on the queue have been completed.
			/// Setting the policy to WAIT_INDEFINITELY will make worker threads continue to wait for new jobs.
			/// This does not affect the thread that created the queue, which will always return when all jobs
			/// have been completed.
		void setWaitPolicy( WaitPolicy waitPolicy );

			/// Get the wait policy. See comments for setWaitPolicy for details.
		WaitPolicy getWaitPolicy();

		int getMasterThreadFinishingFlags();

		void setMasterThreadFinishingFlags( int flags );

			/// Sets the minimum queue capacity for a given job type
		void setQueueCapacityForJobType( hkJobType type, int queueCapacity );

		void setQueueCapacityForCpuCache( int queueCapacity );

			/// This function will process jobs from the queue until the queue is empty, then return.
			/// This does not guarantee that any helper threads also processing jobs have completed
			/// their work (see hkJobThreadPool::waitForCompletion).
		JobStatus processAllJobs( );



		void spawnSpuTasks( DynamicData* data );

		void* getNextElfToLoad();


			/// Register a handler for the supplied jobs
			/// Only the upper 8 bits of the jobId is used so register with one of the types in hkJobBaseId
		void registerJobHandler( hkJobType jobId, hkJobHandlerFuncs jobHandlerFuncs );


		//
		// Runtime functions
		//

			/// Adds a new job to the queue with a given priority
		void addJob( hkJob& job, JobPriority priority );

			/// Adds a new job to the queue for a given priority and job thread type.
			/// This call requires you to pass the job as a JobQueueEntry struct, and so avoids the memcpy();
		void addJob( JobQueueEntry& job, JobPriority priority );


			// Can be used to add jobs when the job queue is already locked. This can only be called from
			// pop and finish callback functions. It should not be called elsewhere.
		void addJobQueueLocked( DynamicData* data, const JobQueueEntry& job, JobPriority priority );


			/// Gets a new job. Note: this function should only be called at the start of the main loop.
			/// If you can you should call addAndGetNextJob
		JobStatus getNextJob( JobQueueEntry& job, WaitStatus waitStatus = WAIT_FOR_NEXT_JOB );

		enum FinishJobFlag
		{
			FINISH_FLAG_NORMAL,
			FINISH_FLAG_JUST_CALL_FINISH_CALLBACK
		};

			/// Call this to finish a job if you do not wish to get another job. If you wish to get another job it is faster to
			/// call finishJobAndGetNextJob. The FinishJobFlag should left to i ts default setting. It is used internally
			/// to just trigger the finish callback without decrementing the active jobs counter.
		void finishJob( const JobQueueEntry* oldJob, FinishJobFlag flag = FINISH_FLAG_NORMAL );

		//
		// Compound functions
		//

			/// Call this when one job is finished and you are not calling finishAddAndGetNextJob
		JobStatus finishJobAndGetNextJob( const JobQueueEntry* oldJob, JobQueueEntry& jobOut, WaitStatus waitStatus = WAIT_FOR_NEXT_JOB );

			/// Finishes a job and gets a new one
			/// This is faster than calling addJob and getNextJob separately, because it can do the
			/// entire operation in one critical section, rather than two critical sections.
		JobStatus finishAddAndGetNextJob( hkJobType oldJobType, JobPriority priority, JobQueueEntry& jobInOut, WaitStatus waitStatus = WAIT_FOR_NEXT_JOB );

			/// Locks the queue and sets the number of running SPUs (used when starting up the tasks in hkSpuJobThreadPool::processAllJobs)
		void setNumRunningSpus( int numSpus );

			/// Changes the SPU scheduling policy - this is not advised to be called while the jobs are being processed
			/// Returns the previously active policy
		hkJobQueueHwSetup::SpuSchedulePolicy setSpuSchedulingPolicy( hkJobQueueHwSetup::SpuSchedulePolicy policy );

	public:
		//
		// Internal helper functions
		//

			// Release a single waiting thread
		void releaseOneWaitingThread( DynamicData* data );

			// Check if a thread with the given threadType is waiting for a job on a semaphore.
			// If one thread is, release the semaphore
		void checkQueueAndReleaseOneWaitingThread( QueueIndex queueIndex, DynamicData* data );

			// Call this to release all waiting threads
		void releaseWaitingThreads(DynamicData* data);

		HK_FORCE_INLINE QueueIndex findNextJob( JobQueueEntry& jobOut, DynamicData* data );

		QueueIndex getQueueIndexForJob( const hkJob& job );
		
		HK_FORCE_INLINE hkBool allQueuesEmpty( DynamicData* data );

		DynamicData* lockQueue( char* dynamicDataStorage );

		void unlockQueue( DynamicData* dynamicDataStorage );



		HK_FORCE_INLINE int getSemaphoreIndex( int threadNumber );

	public:


		HK_ALIGN( hkCriticalSection m_criticalSection, 16 );


			// Data to each time the queue is locked
		DynamicData* m_data;

			//
			// Static, locally set data
			//

		JobPopFunc m_popJobFunc; 
		FinishJobFunc m_finishJobFunc;



	public:

		// The number of jobs types the job queue will handle - a queue is created for each type
		const int m_numJobTypes;

		// Offsets into the queue list, in order.
		// Note that SPU queus, if there, start from 0
		int m_cpuCacheQueuesBegin;
		int m_cpuCustomQueuesBegin;
		int m_cpuTypesQueuesBegin;
		int m_numJobQueues;

		struct CustomJobType
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, CustomJobType );
			hkJobType m_jobType;
			hkUint8 m_jobSubType;
			hkUint8 m_queueId;
		};
		CustomJobType m_customJobs[CUSTOM_JOB_TYPE_MAX];
		int m_numCustomJobs;

		int m_cpuSemaphoreBegin; 
		int m_directMapSemaphoreEnd; 

		int m_masterThreadQueue;

			// Flags relating to how the hardware should be used
		hkJobQueueHwSetup m_hwSetup;

		hkBool m_queryRulesAreUpdated;

		hkSemaphoreBusyWait* m_queueSemaphores[ MAX_NUM_THREAD_TYPES ]; // one semaphore for every thread (can be shared)
		int m_numQueueSemaphores;

		hkInt8 m_nextQueueToGet  [MAX_NUM_THREAD_TYPES][MAX_NUM_QUEUES + 2];	// +2 (end marker and padding)

			// Used to decide what semaphore to wait on for CPU threads.
			// SPU threads have a 1-1 mapping. CPU threads also go from 0-N but need a map to map them to the right semaphore
			// (depending on shared caches / custom jobs)
		hkInt8 m_cpuThreadIndexToSemaphoreIndex[ MAX_NUM_CPU_THREADS ]; 

		hkJobHandlerFuncs m_jobFuncs[HK_JOB_TYPE_MAX];

		// reference to the SPU thread pool
		hkSpuJobThreadPool* m_threadPool;

		//
		// Setup info
		//
		struct CustomJobTypeSetup
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, CustomJobTypeSetup );
			hkJobType m_jobType;
			hkUint8 m_jobSubType;
			int m_threadId;
		};
		hkArray<CustomJobTypeSetup> m_customJobSetup;
};

#include <Common/Base/Thread/JobQueue/hkJobQueue.inl>

#endif // HK_BASE_THREAD_JOB_QUEUE_H

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
