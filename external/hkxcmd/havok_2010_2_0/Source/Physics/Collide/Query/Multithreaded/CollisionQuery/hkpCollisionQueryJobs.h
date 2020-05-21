/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLISION_JOBS_H
#define HK_COLLISION_JOBS_H


#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Physics/Collide/Shape/Compound/Tree/hkpBvTreeShape.h>

#include <Physics/Collide/hkpCollide.h>
#include <Physics/Collide/Agent/Collidable/hkpCollidable.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>

#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTree.h>

#include <Common/Base/hkBase.h>

struct hkpRootCdPoint;
class hkpShapeCollectionFilter;
class hkpBroadPhase;
class hkpWorld;
struct hkpShapeRayCastInput;
struct hkpShapeRayCastOutput;
struct hkpWorldRayCastOutput;
class hkpCollisionFilter;


/// Each collision query job needs an hkpCollisionQueryJobHeader. Usually this is passed in through the job's constructor.
/// PlayStation(R)3 note: this header will be DMAd back and forth between PPU and SPU and therefore must not be allocated on PPU stack.
struct hkpCollisionQueryJobHeader
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCollisionQueryJobHeader );

			// This member can only be accessed if the job queue is locked once its governing job has been added to the job queue.
		HK_ALIGN16( mutable int m_openJobs );
};


//
// The base class for all collision query jobs
// Important: the 16bit m_jobType HAS to be the first member of this class and it HAS to be 16byte aligned! See hkJob for more details.
//
struct hkpCollisionQueryJob : public hkJob
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCollisionQueryJob );

		enum JobSubType
		{
			COLLISION_QUERY_MOPP_AABB,
			COLLISION_QUERY_PAIR_GET_CLOSEST_POINTS,
			COLLISION_QUERY_WORLD_GET_CLOSEST_POINTS,
			COLLISION_QUERY_PAIR_LINEAR_CAST,
			COLLISION_QUERY_WORLD_LINEAR_CAST,
			COLLISION_QUERY_KD_TREE_AABB_QUERY_JOB,
			COLLISION_QUERY_AABB_TREE_AABB_QUERY_JOB,
			COLLISION_QUERY_JOB_END
		};

		void atomicIncrementAndReleaseSemaphore() const;

	protected:

		HK_FORCE_INLINE hkpCollisionQueryJob( JobSubType subType, hkUint16 size );

	public:

			// This semaphore is released once the original job (and all its spawned children) has finished.
		hkSemaphoreBusyWait*			m_semaphore;

			// this header must be set for all jobs that potentially spawn additional jobs or that have been spawned by other jobs
		hkpCollisionQueryJobHeader*		m_sharedJobHeaderOnPpu;

		const hkpProcessCollisionInput*	m_collisionInput;

			// The variable at this location will be incremented (atomically) when the job is done.
		hkUint32*						m_jobDoneFlag;

};



// ===============================================================================================================================================================================================
//   PAIR LINEAR CAST 
// ===============================================================================================================================================================================================

/// An hkpPairLinearCastCommand can be used to calculate the linear casts between two objects.
/// The first object is cast against the second one, with endpoints specified by the user.
/// More than one collision result may return.
struct hkpPairLinearCastCommand
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpPairLinearCastCommand );

		enum { MAXIMUM_RESULTS_CAPACITY = 16 };		// the capacity has to be limited as we need to allocate this array on the SPU stack

	public:

		// ===================================================================
		// Input
		// ===================================================================

			/// The starting point of the linear cast path. The first collidable will move along this path.
		hkVector4				m_from;

			/// The endpoint of the linear cast path. The first collidable will move along this path.
		hkVector4				m_to;

			/// The first collidable will be cast along a provided path against the second collidable.
		const hkpCollidable*	m_collidableA;

			/// The second collidable remains fixed.
		const hkpCollidable*	m_collidableB;

		// ===================================================================
		// Output
		// ===================================================================

			/// Pointer to an hkpRootCdPoint array. The user has to pre-allocate this manually. Once the job has finished, this array will hold the results.
			/// Note that the results will be arbitrarily placed in the results array (i.e., without any specific order).
			/// PlayStation(R)3 note: this array will be DMAd from SPU and therefore must not be allocated on PPU stack.
		hkpRootCdPoint*	m_results;

			/// The maximum number of results pre-allocated in m_results.
		int				m_resultsCapacity;

			/// The number of results.
			/// This value is only valid after the job's semaphore has been released.
		int				m_numResultsOut;

		hkpRootCdPoint*	m_startPointResults;
		int				m_startPointResultsCapacity;
		int				m_startPointNumResultsOut;
};


