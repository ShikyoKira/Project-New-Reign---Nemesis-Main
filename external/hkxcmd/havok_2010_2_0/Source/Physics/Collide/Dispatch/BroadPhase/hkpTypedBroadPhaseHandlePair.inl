/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE hkpTypedBroadPhaseHandle* hkpTypedBroadPhaseHandlePair::getElementA() const
{
	return static_cast<hkpTypedBroadPhaseHandle*>(m_a);
}

HK_FORCE_INLINE hkpTypedBroadPhaseHandle* hkpTypedBroadPhaseHandlePair::getElementB() const
{
	return static_cast<hkpTypedBroadPhaseHandle*>(m_b);
}


HK_FORCE_INLINE hkpTypedBroadPhaseHandle* hkpTypedBroadPhaseHandlePair::getOther(hkpTypedBroadPhaseHandle* elem) const
{
	hkUlong a = hkUlong(m_a);
	hkUlong b = hkUlong(m_b);
	hkUlong e = hkUlong(elem);
	return reinterpret_cast<hkpTypedBroadPhaseHandle*>(a^b^e);
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
