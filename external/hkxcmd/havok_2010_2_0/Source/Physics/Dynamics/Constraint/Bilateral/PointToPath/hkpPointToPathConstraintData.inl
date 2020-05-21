/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
// inline functions for hkPointToPathCinfo and hkpPointToPathConstraintData.
//
inline void hkpPointToPathConstraintData::setMaxFrictionForce(hkReal fmag)
{
	m_maxFrictionForce = fmag;
}


inline hkReal hkpPointToPathConstraintData::getMaxFrictionForce()
{
	return m_maxFrictionForce;
}


inline hkpParametricCurve* hkpPointToPathConstraintData::getPath()   const
{
	return m_path;
}

inline const hkTransform& hkpPointToPathConstraintData::getConstraintToLocalTransform(int i) const
{
	HK_ASSERT(0x6deb57fe, (i >=0) && ( i <= 2));
	return m_transform_OS_KS[i];
}

inline void hkpPointToPathConstraintData::setReferenceBodyPathTransform(const hkTransform& t)
{
	m_transform_OS_KS[1] = t;
}

inline void hkpPointToPathConstraintData::setAttachedBodyPivotTransform(const hkTransform& t)
{
	m_transform_OS_KS[0] = t;
}

inline void hkpPointToPathConstraintData::setAttachedBodyPivotOrientation(const hkRotation& r)
{
	m_transform_OS_KS[0].setRotation(r);
}

inline void hkpPointToPathConstraintData::setOrientationType(OrientationConstraintType otype)
{
	m_angularConstrainedDOF = otype;
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
