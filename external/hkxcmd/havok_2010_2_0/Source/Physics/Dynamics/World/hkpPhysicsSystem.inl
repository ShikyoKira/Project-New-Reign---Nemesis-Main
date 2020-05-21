/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline const hkArray<hkpRigidBody*>& hkpPhysicsSystem::getRigidBodies() const 
{ 
	return m_rigidBodies; 
}

inline const hkArray<hkpConstraintInstance*>& hkpPhysicsSystem::getConstraints() const 
{ 
	return m_constraints; 
}

inline const hkArray<hkpAction*>& hkpPhysicsSystem::getActions() const 
{ 
	return m_actions; 
}

inline const hkArray<hkpPhantom*>& hkpPhysicsSystem::getPhantoms() const 
{ 
	return m_phantoms; 
}

inline const char* hkpPhysicsSystem::getName() const
{
	return m_name;
}

inline void hkpPhysicsSystem::setName(const char* name)
{
	m_name = name ;
}

inline hkUlong hkpPhysicsSystem::getUserData() const
{
	return m_userData;
}

inline void hkpPhysicsSystem::setUserData(hkUlong d)
{
	m_userData = d;
}

inline hkBool hkpPhysicsSystem::isActive() const
{
	return m_active;
}

inline void hkpPhysicsSystem::setActive(hkBool active)
{
	m_active = active;
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
