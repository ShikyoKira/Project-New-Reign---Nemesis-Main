/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_SUB_STREAMWRITER_H
#define HKBASE_SUB_STREAMWRITER_H

#include <Common/Base/System/Io/Writer/hkStreamWriter.h>

/// Writer which is a "window" into another stream.
/// Write requests are simply forwarded. Seek/tell
/// requests are offset before forwarding so that the
/// start (tell() == 0) of the substream is at a
/// specified offset in the child stream.
class hkSubStreamWriter : public hkStreamWriter
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Substream starts at the child's current offset.
		hkSubStreamWriter( hkStreamWriter* child )
			: m_childStream( child )
		{
			HK_ASSERT( 0xaa36a77f, child );
			m_startOffset = m_childStream->tell();
		}

			/// The zero offset is at the specified offset into child.
		hkSubStreamWriter( hkStreamWriter* child, int offset )
			: m_childStream( child ), m_startOffset(offset)
		{
			HK_ASSERT( 0xaa36a77f, child );
		}

		virtual hkBool isOk() const
		{ 
			return m_childStream->isOk(); 
		}

		virtual int write(const void* buf, int nbytes) 
		{ 
			return m_childStream->write( buf, nbytes );
		}

		virtual void flush() 
		{ 
			m_childStream->flush(); 
		}

		virtual hkBool seekTellSupported() const 
		{ 
			return m_childStream->seekTellSupported(); 
		}

		virtual hkResult seek(int offset, hkStreamWriter::SeekWhence whence) 
		{ 
			int realOffset = ( whence == STREAM_SET )
				? m_startOffset + offset
				: offset;
		
			return m_childStream->seek( realOffset, whence );
		}

		virtual int tell() const 
		{ 
			return m_childStream->tell() - m_startOffset; 
		}

	protected: 

		hkStreamWriter* m_childStream;
		int m_startOffset;
};

#endif // HKBASE_OFFSET_STREAMWRITER_H

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
