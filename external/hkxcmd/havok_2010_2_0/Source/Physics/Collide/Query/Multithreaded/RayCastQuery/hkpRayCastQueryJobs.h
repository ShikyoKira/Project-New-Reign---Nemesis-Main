/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KD_TREE_JOBS_H
#define HK_KD_TREE_JOBS_H


#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Physics/Collide/Shape/Compound/Tree/hkpBvTreeShape.h>

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Collide/hkpCollide.h>
#include <Physics/Collide/Agent/Collidable/hkpCollidable.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>

#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>

#include <Common/Internal/KdTree/Build/hkKdTreeBuilder.h>
#include <Common/Internal/KdTree/Build/hkKdTreeBuildingUtils.h>
#include <Common/Base/hkBase.h>

#include <Physics/Collide/Shape/Query/hkpShapeRayBundleCastInput.h>

#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTree.h>


class hkpBroadPhase;
class hkpWorld;
class hkpTreeWorldManager;

struct hkpWorldRayCastOutput;

struct hkAabbTreeSortData;



#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobs.h>
typedef hkpCollisionQueryJobHeader hkpRayCastQueryJobHeader;


//
// The base class for all collision query jobs
//
struct hkpRayCastQueryJob : public hkJob
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpRayCastQueryJob );

		// Kd tree subtypes start at 128 to keep them separate from other pathfinding jobs
		enum JobSubType
		{
			// Query jobs
			RAYCAST_QUERY_SHAPE_RAYCAST,
			RAYCAST_QUERY_WORLD_RAYCAST,

			// Build jobs
			RAYCAST_QUERY_KD_TREE_BUILD_FAST,
			RAYCAST_QUERY_KD_TREE_BUILD_FAST_DISTRIBUTED_JOB,
			RAYCAST_QUERY_KD_TREE_BUILD_FAST_DISTRIBUTED_SUBJOB,
			RAYCAST_QUERY_KD_TREE_BUILD_SETUP_JOB,

			// Build coordination job, sets up other build jobs
			RAYCAST_QUERY_KD_TREE_BUILD_COORDINATOR_JOB,

 			RAYCAST_QUERY_WORLD_RAYCAST_WITH_AUTOBUNDLE, 

			RAYCAST_QUERY_JOB_END
		};

		void atomicIncrementAndReleaseSemaphore() const;

	protected:

		HK_FORCE_INLINE hkpRayCastQueryJob( JobSubType subType, hkUint16 size );

	public:

			// This semaphore is released once the original job (and all its spawned children) has finished.
		hkSemaphoreBusyWait*			m_semaphore;

			// this header must be set for all jobs that potentially spawn additional jobs or that have been spawned by other jobs
		hkpRayCastQueryJobHeader*		m_sharedJobHeaderOnPpu;

			// The variable at this location will be incremented (atomically) when the job is done.
		hkUint32*						m_jobDoneFlag;

			// Needed by raycast jobs, and by kdtree building
		const hkpProcessCollisionInput*	m_collisionInput;
};


// ===============================================================================================================================================================================================
//  SHAPE RAYCAST
// ===============================================================================================================================================================================================

/// An hkpShapeRayCastCommand can be used to cast exactly one ray against an arbitrary number of collidables. Depending on how many hits you want
/// to be reported you have to supply a large enough m_results output array. Once this array has reached its capacity, the furthest
/// hit will be dropped.
struct hkpShapeRayCastCommand
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpShapeRayCastCommand );

		enum { MAXIMUM_RESULTS_CAPACITY = 96 };	// the capacity has to be limited as we need to allocate this array on the SPU stack

		enum { MAXIMUM_NUM_COLLIDABLES = 64 };		// the maximum number of collidables to cast the ray against has to be limited as we need to allocate a hkpCollidable pointer array on the SPU stack

	public:

		// ===================================================================
		// Input
		// ===================================================================

			/// The ray's input data.
		hkpShapeRayCastInput	m_rayInput;

			/// Type of the filter referenced by m_rayInput.m_rayShapeCollectionFilter.
			/// Must be assigned in order for the filter to work on SPU.
		hkUint32				m_filterType; // type is: hkpFilterType

			/// Size of the filter referenced by m_rayInput.m_rayShapeCollectionFilter.
			/// Must be assigned in order for the filter to work on SPU.
		hkInt32					m_filterSize;

			/// Pointer to an array of hkpCollidable pointers. The ray will be cast against these collidables.
			/// This array has to be 16byte aligned.
			/// PlayStation(R)3 note: this array will be DMAd to SPU and therefore must not be allocated on PPU stack.
		const hkpCollidable**	m_collidables;

			/// Number of collidables in the m_collidables array.
		int						m_numCollidables;

		// ===================================================================
		// Output
		// ===================================================================

			/// Pointer to a hkpShapeRayCastOutput array. The user has to pre-allocate this manually. Once the job has finished, this array will hold the results.
			/// PlayStation(R)3 note: this array will be DMAd from SPU and therefore must not be allocated on PPU stack.
		hkpWorldRayCastOutput*	m_results;

			/// The maximum number of results pre-allocated in m_results.
		int						m_resultsCapacity;

			/// The number of results. Remains untouched until the command has been finished.
			/// You can use this to check manually whether the command already has been completed.
		int						m_numResultsOut;

			/// When a collector is not used, only one point per hkpCollidable can be returned.
		hkBool					m_useCollector;
};


