/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_MEMORY_STREAMREADER_H
#define HKBASE_MEMORY_STREAMREADER_H

#include <Common/Base/System/Io/Reader/hkStreamReader.h>

/// Wraps a reader around a memory fragment.
class hkMemoryStreamReader : public hkStreamReader
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
            ///
        enum MemoryType
        {
            MEMORY_COPY,
            MEMORY_TAKE,
            MEMORY_INPLACE
        };

			/// Create a stream from the specified memory.
			/// If MemoryType is MEMORY_COPY, the memory is copied into
			/// an internal buffer and freed on destruction.
			/// MEMORY_TAKE will take ownership of the memory (which was
			/// allocated with hkAllocate) and hkDeallocate it on destruction.
			/// MEMORY_INPLACE will use the memory in place
			/// and must exist for the lifetime of this object.
		hkMemoryStreamReader(const void* mem, int memSize, MemoryType t);

		~hkMemoryStreamReader();

		virtual int read(void* buf, int nbytes);

		virtual int skip(int nbytes);

		virtual hkBool isOk() const;


			/// Marks up to buffer size are supported.
		virtual hkBool markSupported() const;

			/// Marks up to buffer size are supported.
		virtual hkResult setMark(int markLimit);

			/// Marks up to buffer size are supported.
		virtual hkResult rewindToMark();

			/// Seek/tell is supported.
		virtual hkBool seekTellSupported() const;

		virtual hkResult seek(int offset, SeekWhence whence);

		virtual int tell() const;

	protected:

		char* m_buf;
		int m_bufCurrent; // current byte index
		int m_bufSize; // end of usable buffer size
		int m_markPos; // start of marked region
		MemoryType m_memType; // owned or referenced
};

#endif //HKBASE_MEMORY_STREAMREADER_H

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
