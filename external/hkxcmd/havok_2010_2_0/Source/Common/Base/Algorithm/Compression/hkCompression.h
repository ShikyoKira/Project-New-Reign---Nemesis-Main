/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COMPRESSION
#define HK_COMPRESSION

	/// Compression and decompression of arbitrary amounts of data block by block
namespace hkCompression
{
	enum Result_
	{
		COMP_ERROR,      // Stream is corrupted
		COMP_NEEDINPUT,  // Success, zero or more blocks compressed/decompressed, waiting for more input
		COMP_NEEDOUTPUT, // Success, zero or more blocks compressed/decompressed, ran out of output buffer space
	};
	typedef hkEnum<Result_, int> Result;
	typedef void* voidptr;
	typedef const void* const_voidptr;

		/// Compress "inlen" bytes of data starting at "in" into a buffer of size "outlen" starting at "out"
		/// "bufsize" gives the maximum input/output buffer size necessary for decompression. Larger buffer
		/// sizes will give somewhat better compression.
		/// The concatenation of two compressed streams is a valid compressed stream, which gives the concatenation
		/// of the uncompressed data when decompressed.
		/// The "in" and "out" pointers are updated to point one-past-the-last byte consumed or produced.
		/// The "successful" return from this function is COMP_NEEDINPUT, indicating that all of the input was compressed
		/// and more can be added if desired.
	Result compress(const_voidptr& in, hk_size_t inlen, voidptr& out, hk_size_t outlen, hk_size_t bufsize = (1<<16));

		/// Decompress "inlen" bytes of data starting at "in" into a buffer of size "outlen" starting at "out"
		/// The "in" and "out" pointers are updated to point one-past-the-last byte consumed or produced.
		/// If "checkinput" is false, this will run somewhat faster but omit bounds checks on input and output.
		/// The "successful" return from this function is COMP_NEEDINPUT, indicating that all of the input was compressed
		/// and more can be added if desired.
	Result decompress(const_voidptr& in, hk_size_t inlen, voidptr& out, hk_size_t outlen, bool checkinput = true);

		/// Calculate the minimum amounts of input data and output space which must be given to decompress() to
		/// ensure that it will produce at least some data. At least BLOCK_HEADER_SIZE bytes of input must already
		/// be read to use this function.
	hkResult getDecompressionBufferSize(const void* in, hk_size_t& compressed_out, hk_size_t& uncompressed_out);

		/// The minimum amount to be read for a stream before we know the decompression size.
	enum { BLOCK_HEADER_SIZE = 16 };
}

#endif // HK_COMPRESSION

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
