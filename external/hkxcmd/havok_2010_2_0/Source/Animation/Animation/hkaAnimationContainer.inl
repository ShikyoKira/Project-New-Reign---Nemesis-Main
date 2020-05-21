/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE hkaAnimationContainer::hkaAnimationContainer()
{
}

HK_FORCE_INLINE hkaAnimationContainer::hkaAnimationContainer(const hkaAnimationContainer& other)
: hkReferencedObject(other)
{
	*this = other;
}

HK_FORCE_INLINE hkaAnimationContainer::hkaAnimationContainer(hkFinishLoadedObjectFlag f)
	: hkReferencedObject(f), m_skeletons(f), m_animations(f), m_bindings(f), m_attachments(f), m_skins(f)
{
}

HK_FORCE_INLINE hkaAnimationContainer& hkaAnimationContainer::operator=(const hkaAnimationContainer& other)
{
	if( this == &other )
	{
		return *this;
	}
	m_skeletons = other.m_skeletons;
	m_animations = other.m_animations;
	m_bindings = other.m_bindings;
	m_attachments = other.m_attachments;
	m_skins = other.m_skins;
	return *this;
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
