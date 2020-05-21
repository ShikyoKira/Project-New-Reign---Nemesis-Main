/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


//////////////////////////////////////////////////////////////////////////
// hkLimitedHinge set and get methods

//////////////////////////////////////////////////////////////////////////

inline void hkpHingeLimitsData::setMaxAngularLimit(hkReal rad)
{
	m_atoms.m_angLimit.m_maxAngle = rad;
}

inline void hkpHingeLimitsData::setMinAngularLimit(hkReal rad)
{
	m_atoms.m_angLimit.m_minAngle = rad;
}

inline hkReal hkpHingeLimitsData::getMaxAngularLimit() const
{
	return m_atoms.m_angLimit.m_maxAngle;
}

inline hkReal hkpHingeLimitsData::getMinAngularLimit() const
{
	return m_atoms.m_angLimit.m_minAngle;
}


inline void hkpHingeLimitsData::setAngularLimitsTauFactor( hkReal mag )
{
	m_atoms.m_angLimit.m_angularLimitsTauFactor = mag;
}

inline hkReal hkpHingeLimitsData::getAngularLimitsTauFactor() const
{
	return m_atoms.m_angLimit.m_angularLimitsTauFactor;
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
