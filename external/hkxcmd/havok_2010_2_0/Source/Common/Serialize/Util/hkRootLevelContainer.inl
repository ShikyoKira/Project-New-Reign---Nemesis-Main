/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE hkRootLevelContainer::hkRootLevelContainer()
{
}

HK_FORCE_INLINE hkRootLevelContainer::hkRootLevelContainer(const hkRootLevelContainer& other)
{
	*this = other;
}

HK_FORCE_INLINE hkRootLevelContainer::~hkRootLevelContainer()
{
}

template<typename T>
inline T* hkRootLevelContainer::findObject( const void* prevObject ) const
{
	if( void* p = findObjectByType(T::staticClass().getName(), prevObject) )
	{
		return static_cast<T*>(p);
	}
	return HK_NULL;
}

HK_FORCE_INLINE hkRootLevelContainer::NamedVariant::NamedVariant()
{
}

HK_FORCE_INLINE void hkRootLevelContainer::NamedVariant::set(const char* name, void* object, const hkClass* klass)
{
	m_name = name;
	m_variant.set(object, klass);
	m_className = m_variant.getClass() ? m_variant.getClass()->getName() : HK_NULL;
}

HK_FORCE_INLINE void hkRootLevelContainer::NamedVariant::set(const char* name, const hkVariant& v)
{
	m_name = name;
	m_variant = v;
	m_className = m_variant.getClass() ? m_variant.getClass()->getName() : HK_NULL;
}

HK_FORCE_INLINE const char* hkRootLevelContainer::NamedVariant::getTypeName() const
{
	return  m_variant.getClass() ? m_variant.getClass()->getName() : static_cast<const char*>(m_className);
}

HK_FORCE_INLINE const char* hkRootLevelContainer::NamedVariant::getName() const
{
	return m_name;
}

HK_FORCE_INLINE void* hkRootLevelContainer::NamedVariant::getObject() const
{
	return m_variant.val();
}

HK_FORCE_INLINE const hkClass* hkRootLevelContainer::NamedVariant::getClass() const
{
	return m_variant.getClass();
}

HK_FORCE_INLINE const hkRefVariant& hkRootLevelContainer::NamedVariant::getRefVariant() const
{
	return m_variant;
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