/// An hkpPairLinearCastJob will take an arbitrary number of hkpPairLinearCastCommand and calculate the linear casts between the specified pairs. The job is able
/// to split itself into two jobs if it holds more commands than the maximum allowed number that can be executed in one go. Jobs will be processed multithreaded
/// (i.e., in parallel by different PPU and/or SPU threads, if available).
struct hkpPairLinearCastJob : public hkpCollisionQueryJob
{
	public:

		friend struct hkpCollisionQueryJobQueueUtils;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpPairLinearCastJob );

		enum { MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK = 128 };

	public:

			/// When creating an hkpPairLinearCastJob you have to pass in an unique jobHeader as well as an array of commands.
			/// Only points closer than the supplied tolerance are reported.
			/// The supplied filter is used if any shape collections are queried.
			/// The supplied semaphore is released once all commands of this job have been completed and the job has been removed from the job queue.
			/// The number of commands that are grouped into one task is customizable.
			/// PlayStation(R)3 note: the jobHeader and the commandArray will be DMAd to SPU and therefore must not be allocated on PPU stack.
		HK_FORCE_INLINE hkpPairLinearCastJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpPairLinearCastCommand* commandArray, int numCommands, const hkpShapeCollectionFilter* filter, hkReal tolerance, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask = MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK);

			/// This constructor is for internal use only! Please use the other constructor.
		HK_FORCE_INLINE hkpPairLinearCastJob();

			/// Only necessary on PlayStation(R)3.
			/// Use this method to assign this job to be processed on the SPU or PPU. This is automatically set depending on what the job references.
			/// If it references objects which are not supported on the SPU
			/// this function will produce a warning (in debug) and the job will be processed on PPU.
		void setRunsOnSpuOrPpu();

	protected:

		HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpPairLinearCastJob& out );

	public:

		hkReal							m_tolerance;

		// Linear cast tolerances
		hkReal							m_maxExtraPenetration;
		hkReal							m_iterativeLinearCastEarlyOutDistance;	// For iterative linear cast only
		int								m_iterativeLinearCastMaxIterations;

		int								m_numCommandsPerTask;	// maximum # of commands per task; once this limit is breached a subjob is spawned
		const hkpPairLinearCastCommand*	m_commandArray;			
		int								m_numCommands;		
};


// ===============================================================================================================================================================================================
//  WORLD LINEAR CAST
// ===============================================================================================================================================================================================

/// An hkpWorldLinearCastCommand can be used to cast exactly one collidable through the broadphase. Depending on how many hits you want
/// to be reported you have to supply a large enough m_results output array. Once this array has reached its capacity, the furthest
/// hit will be dropped.
/// Performance note: when supplying a m_results array-size of exactly 1, the broadphase will use an early-out algorithm to significantly
/// speedup things. With an array-size > 1 this speedup will be lost.
struct hkpWorldLinearCastCommand
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpWorldLinearCastCommand );

		enum { MAXIMUM_RESULTS_CAPACITY = 16 };		// the capacity has to be limited as we need to allocate this array on the SPU stack

	public:

		// ===================================================================
		// Input
		// ===================================================================

			/// The linear cast input data.
		hkpLinearCastInput		m_input;

			/// This collidable will be cast along the provided path against.
		const hkpCollidable*	m_collidable;

		// ===================================================================
		// Output
		// ===================================================================

			/// Pointer to an hkpRootCdPoint array. The user has to pre-allocate this manually. Once the job has finished, this array will hold the results.
			/// Note that the results in this array will be only grouped on an object-pair-basis, but for one object pair they are arbitrarily placed in the results array (i.e., without any specific order).
			/// PlayStation(R)3 note: this array will be DMAd from SPU and therefore must not be allocated on PPU stack.
		hkpRootCdPoint*	m_results;

			/// The maximum number of results pre-allocated in m_results.
		int				m_resultsCapacity;

			/// The number of results.
			/// This value is only valid after the job's semaphore has been released.
		int				m_numResultsOut;
};


