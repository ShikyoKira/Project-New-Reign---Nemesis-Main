/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_PARSER_BUFFER_H
#define HK_PARSER_BUFFER_H

#include <Common/Base/Container/SubString/hkSubString.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>

/// For simplifying parsing. Always 0 terminates (no need to check for the end)
///
/// Pre is pre the current position. From the start of the current position forwards
/// Post is the num characters (including current char) after position
class hkParserBuffer : public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		
			/// Ctor
		hkParserBuffer(hkStreamReader* reader);
			/// Dtor
		~hkParserBuffer();

			/// Returns true if it matches
		hkBool match(const char* text);
			/// Match - and if match is successful consume
		hkBool matchAndConsume(const char* text);
			/// Match - returns true if matches
		hkBool match(const char* text, int len);

			/// Get the row index (indexed from 0)
		int getRow() const { return m_row; }
			/// Get the column index (on the current row)
		int getColumn() const { return m_col; }

			/// Get the current char. Returns 0 if there is no character if at the end.
		inline char getChar();
			/// Get the char at the index, can be forwards or backwards - but must be in buffer range
		inline char getCharAt(int offset) const;
			
			/// Start of current lexeme
		const char* getLexemeStart() const { return m_buffer.begin() + m_lexemeStart; }
			/// Get the end of current lexeme (which is the current read position)
		const char* getLexemeEnd() const { return m_pos; }

			/// Get the chars at the current position
		const char* getCurrentPosition() const { return m_pos; }
			/// Get the buffer start
		//const char* getBufferStart() const { return m_buffer.begin() + m_start; }

			/// Get the lexeme start index. This will remain valid until a commitBuffer
		HK_FORCE_INLINE int getLexemeStartIndex() const { return m_lexemeStart - m_bufferStart; }
			/// Get the lexeme end index. This will remain valid until a commitBuffer
		HK_FORCE_INLINE int getLexemeEndIndex() const { return int(m_pos - m_buffer.begin()) - m_bufferStart; }

			/// Get the lexeme
		HK_FORCE_INLINE hkSubString getLexeme(int startIndex, int endIndex) const;

			/// Returns the current character, and advances. Returns 0 if at the end
		HK_FORCE_INLINE char nextChar();

			/// Move the positions numChars forwards. numChars <= postSize.
		inline void advance(int numChars = 1);

			/// Rewind a char
		inline void rewindChar();

			/// Set the position
		void setLexemePosition(int pos);
			/// Get the current position, as an index
		int getLexemePosition() const { return int(m_pos - m_buffer.begin()) - m_lexemeStart; }

			/// Get the buffer size
		HK_FORCE_INLINE int getBufferSize() const { return m_buffer.getSize() - m_bufferStart; }

			/// The post size
		HK_FORCE_INLINE int getPostSize() const { return int(m_buffer.end() - m_pos); }
			/// Require the post size. Returns the new post size
		inline int requirePostSize(int size);
			/// Get the amount of characters before the current position
		HK_FORCE_INLINE int getPreSize() const { return int (m_pos - m_lexemeStart - m_buffer.begin()); }
			/// Ensure there are the num characters specified. Returns num post characters.
			/// May be less than numChars, if at the end of the file
		int ensureNumChars(int numChars);

			/// Read size characters to the end of the buffer
			/// Returns the actual amount of characters that were added
		int read(int size);

			/// Moves the current buffer start to the current read position
		void lexemeCommit();

			/// Commit the buffers contents
		void bufferCommit();

	protected:
	
		enum { MIN_READ_SIZE = 256 };

		int m_bufferStart;			///< The starting position of the buffer (inside m_buffer).
		int m_lexemeStart;			///< The starting position of the current lexeme
		char* m_pos;				///< The current read position in the buffer (and when lexeme is interpreted, the end of the lexeme)
		hkArray<char> m_buffer;		///< Buffer that holds the current section of parsed data

		hkStreamReader* m_reader;

		int m_row;					///< The current line
		int m_col;					///< The current col
};

#include <Common/Serialize/Util/Xml/hkParserBuffer.inl>

#endif // HK_PARSER_BUFFER_H

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
