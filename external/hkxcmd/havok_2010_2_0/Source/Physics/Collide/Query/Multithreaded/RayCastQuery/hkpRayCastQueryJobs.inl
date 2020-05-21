/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkpRayCastQueryJob::hkpRayCastQueryJob( JobSubType subType,  hkUint16 size )
: hkJob( HK_JOB_TYPE_RAYCAST_QUERY, subType, size, HK_JOB_SPU_TYPE_DISABLED ),
	m_semaphore(HK_NULL), m_jobDoneFlag(HK_NULL)
{
	m_jobSpuType = HK_JOB_SPU_TYPE_ENABLED; // all kd tree jobs run on the SPU
	m_sharedJobHeaderOnPpu = HK_NULL;
}

inline void hkpRayCastQueryJob::atomicIncrementAndReleaseSemaphore() const
{
	if (m_jobDoneFlag)
	{
		hkCriticalSection::atomicExchangeAdd(m_jobDoneFlag, 1);
	}

	if (m_semaphore)
	{
		hkSemaphoreBusyWait::release(m_semaphore);
	}
}

// ===============================================================================================================================================================================================
//  SHAPE RAYCAST
// ===============================================================================================================================================================================================

hkpShapeRayCastJob::hkpShapeRayCastJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpShapeRayCastCommand* commandArray, int numCommands, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpRayCastQueryJob(RAYCAST_QUERY_SHAPE_RAYCAST, sizeof(hkpShapeRayCastJob))
{
	HK_ASSERT2( 0xaf236153, jobHeader,							"jobHeader has to be set." );
	HK_ASSERT2( 0xaf3647f2, numCommands > 0,					"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647e3, commandArray,						"commandArray has to be set." );

	HK_ASSERT2( 0xaf236158, ((hkUlong)jobHeader & 0xf) == 0,	"jobHeader has to be 16 byte aligned." );
	HK_ASSERT2( 0xaf3647e9, ((hkUlong)commandArray & 0xf) == 0,	"commandArray has to be 16 byte aligned." );

	HK_ASSERT2( 0xaf3647ff, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	{
		for (int i=0; i<numCommands; i++)
		{
			const hkpShapeRayCastCommand* command = &commandArray[i];

			HK_ASSERT2( 0xaf3647e4, command->m_collidables, "command[ " << i << " ]->m_collidables has to be set." );
			HK_ASSERT2( 0xaf3647e5, command->m_results,		"command[ " << i << " ]->m_results has to be set." );

			HK_ASSERT2( 0xaf3647e6, ((hkUlong)command->m_collidables & 0xf) == 0, "command[ " << i << " ]->m_collidables (address 0x" << command->m_collidables << " has to be 16 byte aligned." );
			HK_ASSERT2( 0xaf3647e7, ((hkUlong)command->m_results     & 0xf) == 0, "command[ " << i << " ]->m_results (address 0x" << command->m_results << " has to be 16 byte aligned." );

			HK_ASSERT2( 0xaf3648e0, command->m_numCollidables > 0 && command->m_numCollidables <= hkpShapeRayCastCommand::MAXIMUM_NUM_COLLIDABLES,		"hkpShapeRayCastCommand::m_numCollidables has to be > 0 and <= " << hkpShapeRayCastCommand::MAXIMUM_NUM_COLLIDABLES << "." );
			HK_ASSERT2( 0xaf3648e2, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpShapeRayCastCommand::MAXIMUM_RESULTS_CAPACITY,	"hkpShapeRayCastCommand::m_resultsCapacity has to be > 0 and <= " << hkpShapeRayCastCommand::MAXIMUM_RESULTS_CAPACITY << "." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253411, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;
	m_sharedJobHeaderOnPpu	= jobHeader;
	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}


hkJobQueue::JobPopFuncResult hkpShapeRayCastJob::popJobTask( hkpShapeRayCastJob& out )
{
	//
	// split off a fully filled child job if there are more commands left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpShapeRayCastCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  WORLD RAYCAST
// ===============================================================================================================================================================================================

hkpWorldRayCastJob::hkpWorldRayCastJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpWorldRayCastCommand* commandArray, int numCommands, const hkpBroadPhase* broadphase, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask)
:	hkpRayCastQueryJob( RAYCAST_QUERY_WORLD_RAYCAST, sizeof(hkpWorldRayCastJob) )
{
	HK_ASSERT2( 0xaf236152, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned." );
	//	for cases that only have bundledJob
//	HK_ASSERT2( 0xaf3647f6, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647f7, (numCommands == 0) ||  (commandArray && ((hkUlong)commandArray & 0xf)) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf3647fd, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	{
		for (int i=0; i<numCommands; i++)
		{
			const hkpWorldRayCastCommand* command = &commandArray[i];
			HK_ASSERT2( 0xaf3647fe, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,	"hkpWorldRayCastCommand::m_results has to be set and 16 byte aligned." );

			HK_ASSERT2( 0xaf3638e2, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpWorldRayCastCommand::MAXIMUM_RESULTS_CAPACITY, "hkpWorldRayCastCommand::m_resultsCapacity has to be > 0 and <= " << hkpWorldRayCastCommand::MAXIMUM_RESULTS_CAPACITY << "." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253412, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;

	m_sharedJobHeaderOnPpu	= jobHeader;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	m_broadphase			= broadphase;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = (numCommands==0)? 1 : ((numCommands-1)/numCommandsPerTask) + 1;

	m_aabbTreeFp = HK_NULL;
	m_aabbTreeInt16 = HK_NULL;
	m_kdTree = HK_NULL;
	m_bundleCommandArray = HK_NULL;
	m_numBundleCommands = 0;
}


hkJobQueue::JobPopFuncResult hkpWorldRayCastJob::popJobTask( hkpWorldRayCastJob& out )
{
	//
	// split off a fully filled child job if there are more commands left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpWorldRayCastCommand));

		//	put all the bundle commands in the first job
		m_numBundleCommands = 0;

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}

hkpKdTreeBuildJobFast::hkpKdTreeBuildJobFast()
:	hkpRayCastQueryJob( RAYCAST_QUERY_KD_TREE_BUILD_FAST, sizeof(hkpKdTreeBuildJobFast) ), 
	m_treeDepthOut(HK_NULL)
{
}

hkpKdTreeBuildJobFast::hkpKdTreeBuildJobFast(hkKdTree *tree)
:	hkpRayCastQueryJob( RAYCAST_QUERY_KD_TREE_BUILD_FAST, sizeof(hkpKdTreeBuildJobFast) )
{
	m_numCollidables			= tree->getNumPrimitives();
	m_treeOut					= tree->getRoot();
	m_nodeArraySize				= tree->getTotalNumNodes();
	m_projEntOut				= tree->getProjectedEntries();
	m_numAvailableEmptyNodes	= tree->getNumReservedEmptyNodes();
	m_treeDepthOut				= &tree->m_maxDepth;

}

hkpKdTreeBuildDistributedJob::hkpKdTreeBuildDistributedJob(hkpRayCastQueryJobHeader* jobHeader)
:	hkpRayCastQueryJob(RAYCAST_QUERY_KD_TREE_BUILD_FAST_DISTRIBUTED_JOB, sizeof(hkpKdTreeBuildDistributedJob) ),
	m_branchDepthsOut(HK_NULL)
{
	m_sharedJobHeaderOnPpu	= jobHeader;
}

hkpKdTreeBuildDistributedJob::hkpKdTreeBuildDistributedJob(hkpRayCastQueryJobHeader* jobHeader, hkKdTree* tree)
:	hkpRayCastQueryJob(RAYCAST_QUERY_KD_TREE_BUILD_FAST_DISTRIBUTED_JOB, sizeof(hkpKdTreeBuildDistributedJob) ),
	m_branchDepthsOut(HK_NULL)
{
	m_sharedJobHeaderOnPpu		= jobHeader;
	m_numPrimitives				= tree->getNumPrimitives();
	m_treeOut					= tree->getRoot();
	m_nodeArraySize				= tree->getTotalNumNodes();
	m_projEntOut				= tree->getProjectedEntries();
	m_numAvailableEmptyNodes	= tree->getNumReservedEmptyNodes();
}

hkpKdTreeBuildDistributedSubJob::hkpKdTreeBuildDistributedSubJob()
:	hkpRayCastQueryJob(RAYCAST_QUERY_KD_TREE_BUILD_FAST_DISTRIBUTED_SUBJOB, sizeof(hkpKdTreeBuildDistributedSubJob) ),
	m_branchDepthOut(HK_NULL)
{
}

hkpKdTreeBuildCoordinatorJob::hkpKdTreeBuildCoordinatorJob(hkpRayCastQueryJobHeader* jobHeader, const class hkpWorld* world)
:	hkpKdTreeBuildDistributedJob(jobHeader) 
{
	m_jobSubType = RAYCAST_QUERY_KD_TREE_BUILD_COORDINATOR_JOB;
	m_size = sizeof(hkpKdTreeBuildCoordinatorJob);
	m_collisionInput = world->getCollisionInput();
}

hkpKdTreeBuildSetupJob::hkpKdTreeBuildSetupJob()
:	hkpKdTreeBuildDistributedJob(HK_NULL), 
	m_spawnBuildDistributedJob(false)
{
	m_jobSubType = RAYCAST_QUERY_KD_TREE_BUILD_SETUP_JOB;
	m_size = sizeof(hkpKdTreeBuildSetupJob);
}

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
