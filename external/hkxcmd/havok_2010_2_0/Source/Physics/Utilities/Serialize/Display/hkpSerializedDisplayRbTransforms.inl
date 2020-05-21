/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkpSerializedDisplayRbTransforms::hkpSerializedDisplayRbTransforms() 
{ 
}

int hkpSerializedDisplayRbTransforms::getNumPairs() const
{
	return m_transforms.getSize();
}

int hkpSerializedDisplayRbTransforms::findBodyIndex(const hkpRigidBody* b) const
{
	int nt = m_transforms.getSize();
	for (int i=0; i < nt; ++i)
	{
		if (m_transforms[i].m_rb == b)
			return i;
	}
	return -1;
}

hkTransform& hkpSerializedDisplayRbTransforms::getTransform(int pi) 
{
	HK_ASSERT(0x3f539833,  (pi>=0) && (pi<m_transforms.getSize()) ); 
	return m_transforms[pi].m_localToDisplay;
}

hkpRigidBody* hkpSerializedDisplayRbTransforms::getBody(int pi) 
{
	HK_ASSERT(0x2d2e856c,  (pi>=0) && (pi<m_transforms.getSize()) ); 
	return m_transforms[pi].m_rb;
}

void hkpSerializedDisplayRbTransforms::addPair( hkpRigidBody* m_rb, const hkTransform& t)
{
	DisplayTransformPair* p = m_transforms.expandBy(1);
	p->m_localToDisplay = t;
	p->m_rb = m_rb;

	m_rb->addReference();
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
