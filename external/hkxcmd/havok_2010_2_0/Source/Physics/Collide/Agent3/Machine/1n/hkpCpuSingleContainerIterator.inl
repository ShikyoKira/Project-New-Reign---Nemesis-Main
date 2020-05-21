/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkpCpuSingleContainerIterator::hkpCpuSingleContainerIterator( const HK_SHAPE_CONTAINER* container, const hkpShapeKey* shapeKeys )
: m_container( container ), m_shapeKeys( shapeKeys ), m_lastShapeKey( HK_INVALID_SHAPE_KEY ) 
{
}

void hkpCpuSingleContainerIterator::update() 
{
	m_lastShapeKey = *m_shapeKeys;
}

hkpShapeKey hkpCpuSingleContainerIterator::getShapeKey()
{
	return m_lastShapeKey;
}

void hkpCpuSingleContainerIterator::setShape( hkpCdBody& body )
{
	HK_ASSERT2( 0x342e45a4, m_lastShapeKey != HK_INVALID_SHAPE_KEY, "Shape requested without calling update or when iteration has finished" );
	const hkpShape* extractedShape = m_container->getChildShape( m_lastShapeKey, m_shapeBuffer );
	body.setShape( extractedShape, m_lastShapeKey );
}

hkBool hkpCpuSingleContainerIterator::isCollisionEnabled( const hkpProcessCollisionInput* input, const hkpCdBody* a, const hkpCdBody* b )
{
	HK_ASSERT2( 0x342e45a4, m_lastShapeKey != HK_INVALID_SHAPE_KEY, "Collision filter called without calling update or when iteration has finished" );
	return input->m_filter->isCollisionEnabled( *input, *a, *b, *m_container, m_lastShapeKey );
}

void hkpCpuSingleContainerIterator::advance()
{
	++m_shapeKeys;
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
