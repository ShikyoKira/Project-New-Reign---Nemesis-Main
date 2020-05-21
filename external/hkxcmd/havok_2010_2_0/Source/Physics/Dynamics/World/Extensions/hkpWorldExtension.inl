/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline int hkpWorldExtension::getId() const
{
	return m_id; 
}

inline hkpWorld* hkpWorldExtension::getWorld() const
{
	return m_world;
}

inline void hkpWorldExtension::addedToWorld( hkpWorld* world )
{
	addReference();
	m_world = world;
	performAttachments( world );
}

inline void hkpWorldExtension::removedFromWorld( hkpWorld* world )
{
	performDetachments( world );
	m_world = HK_NULL;
	removeReference();
}

template<typename T>
void hkpWorldExtension::requireExtension( hkpWorld* world )
{
	hkpWorldExtension* extension = world->findWorldExtension( T::s_id );
	if ( !extension )
	{
		extension = new T();
		world->addWorldExtension( extension );
		extension->removeReference();
	}
	++extension->m_attachmentCount;
}

template<typename T>
void hkpWorldExtension::releaseExtension( hkpWorld* world )
{
	hkpWorldExtension* extension = world->findWorldExtension( T::s_id );
	HK_ASSERT2( 0xae8b223f, extension, "Cannot release an extension when it hasn't been added to the world." );
	--extension->m_attachmentCount;
	if ( extension->m_attachmentCount == 0 )
	{
		world->removeWorldExtension( extension );
	}
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
