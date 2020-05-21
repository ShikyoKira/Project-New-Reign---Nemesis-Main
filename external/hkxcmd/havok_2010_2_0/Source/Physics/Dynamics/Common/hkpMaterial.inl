/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkpMaterial::hkpMaterial() 
:	m_rollingFrictionMultiplier( 0.0f ),
	m_friction( 0.5f ),
	m_restitution( 0.4f )
{
}

inline hkReal hkpMaterial::getFriction() const 
{ 
	return m_friction; 
}

inline hkReal hkpMaterial::getRestitution() const 
{
	return m_restitution; 
}

inline hkReal hkpMaterial::getRollingFrictionMultiplier() const
{
	return m_rollingFrictionMultiplier;
}
	
inline void hkpMaterial::setFriction( hkReal newFriction )
{ 
	HK_ASSERT2( 0xf0160299, newFriction>=0.0f && newFriction <= 255.0f, "Friction value out of range" );
	m_friction = newFriction; 
}

inline void hkpMaterial::setRestitution( hkReal newRestitution ) 
{ 
	HK_ASSERT2( 0xf0160256, newRestitution >= 0.0f && newRestitution < 1.99f, "Restitution value out of range" );
	m_restitution = newRestitution; 
}

inline void hkpMaterial::setRollingFrictionMultiplier( hkReal newRollingFrictionMultiplier )
{
	m_rollingFrictionMultiplier = newRollingFrictionMultiplier;
}


inline hkReal HK_CALL hkpMaterial::getCombinedFriction( hkReal frictionA, hkReal frictionB )
{
	return hkMath::sqrt( frictionA * frictionB );
}

inline hkReal HK_CALL hkpMaterial::getCombinedRestitution( hkReal restitutionA, hkReal restitutionB )
{
	return hkMath::sqrt( restitutionA * restitutionB );
}

inline hkReal HK_CALL hkpMaterial::getCombinedRollingFrictionMultiplier( hkReal multiplierA, hkReal multiplierB )
{
	
	return hkMath::max2( multiplierA, multiplierB );
}

inline void hkpMaterial::setResponseType( enum hkpMaterial::ResponseType t )
{
	m_responseType = t;
}

inline enum hkpMaterial::ResponseType hkpMaterial::getResponseType() const
{
	return m_responseType;
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
