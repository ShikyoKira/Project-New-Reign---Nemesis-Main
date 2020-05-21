/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// hkMemUtil
namespace hkMemUtil
{
	// called from hkArray::removeAt
	template<unsigned int ELEMENTSIZE, unsigned int ALIGN>
	HK_FORCE_INLINE void memCpyOneAligned(void* dst, const void* src)
	{
		typedef typename TypeFromAlign<ALIGN>::type CopyType;
		unsigned int i = 0;
		do
		{
			static_cast<CopyType*>(dst)[i] = static_cast<const CopyType*>(src)[i];
		}
		while( ++i < ELEMENTSIZE/sizeof(CopyType) );
	}

	template<unsigned int ELEMENTALIGN>
	HK_FORCE_INLINE void memCpy(void* dst, const void* src, int n)
	{
		typedef typename TypeFromAlign<ELEMENTALIGN>::type CopyType;
		HK_ON_DEBUG( if ( dst == src ) HK_BREAKPOINT(0x0) ); // Can't use assert here because of include order
		for( int i = 0, j = 0; i < n; i += sizeof(CopyType), ++j )
		{
			static_cast<CopyType*>(dst)[j] = static_cast<const CopyType*>(src)[j];
		}
	}
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
