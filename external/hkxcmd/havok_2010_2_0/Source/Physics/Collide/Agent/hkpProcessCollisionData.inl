/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline int hkpProcessCollisionData::getNumContactPoints() const
{
	return int(m_firstFreeContactPoint - &m_contactPoints[0]);
}

inline hkpProcessCdPoint* hkpProcessCollisionData::getFirstContactPoint()
{
	return &m_contactPoints[0];
}

inline hkpProcessCdPoint* hkpProcessCollisionData::getEnd()
{
	return m_firstFreeContactPoint;
}

inline hkpProcessCdPoint& hkpProcessCollisionData::getContactPoint( int i )
{
	return m_contactPoints[i];
}

inline hkBool hkpProcessCollisionData::isEmpty() const
{
	return m_firstFreeContactPoint == &m_contactPoints[0];
}

inline hkBool hkpProcessCollisionData::hasToi() const
{
	return m_toi.m_time != HK_REAL_MAX;
}

inline hkContactPoint& hkpProcessCollisionData::getToiContactPoint()
{
	return m_toi.m_contactPoint;
}

inline hkTime hkpProcessCollisionData::getToi() const
{
	return m_toi.m_time;
}

inline hkpContactPointProperties& hkpProcessCollisionData::getToiProperties()
{
	return m_toi.m_properties;
}


hkpProcessCollisionData::ToiInfo::ToiInfo() : m_time( HK_REAL_MAX ) 
{
}

hkpProcessCollisionData::hkpProcessCollisionData( hkCollisionConstraintOwner* owner )
{
	m_constraintOwner = owner;
}


void hkpProcessCollisionData::ToiInfo::flip()
{
	hkVector4& norm = m_contactPoint.getSeparatingNormal();
	norm.setNeg3( norm );
	m_gskCache.flip();
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
