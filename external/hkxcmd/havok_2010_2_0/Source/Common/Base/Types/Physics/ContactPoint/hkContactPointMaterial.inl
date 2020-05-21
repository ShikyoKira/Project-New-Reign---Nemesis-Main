/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkReal hkContactPointMaterial::getFriction() const
{ 
	return m_friction; 
}



inline void hkContactPointMaterial::setFriction( hkReal r )
{ 
	m_friction = r; 
}

inline hkReal hkContactPointMaterial::getRestitution() const
{ 
	return m_restitution * (1.0f / 128.0f ); 
}

inline void hkContactPointMaterial::setRestitution( hkReal r )
{ 
	HK_ASSERT2( 0xf0160256, r>=0.0f && r < 1.99f, "Restitution value out of range" );
	m_restitution = hkUchar(hkMath::hkToIntFast(r* 128.0f)); 
}

inline hkUlong hkContactPointMaterial::getUserData() const
{ 
	return m_userData; 
}

inline void hkContactPointMaterial::setUserData( hkUlong data )
{ 
	m_userData = data; 
}

inline hkBool32 hkContactPointMaterial::isPotential()
{
	return m_flags & CONTACT_IS_NEW;
}

inline void hkContactPointMaterial::setMaxImpulsePerStep( hkUFloat8 maxImpulse )
{
	m_flags &= ~hkContactPointMaterial::CONTACT_USES_SOLVER_PATH2;
	m_maxImpulse = maxImpulse;
}

inline hkReal hkContactPointMaterial::getMaxImpulsePerStep()
{
	return m_maxImpulse;
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
