/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkpSimpleContactConstraintData::~hkpSimpleContactConstraintData()
{
	if ( m_atom )
	{
		hkpSimpleContactConstraintAtomUtil::deallocateAtom(m_atom);
	}
}

inline int hkpSimpleContactConstraintData::getNumContactPoints() const
{
	return m_atom->m_numContactPoints;
}

hkContactPointId hkpSimpleContactConstraintData::getContactPointIdAt( int id ) const
{
	int rid = m_idMgrA.indexOf( id );
	return hkContactPointId( rid ); 

}

inline const hkContactPoint& hkpSimpleContactConstraintData::getContactPoint( int /*hkContactPointId*/ id ) const
{
	int index = m_idMgrA.getValueAt(id);
	HK_ASSERT2( 0x456e434e, index != hkpDynamicsCpIdMgr::FREE_VALUE, "Invalid contact point");
	return HK_GET_LOCAL_CONTACT_ATOM(m_atom)->getContactPoints()[ index ]; 
}

inline hkContactPoint& hkpSimpleContactConstraintData::getContactPoint( int /*hkContactPointId*/ id ) 
{
	int index = m_idMgrA.getValueAt(id);
	HK_ASSERT2( 0x456e434e, index != hkpDynamicsCpIdMgr::FREE_VALUE, "Invalid contact point");
	return HK_GET_LOCAL_CONTACT_ATOM(m_atom)->getContactPoints()[ index ]; 
}

inline hkpContactPointProperties* hkpSimpleContactConstraintData::getContactPointProperties( int /*hkContactPointId*/ id )
{
	int index = m_idMgrA.getValueAt(id);

	if ( index == hkpDynamicsCpIdMgr::FREE_VALUE )
	{
		return HK_NULL;
	}

	return HK_GET_LOCAL_CONTACT_ATOM(m_atom)->getContactPointPropertiesStream(index)->asProperties();
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
