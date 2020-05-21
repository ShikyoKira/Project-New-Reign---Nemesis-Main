/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_COMPRESSED_STREAMREADER_H
#define HKBASE_COMPRESSED_STREAMREADER_H

#include <Common/Base/System/Io/Reader/hkStreamReader.h>

	/// Wraps an existing stream, uncompressing the underlying stream's data.
class hkCompressedStreamReader : public hkStreamReader
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

			/// Create a compressed stream from stream 's'.
			/// Adds a reference to 's'.
		hkCompressedStreamReader(hkStreamReader* s);

			/// Removes reference to the reader in the constructor if applicable.
		~hkCompressedStreamReader();

		virtual int read(void* buf, int nbytes);

		virtual int skip(int nbytes);

		virtual hkBool isOk() const;

		// Neither mark/rewind nor seek/tell are supported

	protected:

		hkResult loadNewBlock();

		hkStreamReader* m_stream; // child stream
		hkBool m_ok;

		hkUchar* m_compbuf; // buffer for compressed data
		hkUchar* m_uncompbuf; // buffer for uncompressed data

		int m_uncomppos;
		int m_uncomplen;
		int m_uncompbufsize;

		int m_compbufpos;
		int m_compbuflen;
		int m_compbufsize;
};

#endif //HKBASE_COMPRESSED_STREAMREADER_H

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