/// An hkpWorldLinearCastJob will take an arbitrary number of hkWorldLinearCastCommands and perform the linear casts. The job is able
/// to split itself into two jobs if it holds more commands than the maximum allowed number that can be executed in one go.
/// Jobs will be processed multithreaded (i.e., in parallel by different PPU and/or SPU threads, if available).
struct hkpWorldLinearCastJob : public hkpCollisionQueryJob
{
	public:

		friend struct hkpCollisionQueryJobQueueUtils;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpWorldLinearCastJob );

		enum { MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK = 64 };

	public:

			/// When creating an hkpWorldLinearCastJob you have to pass in an unique jobHeader as well as an array of commands.
			/// The supplied broadphase is used to limit the number of possible object pairs and thus helps increasing performance.
			/// The supplied semaphore is released once all commands of this job have been completed and the job has been removed from the job queue.
			/// The number of commands that are grouped into one task is customizable.
			/// PlayStation(R)3 note: the jobHeader and the commandArray will be DMAd to SPU and therefore must not be allocated on PPU stack.
		HK_FORCE_INLINE hkpWorldLinearCastJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpWorldLinearCastCommand* commandArray, int numCommands, const hkpBroadPhase* broadphase, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask = MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK);

			/// Only necessary on PlayStation(R)3.
			/// Use this method to assign this job to be processed on the SPU or PPU. This is automatically set depending on what the job references.
			/// If it references objects which are not supported on the SPU
			/// this function will produce a warning (in debug) and the job will be processed on PPU.
		void setRunsOnSpuOrPpu();

	protected:

		HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpWorldLinearCastJob& out );

	public:

			// Inputs
		int									m_numCommandsPerTask;	// maximum # of commands per task; once this limit is breached a subjob is spawned
		const hkpWorldLinearCastCommand*	m_commandArray;
		int									m_numCommands;

		const hkpBroadPhase*											m_broadphase;
		const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb, hkUint16>*		m_aabbTree;
		const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*	m_aabbTreeInt16;
		const hkKdTree*													m_kdTree;
};


// ===============================================================================================================================================================================================
//  MOPP AABB
// ===============================================================================================================================================================================================

/// An hkpMoppAabbCommand can be used to cast exactly one AABB cast against the job's global MOPP. It will return a maximum of MAX_OUTPUT_KEYS_PER_QUERY
/// results. The last valid results in this array will be followed by an HK_INVALID_SHAPE_KEY.
struct hkpMoppAabbCommand
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpMoppAabbCommand );

		enum { MAX_OUTPUT_KEYS_PER_QUERY = HK_MAX_NUM_HITS_PER_AABB_QUERY };

		typedef hkpShapeKey QueryOutput[MAX_OUTPUT_KEYS_PER_QUERY];

	public:

		// ===================================================================
		// Input
		// ===================================================================

			/// The AABB's input data.
		hkAabb			m_aabbInput;

		// ===================================================================
		// Output
		// ===================================================================

			/// Pointer to an hkpShapeKey array.
			/// PlayStation(R)3 note: this array will be DMAd from SPU and therefore must not be allocated on PPU stack.
		QueryOutput*	m_results;
};