/// An hkpShapeRayCastJob will take an arbitrary number of hkShapeRayCastCommands and perform the raycasts. The job is able
/// to split itself into two jobs if it holds more commands than the maximum allowed number that can be executed in one go.
/// Jobs will be processed multithreaded (i.e., in parallel by different PPU and/or SPU threads, if available).
struct hkpShapeRayCastJob : public hkpRayCastQueryJob
{
	public:

		friend struct hkpRayCastQueryJobQueueUtils;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpShapeRayCastJob );

		enum { MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK = 90 };

	public:

			/// When creating an hkpShapeRayCastJob you have to pass in an unique jobHeader as well as an array of commands.
			/// The supplied semaphore is released once all commands of this job have been completed and the job has been removed from the job queue.
			/// The number of commands that are grouped into one task is customizable.
			/// PlayStation(R)3 note: the jobHeader and the commandArray will be DMAd to SPU and therefore must not be allocated on PPU stack.
		HK_FORCE_INLINE hkpShapeRayCastJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpShapeRayCastCommand* commandArray, int numCommands, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask = MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK);

			/// Only necessary on PlayStation(R)3.
			/// Use this method to assign this job to be processed on the SPU or PPU. This is automatically set depending on what the job references.
			/// If it references objects which are not supported on the SPU
			/// this function will produce a warning (in debug) and the job will be processed on PPU.
		void setRunsOnSpuOrPpu();

	public:

		HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpShapeRayCastJob& out );

		int								m_numCommandsPerTask;	// maximum # of commands per task; once this limit is breached a subjob is spawned
		const hkpShapeRayCastCommand*	m_commandArray;
		int								m_numCommands;
};


// ===============================================================================================================================================================================================
//  WORLD RAYCAST
// ===============================================================================================================================================================================================

/// An hkpWorldRayCastCommand can be used to cast exactly one ray through the broadphase. Depending on how many hits you want
/// to be reported you have to supply a large enough m_results output array. Once this array has reached its capacity, the furthest
/// hit will be dropped.
/// Performance note: when supplying a m_results array-size of exactly 1, the broadphase will use an early-out algorithm to significantly
/// speedup things. With an array-size > 1 this speedup will be lost.
struct hkpWorldRayCastCommand
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpWorldRayCastCommand );

		enum { MAXIMUM_RESULTS_CAPACITY = 32 };		// the capacity has to be limited as we need to allocate this array on the SPU stack

	public:
		hkpWorldRayCastCommand(){m_useCollector = false; m_stopAfterFirstHit = false;}

		// ===================================================================
		// Input
		// ===================================================================

			/// The ray's input data.
		hkpWorldRayCastInput	m_rayInput;

		// ===================================================================
		// Output
		// ===================================================================

			/// Pointer to a hkpWorldRayCastOutput array. The user has to pre-allocate this manually. Once the job has finished, this array will hold the results.
			/// PlayStation(R)3 note: this array will be DMAd from SPU and therefore must not be allocated on PPU stack.
			/// If m_stopAfterFirstHit is set m_results->m_hitFraction does not work(just set to 0 when hit)
		hkpWorldRayCastOutput*	m_results;

			/// The maximum number of results pre-allocated in m_results.
		int						m_resultsCapacity;

			/// The number of results.
			/// This value is only valid after the job's semaphore has been released.
		int						m_numResultsOut;

			/// When a collector is not used, only one point per hkpCollidable can be returned.
			/// When treeManager is set to the job, this is ignored
		hkBool					m_useCollector;

		/// Whether or not the raycast should terminate after the first (not necessarily closest) hit
		/// This can speed up queries such as line-of-sight checks, when you only care if the path is clear or not.
		/// If this is set m_results->m_hitFraction does not work(just set to 0 when hit)
		hkBool					m_stopAfterFirstHit;
};

