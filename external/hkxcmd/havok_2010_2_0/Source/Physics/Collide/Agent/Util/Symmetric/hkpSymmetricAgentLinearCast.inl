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
void hkpSymmetricAgentLinearCast<AGENT>::processCollision(	const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result)
{
	hkpProcessCdPoint* pp = result.m_firstFreeContactPoint;

	hkTime oldToi = result.m_toi.m_time;

	AGENT::processCollision(bodyB, bodyA, input, result);

	//
	//	Flip all new normals
	//
	for ( ; pp < result.m_firstFreeContactPoint; pp++)
	{
		pp->m_contact.setFlipped( pp->m_contact );
	}

	// Flip the TOI normal, if a new TOI was reported by this processCollision call
	if( oldToi != result.m_toi.m_time )
	{
		result.m_toi.flip();
	}
}

template<typename AGENT>	
void hkpSymmetricAgentLinearCast<AGENT>::getClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector ) 
{
	hkpSymmetricAgentFlipCollector flip( collector );
	AGENT::getClosestPoints(bodyB , bodyA , input, flip );
}

template<typename AGENT>	
void hkpSymmetricAgentLinearCast<AGENT>::staticGetClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector ) 
{
	hkpSymmetricAgentFlipCollector flip( collector );
	AGENT::staticGetClosestPoints(bodyB , bodyA , input, flip );
}


template<typename AGENT>
void hkpSymmetricAgentLinearCast<AGENT>::getPenetrations(	const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector)
{
	hkpSymmetricAgentFlipBodyCollector flip(collector);
	AGENT::getPenetrations(bodyB, bodyA, input, flip);
}


template<typename AGENT>
void hkpSymmetricAgentLinearCast<AGENT>::staticGetPenetrations(	const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector)
{
	hkpSymmetricAgentFlipBodyCollector flip(collector);
	AGENT::staticGetPenetrations(bodyB, bodyA, input, flip);
}


template<typename AGENT>
inline hkpSymmetricAgentLinearCast<AGENT>::hkpSymmetricAgentLinearCast( const hkpCdBody& A, const hkpCdBody& B, const hkpCollisionInput& input, hkpContactMgr* mgr ) 
:	AGENT( B, A, input, mgr )
{
}



template<typename AGENT>
void hkpSymmetricAgentLinearCast<AGENT>::staticLinearCast( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpLinearCastCollisionInput& input, hkpCdPointCollector& collector, hkpCdPointCollector* startCollector )
{
	hkpLinearCastCollisionInput	flippedInput = input;
	flippedInput.m_path.setNeg4( input.m_path );

	hkpSymmetricAgentFlipCastCollector flip( input.m_path, collector );
	if ( startCollector )
	{
		hkpSymmetricAgentFlipCastCollector startFlip( input.m_path, *startCollector );
		AGENT::staticLinearCast(bodyB, bodyA, flippedInput, flip, &startFlip );
	}
	else
	{
		AGENT::staticLinearCast(bodyB , bodyA , flippedInput, flip, HK_NULL );
	}
}

template<typename AGENT>
void hkpSymmetricAgentLinearCast<AGENT>::updateShapeCollectionFilter( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkCollisionConstraintOwner& constraintOwner )
{
	AGENT::updateShapeCollectionFilter( bodyB, bodyA, input, constraintOwner );
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