/// An hkpMoppAabbJob will take an arbitrary number of hkMoppAabbCommands and perform the AABB casts on the job's MOPP. The job is able
/// to split itself into two jobs if it holds more commands than the maximum allowed number that can be executed in one go.
/// Jobs will be processed multithreaded (i.e., in parallel by different PPU and/or SPU threads, if available).
struct hkpMoppAabbJob : public hkpCollisionQueryJob
{
	public:

		friend struct hkpCollisionQueryJobQueueUtils;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpMoppAabbJob );

		enum { MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK = 256 };

	public:

			/// When creating an hkpMoppAabbJob you have to pass in an unique jobHeader as well as an array of commands.
			/// All the job's commands are executed on the same MOPP code and info.
			/// The supplied semaphore is released once all commands of this job have been completed and the job has been removed from the job queue.
			/// The number of commands that are grouped into one task is customizable.
			/// PlayStation(R)3 note: the jobHeader and the commandArray will be DMAd to SPU and therefore must not be allocated on PPU stack.
		HK_FORCE_INLINE hkpMoppAabbJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpMoppAabbCommand* commandArray, int numCommands, const hkUint8* moppCodeData, const hkpMoppCode::CodeInfo& moppCodeInfo, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask = MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK);

	protected:

		HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpMoppAabbJob& out );

	public:

		hkpMoppCode::CodeInfo		m_moppCodeInfo;			// MOPP code info
		const hkUint8*				m_moppCodeData;			// MOPP code data

		int							m_numCommandsPerTask;	// maximum # of commands per task; once this limit is breached a subjob is spawned
		const hkpMoppAabbCommand*	m_commandArray;
		int							m_numCommands;
};


// ===============================================================================================================================================================================================
//  PAIR GET CLOSEST POINTS
// ===============================================================================================================================================================================================

/// An hkpPairGetClosestPointsCommand can be used to calculate the shortest distances (i.e., closest points) between two objects. Depending on the
/// objects' shapes, more than 1 distance can exist. Depending on how many distances you want reported you have to supply a large
/// enough m_results output array. Once this array is full, every time a closer point is added the currently longest distance will be dropped.
HK_CLASSALIGN16(struct) hkpPairGetClosestPointsCommand
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpPairGetClosestPointsCommand );

		enum { MAXIMUM_RESULTS_CAPACITY = 16 };		// the capacity has to be limited as we need to allocate this array on the SPU stack

	public:

		// ===================================================================
		// Input
		// ===================================================================

			/// The first of the two collidables for which we would like to find the closest point between.
		const hkpCollidable*	m_collidableA;

			/// The second of the two collidables for which we would like to find the closest point between.
		const hkpCollidable*	m_collidableB;

		// ===================================================================
		// Output
		// ===================================================================

			/// Pointer to an hkpRootCdPoint array. The user has to pre-allocate this manually. Once the job has finished, this array will hold the results.
			/// Note that the results will be arbitrarily placed in the results array (i.e., without any specific order).
			/// PlayStation(R)3 note: this array will be DMAd from SPU and therefore must not be allocated on PPU stack.
		hkpRootCdPoint*	m_results;

			/// The maximum number of results pre-allocated in m_results.
		int				m_resultsCapacity;

			/// The number of results.
			/// This value is only valid after the job's semaphore has been released.
		int				m_numResultsOut;

			/// This variable is set automatically. Do not touch.
			// Automatically set to HK_NULL if m_results is private to this command.
			// If m_results is shared between multiple hkPairGetClosestPointsCommands this variable points to an index into m_results. Currently this is only the case for 
			// hkPairGetClosestPointsCommands that have been created by an hkpWorldGetClosestPointsJob and thus points to hkpWorldGetClosestPointsCommand::m_numResultsOut.
		hkUint32*		m_indexIntoSharedResults;
};


