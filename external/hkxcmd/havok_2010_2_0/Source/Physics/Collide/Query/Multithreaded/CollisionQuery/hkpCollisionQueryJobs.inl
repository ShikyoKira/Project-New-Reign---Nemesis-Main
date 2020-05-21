/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



template<typename T>
static void hkKdTreeCheckCommandPointers (const T* commandPtr, int numCommands)
{
	for (int i=0; i<numCommands; i++)
	{
		const T* command = &commandPtr[i];
		HK_ASSERT2( 0xaf3647fe, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,	"Command's m_results has to be set and 16 byte aligned." );
	}
}

template<typename T>
static void hkKdTreeCheckCommandCapacity (const T* commandPtr, int numCommands)
{
	for (int i=0; i<numCommands; i++)
	{
		const T* command = &commandPtr[i];
		HK_ASSERT2( 0xaf3638e2, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= T::MAXIMUM_RESULTS_CAPACITY, "Command's m_resultsCapacity has to be > 0 and <= " << T::MAXIMUM_RESULTS_CAPACITY << "." );
	}
}




hkpCollisionQueryJob::hkpCollisionQueryJob( JobSubType subType, hkUint16 size )
	: hkJob( HK_JOB_TYPE_COLLISION_QUERY, subType, size, HK_JOB_SPU_TYPE_DISABLED ),
	m_semaphore(HK_NULL), m_jobDoneFlag(HK_NULL)
{
	m_sharedJobHeaderOnPpu = HK_NULL;
	m_collisionInput = HK_NULL;
}

inline void hkpCollisionQueryJob::atomicIncrementAndReleaseSemaphore() const
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
// PAIR LINEAR CAST
// ===============================================================================================================================================================================================

