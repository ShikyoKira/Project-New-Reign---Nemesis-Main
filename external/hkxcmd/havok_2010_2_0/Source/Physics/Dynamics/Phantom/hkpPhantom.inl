/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


/// Helper function that returns a hkpPhantom if the collidable's broadphase handle is of type hkpWorldObject::BROAD_PHASE_PHANTOM
inline hkpPhantom* HK_CALL hkpGetPhantom(const hkpCollidable* collidable)
{
	if ( collidable->getType() == hkpWorldObject::BROAD_PHASE_PHANTOM )
	{
		return static_cast<hkpPhantom*>( hkGetWorldObject(collidable) );
	}
	return HK_NULL;
}

hkpCollidableAccept hkpPhantom::fireCollidableAdded( const hkpCollidable* collidable )
{
	hkpCollidableAddedEvent event;
	event.m_collidable = collidable;
	event.m_phantom = this;
	event.m_collidableAccept = HK_COLLIDABLE_ACCEPT;

	for ( int i = m_overlapListeners.getSize()-1; i >= 0; i-- )
	{
		if (m_overlapListeners[i] != HK_NULL)
		{
			m_overlapListeners[i]->collidableAddedCallback( event );
		}
	}
	// cleanupNullPointers done at the end of updateBroadPhase 
	return event.m_collidableAccept;
}

void hkpPhantom::fireCollidableRemoved( const hkpCollidable* collidable, hkBool collidableWasAdded )
{
	hkpCollidableRemovedEvent event;
	event.m_collidable = collidable;
	event.m_phantom = this;
	event.m_collidableWasAdded = collidableWasAdded;

	for ( int i = m_overlapListeners.getSize()-1; i >= 0; i-- )
	{
		if (m_overlapListeners[i] != HK_NULL)
		{
			m_overlapListeners[i]->collidableRemovedCallback( event );
		}
	}
	// cleanupNullPointers done at the end of updateBroadPhase 
}


inline hkpPhantom::hkpPhantom( const hkpShape* shape )
	: hkpWorldObject( shape, BROAD_PHASE_PHANTOM )
{
	m_collidable.setOwner( this );
}

inline const hkArray<hkpPhantomListener*>& hkpPhantom::getPhantomListeners() const
{
	return m_phantomListeners;
}

inline const hkArray<hkpPhantomOverlapListener*>& hkpPhantom::getPhantomOverlapListeners() const
{
	return m_overlapListeners;
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
