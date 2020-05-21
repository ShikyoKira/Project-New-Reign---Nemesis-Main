/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


hkpVehicleJob::hkpVehicleJob( JobSubType subType, hkUint16 size )
: hkJob( HK_JOB_TYPE_VEHICLE, subType, size, HK_JOB_SPU_TYPE_DISABLED )
{
}

hkpVehicleIntegrateJob::hkpVehicleIntegrateJob(	const hkpVehicleCommand* commandArray,
												hkpVehicleInstance** vehicleArrayPtr,
												int numCommands,
												const hkStepInfo& stepInfo ) 
:	hkpVehicleJob( VEHICLE_JOB_INTEGRATE, sizeof( hkpVehicleIntegrateJob ) ),
	m_stepInfo( stepInfo )
{
	HK_ASSERT2( 0xaf1647f5, numCommands > 0, "numCommands has to be > 0." );

	HK_ASSERT2( 0xaf1647e0, commandArray && ( (hkUlong)commandArray & 0xf ) == 0, "commandArray has to be set and 16 byte aligned." );

#if defined(HK_DEBUG)
	// check memory allocated on commands
	{
		for ( int i = 0; i < numCommands; ++i )
		{
			hkpVehicleCommand* command = const_cast<hkpVehicleCommand*>( &commandArray[i] );

			HK_ASSERT2( 0xaf2647fe, command->m_jobResults && ( (hkUlong)command->m_jobResults & 0xf ) == 0,
						"hkpVehicleCommand::m_jobResults has to be set and 16 byte aligned." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2( 0xaf253413, commandArray[0].m_jobResults != commandArray[1].m_jobResults, 
						"You are not allowed to re-use the same results buffer for two different query commands." );
	}
#endif

	m_commandArray			= commandArray;
	m_vehicleArrayPtr		= vehicleArrayPtr;
	m_numCommands			= numCommands;
}

hkJobQueue::JobPopFuncResult hkpVehicleIntegrateJob::popJobTask( hkpVehicleIntegrateJob& out )
{
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
