/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hk_size_t hkBufferCompression::compressBuffer(const void* in, hk_size_t inlen, void* out, hk_size_t outlen, hkBool fastest)
{
	hk_size_t compressedSize;
	if (fastest)
	{
		compressedSize = hashCompress(in, inlen, out, outlen);
	}
	else
	{
		compressedSize = suffixTreeCompress(in, inlen, out, outlen);
	}

	#ifdef HK_DEBUG
		if (compressedSize)
		{
			hkUchar* block = hkMemTempBlockAlloc<hkUchar>((int)inlen);
			void* buf = block;
			hkResult res = decompressBufferChecked(out, compressedSize, buf, inlen);
			HK_ASSERT(0x62d1b071, res == HK_SUCCESS);
			HK_ASSERT(0x528d911b, block+int(inlen) == buf);
			HK_ASSERT(0x597ae5e1, hkString::memCmp(block, in, (int)inlen) == 0 );
			hkMemTempBlockFree(block, (int)inlen);
		}
	#endif

	return compressedSize;
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
