/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/Container/RelArray/hkRelArrayUtil.h>

template<typename EnclosingClass>
hkRelArrayBuilder<EnclosingClass>::hkRelArrayBuilder(EnclosingClass* enclosingObject) : m_enclosingObject(enclosingObject), m_offsetSoFar(0)
{
}

template<typename EnclosingClass>
template<typename RelArrayType>
RelArrayType* hkRelArrayBuilder<EnclosingClass>::addRelArray(hkArray<RelArrayType>& relArray)
{
	//HK_ASSERT2(0x4204afe6, size >= 0 && size < 65536, "Array size too large for hkRelArray");
	//HK_ASSERT2(0x539c546b, (size * hkSizeOf(RelArrayType) + m_offsetSoFar) < 65536, "Object too large for hkRelArray");
	const int size = relArray.getSize();
	
	// Pad to 16 bytes
	m_offsetSoFar = HK_NEXT_MULTIPLE_OF(hkRelArrayUtil::RELARRAY_ALIGNMENT, m_offsetSoFar);
	RelArrayType* oldPtr = relArray.begin();
	relArray._setDataUnchecked(reinterpret_cast<RelArrayType*>(hkSizeOf(EnclosingClass) + reinterpret_cast<char *>(m_enclosingObject) + m_offsetSoFar), size, relArray.getCapacityAndFlags());
	//new(&relArray) hkRelArray<RelArrayType>(hkUint16(size), reinterpret_cast<void*>(hkSizeOf(EnclosingClass) + reinterpret_cast<char *>(m_enclosingObject) + m_offsetSoFar));
	m_offsetSoFar += size * hkSizeOf(RelArrayType);
	return oldPtr;
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
