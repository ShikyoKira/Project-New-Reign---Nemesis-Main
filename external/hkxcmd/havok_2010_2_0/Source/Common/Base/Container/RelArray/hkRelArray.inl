/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template <typename T>
HK_FORCE_INLINE hkRelArray<T>::hkRelArray(hkUint16 size, const void* ptr) : m_size(size)
{
	HK_ASSERT2(0x5259899b, (reinterpret_cast<hkUlong>(ptr) & hkUlong(0x0f)) == 0, "Incorrect alignment for hkRelArray member");
	m_offset = hkUint16(reinterpret_cast<const char*>(ptr) - reinterpret_cast<const char*>(this));
}

template <typename T>
HK_FORCE_INLINE const T& hkRelArray<T>::operator [](int index) const
{
	HK_ASSERT2(0x46e01b1d, (index >= 0) && (index < m_size), "Invalid index in hkRelArray");
	return *(reinterpret_cast<const T*>(reinterpret_cast<const char*>(this) + m_offset) + index);
}

template <typename T>
HK_FORCE_INLINE T& hkRelArray<T>::operator [](int index)
{
	HK_ASSERT2(0x4921797f, (index >= 0) && (index < m_size), "Invalid index in hkRelArray");
	return *(reinterpret_cast<T*>(reinterpret_cast<char*>(this) + m_offset) + index);
}

template <typename T>
HK_FORCE_INLINE hkUint16 hkRelArray<T>::getSize() const
{
	return m_size;
}

template <typename T>
HK_FORCE_INLINE hkUint16 hkRelArray<T>::getOffset() const
{
	return m_offset;
}

template <typename T>
HK_FORCE_INLINE const T* hkRelArray<T>::begin() const
{
	return reinterpret_cast<const T*>(reinterpret_cast<const char*>(this) + m_offset);
}

template <typename T>
HK_FORCE_INLINE T* hkRelArray<T>::begin()
{
	return reinterpret_cast<T*>(reinterpret_cast<char*>(this) + m_offset);
}

template <typename T>
HK_FORCE_INLINE void hkRelArray<T>::_setSize(hkUint16 size)
{
	m_size = size;
}

template <typename T>
HK_FORCE_INLINE void hkRelArray<T>::_setOffset(hkUint16 offset)
{
	m_offset = offset;
}

template <typename T>
HK_FORCE_INLINE void* hkRelArray<T>::operator new(hk_size_t size, hkRelArray<T>* object)
{
	HK_ASSERT2(0x1f4a821b, size == hkSizeOf(hkRelArray<T>), "Invalid size for hkRelArray placement");
	return object;
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
