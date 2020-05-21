/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BUFFER_COMPRESSION
#define HK_BUFFER_COMPRESSION

	/// These are low-level compression routines for single buffers.
	/// For a more user-friendly interface, see hkCompression.h.
	/// For a streaming interface, see hkCompressedStreamReader and hkCompressedStreamWriter.
namespace hkBufferCompression
{
		/// Decompress "inlen" bytes starting at "in" into the buffer of "outlen" bytes starting at "out"
		/// This version does basic error-checking and won't crash on corrupted data (but no integrity checking
		/// is done, so it will happily produce corrupted output). out_start is modified to point one-past-the-last
		/// byte written.
	hkResult decompressBufferChecked(const void* in, hk_size_t inlen, void*& out, hk_size_t outlen);

		/// As decompressBufferChecked, but no checks are done. Invalid input can cause invalid memory accesses and
		/// crashes, and the output buffer is not checked for overflow. Use only when the input is known to be
		/// uncorrupted and decompresses to a known size. Runs somewhat faster than decompressBufferChecked.
	void decompressBufferFast(const void* in, hk_size_t inlen, void*& out);

		/// Compress "inlen" bytes starting at "in" into the buffer of "outlen" bytes starting at "out"
		/// If there was not enough room in the output buffer for the compressed data, returns 0.
		/// Otherwise returns the number of bytes of output produced.
		/// "fastest" selects between the hash compression algorithm (reasonable compression ratio, quite fast) and the
		/// suffix-tree compression algorithm (produces optimal compressed streams, slow).
		/// Decompression speed is not affected by this parameter.
	inline hk_size_t compressBuffer(const void* in, hk_size_t inlen, void* out, hk_size_t outlen, hkBool fastest = true);

		/// Compress using a hashing tree. This gives good compression at reasonable speed.
	hk_size_t hashCompress(const void* in, hk_size_t inlen, void* outbuf, hk_size_t outlen);

		/// Compress using a suffix tree. This gives optimal compression in exchange for
		/// memory and speed.
	hk_size_t suffixTreeCompress(const void* data, hk_size_t length, void* outbuf, hk_size_t outlength);
}

#include <Common/Base/Algorithm/Compression/hkBufferCompression.inl>

#endif // HK_BUFFER_COMPRESSION

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
