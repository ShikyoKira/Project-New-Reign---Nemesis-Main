/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


hkpCharacterProxyJob::hkpCharacterProxyJob( JobSubType subType, hkUint16 size )
	: hkJob( HK_JOB_TYPE_CHARACTER_PROXY, subType, size, HK_JOB_SPU_TYPE_DISABLED ),
	m_semaphore(HK_NULL), m_jobDoneFlag(HK_NULL)
{
	m_sharedJobHeaderOnPpu = HK_NULL;
}

inline void hkpCharacterProxyJob::atomicIncrementAndReleaseSemaphore() const
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

// =====================================================================================================================
// CHARACTER PROXY INTEGRATE JOB
// =====================================================================================================================

hkpCharacterProxyIntegrateJob::hkpCharacterProxyIntegrateJob( const hkpProcessCollisionInput* input
	, hkpCharacterProxyJobHeader* jobHeader
	, hkSemaphoreBusyWait* semaphore
	, const hkpCharacterProxyIntegrateCommand* commandArray
	, int numCommands
	, hkReal deltaTime
	, hkReal invDeltaTime
	, hkVector4Parameter worldGravity
	, hkCreateCdPointCollectorOnCpuFunc createAllCdPointCollectorOnCpuFunc
	, const hkpBroadPhase* broadphase
	, const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*	aabbTreeInt16
	, const hkKdTree* kdTree	
	, const int numCommandsPerTask ) 
		: hkpCharacterProxyJob(CHARACTER_PROXY_JOB_INTEGRATE, sizeof(hkpCharacterProxyIntegrateJob))
		, m_commandArray( commandArray )
		, m_numCommands ( numCommands )
		, m_numCommandsPerTask(numCommandsPerTask)
		, m_deltaTime( deltaTime )
		, m_invDeltaTime( invDeltaTime )
		, m_worldGravity( worldGravity )
		, m_createCdPointCollectorOnCpuFunc( createAllCdPointCollectorOnCpuFunc )
		, m_broadphase( broadphase )
		, m_aabbTreeInt16( aabbTreeInt16 )
		, m_kdTree( kdTree )		
{

#if defined HK_DEBUG

	HK_ASSERT2( 0xaf136151, jobHeader && ((hkUlong)jobHeader & 0xf) == 0, "jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf1647f5, numCommands > 0, "numCommands has to be > 0." );
	HK_ASSERT2( 0xaf1647e0, commandArray && ((hkUlong)commandArray & 0xf) == 0, "commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf1647b4, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,
			   "numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );	

	
	// check memory allocated on commands
	{
		for (int i = 0; i < numCommands; i++)
		{
			hkpCharacterProxyIntegrateCommand* command = const_cast<hkpCharacterProxyIntegrateCommand*>( &commandArray[i] );

			HK_ASSERT2( 0xaf2647ff, command->m_maxInteractions, "You must allow some object interactions" );
		
			HK_ASSERT2( 0xaf2647fa, command->m_objectInteraction && ( ( (hkUlong)command->m_objectInteraction & 0xf ) == 0 ),
						"If you want object interactions, you must set hkpCharacterProxyCommand::m_objectInteraction and it must be 16 byte aligned." );
			
			HK_ASSERT2( 0xaf2647fe, command->m_character && ( ( (hkUlong)command->m_character & 0xf ) == 0 ),
				"hkpCharacterProxyCommand::m_character has to be set and 16 byte aligned." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2( 0xaf253413, commandArray[0].m_objectInteraction != commandArray[1].m_objectInteraction, 
						"You are not allowed to re-use the same results buffer for two different query commands.");
	}

#endif
	
	m_collisionInput		= input;	

	m_sharedJobHeaderOnPpu	= jobHeader;
	m_semaphore				= semaphore;

	
	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
	
}

hkJobQueue::JobPopFuncResult hkpCharacterProxyIntegrateJob::popJobTask( hkpCharacterProxyIntegrateJob& out )
{
	//
	// split off a fully filled child job if there are more tasks left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpCharacterProxyIntegrateCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
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
