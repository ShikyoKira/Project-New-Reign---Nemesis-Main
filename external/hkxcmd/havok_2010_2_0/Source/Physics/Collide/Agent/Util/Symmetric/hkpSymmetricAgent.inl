/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/***
 *** INLINE FUNCTION DEFINITIONS
 ***/




template<typename AGENT>
inline hkpSymmetricAgent<AGENT>::hkpSymmetricAgent( const hkpCdBody& A, const hkpCdBody& B, const hkpCollisionInput& input, hkpContactMgr* mgr ) 
:	hkpSymmetricAgentLinearCast<AGENT>( A, B, input, mgr )
{
}



template<typename AGENT>
void hkpSymmetricAgent<AGENT>::linearCast( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpLinearCastCollisionInput& input, hkpCdPointCollector& collector, hkpCdPointCollector* startCollector )
{
	hkpLinearCastCollisionInput	flippedInput = input;
	flippedInput.m_path.setNeg4( input.m_path );

	hkpSymmetricAgentFlipCastCollector flip( input.m_path, collector );
	if ( startCollector )
	{
		hkpSymmetricAgentFlipCastCollector startFlip( input.m_path, *startCollector );
		AGENT::linearCast(bodyB, bodyA, flippedInput, flip, &startFlip );
	}
	else
	{
		AGENT::linearCast(bodyB , bodyA , flippedInput, flip, HK_NULL );
	}
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
