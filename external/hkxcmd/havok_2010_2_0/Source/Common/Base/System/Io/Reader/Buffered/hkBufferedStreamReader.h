/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_BUFFERED_STREAMREADER_H
#define HKBASE_BUFFERED_STREAMREADER_H

#include <Common/Base/System/Io/Reader/hkStreamReader.h>

/// Wraps and buffers an existing unbuffered stream.
class hkBufferedStreamReader : public hkStreamReader
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Create a buffered stream from stream 's' with size 'bufSize'.
			/// Adds a reference to 's'.
		hkBufferedStreamReader(hkStreamReader* s, int bufSize=4096);

			/// Removes reference to the reader in the constructor if applicable.
		~hkBufferedStreamReader();

		virtual int read(void* buf, int nbytes);

		virtual int skip(int nbytes);

		virtual hkBool isOk() const;


			/// Marks up to buffer size are supported.
		virtual hkBool markSupported() const;

			/// Marks up to buffer size are supported.
		virtual hkResult setMark(int markLimit);

			/// Marks up to buffer size are supported.
		virtual hkResult rewindToMark();
		
		virtual hkBool seekTellSupported() const;

		virtual hkResult seek( int offset, SeekWhence whence);

		virtual int tell() const;

	protected:

		hkStreamReader* m_stream; // child stream

		struct Buffer
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkBufferedStreamReader::Buffer );

			Buffer(int cap);
			~Buffer();
			char* begin;
			int current; // current byte index
			int size; // end of usable buffer size - only less than capacity on short read
			int capacity; // buffer region is begin,begin+m_capacity
			int markPos; // start of marked region
			int markLimit; // end of marked region
		};

		Buffer m_buf;

	protected:

			/// Move marked area to start of buffer if necessary.
		void prepareBufferForRefill();
			/// Fill the available area of buffer with a specific number of bytes, and return success/failure of refill. Value of -1 fully fills buffer.
		virtual hkResult refillBuffer(int maxBytes = -1);
};

#endif //HKBASE_BUFFERED_STREAMREADER_H

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