hkpPairLinearCastJob::hkpPairLinearCastJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpPairLinearCastCommand* commandArray, int numCommands, const hkpShapeCollectionFilter* filter, hkReal tolerance, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpCollisionQueryJob(COLLISION_QUERY_PAIR_LINEAR_CAST, sizeof(hkpPairLinearCastJob))
{
	HK_ASSERT2( 0xaf136151, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf1647f5, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf1647e0, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf1647b4, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	// check memory allocated on commands
	{
		for (int i = 0; i < numCommands; i++)
		{
			hkpPairLinearCastCommand* command = const_cast<hkpPairLinearCastCommand*>( &commandArray[i] );

			HK_ASSERT2( 0xaf16e629, command->m_collidableA && ((hkUlong)command->m_collidableA & 0xf) == 0,												"hkpPairLinearCastCommand::m_collidableA has to be set and 16 byte aligned." );
			HK_ASSERT2( 0xaf16e630, command->m_collidableB && ((hkUlong)command->m_collidableB & 0xf) == 0,												"hkpPairLinearCastCommand::m_collidableB has to be set and 16 byte aligned." );
			HK_ASSERT2( 0xaf16e629, command->m_collidableA->m_shape,																					"hkpPairLinearCastCommand::m_collidableA has no shape." );
			HK_ASSERT2( 0xaf16e630, command->m_collidableB->m_shape,																					"hkpPairLinearCastCommand::m_collidableB has no shape." );
			HK_ASSERT2( 0xaf2647fe, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,														"hkpPairLinearCastCommand::m_results has to be set and 16 byte aligned." );
			HK_ASSERT2( 0xaf2638e2, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpPairLinearCastCommand::MAXIMUM_RESULTS_CAPACITY,	"hkpPairLinearCastCommand::m_resultsCapacity has to be > 0 and <= " << hkpPairGetClosestPointsCommand::MAXIMUM_RESULTS_CAPACITY << "." );

			// m_startPointResults aren't required. These should still catch uninitialized memory, though.
			HK_ASSERT2( 0xaf2647fe, ( command->m_startPointResultsCapacity == 0 ) || ( ((hkUlong)command->m_startPointResults & 0xf) == 0 ) , "hkpPairLinearCastCommand::m_startPointResults has to be 16 byte aligned." );
			HK_ASSERT2( 0xaf2638e2, command->m_startPointResultsCapacity >= 0 && command->m_startPointResultsCapacity <= hkpPairLinearCastCommand::MAXIMUM_RESULTS_CAPACITY,	"hkpPairLinearCastCommand::m_startPointResultsCapacity has to be >= 0 and <= " << hkpPairGetClosestPointsCommand::MAXIMUM_RESULTS_CAPACITY << "." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253413, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;

	m_sharedJobHeaderOnPpu	= jobHeader;

	m_tolerance				= tolerance;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;


	
	m_maxExtraPenetration					= HK_REAL_EPSILON;
	m_iterativeLinearCastEarlyOutDistance	= 0.01f;
	m_iterativeLinearCastMaxIterations		= 10;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}

hkpPairLinearCastJob::hkpPairLinearCastJob() : hkpCollisionQueryJob(COLLISION_QUERY_PAIR_LINEAR_CAST, sizeof(hkpPairLinearCastJob))
{
}

hkJobQueue::JobPopFuncResult hkpPairLinearCastJob::popJobTask( hkpPairLinearCastJob& out )
{
	//
	// split off a fully filled child job if there are more tasks left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpPairLinearCastCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  WORLD LINEAR CAST
// ===============================================================================================================================================================================================

hkpWorldLinearCastJob::hkpWorldLinearCastJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpWorldLinearCastCommand* commandArray, int numCommands, const hkpBroadPhase* broadphase, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpCollisionQueryJob(COLLISION_QUERY_WORLD_LINEAR_CAST, sizeof(hkpWorldLinearCastJob))
{
	HK_ASSERT2( 0xaf736152, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf7647f6, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf7647f7, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf7647f8, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	{
		for (int i=0; i<numCommands; i++)
		{
			const hkpWorldLinearCastCommand* command = &commandArray[i];
			HK_ASSERT2( 0xaf7647f9, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,	"hkpWorldLinearCastCommand::m_results has to be set and 16 byte aligned." );

			HK_ASSERT2( 0xaf7647fb, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpWorldLinearCastCommand::MAXIMUM_RESULTS_CAPACITY, "hkpWorldLinearCastCommand::m_resultsCapacity has to be > 0 and <= " << hkpWorldLinearCastCommand::MAXIMUM_RESULTS_CAPACITY << "." );

		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253414, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
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
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
	m_aabbTree = HK_NULL;
	m_aabbTreeInt16 = HK_NULL;
	m_kdTree = HK_NULL;
}


hkJobQueue::JobPopFuncResult hkpWorldLinearCastJob::popJobTask( hkpWorldLinearCastJob& out )
{
	//
	// split off a fully filled child job if there are more commands left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpWorldLinearCastCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  MOPP AABB
// ===============================================================================================================================================================================================

hkpMoppAabbJob::hkpMoppAabbJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpMoppAabbCommand* commandArray, int numCommands, const hkUint8* moppCodeData, const hkpMoppCode::CodeInfo& moppCodeInfo, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpCollisionQueryJob(COLLISION_QUERY_MOPP_AABB, sizeof(hkpMoppAabbJob))
{
	m_jobSpuType = HK_JOB_SPU_TYPE_ENABLED;

	HK_ASSERT2( 0xaf236154, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned.");
	HK_ASSERT2( 0xaf3647d2, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647d3, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf3647df, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	{
		for (int i=0; i<numCommands; i++)
		{
			const hkpMoppAabbCommand* command = &commandArray[i];
			HK_ASSERT2( 0xaf3647d5, command->m_results && ((hkUlong)command->m_results & 0xf) == 0, "hkpMoppAabbCommand::m_results has to be set and 16 byte aligned." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253415, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;
	m_sharedJobHeaderOnPpu	= jobHeader;

	m_moppCodeInfo			= moppCodeInfo;
	m_moppCodeData			= moppCodeData;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}


hkJobQueue::JobPopFuncResult hkpMoppAabbJob::popJobTask( hkpMoppAabbJob& out )
{
	//
	// split off a fully filled child job if there are more commands left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpMoppAabbCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  PAIR GET CLOSEST POINTS
// ===============================================================================================================================================================================================

hkpPairGetClosestPointsJob::hkpPairGetClosestPointsJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpPairGetClosestPointsCommand* commandArray, int numCommands, hkReal tolerance, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpCollisionQueryJob(COLLISION_QUERY_PAIR_GET_CLOSEST_POINTS,  sizeof(hkpPairGetClosestPointsJob))
{
	HK_ASSERT2( 0xaf236151, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf3647f5, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647e0, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf3647b4, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

	{
		for (int i = 0; i < numCommands; i++)
		{
			hkpPairGetClosestPointsCommand* command = const_cast<hkpPairGetClosestPointsCommand*>( &commandArray[i] );

#if defined(HK_DEBUG)		
			HK_ASSERT2( 0xaf16e729, command->m_collidableA && ((hkUlong)command->m_collidableA & 0xf) == 0,														"hkpPairGetClosestPointsCommand::m_collidableA has to be set and 16 byte aligned." );
			HK_ASSERT2( 0xaf16e730, command->m_collidableB && ((hkUlong)command->m_collidableB & 0xf) == 0,														"hkpPairGetClosestPointsCommand::m_collidableB has to be set and 16 byte aligned." );
			HK_ASSERT2( 0xaf16f629, command->m_collidableA->m_shape,																							"hkpPairGetClosestPointsCommand::m_collidableA has no shape." );
			HK_ASSERT2( 0xaf16f630, command->m_collidableB->m_shape,																							"hkpPairGetClosestPointsCommand::m_collidableB has no shape." );
			HK_ASSERT2( 0xaf1647fe, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,																"hkpPairGetClosestPointsCommand::m_results has to be set and 16 byte aligned." );
			HK_ASSERT2( 0xaf1638e2, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpPairGetClosestPointsCommand::MAXIMUM_RESULTS_CAPACITY,	"hkpPairGetClosestPointsCommand::m_resultsCapacity has to be > 0 and <= " << hkpPairGetClosestPointsCommand::MAXIMUM_RESULTS_CAPACITY << "." );
#endif

			command->m_indexIntoSharedResults = HK_NULL;
		}
	}

#if defined(HK_DEBUG)
	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253416, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;

	m_sharedJobHeaderOnPpu	= jobHeader;

	m_tolerance				= tolerance;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}

hkpPairGetClosestPointsJob::hkpPairGetClosestPointsJob() : hkpCollisionQueryJob(COLLISION_QUERY_PAIR_GET_CLOSEST_POINTS, sizeof(hkpPairGetClosestPointsJob))
{
}


hkJobQueue::JobPopFuncResult hkpPairGetClosestPointsJob::popJobTask( hkpPairGetClosestPointsJob& out )
{
	//
	// split off a fully filled child job if there are more tasks left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpPairGetClosestPointsCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  WORLD GET CLOSEST POINTS
// ===============================================================================================================================================================================================

hkpWorldGetClosestPointsJob::hkpWorldGetClosestPointsJob( const hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpWorldGetClosestPointsCommand* commandArray, int numCommands, const hkpBroadPhase* broadphase, hkReal tolerance, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask ) 
 :	hkpCollisionQueryJob(COLLISION_QUERY_WORLD_GET_CLOSEST_POINTS, sizeof(hkpWorldGetClosestPointsJob))
{
	HK_ASSERT2( 0xaf236152, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,							"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf3647f6, numCommands > 0,														"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647f7, commandArray && ((hkUlong)commandArray & 0xf) == 0,						"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf3647fd, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

	{
		for (int i=0; i<numCommands; i++)
		{
			hkpWorldGetClosestPointsCommand* command = const_cast<hkpWorldGetClosestPointsCommand*>( &commandArray[i] );

#if defined(HK_DEBUG)
			HK_ASSERT2( 0xaf3647f9, command->m_resultsCapacity > 0,									"hkpWorldGetClosestPointsCommand::m_resultsCapacity has to be > 0." );
			HK_ASSERT2( 0xaf3647fb, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,	"hkpWorldGetClosestPointsCommand::m_results has to be set and 16 byte aligned." );
#endif

			command->m_numResultsOut = 0;
		}
	}

#if defined(HK_DEBUG)
	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253417, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;
	m_sharedJobHeaderOnPpu	= jobHeader;
	m_numCommandsPerTask	= numCommandsPerTask;

	m_broadphase			= broadphase;
	m_tolerance				= tolerance;

	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;

	m_aabbTreeInt16 = HK_NULL;
	m_kdTree = HK_NULL;
}

hkJobQueue::JobPopFuncResult hkpWorldGetClosestPointsJob::popJobTask( hkpWorldGetClosestPointsJob& out )
{
	//
	// split off a fully filled child job if there are more tasks left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpWorldGetClosestPointsCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  KD-TREE AABB QUERY
// ===============================================================================================================================================================================================

hkpKdTreeAabbJob::hkpKdTreeAabbJob(hkpCollisionQueryJobHeader* jobHeader, hkpKdTreeAabbCommand* commandArray, int numCommands, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
: 	hkpCollisionQueryJob(COLLISION_QUERY_KD_TREE_AABB_QUERY_JOB, sizeof(hkpKdTreeAabbJob) )
{
	HK_ASSERT2( 0xaf236152, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf3647f6, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647f7, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf3647fd, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	{
		hkKdTreeCheckCommandPointers<hkpKdTreeAabbCommand>(commandArray, numCommands);
	}
#endif

	m_sharedJobHeaderOnPpu	= jobHeader;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_aabbCommandArray		= commandArray;
	m_numAabbCommands		= numCommands;

	m_numTrees = 0;
	m_trees[0] = HK_NULL;


	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;

	m_jobSpuType = HK_JOB_SPU_TYPE_ENABLED; // all kd tree jobs run on the SPU
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
