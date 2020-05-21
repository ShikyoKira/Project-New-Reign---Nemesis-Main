/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline void hkpStiffSpringConstraintData::setInWorldSpace(const hkTransform& bodyATransform,
													 const hkTransform& bodyBTransform,
													 const hkVector4& pivotAW,
													 const hkVector4& pivotBW) 
{
	m_atoms.m_pivots.m_translationA.setTransformedInversePos(bodyATransform,pivotAW);
	m_atoms.m_pivots.m_translationB.setTransformedInversePos(bodyBTransform,pivotBW);

	hkVector4 sepDist;
	sepDist.setSub4( pivotAW, pivotBW );
	m_atoms.m_spring.m_length = sepDist.length3().getReal();
}


inline void hkpStiffSpringConstraintData::setInBodySpace( const hkVector4& pivotA,const hkVector4& pivotB, hkReal restLength )
{
	m_atoms.m_pivots.m_translationA = pivotA;
	m_atoms.m_pivots.m_translationB = pivotB;	
	m_atoms.m_spring.m_length = restLength;
}


inline hkReal hkpStiffSpringConstraintData::getSpringLength() const
{
	return m_atoms.m_spring.m_length;
}


inline void hkpStiffSpringConstraintData::setSpringLength(hkReal length)
{
	m_atoms.m_spring.m_length = length;
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
