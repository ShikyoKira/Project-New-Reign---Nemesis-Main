/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
hkpSerializedDisplayMarkerList::hkpSerializedDisplayMarkerList()
{
}

hkpSerializedDisplayMarkerList::~hkpSerializedDisplayMarkerList()
{
	int i;
	for (i=0; i < m_markers.getSize(); ++i)
		m_markers[i]->removeReference();
}

int hkpSerializedDisplayMarkerList::getNumMarkers() const
{
	return m_markers.getSize();
}

hkpSerializedDisplayMarker* hkpSerializedDisplayMarkerList::getMarker(int pi) const
{
	HK_ASSERT(0x6b15722e,  pi >= 0 && pi < m_markers.getSize() );
	return m_markers[pi];
}

void hkpSerializedDisplayMarkerList::addMarker( hkpSerializedDisplayMarker* m )
{
	hkpSerializedDisplayMarker** nm = m_markers.expandBy(1);
	*nm = m;
	m->addReference();
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
