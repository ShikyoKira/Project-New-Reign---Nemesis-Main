/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline const hkVector4& hkContactPoint::getPosition() const
{ 
	return m_position; 
}

inline const hkVector4& hkContactPoint::getNormal() const
{ 
	return m_separatingNormal; 
}

inline hkVector4& hkContactPoint::getPosition()
{ 
	return m_position; 
}

inline void hkContactPoint::setPosition( const hkVector4& position )
{	
	m_position = position; 
}


inline const hkVector4& hkContactPoint::getSeparatingNormal() const
{ 
	return m_separatingNormal; 
}

inline hkVector4& hkContactPoint::getSeparatingNormal()
{ 
	return m_separatingNormal; 
}


inline void hkContactPoint::setSeparatingNormal( const hkVector4& normal, hkReal dist )
{
	m_separatingNormal = normal;
	m_separatingNormal(3) = dist;
}

inline void hkContactPoint::setSeparatingNormal( const hkVector4& separatingNormal )
{
	m_separatingNormal = separatingNormal;
}


inline hkReal hkContactPoint::getDistance() const
{
	return m_separatingNormal(3); 
}

inline hkSimdReal hkContactPoint::getDistanceSimdReal() const
{
	return m_separatingNormal.getSimdAt(3); 
}

inline void hkContactPoint::setDistance( hkReal d )
{ 
	m_separatingNormal(3) = d; 
}


inline void hkContactPoint::set( const hkVector4& position, const hkVector4& normal, hkReal dist )
{ 
	m_position = position; 
	m_separatingNormal = normal;
	m_separatingNormal(3) = dist; 
}

inline void hkContactPoint::setNormal( const hkVector4& normal )
{ 
	m_separatingNormal.setXYZ( normal );
}

inline void hkContactPoint::setFlipped( const hkContactPoint& point )
{
	hkVector4 dist; dist.setBroadcast( point.m_separatingNormal, 3 ); 
	m_position.setAddMul4( point.m_position, point.m_separatingNormal, dist );
	m_separatingNormal.setNeg3( point.m_separatingNormal );
}

inline void hkContactPoint::flip()
{
	setFlipped(*this);
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
