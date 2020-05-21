/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HKMEMUTIL_H
#define HKBASE_HKMEMUTIL_H

/// Utility functions to copy memory.
namespace hkMemUtil
{
	template <unsigned int NBYTES> struct TypeFromAlign;
	template <> struct TypeFromAlign<1> { typedef char type; };
	template <> struct TypeFromAlign<2> { typedef hkInt16 type; };
	template <> struct TypeFromAlign<4> { typedef hkInt32 type; };
	template <> struct TypeFromAlign<8> { typedef hkInt64 type; };
	template <unsigned int NBYTES> struct TypeFromAlign: public TypeFromAlign<NBYTES/2> {};

	// called from hkArray::removeAt
	template <unsigned int ELEMENTSIZE, unsigned int ELEMENTALIGN>
	HK_FORCE_INLINE void memCpyOneAligned(void* dst, const void* src);

	template <unsigned int ELEMENTALIGN>
	HK_FORCE_INLINE void memCpy(void* dst, const void* src, int nbytes);

	void HK_CALL memCpy(void* dst, const void* src, int nbytes);
	void HK_CALL memCpyBackwards(void* dst, const void* src, int nbytes);
	void HK_CALL memMove(void* dst, const void* src, int nbytes);
	void HK_CALL memSet(void* dst, const int c, int n);
}

#include <Common/Base/Memory/Util/hkMemUtil.inl>

#endif // HKBASE_HKMEMUTIL_H

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
