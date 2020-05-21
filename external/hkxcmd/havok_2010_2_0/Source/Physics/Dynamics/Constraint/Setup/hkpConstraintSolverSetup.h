/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_CONSTRAINT_SOLVER_SETUP_H
#define HK_DYNAMICS2_CONSTRAINT_SOLVER_SETUP_H

//class SolverInfo;
class hkpConstraintInstance;
class hkpEntity;
class hkStepInfo;
class hkpVelocityAccumulator;
class hkpSimulationIsland;
class hkpVelocityAccumulator;


#include <Common/Base/DebugUtil/DeterminismUtil/hkCheckDeterminismUtil.h>
#include <Physics/ConstraintSolver/Solve/hkpSolve.h>
#include <Physics/ConstraintSolver/Constraint/hkpConstraintQueryIn.h>


struct hkpConstraintSchemaInfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintSchemaInfo );

	hkpConstraintInstance*     m_constraint;
	hkpJacobianSchema* m_schema;
	hkReal            m_allowedPenetrationDepth;
};

struct hkpBuildJacobianTaskHeader;

	// setting this value so that hkpBuildJacobianTask ~< 2^x
struct hkpBuildJacobianTask
{
#if (HK_POINTER_SIZE == 4)
		enum { MAX_NUM_ATOM_INFOS = 144 }; // For testing this should be reduced to e.g. 2
#else
		enum { MAX_NUM_ATOM_INFOS = 80 }; // Might need to be corrected
#endif

	struct AtomInfo
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpBuildJacobianTask::AtomInfo );

		// atom ptr
		HK_CPU_PTR(hkpConstraintAtom*)	    m_atoms;
		HK_CPU_PTR(hkpConstraintInstance*)	m_instance;
		HK_CPU_PTR(hkpConstraintRuntime*)	m_runtime;
		HK_CPU_PTR(const hkTransform*)		m_transformA;
		HK_CPU_PTR(const hkTransform*)		m_transformB;

		// atom size -- can be hkUint16 as long as we don't do chains in Spu's
		hkUint16 m_atomsSize;

		hkUint16 m_runtimeSize;

		// accumulator indices --  still needed by build jacobians, but might be replaced by the alternative offsets below (for _platform_has_spu)
		// these indices are used for building jacobians
		hkUint16 m_accumulatorIndexA;
		hkUint16 m_accumulatorIndexB;

		// thes indices are not used for buliding jacobians and replace the above indices for solving, if they're defined
		HK_ON_PLATFORM_HAS_SPU( hkUint16 m_accumulatorInterIndexA );
		HK_ON_PLATFORM_HAS_SPU( hkUint16 m_accumulatorInterIndexB );
	};

	inline hkpBuildJacobianTask();

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpBuildJacobianTask );
	HK_ALIGN128( HK_CPU_PTR(hkpBuildJacobianTask*) m_next );

	HK_CPU_PTR(hkpBuildJacobianTaskHeader*)	m_taskHeader;

	HK_CPU_PTR(const hkpVelocityAccumulator*)m_accumulators;
	HK_CPU_PTR(hkpJacobianSchema*)			m_schemas;
	HK_CPU_PTR(hkpJacobianSchema*)			m_schemasOfNextTask;

	HK_ON_PLATFORM_HAS_SPU( int m_onPpuOnly );

	int m_numAtomInfos;
	AtomInfo m_atomInfos[MAX_NUM_ATOM_INFOS];
};

struct hkpBuildJacobianTaskCollection
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpBuildJacobianTaskCollection );

	hkpBuildJacobianTaskCollection()
	{
		m_buildJacobianTasks = HK_NULL;
		m_numBuildJacobianTasks = 0;
		m_callbackConstraints = HK_NULL;
		m_numCallbackConstraints = 0;
	}

	// this is a list to the tasks (formerly 'batches') which can be executed by the solver
	// during buildJacobiansTasks this variable is owned and created by the thread which initiated the buildJacobiansTasks.
	// This variable is read only once initialized
	HK_CPU_PTR(struct hkpBuildJacobianTask*) m_buildJacobianTasks;
	int										m_numBuildJacobianTasks;


		/// This links a constraint which needs to fire a callback with its corresponding buildJacobianTask.
		/// This allows a contactPointCallback to make changes.
	struct CallbackPair
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpBuildJacobianTaskCollection::CallbackPair);
		const hkConstraintInternal* m_callbackConstraints;
		hkpBuildJacobianTask::AtomInfo* m_atomInfo;
	};
	
	// a list of constraints that need to fire callbacks before the jacobian setup job is executed
	HK_CPU_PTR(CallbackPair*) m_callbackConstraints;
	int	m_numCallbackConstraints;
};

