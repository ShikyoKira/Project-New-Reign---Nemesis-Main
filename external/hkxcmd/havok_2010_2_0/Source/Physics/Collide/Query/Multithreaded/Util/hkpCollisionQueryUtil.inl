/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


HK_FORCE_INLINE hkpWorldRayCastOutputPpu* hkpShapeRayCastJobUtil::getNextFreeResult(hkpShapeRayCastCommand* command, hkpWorldRayCastOutputPpu* resultsArray, hkpWorldRayCastOutputPpu*& nextFreeResult, hkReal hitFraction)
{
	hkpWorldRayCastOutputPpu* resultToReturn = HK_NULL;

	// check if there's still room left in the array for the new results or if we have to evict the furthest hit
	if ( command->m_numResultsOut < command->m_resultsCapacity )
	{
		resultToReturn = nextFreeResult;
		command->m_numResultsOut++;
		nextFreeResult = nextFreeResult + 1;
	}
	else
	{
		hkpWorldRayCastOutputPpu* furthestHit		= resultsArray;
		hkpWorldRayCastOutputPpu* currentHitInArray	= resultsArray + 1;

		//
		// search for the furthest hit
		//
		
		{
			for (int i = 1; i < command->m_numResultsOut; i++)
			{
				if ( currentHitInArray->m_hitFraction > furthestHit->m_hitFraction )
				{
					furthestHit = currentHitInArray;
				}
				currentHitInArray++;
			}
		}

		if ( hitFraction < furthestHit->m_hitFraction )
		{
			resultToReturn = furthestHit;
		}
	}

	return resultToReturn;
}


HK_FORCE_INLINE hkpWorldRayCastOutputPpu* hkpWorldRayCastJobUtil::getNextFreeResult(hkpWorldRayCastCommand* command, hkpWorldRayCastOutputPpu* resultsArray, hkPadSpu<hkpWorldRayCastOutputPpu*>& nextFreeResult, hkReal hitFraction)
{
	hkpWorldRayCastOutputPpu* resultToReturn = HK_NULL;

	// check if there's still room left in the array for the new results or if we have to evict the furthest hit
	if ( command->m_numResultsOut < command->m_resultsCapacity )
	{
		resultToReturn = nextFreeResult;
		command->m_numResultsOut++;
		nextFreeResult = nextFreeResult + 1;
	}
	else
	{
		hkpWorldRayCastOutputPpu* furthestHit		= resultsArray;
		hkpWorldRayCastOutputPpu* currentHitInArray	= resultsArray + 1;

		//
		// search for the furthest hit
		//
		
		{
			for (int i = 1; i < command->m_numResultsOut; i++)
			{
				if ( currentHitInArray->m_hitFraction > furthestHit->m_hitFraction )
				{
					furthestHit = currentHitInArray;
				}
				currentHitInArray++;
			}
		}

		if ( hitFraction < furthestHit->m_hitFraction )
		{
			resultToReturn = furthestHit;
		}
	}

	return resultToReturn;
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
