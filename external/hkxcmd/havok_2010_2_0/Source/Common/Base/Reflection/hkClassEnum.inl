/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkClassEnum::hkClassEnum( const char* name, const hkClassEnum::Item* items, int numItems )
	:	m_name(name),
		m_items(items),
		m_numItems(numItems),
		m_attributes(HK_NULL),
		m_flags(0)
{
}

inline const char* hkClassEnum::getName() const
{
 	return m_name;
}

inline int hkClassEnum::getNumItems() const
{
	return m_numItems;
}

inline const hkClassEnum::Item& hkClassEnum::getItem(int i) const
{
	HK_ASSERT(0x75c7fff1, i >= 0 && i < m_numItems );
	return m_items[i];
}

inline const hkClassEnum::Flags& hkClassEnum::getFlags() const
{
	return m_flags;
}

inline hkClassEnum::Flags& hkClassEnum::getFlags()
{
	return m_flags;
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
