/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline hkpWorld* hkpWorldObject::getWorld() const
{ 
	return m_world;
}

hkpWorldObject::~hkpWorldObject()
{
	// Do not delete this if it is still in a world.
	HK_ASSERT2(0x3933b5a9,  getWorld() == HK_NULL, "hkpWorldObject is in an hkpWorld and cannot be deleted.");

	if (m_collidable.getShape())
	{
		m_collidable.getShape()->removeReference();
	}
}


inline hkpCollidable* hkpWorldObject::getCollidableRw()
{
#ifdef HK_DEBUG_MULTI_THREADING
	checkReadWrite();
#endif
	return &m_collidable;
}

inline const hkpCollidable* hkpWorldObject::getCollidable() const
{
#ifdef HK_DEBUG_MULTI_THREADING
	checkReadOnly();
#endif
	return &m_collidable;
}


inline hkpLinkedCollidable* hkpWorldObject::getLinkedCollidable()
{
#ifdef HK_DEBUG_MULTI_THREADING
	checkReadWrite();
#endif
	return &m_collidable;
}

inline const hkpLinkedCollidable* hkpWorldObject::getCollidableMtUnchecked() const
{
	return &m_collidable;
}

inline const hkpLinkedCollidable* hkpWorldObject::getLinkedCollidable() const
{
#ifdef HK_DEBUG_MULTI_THREADING
	checkReadOnly();
#endif
	return &m_collidable;
}

inline hkBool hkpWorldObject::isAddedToWorld() const
{
	return m_world != HK_NULL;
}

void hkpWorldObject::setWorld( hkpWorld* world )
{
	m_world = world;
}

inline hkpPropertyValue hkpWorldObject::getProperty( hkUint32 key, MtChecks mtCheck ) const
{
	HK_ON_DEBUG_MULTI_THREADING( if ( mtCheck == MULTI_THREADING_CHECKS_ENABLE) {checkReadOnly(); } );

	for (int i = 0; i < m_properties.getSize(); ++i)
	{
		if (m_properties[i].m_key == key)
		{
			return m_properties[i].m_value;
		}
	}

	hkpPropertyValue returnValue;
	returnValue.m_data = 0;

	return returnValue;
}

inline bool hkpWorldObject::hasProperty( hkUint32 key, MtChecks mtCheck ) const
{
	HK_ON_DEBUG_MULTI_THREADING( if ( mtCheck == MULTI_THREADING_CHECKS_ENABLE) {checkReadOnly(); } );

	for (int i = 0; i < m_properties.getSize(); ++i)
	{
		if (m_properties[i].m_key == key)
		{
			return true;
		}
	}
	return false;
}

inline void hkpWorldObject::clearAndDeallocateProperties()
{
	m_properties.clearAndDeallocate();
}

inline hkUlong hkpWorldObject::getUserData() const
{
	return m_userData;
}

inline void hkpWorldObject::setUserData( hkUlong data )
{
	m_userData = data;
}

inline const char* hkpWorldObject::getName() const
{
	return m_name;
}

inline void hkpWorldObject::setName( const char* name )
{
	m_name = name;
}

 
inline hkMultiThreadCheck& hkpWorldObject::getMultiThreadCheck()
{
	return m_multiThreadCheck;
}

inline void hkpWorldObject::markForRead( )
{
	getMultiThreadCheck().markForRead();
}

inline void hkpWorldObject::markForWrite( )
{
#ifdef HK_DEBUG_MULTI_THREADING
	hkpWorldObject::markForWriteImpl();
#endif
}

inline void hkpWorldObject::unmarkForRead( )
{
	getMultiThreadCheck().unmarkForRead();
}

inline void hkpWorldObject::unmarkForWrite()
{
	getMultiThreadCheck().unmarkForWrite();
}



inline const hkMultiThreadCheck& hkpWorldObject::getMultiThreadCheck() const
{
	return m_multiThreadCheck;
}


inline void hkpWorldObject::copyProperties( const hkpWorldObject* otherObj )
{
	m_properties = otherObj->m_properties;
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
