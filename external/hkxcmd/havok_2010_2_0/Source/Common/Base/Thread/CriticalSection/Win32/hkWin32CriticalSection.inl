/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

	//
	// Win32
	//

inline hkCriticalSection::~hkCriticalSection()
{
	DeleteCriticalSection(&m_section );
}

inline void hkCriticalSection::setTimersEnabled()
{
#ifdef HK_TIME_CRITICAL_SECTION_LOCKS
	HK_THREAD_LOCAL_SET(hkCriticalSection__m_timeLocks, 1);
#endif
}
inline void hkCriticalSection::setTimersDisabled()
{
#ifdef HK_TIME_CRITICAL_SECTION_LOCKS
	HK_THREAD_LOCAL_SET(hkCriticalSection__m_timeLocks, 0);
#endif
}

#ifndef HK_TIME_CRITICAL_SECTION_LOCKS
	inline void hkCriticalSection::enter()
	{
		EnterCriticalSection(&m_section );
	}

	inline void hkCriticalSection::leave()
	{
		LeaveCriticalSection(&m_section );
	}
#else // HK_TIME_CRITICAL_SECTION_LOCKS



	inline void hkCriticalSection::leave()
	{
		LeaveCriticalSection(&m_section );
	}
#endif // HK_TIME_CRITICAL_SECTION_LOCKS


HK_FORCE_INLINE hkUint32 HK_CALL hkCriticalSection::atomicExchangeAdd(hkUint32* var, int increment)
{
	return InterlockedExchangeAdd( (LONG*)var, increment);
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
