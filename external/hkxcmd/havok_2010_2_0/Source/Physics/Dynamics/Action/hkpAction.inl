/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline hkpAction::hkpAction( hkUlong userData )
:	m_world(HK_NULL),
	m_island(HK_NULL),
	m_userData(userData)
{
}

inline hkpSimulationIsland* hkpAction::getSimulationIsland()
{ 
	return m_island; 
}

inline void hkpAction::setSimulationIsland( hkpSimulationIsland* isl )
{ 
	m_island = isl; 
}

inline void hkpAction::setWorld( hkpWorld* world )
{
	m_world = world;
}

inline hkpWorld* hkpAction::getWorld()
{
	return m_world;
}

inline hkUlong hkpAction::getUserData() const
{
	return m_userData;
}

inline void hkpAction::setUserData( hkUlong data )
{
	m_userData = data;
}

inline const char* hkpAction::getName() const
{
	return m_name;
}

inline void hkpAction::setName( const char* name )
{
	m_name = name;
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