struct hkpSolveJacobiansTaskCollection
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpSolveJacobiansTaskCollection );

	hkpSolveJacobiansTaskCollection() : m_firstSolveJacobiansTask(HK_NULL), m_firstBatchSize(0) {}

	HK_CPU_PTR(struct hkpSolveConstraintBatchTask*) m_firstSolveJacobiansTask;
	hkUint16 m_firstBatchSize;
};

	// Rules for reading and writing:
	// All mutables can only be accessed (read or write) in single threaded mode
	// All other variables can be accessed read only
	// this structure is allocated in buildJacobianTasksJob
	// and deleted in the broadphase job
struct hkpBuildJacobianTaskHeader
{
	public:

	    HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpBuildJacobianTaskHeader );

		HK_FORCE_INLINE hkpBuildJacobianTaskHeader() : m_referenceCount(1), m_numIslandsAfterSplit(1), m_buffer(0), m_bufferSize(0), m_bufferCapacity(0) {}

		HK_FORCE_INLINE ~hkpBuildJacobianTaskHeader(){}
	public:

			// can only be accessed if the job queue is locked
		HK_ALIGN16( mutable int	  m_openJobs );

			// the open jobs before triggering the broadphase, jobs which modify this variable:
			//   - DYNAMICS_JOB_SPLIT_ISLAND
			//   - DYNAMICS_JOB_INTEGRATE_MOTION
			// can only be accessed if the job queue is locked
		mutable int m_numUnfinishedJobsForBroadphase;

			// set to true by the finish job function, 
			// maybe set to 0 by the integrate motion jobs
			// this variable will be examined by the broadphase, which will optionally deactivate the island
		mutable int m_islandShouldBeDeactivated;

			// the broadphase job should delete this structure. 
			// As the split job can create several islands, we get several broadphase jobs for this structure.
			// The last broadphase job should delete this.
			// You are only allowed to access this variable using hkCriticalSection::atomicExchangeAdd()
		mutable long m_referenceCount;

			// the number of islands which came out of a split island job
		mutable int m_numIslandsAfterSplit;

			//
			//	constraints info
			//
	    HK_CPU_PTR(void*) m_buffer;

		int   m_bufferSize;

		int   m_bufferCapacity;

			// flag set to 1 by the export job to indicate that the export is finished,
			// later this flag is set to 2 to indicate that the solver callbacks have been fired
		hkChar m_exportFinished;

			// this is set to true, if the island requests a split and a split island job should be created
		hkChar m_splitCheckRequested;

			// Serial Id of this job 
		HK_ON_DETERMINISM_CHECKS_ENABLED( hkUint16 m_sidForNextJobType );

			// a linked list of breached contact forces. Set by 
			//    - on spu by finishDynamicsJob in the export job 
			//    - on spu by the solve job for single threaded islands
			//    - on ppu by finishDynamicsJob
		mutable hkpImpulseLimitBreachedHeader* m_impulseLimitsBreached;

	    
			// Aligned to be copied directly into an spu job: see hkpSolveApplyGravityJob::hkpSolveApplyGravityJob
		HK_ALIGN16( HK_CPU_PTR(hkpVelocityAccumulator*)	m_accumulatorsBase );
		HK_CPU_PTR(hkpVelocityAccumulator*)	m_accumulatorsEnd;

		hkObjectIndex m_numApplyGravityJobs;
		hkObjectIndex m_numIntegrateVelocitiesJobs;

	    HK_CPU_PTR(hkpJacobianSchema*)		m_schemasBase;
	    HK_CPU_PTR(hkpSolverElemTemp*)		m_solverTempBase;

	    hkInt32  m_numSolverResults;
		hkInt32  m_numSolverElemTemps;

			// pointer to the ppu version of a hkpConstraintQueryIn
		HK_CPU_PTR(const hkpConstraintQueryIn*)    m_constraintQueryIn;

			// the entities of the original islands, if m_newSplitIslands is empty,
			// these entities are identical to the island entities, else they have
			// to be freed when the task header is deleted.
		hkpEntity*const* m_allEntities;		
		
		hkObjectIndex   m_numAllEntities;
		hkObjectIndex   m_entitiesCapacity;		

		hkBool m_solveInSingleThread;
		hkBool m_solveInSingleThreadOnPpuOnly;

			//
			// variables set if the island is split by the hkSplitIslandJob
			//
		hkArray<hkpSimulationIsland*> m_newSplitIslands;

	public:
		friend struct hkpJobQueueUtils;
		friend struct hkpSingleThreadedJobsOnIsland;

		mutable struct hkpBuildJacobianTaskCollection m_tasks;

		struct hkpSolveJacobiansTaskCollection m_solveTasks;
};


