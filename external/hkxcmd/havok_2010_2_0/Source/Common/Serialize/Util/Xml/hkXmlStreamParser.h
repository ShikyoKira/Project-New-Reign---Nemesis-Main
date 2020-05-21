/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_XML_STREAM_PARSER_H
#define HK_XML_STREAM_PARSER_H

#include <Common/Base/Container/SubString/hkSubString.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>

#include <Common/Base/Container/StringMap/hkStringMap.h>

#include <Common/Serialize/Util/Xml/hkXmlLexAnalyzer.h>

class hkXmlStreamParser : public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		
		enum Token
		{
			TOKEN_QBLOCK,
			TOKEN_BLOCK_START,
			TOKEN_BLOCK_START_END,
			TOKEN_BLOCK_END,
			TOKEN_TEXT,
			TOKEN_WHITESPACE,

			TOKEN_ERROR,
			TOKEN_EOF,
		};

			/// Advance
		Token advance();
			/// Ctor
		hkXmlStreamParser(hkStreamReader* reader);

			///
		hkSubString getBlockName() const;

			/// Get the current token
		HK_FORCE_INLINE Token getToken() const { return m_token; }

			/// Get the keys
		HK_FORCE_INLINE const hkArray<const char*>& getKeys() const { return m_keys; }
			/// Get the value
		hkResult getValue(const char* key, hkSubString& value) const;
	
			/// Get an integer attribute
		hkResult getIntAttribute(const char* key, int& value) const;
			/// Get the real attribute
		hkResult getRealAttribute(const char* key, hkReal& value) const;
			/// Returns true if the named attribute has a definition
		hkBool hasAttribute(const char* key) const;

			/// Get the lexeme
		hkSubString getLexeme() const;

		void dumpParse(hkOstream& out);

			/// Returns true if the subString is escaped such that it needs decoding
		static hkBool HK_CALL needsDecode(const hkSubString& subString);
			/// Decode an input string into the buf
		static hkResult HK_CALL decodeString(const hkSubString& subString, hkStringBuf& buf);

			/// Parse an int
		static hkResult HK_CALL parseInt(const hkSubString& subString, int& valueOut);
			/// Parse an real
		static hkResult HK_CALL parseReal(const hkSubString& subString, hkReal& valueOut);

	protected:

		struct SubString
		{
			int m_start;
			int m_end;
		};
		void _pushLexeme();
		Token _parseBlock();
		Token _parseQBlock();
		void _extractAttributes();
		Token _advance();

		hkStringMap<int> m_attribMap;

		hkArray<const char*> m_keys;
		hkArray<char> m_keyStorage;

		hkXmlLexAnalyzer m_lex;						///< Lex analyzer
		hkArray<SubString> m_lexemes;				///< Offsets to the contents

		Token m_token;
};

#endif // HK_XML_STREAM_PARSER_H

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
