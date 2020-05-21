/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_SEEKABLE_STREAMREADER_H
#define HKBASE_SEEKABLE_STREAMREADER_H

#include <Common/Base/hkBase.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>

/// Utility class for readers which implement seek.
/// Derived classes must implement seek and tell. The mark
/// interface is implemented with calls to these methods.
/// Note that while this always works, it may be quite slow
/// if the streams underlying seek and tell are slow. In this
/// case it may be a better solution to wrap the stream in an
/// hkBufferedStreamReader instead which is guaranteed to be fast.
class hkSeekableStreamReader : public hkStreamReader
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkSeekableStreamReader()
			: m_markPos(-1)
		{
		}

		virtual hkBool markSupported() const
		{
			return true;
		}

		virtual hkResult setMark(int markLimit)
		{
			m_markPos = tell();
			return (m_markPos != -1) ? HK_SUCCESS : HK_FAILURE;
		}

		virtual hkResult rewindToMark()
		{
			return seek(m_markPos, STREAM_SET);
		}

		virtual hkBool seekTellSupported() const
		{
			return true;
		}

		// must implement seek

		// must implement tell

	protected:

		int m_markPos;
};

#endif //HKBASE_SEEKABLE_STREAMREADER_H

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