struct hkpSolveConstraintBatchTask
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpSolveConstraintBatchTask );

	inline hkpSolveConstraintBatchTask()
	{
		m_next = 0; 
		m_isLastTaskInBatch = false;
		m_sizeOfNextBatch = 0;
		m_firstTaskInNextBatch = HK_NULL;
	}

	HK_ALIGN16( HK_CPU_PTR(hkpSolveConstraintBatchTask*) m_next );

	// Not used by this job, but needed to pass it to the next job
	HK_CPU_PTR(hkpBuildJacobianTaskHeader*)	m_taskHeader;

	HK_CPU_PTR(hkpVelocityAccumulator*)		m_accumulators; 
	HK_CPU_PTR(const hkpJacobianSchema*)		m_schemas;
	HK_CPU_PTR(hkpSolverElemTemp*)			m_solverElemTemp;
	hkInt32 m_sizeOfSchemaBuffer;
	hkInt32 m_sizeOfSolverElemTempBuffer;

	hkBool m_isLastTaskInBatch;
	hkUint16 m_sizeOfNextBatch;

	HK_CPU_PTR(hkpSolveConstraintBatchTask*) m_firstTaskInNextBatch;
	


};


struct hkpConstraintSolverResources
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintSolverResources );

	template <typename elem>
	struct BufferState
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintSolverResources::BufferState<elem> );

		elem*	   m_begin;
		elem*	   m_end;
		elem*	   m_current;
		elem*	   m_lastProcessed;
	};

		// a struct used to store a backup of the transform
		// in the velocity accumulator, because the solver expects
		// a zero for the sumVelocities
	struct VelocityAccumTransformBackup
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintSolverResources::VelocityAccumTransformBackup );

		hkRotation m_coreTworldRotation;
	};


	// External data
	hkStepInfo*			   m_stepInfo;
	hkpSolverInfo*		   m_solverInfo;
	hkpConstraintQueryIn*   m_constraintQueryInput;

	// Memory
	hkpVelocityAccumulator* m_accumulators;
	hkpVelocityAccumulator* m_accumulatorsEnd;
	hkpVelocityAccumulator* m_accumulatorsCurrent;
	VelocityAccumTransformBackup* m_accumulatorsBackup;
	VelocityAccumTransformBackup* m_accumulatorsBackupEnd;

		/// The number of priority classes supported.
		/// There is currently no point in going above three as there are
		/// only three priorities that can map to them (TOI, TOI_HIGHER and
		/// TOI_FORCED).
	static const int NUM_PRIORITY_CLASSES = 3;
	BufferState<hkpJacobianSchema> m_schemas[NUM_PRIORITY_CLASSES];
		/// Map from hkpConstraintInstance::ConstraintPriority to priority
		/// class.
	hkUint8 m_priorityClassMap[hkpConstraintInstance::NUM_PRIORITIES];
	
	
	//BufferState<hkpSolverElemTemp> m_elemTemp[2];
	
	hkpSolverElemTemp*	   m_elemTemp;
	hkpSolverElemTemp*      m_elemTempEnd;
	hkpSolverElemTemp*      m_elemTempCurrent;
	hkpSolverElemTemp*      m_elemTempLastProcessed;
};


	/// some helper functions for using the constraint solver
