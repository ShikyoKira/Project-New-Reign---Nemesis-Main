/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkpBroadPhaseListener* hkpTypedBroadPhaseDispatcher::getBroadPhaseListener( int a, int b )
{
	HK_ASSERT2(0x4d465ed6, a < HK_MAX_BROADPHASE_TYPE && b < HK_MAX_BROADPHASE_TYPE, "A broadphase listener which has not been set is being called");
	return m_broadPhaseListeners[a][b];
}

inline void hkpTypedBroadPhaseDispatcher::setBroadPhaseListener( hkpBroadPhaseListener* listener, int a, int b )
{
	HK_ASSERT2(0x7e237c69, a < HK_MAX_BROADPHASE_TYPE && b < HK_MAX_BROADPHASE_TYPE, "A broadphase listener which has not been set is being called");
	m_broadPhaseListeners[a][b] = listener;
}

inline hkpBroadPhaseListener* hkpTypedBroadPhaseDispatcher::getNullBroadPhaseListener()
{
	return &m_nullBroadPhaseListener;
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
