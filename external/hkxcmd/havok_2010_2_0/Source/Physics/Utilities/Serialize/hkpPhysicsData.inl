/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkpWorldCinfo* hkpPhysicsData::getWorldCinfo()
{
	return m_worldCinfo;
}

void hkpPhysicsData::setWorldCinfo( hkpWorldCinfo* info )
{	
	if (info != HK_NULL)
	{
		info->addReference();
	}
	if (m_worldCinfo != HK_NULL)
	{
		m_worldCinfo->removeReference();
	}
	m_worldCinfo = info;
}

void hkpPhysicsData::addPhysicsSystem( hkpPhysicsSystem* system )
{
	HK_ASSERT2( 0x76e8b552, system != HK_NULL, "Null hkpPhysicsSystem pointer passed to hkpPhysicsData::addPhysicsSystem");
	HK_ASSERT2( 0x98765eee, m_systems.indexOf(system) == -1, "Trying to add a system that is already in the physics data" );

	system->addReference();
	m_systems.pushBack(system);
}

void hkpPhysicsData::removePhysicsSystem( int i )
{
	m_systems[i]->removeReference();
	m_systems.removeAt(i);
}

const hkArray<hkpPhysicsSystem*>& hkpPhysicsData::getPhysicsSystems() const
{
	return m_systems;
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
