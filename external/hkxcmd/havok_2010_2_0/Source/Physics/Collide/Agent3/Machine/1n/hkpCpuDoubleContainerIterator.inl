/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkpCpuDoubleContainerIterator::hkpCpuDoubleContainerIterator( const HK_SHAPE_CONTAINER* containerA, const HK_SHAPE_CONTAINER* containerB, const hkpShapeKeyPair* shapeKeys )
: m_containerA( containerA ), m_containerB( containerB ), m_shapeKeys( shapeKeys ), m_extractedShapeKeyA( HK_INVALID_SHAPE_KEY ), m_extractedShapeKeyB( HK_INVALID_SHAPE_KEY ) 
{ 
	m_lastShapeKeyPair.m_shapeKeyA = HK_INVALID_SHAPE_KEY; 
	m_lastShapeKeyPair.m_shapeKeyB = HK_INVALID_SHAPE_KEY; 
}

void hkpCpuDoubleContainerIterator::update() 
{
	m_lastShapeKeyPair = *m_shapeKeys;
}

hkpShapeKeyPair hkpCpuDoubleContainerIterator::getShapeKeyPair()
{
	return m_lastShapeKeyPair;
}

void hkpCpuDoubleContainerIterator::setShapes( hkpCdBody& bodyA, hkpCdBody& bodyB )
{
	HK_ASSERT2( 0x342e45a4, m_lastShapeKeyPair.m_shapeKeyA != HK_INVALID_SHAPE_KEY, "Shape requested without calling update or when iteration has finished" );
	if ( m_extractedShapeKeyA != m_lastShapeKeyPair.m_shapeKeyA )
	{
		m_extractedShapeA = m_containerA->getChildShape( m_lastShapeKeyPair.m_shapeKeyA, m_shapeBufferA );
		m_extractedShapeKeyA = m_lastShapeKeyPair.m_shapeKeyA;
		bodyA.setShape( m_extractedShapeA, m_lastShapeKeyPair.m_shapeKeyA );
	}
	if ( m_extractedShapeKeyB != m_lastShapeKeyPair.m_shapeKeyB )
	{
		m_extractedShapeB = m_containerB->getChildShape( m_lastShapeKeyPair.m_shapeKeyB, m_shapeBufferB );
		m_extractedShapeKeyB = m_lastShapeKeyPair.m_shapeKeyB;
		bodyB.setShape( m_extractedShapeB, m_lastShapeKeyPair.m_shapeKeyB );
	}
}


hkBool hkpCpuDoubleContainerIterator::isCollisionEnabled( const hkpProcessCollisionInput* input, const hkpCdBody* collectionBodyA, const hkpCdBody* collectionBodyB )
{
	HK_ASSERT2( 0x342e45a4, m_lastShapeKeyPair.m_shapeKeyA != HK_INVALID_SHAPE_KEY, "Collision filter called without calling update or when iteration has finished" );
	return input->m_filter->isCollisionEnabled( *input, *collectionBodyA, *collectionBodyB, *m_containerA, *m_containerB, m_lastShapeKeyPair.m_shapeKeyA, m_lastShapeKeyPair.m_shapeKeyB );
}

void hkpCpuDoubleContainerIterator::advance()
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