struct hkpWorldRayCastBundleCommand
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpWorldRayCastBundleCommand );

public:

	// ===================================================================
	// Input
	// ===================================================================

	/// The rays' input data.
	hkpWorldRayCastInput	m_rayInput[4];

	/// Rays have to filled from the start
	int						m_numActiveRays;

	/// Whether or not the raycast should terminate after the first (not necessarily closest) hit
	/// This can speed up queries such as line-of-sight checks, when you only care if the path is clear or not.
	hkBool					m_stopAfterFirstHit;

	// ===================================================================
	// Output
	// ===================================================================

	/// Pointer to a hkpWorldRayCastOutput array. The user has to pre-allocate this manually. Once the job has finished, this array will hold the results.
	/// PlayStation(R)3 note: this array will be DMAd from SPU and therefore must not be allocated on PPU stack.
	hkpWorldRayCastOutput*	m_results[4];

	/// The number of results.
	/// This value is only valid after the job's semaphore has been released.
	HK_ALIGN16(int			m_numResultsOut[4]);

	//	for broadphase implementation
	hkBool					m_useCollector;
};


/// An hkpWorldRayCastJob will take an arbitrary number of hkWorldRayCastCommands and perform the raycasts. The job is able
/// to split itself into two jobs if it holds more commands than the maximum allowed number that can be executed in one go.
/// Jobs will be processed multithreaded (i.e., in parallel by different PPU and/or SPU threads, if available).
struct hkpWorldRayCastJob : public hkpRayCastQueryJob
{
	public:

		friend struct hkpRayCastQueryJobQueueUtils;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpWorldRayCastJob );

		enum { MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK = 64 };

	public:

			/// When creating an hkpWorldRayCastJob you have to pass in an unique jobHeader as well as an array of commands.
			/// The supplied broadphase is used to limit the number of possible object pairs and thus helps increasing performance.
			/// The supplied semaphore is released once all commands of this job have been completed and the job has been removed from the job queue.
			/// The number of commands that are grouped into one task is customizable.
			/// PlayStation(R)3 note: the jobHeader and the commandArray will be DMAd to SPU and therefore must not be allocated on PPU stack.
		HK_FORCE_INLINE hkpWorldRayCastJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpWorldRayCastCommand* commandArray, int numCommands, const hkpBroadPhase* broadphase, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask = MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK);

			/// Only necessary on PlayStation(R)3.
			/// Use this method to assign this job to be processed on the SPU or PPU. This is automatically set depending on what the job references.
			/// If it references objects which are not supported on the SPU
			/// this function will produce a warning (in debug) and the job will be processed on PPU.
		void setRunsOnSpuOrPpu();

	protected:

		HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpWorldRayCastJob& out );

	public:

			// Inputs
		int								m_numCommandsPerTask;	// maximum # of commands per task; once this limit is breached a subjob is spawned
		const hkpWorldRayCastCommand*	m_commandArray;
		int								m_numCommands;

		//	if broadphase is used, these bundles are calculated one by one like single commands
		const hkpWorldRayCastBundleCommand* m_bundleCommandArray;
		int									m_numBundleCommands;

		const hkpBroadPhase*											m_broadphase;
		const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb, hkUint16>*		m_aabbTreeFp;
		const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*	m_aabbTreeInt16;
		const hkKdTree*													m_kdTree;
};


// ===============================================================================================================================================================================================
//  KD-TREE WORLD RAYCAST
// ===============================================================================================================================================================================================


/// An hkKdTreeRayCastBundleCommand can be used to cast four rays through the scene. Only the closest hit for each ray is kept.
/// Bundling the rays together can be significantly faster than casting them individually, as long as they are close together.
struct hkKdTreeRayCastBundleCommand
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkKdTreeRayCastBundleCommand );

public:

	// ===================================================================
	// Input
	// ===================================================================
		/// The rays' input data.
	hkpWorldRayCastInput	m_rayInput[4];

		/// Whether or not the raycast should terminate after the first (not necessarily closest) hit
		/// This can speed up queries such as line-of-sight checks, when you only care if the path is clear or not.
	hkBool					m_stopAfterFirstHit;

	// ===================================================================
	// Output
	// ===================================================================

		/// Pointer to a hkpWorldRayCastOutput array. The user has to pre-allocate this manually. Once the job has finished, this array will hold the results.
		/// PlayStation(R)3 note: this array will be DMAd from SPU and therefore must not be allocated on PPU stack.
		/// The size of this array is assumed to be 4.
	hkpWorldRayCastOutput*	m_results;

		/// The number of results.
		/// This value is only valid after the job's semaphore has been released.
	HK_ALIGN16(int			m_numResultsOut[4]);
};


