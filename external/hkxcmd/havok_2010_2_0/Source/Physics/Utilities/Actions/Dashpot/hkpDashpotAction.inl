/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline void hkpDashpotAction::setInWorldSpace( const hkVector4& pivotA, const hkVector4& pivotB )

{
	hkpRigidBody* ra = static_cast<hkpRigidBody*>( m_entityA );
	hkpRigidBody* rb = static_cast<hkpRigidBody*>( m_entityB );
	HK_ASSERT2(0xf568efca, ra && rb, "Bodies not set in dashpot.");

	m_point[0].setTransformedInversePos(ra->getTransform(),pivotA);
	m_point[1].setTransformedInversePos(rb->getTransform(),pivotB);
}

inline void hkpDashpotAction::setInBodySpace( const hkVector4& pivotA,const hkVector4& pivotB)
{
	m_point[0] = pivotA;
	m_point[1]= pivotB;
}

inline hkReal hkpDashpotAction::getStrength() const 
{ 
	return m_strength; 
}
	
inline void hkpDashpotAction::setStrength(hkReal s)
{ 
	m_strength = s;
}
	
inline hkReal hkpDashpotAction::getDamping() const 
{ 
	return m_damping; 
}
	
inline void hkpDashpotAction::setDamping(hkReal d) 
{ 
	m_damping = d; 
}

inline const hkVector4& hkpDashpotAction::getPointA() const
{
  return m_point[0];
}

inline const hkVector4& hkpDashpotAction::getPointB() const
{
  return m_point[1];
}

inline void hkpDashpotAction::setPointA(const hkVector4& pa)
{
  m_point[0] = pa;
}

inline void hkpDashpotAction::setPointB(const hkVector4& pb)
{
  m_point[1] = pb;
}

inline void hkpDashpotAction::getPoints(hkVector4& pa, hkVector4& pb)
{
  pa = m_point[0];
  pb = m_point[1];
}

inline void hkpDashpotAction::setPoints(const hkVector4& pa, const hkVector4& pb)
{
  m_point[0] = pa;
  m_point[1] = pb;
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
