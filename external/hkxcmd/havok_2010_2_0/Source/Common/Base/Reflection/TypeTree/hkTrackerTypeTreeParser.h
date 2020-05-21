/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_STANDARD_RTTI_PARSER_H
#define HK_STANDARD_RTTI_PARSER_H

#include <Common/Base/Reflection/TypeTree/hkTrackerTypeTreeNode.h>
#include <Common/Base/Reflection/TypeTree/hkTrackerTypeTreeCache.h>

/// Used to parses a standard C++ type declaration - and produce a syntax tree of hkTrackerTypeTreeNodes.
/// 
/// All memory used in the tree will be stored in the hkTrackerTypeTreeCache.
/// 
/// In terms of caching and sharing - if the same type expression is passed twice to the parseType
/// twice the original one will be returned. That said for a more complicated type - lets say
/// an hkArray<Fred>, the Fred member in the hkArray may not be the same as passing Fred to the parser.
///
/// This restriction means that you can't compare pointers to two Node subtrees in a cache to see if they
/// are the same.
/// 
/// NOTE!
///     - Does not support function types
///     - Does not support non types as template parameters.
class hkTrackerTypeTreeParser //+reflected(false)
{
	public:
		//+hk.MemoryTracker(ignore=True)
		typedef hkTrackerTypeTreeNode Node;
		typedef hkTrackerTypeTreeNode::Type Type;

			/// Either finds the type in the cache, or parses and adds to the cache.
			/// If fails to parse the type returns HK_NULL.
		static const Node* HK_CALL parseType(const char* typeName, hkTrackerTypeTreeCache& cache);
			/// Either finds the type in the cache, or parses and adds to the cache
		static const Node* HK_CALL parseType(const hkSubString& subString, hkTrackerTypeTreeCache& cache);

			/// Parses the type - but does not add to the cache
			/// If fails to parse the type returns HK_NULL.
		static const Node* HK_CALL parseNewType(const hkSubString& subString, hkTrackerTypeTreeCache& cache);

	protected:
		enum TokenType
		{
			TOKEN_REFERENCE,
			TOKEN_POINTER,
			TOKEN_IDENTIFIER,
			TOKEN_CLASS,
			TOKEN_ENUM,
			TOKEN_BUILT_IN,			///< built in type
			TOKEN_UNSIGNED,			///< Unsigned keyword
			TOKEN_SIGNED,			///< Signed keyword
			TOKEN_LT,				///< '<'
			TOKEN_GT,				///< '>'
			TOKEN_COMMA,			///< ,
			TOKEN_TERMINATOR,		///< Identifies the end of the list
			TOKEN_OPEN_SQUARE,		///< '['
			TOKEN_CLOSE_SQUARE,		///< ']'
			TOKEN_INT_VALUE,		///< An integer value
		};

		struct Token //+reflected(false)
		{
			TokenType m_token;						///< Token type
			hkTrackerTypeTreeNode::Type m_builtInType;		///< Set if the built in type is defined
			hkSubString m_string;					///< The string associated with the type (or empty)
		};
		struct TokenRange //+reflected(false)
		{
			const Token* m_start;
			const Token* m_end;
		};
		struct TokenContainer //+reflected(false)
		{
			TokenContainer():
				m_numTokens(0)
			{
			}
				void addToken(TokenType tokenType, const hkSubString& string = hkSubString(), hkTrackerTypeTreeNode::Type builtInType = hkTrackerTypeTreeNode::TYPE_UNKNOWN)
			{
				HK_ASSERT(0x2423423, m_numTokens < MAX_TOKENS);
				Token& token = m_tokens[m_numTokens++];
				token.m_token = tokenType;
				token.m_string = string;
				token.m_builtInType = builtInType;
			}

			enum { MAX_TOKENS = 64 };

			int m_numTokens;
			Token m_tokens[MAX_TOKENS];
		};

		/// Parses the type to produce a token stream. Returns true on success.
		static hkBool HK_CALL _parseTokens(const hkSubString& str, TokenContainer& container);
		HK_FORCE_INLINE static hkBool HK_CALL _isLetter(char c)
		{
			return (c >= 'a' && c <='z') || (c >= 'A' && c <= 'Z');
		}
		HK_FORCE_INLINE static hkBool HK_CALL _isDigit(char c)
		{
			return (c >= '0' && c <= '9');
		}
		/// Returns the basic type or TYPE_UNKNOWN if not a basic type
		static hkTrackerTypeTreeNode::Type HK_CALL _calcBuiltInType(const hkSubString& subString);

		static const Node* HK_CALL _parseType(TokenRange& range, hkTrackerTypeTreeCache& cache);
		static const Node* HK_CALL _parseTerminalType(TokenRange& range, hkTrackerTypeTreeCache& cache);
};

#endif // HK_STANDARD_RTTI_PARSER_H

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