// ===============================================================================================================================================================================================
//   KD-TREE BUILDING
// ===============================================================================================================================================================================================

struct hkpKdTreeBuildJobFast : public hkpRayCastQueryJob
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildJobFast );

	HK_FORCE_INLINE hkpKdTreeBuildJobFast();
	HK_FORCE_INLINE hkpKdTreeBuildJobFast(hkKdTree* tree);


	///// Use this method to find out onto which queue the job is supposed to go and pass the returned value to the queue's addJob() function.
	//hkJobQueue::JobType getJobType();

	
	int								m_numCollidables;

	class hkKdTreeNode*				m_treeOut;
	struct hkKdTreeProjectedEntry*	m_projEntOut;

	int*							m_treeDepthOut;

	int								m_nodeArraySize;
	int								m_numAvailableEmptyNodes;

	hkKdTreeProjectedEntry*			m_entriesIn;
	hkKdTreeProjectedEntry*			m_entriesOut;
};

struct hkpKdTreeBuildDistributedJob : public hkpRayCastQueryJob
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildDistributedJob );

	
	/// PlayStation(R)3 note: the jobHeader and the m_colldiables and m_treeOut will be DMAd to SPU and therefore must not be allocated on PPU stack.
	HK_FORCE_INLINE hkpKdTreeBuildDistributedJob(hkpRayCastQueryJobHeader* jobHeader);
	HK_FORCE_INLINE hkpKdTreeBuildDistributedJob(hkpRayCastQueryJobHeader* jobHeader, hkKdTree* tree);


	///// Use this method to find out onto which queue the job is supposed to go and pass the returned value to the queue's addJob() function.
	//hkJobQueue::JobType getJobType();


	hkKdTreeProjectedEntry*			m_primitivesIn;		// Should point to an aligned buffer in main memory of size numCollidables * sizeof(KdTreeBuilder::ProjectedEntry) 
	hkKdTreeProjectedEntry*			m_primitivesOut;	// Should point to an aligned buffer in main memory of size numCollidables * sizeof(KdTreeBuilder::ProjectedEntry) 
	
	class hkKdTreeNode*				m_treeOut;
	struct hkKdTreeProjectedEntry*	m_projEntOut;

	// depths of the 4 distributed branches
	int*							m_branchDepthsOut;
	int*							m_subJobSizes;

	int								m_numPrimitives;
	int								m_nodeArraySize;
	int								m_numAvailableEmptyNodes;
};

// Creates 4 hkKdTreeBuildSetupJobs
// The last hkpKdTreeBuildSetupJob to finish creates an hkpKdTreeBuildDistributedJob and adds it to the queue
// The hkpKdTreeBuildDistributedJob creates 4 hkKdTreeBuildDistributedSubJobs
struct hkpKdTreeBuildCoordinatorJob: public hkpKdTreeBuildDistributedJob
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildCoordinatorJob );

	HK_FORCE_INLINE hkpKdTreeBuildCoordinatorJob(hkpRayCastQueryJobHeader* jobHeader, const hkpWorld* world);

	// Needed for hkpKdTreeBuildSetupJob
	// hkKdTreeBuildSetupJobs will use hkpKdTreeBuildCoordinatorJob::m_primitivesIn for their m_entriesOut
	const hkpCollidable* const*	m_collidables;
};

struct hkpKdTreeBuildSetupJob : public hkpKdTreeBuildDistributedJob
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildSetupJob );

	HK_FORCE_INLINE hkpKdTreeBuildSetupJob();
	HK_FORCE_INLINE ~hkpKdTreeBuildSetupJob() {}


	///// Use this method to find out onto which queue the job is supposed to go and pass the returned value to the queue's addJob() function.
	//hkJobQueue::JobType getJobType();


	const hkpCollidable* const*	m_collidables;
	hkKdTreeProjectedEntry* m_entriesOut;

	int						m_startIdx;
	int						m_endIdx;

	// Whether or not to spawn a new job when the last hkpKdTreeBuildSetupJob finishes
	int						m_spawnBuildDistributedJob;
};

