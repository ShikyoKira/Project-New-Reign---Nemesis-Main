/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_XML_LEX_ANALYZER_H
#define HK_XML_LEX_ANALYZER_H

#include <Common/Serialize/Util/Xml/hkParserBuffer.h>

class hkXmlLexAnalyzer: public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		
		enum 
		{ 
			IN_BRACKETS = 0x1, 
			IN_QBLOCK   = 0x2 
		};
		enum Token
		{
			// Only outside block
			TOKEN_START_BLOCK = 0,		///< (Only outside brackets)
			TOKEN_START_QBLOCK,			///< <?
			TOKEN_TEXT,					///< General text
			TOKEN_WHITESPACE,
			TOKEN_COMMENT,
			TOKEN_EOF,					///< End of file

			// Only inside a block
			TOKEN_END_BLOCK,			///< >
			TOKEN_END_QBLOCK,			///< ?>
			TOKEN_IDENTIFIER,
			TOKEN_EQUALS,				///< =
			TOKEN_QUOTED_STRING,
			TOKEN_SLASH,				///< / (Only in brackets)

			// Error token
			TOKEN_ERROR,				///< Unable to read

			TOKEN_COUNT_OF,				///< Count
		};
			/// Ctor
		hkXmlLexAnalyzer(hkStreamReader* reader);
			/// Advance
		Token advance();
			/// Get the lexeme
		hkSubString getLexeme() const { return hkSubString(m_buffer.getLexemeStart(), m_buffer.getLexemeEnd()); }

			/// Get the lexeme start index
		HK_FORCE_INLINE int getLexemeStartIndex() const { return m_buffer.getLexemeStartIndex(); }
			/// Get the lexeme end index
		HK_FORCE_INLINE int getLexemeEndIndex() const { return m_buffer.getLexemeEndIndex(); }
			/// Get the lexeme from the indices
		HK_FORCE_INLINE hkSubString getLexeme(int startIndex, int endIndex) const { return m_buffer.getLexeme(startIndex, endIndex); }

			/// Commit the buffer
		void bufferCommit() { m_buffer.bufferCommit(); }

		HK_FORCE_INLINE static hkBool HK_CALL isLetter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
		HK_FORCE_INLINE static hkBool HK_CALL isDigit(char c) { return c >= '0' && c <= '9'; }
		HK_FORCE_INLINE static hkBool HK_CALL isAlphaNumeric(char c) { return isLetter(c) || isDigit(c); }
		HK_FORCE_INLINE static hkBool HK_CALL isWhiteSpace(char c) { return c == ' ' || c == '\t' || c == 0xd || c == 0xa; }

		static const char* const tokenNames[];

	protected:
		Token _lexComment();
		Token _matchInBrackets();
		Token _lexText();
		Token _matchOutsideBrackets();
		Token _lexQuotedString();
		Token _handleError(const char* desc);
		Token _lexIdentifier();
		Token _lexWhiteSpace();

		hkParserBuffer m_buffer;
		int m_state;
};

#endif // HK_XML_LEX_ANALYZER_H

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
