/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// ---------------------------------------------------------------------------
inline char hkParserBuffer::getChar()
{
	char c = *m_pos;
	if (c == 0)
	{
		read(MIN_READ_SIZE);
		c = *m_pos;
	}
	return c;
}

// ---------------------------------------------------------------------------
int hkParserBuffer::requirePostSize(int size)
{
	int postSize = getPostSize();
	if (size <= postSize)
	{
		return postSize;
	}
	else
	{
		read(size - postSize);
		return getPostSize();
	}
}

// ---------------------------------------------------------------------------
void hkParserBuffer::advance(int numChars)
{
	HK_ASSERT(0x242342a3, numChars >= 0);
	int postSize = getPostSize();
	HK_ASSERT(0x242342a3, numChars <= postSize);
	if (numChars <= postSize)
	{
		m_pos += numChars;
	}
}

// ---------------------------------------------------------------------------
void hkParserBuffer::rewindChar()
{
	HK_ASSERT(0x23423432, m_pos > m_buffer.begin() + m_lexemeStart);
	if (m_pos > m_buffer.begin() + m_lexemeStart)
	{
		m_pos--;
	}
}

// ---------------------------------------------------------------------------
char hkParserBuffer::nextChar()
{
	if (m_pos >= m_buffer.end())
	{
		// Try reading
		read(MIN_READ_SIZE);
		if (m_pos >= m_buffer.end())
		{
			return 0;
		}
	}
	return *m_pos++;
}

// ---------------------------------------------------------------------------
char hkParserBuffer::getCharAt(int offset) const
{
	const char* pos = m_pos + offset;
	if (pos >= m_buffer.begin() + m_lexemeStart && pos < m_buffer.end())
	{
		return *pos;
	}
	HK_ASSERT(0x21423432, !"Outside of buffer range. Needs a 'read'?");
	return 0;
}

// ---------------------------------------------------------------------------
HK_FORCE_INLINE hkSubString hkParserBuffer::getLexeme(int startIndex, int endIndex) const
{
	HK_ASSERT(0x324234a2, startIndex >= 0 && endIndex >= startIndex);
	const char* base = m_buffer.begin() + m_bufferStart;
	HK_ASSERT(0xdfa0b0aa, base + endIndex <= m_buffer.end());

	return hkSubString(base + startIndex, base + endIndex);
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