/// An hkpPairGetClosestPointsJob will take an arbitrary number of hkpPairGetClosestPointsCommand and calculate the shortest distances between the specified pairs. The job is able
/// to split itself into two jobs if it holds more commands than the maximum allowed number that can be executed in one go. Jobs will be processed multithreaded
/// (i.e., in parallel by different PPU and/or SPU threads, if available).
struct hkpPairGetClosestPointsJob : public hkpCollisionQueryJob
{
	public:

		friend struct hkpCollisionQueryJobQueueUtils;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpPairGetClosestPointsJob );

		enum { MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK = 256 };

	public:

			/// When creating an hkpPairGetClosestPointsJob you have to pass in an unique jobHeader as well as an array of commands.
			/// Only closest points with a distance of the supplied tolerance (or smaller) are reported.
			/// The supplied semaphore is released once all commands of this job have been completed and the job has been removed from the job queue.
			/// The number of commands that are grouped into one task is customizable.
			/// PlayStation(R)3 note: the jobHeader and the commandArray will be DMAd to SPU and therefore must not be allocated on PPU stack.
		HK_FORCE_INLINE hkpPairGetClosestPointsJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpPairGetClosestPointsCommand* commandArray, int numCommands, hkReal tolerance, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask = MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK);

			/// This constructor is for internal use only! Please use the other constructor.
		HK_FORCE_INLINE hkpPairGetClosestPointsJob();

			/// Only necessary on PlayStation(R)3.
			/// Use this method to assign this job to be processed on the SPU or PPU. This is automatically set depending on what the job references.
			/// If it references objects which are not supported on the SPU
			/// this function will produce a warning (in debug) and the job will be processed on PPU.
		void setRunsOnSpuOrPpu();

	protected:

		HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpPairGetClosestPointsJob& out );

	public:

		hkReal									m_tolerance;

		int										m_numCommandsPerTask;	// maximum # of commands per task; once this limit is breached a subjob is spawned
		const hkpPairGetClosestPointsCommand*	m_commandArray;
		int										m_numCommands;
};


// ===============================================================================================================================================================================================
//  WORLD GET CLOSEST POINTS
// ===============================================================================================================================================================================================

/// An hkpWorldGetClosestPointsCommand can be used to calculate the shortest distances (i.e., closest points) between the supplied object and all its surrounding objects.
/// Processing this command will spawn hkPairGetClosestPointsJobs, so see hkpPairGetClosestPointsJob for more information.
struct hkpWorldGetClosestPointsCommand
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpWorldGetClosestPointsCommand );

	public:

		// ===================================================================
		// Input
		// ===================================================================

			/// We want the closest points all around this particular collidable.
		const hkpCollidable*	m_collidable;

		// ===================================================================
		// Output
		// ===================================================================

			/// Pointer to an hkpRootCdPoint array. The user has to pre-allocate this manually. Once the job has finished, this array will hold the results.
			/// Note that the results in this array will be only grouped on an object-pair-basis, but for one object pair they are arbitrarily placed in the results array (i.e., without any specific order).
			/// PlayStation(R)3 note: this array will be DMAd from SPU and therefore must not be allocated on PPU stack.
		hkpRootCdPoint*			m_results;

			/// The maximum number of results pre-allocated in m_results.
		int						m_resultsCapacity;

			/// The number of results.
			/// This value is only valid after the job's semaphore has been released.
		hkUint32				m_numResultsOut;

};


/// An hkpWorldGetClosestPointsJob will take a limited number of hkWorldGetClosestPointsCommands and calculate the shortest distances between
/// the specified objects and their surroundings.
struct hkpWorldGetClosestPointsJob : public hkpCollisionQueryJob
{
	public:

