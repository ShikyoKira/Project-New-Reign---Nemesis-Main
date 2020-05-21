/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_COMPRESSED_STREAMWRITER_H
#define HKBASE_COMPRESSED_STREAMWRITER_H

#include <Common/Base/System/Io/Writer/hkStreamWriter.h>

	/// Wraps and compresses an existing stream
class hkCompressedStreamWriter : public hkStreamWriter
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Create a buffered stream from stream 's' with block size 'blockSize'.
			/// Adds a reference to 's'.
		hkCompressedStreamWriter(hkStreamWriter* s, int bufSize=(1<<16));

			/// Removes a reference to its sub stream.
		~hkCompressedStreamWriter();

		virtual int write(const void* buf, int nbytes);

		virtual void flush();

		virtual hkBool isOk() const;

		// Seek and tell are not supported

	protected:

		void writeBlock();

		hkStreamWriter* m_stream;
		int m_bufSize;

		hkUchar* m_uncompbuf;
		int m_uncompbufsize;
		int m_uncompbufpos;
};

#endif // HKBASE_BUFFERED_STREAMWRITER_H

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
