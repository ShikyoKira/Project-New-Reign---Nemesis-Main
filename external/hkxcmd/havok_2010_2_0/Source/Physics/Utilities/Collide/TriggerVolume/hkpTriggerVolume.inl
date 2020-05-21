/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkpRigidBody* hkpTriggerVolume::getTriggerBody() const 
{
	return m_triggerBody; 
}

const hkArrayBase<hkpRigidBody*>& hkpTriggerVolume::getOverlappingBodies() const
{
	return m_overlappingBodies;
}

void hkpTriggerVolume::addEvent( hkpRigidBody* body, Operation m_operation )
{
	EventInfo& event = m_eventQueue.expandOne();
	event.m_body = body;
	event.m_operation = m_operation;
	event.m_sortValue = ( (hkUint64) body->m_uid << 32 ) | (hkUint64) m_sequenceNumber;
	++m_sequenceNumber;
	body->addReference();
}

void hkpTriggerVolume::abandonEventQueue()
{
	const int numEvents = m_eventQueue.getSize();
	for ( int i = 0; i < numEvents; ++i )
	{
		m_eventQueue[i].m_body->removeReference();
	}
	m_eventQueue.clear();
}

hkpTriggerVolume* hkpTriggerVolume::getTriggerVolume( hkpRigidBody* body )
{
	return static_cast<hkpTriggerVolume*>( body->getProperty( HK_PROPERTY_TRIGGER_VOLUME ).getPtr() );
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
