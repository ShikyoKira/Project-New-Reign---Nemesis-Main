/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline int hkpBoxBoxManifold::getNumPoints()
{
	return m_numPoints;
}

inline hkpFeatureContactPoint& hkpBoxBoxManifold::operator[]( int index )
{
	return m_contactPoints[index];
}

inline hkBool hkpBoxBoxManifold::findInManifold( const hkpFeatureContactPoint& fcp )
{
	for (int i = m_numPoints-1; i>=0; i--)
	{
		if ( m_contactPoints[i].m_featureIdA == fcp.m_featureIdA && m_contactPoints[i].m_featureIdB == fcp.m_featureIdB )
		{
			return true;
		}
	}
	return false;
}

inline hkBool hkpBoxBoxManifold::isComplete()
{
	return m_isComplete; 
}

inline void hkpBoxBoxManifold::setComplete( hkBool complete )
{
	m_isComplete = complete;	
}

inline hkBool hkpBoxBoxManifold::hasNoPointsLeft()
{
	return (m_numPoints >= HK_BOXBOX_MANIFOLD_MAX_POINTS); 
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
