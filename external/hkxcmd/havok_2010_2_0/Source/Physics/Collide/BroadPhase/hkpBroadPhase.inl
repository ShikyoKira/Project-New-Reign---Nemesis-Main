/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkMultiThreadCheck& hkpBroadPhase::getMultiThreadCheck()
{
	return m_multiThreadCheck;
}

inline const hkMultiThreadCheck& hkpBroadPhase::getMultiThreadCheck() const
{
	// Have to cast away const to add the property if necessary
	hkpBroadPhase* nonConstThis = const_cast<hkpBroadPhase*>(this);
	return nonConstThis->getMultiThreadCheck();
}

inline void hkpBroadPhase::markForRead( ) const
{
	m_multiThreadCheck.markForRead();
}

inline void hkpBroadPhase::markForWrite( )
{
	m_multiThreadCheck.markForWrite();
}

inline void hkpBroadPhase::unmarkForRead( ) const
{
	m_multiThreadCheck.unmarkForRead();
}

inline void hkpBroadPhase::unmarkForWrite()
{
	m_multiThreadCheck.unmarkForWrite();
}

inline void hkpBroadPhase::lock()
{
	if ( m_criticalSection )
	{
		lockImplementation();
	}
	else
	{
		markForWrite();
	}
}

inline void hkpBroadPhase::unlock()
{
	if ( m_criticalSection )
	{
		unlockImplementation();
	}
	else
	{
		unmarkForWrite();
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
