/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline const char* hkClassMember::getName() const
{
	return m_name;
}

inline hkClassMember::Type hkClassMember::getType() const
{
	return static_cast<Type>(m_type);
}

inline void hkClassMember::setType(hkClassMember::Type type)
{
	m_type = type;
}

inline hkClassMember::Type hkClassMember::getSubType() const
{
	return static_cast<Type>(m_subtype);
}

inline void hkClassMember::setSubType(hkClassMember::Type subtype)
{
	m_subtype = subtype;
}

inline int hkClassMember::getOffset() const
{
	return m_offset;
}

inline void hkClassMember::setOffset(int offset)
{
	m_offset = static_cast<hkUint16>(offset);
}

inline const hkClassMember::Flags& hkClassMember::getFlags() const
{
	return m_flags;
}

inline hkClassMember::Flags& hkClassMember::getFlags()
{
	return m_flags;
}

inline hkBool hkClassMember::hasClass() const
{
	return m_class != HK_NULL;
}

inline hkBool hkClassMember::hasEnumClass() const
{
	return m_enum != HK_NULL;
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
