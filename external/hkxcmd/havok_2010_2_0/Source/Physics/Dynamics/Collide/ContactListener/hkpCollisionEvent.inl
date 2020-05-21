/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkpRigidBody* hkpCollisionEvent::getBody( int bodyIdx ) const
{
	HK_ASSERT2( 0x4e66ea80, bodyIdx == 0 || bodyIdx == 1, "The bodyIdx must be 0 or 1." );
	return m_bodies[bodyIdx];
}

hkpCollisionEvent::hkpCollisionEvent( CallbackSource source, hkpRigidBody* bodyA, hkpRigidBody* bodyB, hkpSimpleConstraintContactMgr* mgr )
: m_source( source ), m_contactMgr( mgr ) 
{ 
	m_bodies[0] = bodyA;
	m_bodies[1] = bodyB;
}

hkpSimulationIsland* hkpCollisionEvent::getSimulationIsland() const
{
	// There should be no event in which both entities are fixed.
	hkpSimulationIsland* island = m_bodies[0]->getSimulationIsland();
	if ( island->isFixed() )
	{
		island = m_bodies[1]->getSimulationIsland();
	}
	return island;
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