class hkpConstraintSolverSetup
{
	public:
			// solve and integrate a number of rigid bodies. Returns the number of inactive frames
		static int HK_CALL solve(
							const hkStepInfo& stepInfo, const hkpSolverInfo& solverInfo,
							hkpConstraintQueryIn& constraintQueryIn, hkpSimulationIsland& island,
							void* preallocatedBuffer, int preallocatedBufferSize, 
							hkpEntity*const* bodies, int numBodies	);

	public:

			//
			//	Functions used for multithreaded solving
			//
		static int HK_CALL calcBufferSize( hkpSimulationIsland& island );


		static int HK_CALL calcBufferOffsetsForSolve( const hkpSimulationIsland& island, char* bufferIn, int bufferSize, hkpBuildJacobianTaskHeader& taskHeader );

private:
		static HK_FORCE_INLINE void HK_CALL _buildAccumulators( const hkStepInfo& info, hkpEntity*const* bodiesIn, int numEntities, hkpVelocityAccumulator* accumOut );

		static HK_FORCE_INLINE void HK_CALL _buildJacobianElement( const hkConstraintInternal* c,	hkpConstraintQueryIn& in, hkpVelocityAccumulator* baseAccum,  class hkpConstraintQueryOut& out );

		static HK_FORCE_INLINE void HK_CALL _buildJacobianElements( hkpConstraintQueryIn& in,
										hkpEntity*const* bodies, int numBodies,
										hkpVelocityAccumulator* accusIn,
										hkpJacobianSchema* schemaOut,
										hkpJacobianSchema* schemaOutEnd,
										hkpJacobianSchema* schemaOutB);


	public:

		//
		// Functions used for constraint sub-solving and dynamic feeding of data to the solver
		//
		enum SolverMode
		{
			SOLVER_MODE_PROCESS_ALL,
			SOLVER_MODE_INCREMENTAL_CONTINUE
		};

			/// Acquires the scratchpad and initializes a hkpConstraintSolverResources struct
		static void HK_CALL initializeSolverState(hkStepInfo& stepInfo, hkpSolverInfo& solverInfo, hkpConstraintQueryIn& constraintQueryIn, 
			char* buffer, int bufferSize, const hkUint8* priorityClassMap, const hkReal* priorityClassRatios,
			hkpConstraintSolverResources& s);

			/// Releases the scratchpad
		static void HK_CALL shutdownSolver(hkpConstraintSolverResources& s);

			/// Builds accumulators
		static void HK_CALL internalAddAccumulators(hkpConstraintSolverResources& s, hkpEntity*const * entities, int numEntities);

			///	Each constraint gets queried: It produces a few Jacobian elements, returns the number of Jacobians generated
		static void HK_CALL internalAddJacobianSchemas(hkpConstraintSolverResources& s,
												hkpConstraintInstance** constraints, int numConstraints,
												hkArray<hkpConstraintSchemaInfo>& constraintStatus);

		static void HK_CALL subSolve(hkpConstraintSolverResources& s, SolverMode mode);

			/// This function integrates the rigid bodies by using the data in the linear and angular velocity
			/// field in the accumulators and not the sumLinearVelocity.
			/// The sumLinearVelocities are typically set in the hkSolver::integrateVelocities, however if
			/// you only call hkSolveStepJacobion, this sumLinearVelocities won't be used and you have to use this
			/// function to integrate your rigid bodies.
		static void HK_CALL oneStepIntegrate( const struct hkpSolverInfo& si, const hkStepInfo& info, const hkpVelocityAccumulator* accumulatorsBase, hkpEntity*const* entities, int numEntities ); 


		static hkBool HK_CALL internalIsMemoryOkForNewAccumulators    (hkpConstraintSolverResources& s, hkpEntity**     entities,    int numEntities);
		static hkBool HK_CALL internalIsMemoryOkForNewJacobianSchemas(hkpConstraintSolverResources& s, hkpConstraintInstance** constraints, int numConstraints);

};


hkpBuildJacobianTask::hkpBuildJacobianTask()
{
	m_numAtomInfos = 0;
	m_schemasOfNextTask = HK_NULL;
	m_next = HK_NULL;
	HK_ON_PLATFORM_HAS_SPU( m_onPpuOnly = false );
}


#endif // HK_DYNAMICS2_CONSTRAINT_SOLVER_SETUP_H

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