		friend struct hkpCollisionQueryJobQueueUtils;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpWorldGetClosestPointsJob );

			// The maximum number of commands that can be processed by one single hkpWorldGetClosestPointsJob
		enum { MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK = 64 };

		enum { MAXIMUM_RESULTS_CAPACITY = 16 };		// the capacity has to be limited as we need to allocate this array on the SPU stack

	public:

			/// When creating an hkpWorldGetClosestPointsJob you have to pass in an unique jobHeader as well as an array of commands.
			/// The supplied broadphase is used to limit the number of possible object pairs and thus helps increasing performance.
			/// Only closest points for object pairs with a distance of the supplied tolerance (or smaller) are reported.
			/// The supplied semaphore is released once all commands of this job have been completed and the job has been removed from the job queue.
			/// PlayStation(R)3 note: the jobHeader and the commandArray will be DMAd to SPU and therefore must not be allocated on PPU stack.
		HK_FORCE_INLINE hkpWorldGetClosestPointsJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpWorldGetClosestPointsCommand* commandArray, int numCommands, const hkpBroadPhase* broadphase, hkReal tolerance, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask = MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK );

			/// Only necessary on PlayStation(R)3.
			/// Use this method to assign this job to be processed on the SPU or PPU. This is automatically set depending on what the job references.
			/// If it references objects which are not supported on the SPU
			/// this function will produce a warning (in debug) and the job will be processed on PPU.
		void setRunsOnSpuOrPpu();

	protected:

		HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpWorldGetClosestPointsJob& out );

	public:

		const hkpBroadPhase*					m_broadphase;
		const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*	m_aabbTreeInt16;
		const hkKdTree*			m_kdTree;
		hkReal									m_tolerance;

		int										m_numCommandsPerTask;	// maximum # of commands per task; once this limit is breached a subjob is spawned

		const hkpWorldGetClosestPointsCommand*	m_commandArray;
		int										m_numCommands;

};


// ===============================================================================================================================================================================================
//   KD-TREE AABB QUERY
// ===============================================================================================================================================================================================

struct hkpKdTreeAabbCommand
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeAabbCommand );

	// Input
	hkAabb	m_aabb;
	int		m_resultsCapacity;

	// Ouput
	hkPrimitiveId*	m_results;
	int				m_numResultsOut;
};

struct hkpKdTreeAabbJob : public hkpCollisionQueryJob
{

public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpKdTreeAabbJob );
	enum { MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK = 256 };

public:

	HK_FORCE_INLINE hkpKdTreeAabbJob(hkpCollisionQueryJobHeader* jobHeader, hkpKdTreeAabbCommand* commandArray, int numCommands, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask = MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK);

	HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpKdTreeAabbJob& out );

public:

	enum {MAX_NUM_TREES = 4};

	// Inputs
	int						m_numCommandsPerTask;	// maximum # of commands per task; once this limit is breached a subjob is spawned
	hkpKdTreeAabbCommand*	m_aabbCommandArray;
	int						m_numAabbCommands;

	int						m_numTrees;
	const hkKdTree*			m_trees[MAX_NUM_TREES];

};

struct hkpAabbTreeAabbCommand
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpAabbTreeAabbCommand );

	hkAabb	m_aabb;
	int		m_resultsCapacity;

	void**	m_results;
	int		m_numResultsOut;
};

struct hkpAabbTreeAabbJob : public hkpCollisionQueryJob
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpAabbTreeAabbJob );

		hkpAabbTreeAabbJob() : hkpCollisionQueryJob(COLLISION_QUERY_AABB_TREE_AABB_QUERY_JOB, sizeof(hkpAabbTreeAabbJob) )
		{
			m_aabbTreeFp = HK_NULL;
			m_aabbTreeInt16 = HK_NULL;
			m_kdTree = HK_NULL;
			m_jobSpuType = HK_JOB_SPU_TYPE_ENABLED;
		}

		const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb, hkUint16>*		m_aabbTreeFp;
		const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*	m_aabbTreeInt16;
		const hkKdTree*													m_kdTree;

		hkpAabbTreeAabbCommand*	m_aabbCommandArray;
		int						m_numAabbCommands;
};

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobs.inl>

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
