/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE void hkStopwatch::reset()
{
	m_ticks_at_start = 0;
	m_ticks_total = 0;
	m_ticks_at_split = 0;
	m_split_total = 0;
	m_running_flag = 0;
	m_num_timings = 0;
}

HK_FORCE_INLINE hkStopwatch::hkStopwatch(const char* name)
	: m_name(name)
{
	reset();
}

HK_FORCE_INLINE void hkStopwatch::start()
{
	HK_ASSERT(0x28f9f73c, ! m_running_flag);
	m_running_flag = true;
	m_ticks_at_start = getTickCounter();
	m_ticks_at_split = m_ticks_at_start;
}

HK_FORCE_INLINE void hkStopwatch::stop()
{
	HK_ASSERT(0x1d900cad, m_running_flag);

	m_running_flag = false;
	hkUint64 ticks_now = getTickCounter();
	m_ticks_total += ticks_now - m_ticks_at_start;
	m_split_total += ticks_now - m_ticks_at_split;
	++m_num_timings;
}

HK_FORCE_INLINE void hkStopwatch::resume()
{
	m_running_flag = true;
}

HK_FORCE_INLINE hkBool hkStopwatch::isRunning() const
{
	return m_running_flag;
}

HK_FORCE_INLINE const char* hkStopwatch::getName() const
{
	return m_name;
}

HK_FORCE_INLINE int hkStopwatch::getNumTimings() const
{
	return m_num_timings;
}

HK_FORCE_INLINE hkUint64 hkStopwatch::getSplitTicks()
{
	hkUint64 t = m_split_total;
	if(m_running_flag) 
	{
		hkUint64 now = getTickCounter();
		t += now - m_ticks_at_split;
		m_ticks_at_split = now;
	}
	
	m_split_total = 0;
	return t;
}

HK_FORCE_INLINE hkReal hkStopwatch::getSplitSeconds()
{
	return divide64(getSplitTicks(), getTicksPerSecond());
}

HK_FORCE_INLINE hkUint64 hkStopwatch::getElapsedTicks() const
{
	hkUint64 ticks = m_ticks_total;
	if( m_running_flag )
	{
		ticks += getTickCounter() - m_ticks_at_start;
	}
	return ticks;
}

HK_FORCE_INLINE hkReal hkStopwatch::getElapsedSeconds() const
{
	return divide64(getElapsedTicks(), getTicksPerSecond());
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
