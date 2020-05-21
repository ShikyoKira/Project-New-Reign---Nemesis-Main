/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



void hkpFixedBufferCdPointCollector::reset()
{
	m_numPoints			= 0;
	hkpCdPointCollector::reset();
}


hkpFixedBufferCdPointCollector::hkpFixedBufferCdPointCollector(hkpRootCdPoint* buffer, int capacity) : hkpCdPointCollector()
{
	m_pointsArrayBase	= buffer;
	m_capacity			= capacity;
	reset();
}

void hkpFixedBufferCdPointCollector::removeFirstHitAndCopy()
{
	m_numPoints = m_numPoints - 1;		
	hkMemUtil::memCpy<HK_ALIGN_OF(hkpRootCdPoint)>(m_pointsArrayBase, m_pointsArrayBase + 1, m_numPoints*sizeof(hkpRootCdPoint));
}

const hkpRootCdPoint* hkpFixedBufferCdPointCollector::getHits() const
{
	return m_pointsArrayBase;
}


int hkpFixedBufferCdPointCollector::getNumHits() const
{
	return m_numPoints;
}


hkBool hkpFixedBufferCdPointCollector::hasHit() const
{
	return m_numPoints > 0;
}


void hkpFixedBufferCdPointCollector::addCdPoint(const hkpCdPoint& event)
{
	HK_ASSERT2(0xaf635e1f, g_FixedBufferCdPointCollectorAddCdPointCallbackFunc, "You have to register either the default addCdPoint() function using hkpFixedBufferCdPointCollector::registerDefaultAddCdPointFunction() or a custom function using registerFixedBufferCdPointCollectorAddCdPointCallbackFunction().");
	g_FixedBufferCdPointCollectorAddCdPointCallbackFunc(event, this); 
}

void hkpFixedBufferCdPointCollector::removeHit( int index )
{
	HK_ASSERT2( 0x23f89aa9, index < m_numPoints, "Cannot remove points from an empty collector" );
	m_numPoints = m_numPoints - 1;
	m_pointsArrayBase[index] = m_pointsArrayBase[ m_numPoints ];
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
