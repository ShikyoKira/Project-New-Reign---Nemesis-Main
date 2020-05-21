/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_RELARRAY_H
#define HK_RELARRAY_H

template <typename T>
class hkRelArray
{
	//+hk.MemoryTracker(ignore=True)
public:
	HK_FORCE_INLINE void* operator new(hk_size_t size, hkRelArray<T>* object);
	// RelArray can not be directly reflected or allocated
	hkRelArray() {}
	// Should be constructed in-place
	hkRelArray(hkUint16 size, const void* ptr);
	~hkRelArray() {}

	/// Get array element. Individual elements may be changed,
	/// however the size and storage of the array is fixed
	HK_FORCE_INLINE T& operator [](int index);
	/// Get array element. Individual elements may be changed,
	/// however the size and storage of the array is fixed
	HK_FORCE_INLINE const T& operator [](int index) const;
	
	/// Get array size
	HK_FORCE_INLINE hkUint16 getSize() const;
	/// Get array storage offset
	HK_FORCE_INLINE hkUint16 getOffset() const;

	/// Read-only member access
	HK_FORCE_INLINE const T* begin() const;
	/// Member access
	HK_FORCE_INLINE T* begin();

	/// Set the size directly. For advanced use only
	HK_FORCE_INLINE void _setSize(hkUint16 size);
	/// Set the offset directly. For advanced use only
	HK_FORCE_INLINE void _setOffset(hkUint16 offset);

protected:
	hkUint16 m_size;
	hkUint16 m_offset;
};

#include <Common/Base/Container/RelArray/hkRelArray.inl>

#endif //HK_RELARRAY_H

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