// This job is spawned internally by the hkpKdTreeBuildDistributedJob
struct hkpKdTreeBuildDistributedSubJob : public hkpRayCastQueryJob
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeBuildDistributedSubJob );

	HK_FORCE_INLINE hkpKdTreeBuildDistributedSubJob();


	///// Use this method to find out onto which queue the job is supposed to go and pass the returned value to the queue's addJob() function.
	//hkJobQueue::JobType getJobType();


	class hkKdTreeNode*				m_treeOut;
	struct hkKdTreeProjectedEntry*	m_projEntOut;
	int								m_currentNodeIdx;
	hkKdTreeProjectedEntry*			m_primitivesIn;	
	hkKdTreeProjectedEntry*			m_primitivesOut;
	int								m_startPrimIdx;
	int								m_endPrimIdx;
	hkKdTreeProjectedEntry			m_bounds;
	int*							m_branchDepthOut;

	int								m_nodeArraySize;
	int								m_numAvailableEmptyNodes;
}; 

struct hkpWorldRayCastWithAutoBundleJob : public hkpRayCastQueryJob
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpWorldRayCastWithAutoBundleJob );

	HK_FORCE_INLINE hkpWorldRayCastWithAutoBundleJob() : hkpRayCastQueryJob(RAYCAST_QUERY_WORLD_RAYCAST_WITH_AUTOBUNDLE, sizeof(hkpWorldRayCastWithAutoBundleJob) ){ m_aabbTreeFp = HK_NULL; m_aabbTreeInt16 = HK_NULL;}

	int*					m_rayIndices;
	int						m_octantIdx;
	int						m_numIndices;
	const hkVector4*		m_rayFrom;
	const hkVector4*		m_rayTo;
	int						m_numRays;
	hkReal					m_angleVariance;
	hkReal					m_distanceVariance2;
	hkpWorldRayCastOutput*	m_resultBuffer;

	hkpWorldRayCastCommand*			m_singleCommands;
	int								m_numSingleCommands;
	hkpWorldRayCastBundleCommand*	m_bundledCommands;	
	int								m_numBundledCommands;

	const hkpProcessCollisionInput*									m_collisionInput;
	hkpCollisionQueryJobHeader*										m_jobHeader;
	hkSemaphoreBusyWait*											m_semaphore;
	const hkpBroadPhase*											m_broadPhase;
	const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb, hkUint16>*		m_aabbTreeFp;
	const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*	m_aabbTreeInt16;
	const hkKdTree*													m_kdTree;
};

inline void hkSetupKdTreeBuildSubJob( const hkpKdTreeBuildDistributedJob& jobIn, const hkKdTreeBuildInput::BuildRecursiveInput& splitJob, hkpKdTreeBuildDistributedSubJob& subJobOut )
{
	subJobOut.m_sharedJobHeaderOnPpu	= jobIn.m_sharedJobHeaderOnPpu;
	subJobOut.m_primitivesIn			= splitJob.m_buffers->m_entriesIn;
	subJobOut.m_primitivesOut			= splitJob.m_buffers->m_entriesOut;
	subJobOut.m_currentNodeIdx			= splitJob.m_currentNodeIdx;
	subJobOut.m_nodeArraySize			= splitJob.m_maxNodeIdx;
	subJobOut.m_startPrimIdx			= splitJob.m_startPrimIdx;
	subJobOut.m_endPrimIdx				= splitJob.m_endPrimIdx;
	subJobOut.m_bounds					= splitJob.m_bounds;
	subJobOut.m_semaphore				= jobIn.m_semaphore;
	subJobOut.m_projEntOut				= jobIn.m_projEntOut;
}

// Copy between hkpKdTreeBuildDistributedJob
// Note that the size and subType aren't copied, so that we can go between subclasses
inline void hkInitKdTreeBuildJob( const hkpKdTreeBuildDistributedJob& jobIn, hkpKdTreeBuildDistributedJob& subJobOut)
{
	subJobOut.m_sharedJobHeaderOnPpu	= jobIn.m_sharedJobHeaderOnPpu;
	subJobOut.m_collisionInput			= jobIn.m_collisionInput;
	subJobOut.m_semaphore				= jobIn.m_semaphore;
	subJobOut.m_primitivesIn			= jobIn.m_primitivesIn;
	subJobOut.m_primitivesOut			= jobIn.m_primitivesOut;
	subJobOut.m_numPrimitives			= jobIn.m_numPrimitives;
	subJobOut.m_treeOut					= jobIn.m_treeOut;
	subJobOut.m_projEntOut				= jobIn.m_projEntOut;
	subJobOut.m_branchDepthsOut			= jobIn.m_branchDepthsOut;
	subJobOut.m_nodeArraySize			= jobIn.m_nodeArraySize;
	subJobOut.m_subJobSizes				= jobIn.m_subJobSizes;
	subJobOut.m_numAvailableEmptyNodes	= jobIn.m_numAvailableEmptyNodes;
}


#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.inl>

#endif // HK_COLLISION_JOBS_H

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
