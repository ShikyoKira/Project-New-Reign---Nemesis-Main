/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_XMLPARSER_H
#define HK_BASE_XMLPARSER_H

#include <Common/Base/Container/Tree/hkTree.h>

class hkIstream;
class hkStreamReader;
class hkLineNumberStreamReader;

/// A class to parse and write a sensible subset of XML.
/// Three parsing methods are supported - a SAX-like interface
/// using nextNode(), a DOM-like interface parse() and a pull
/// based DOM interface expandNode().
/// Note that whitespace is compressed to a single space and
/// nodes containing only whitespace are discarded.
class hkXmlParser : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE);

			/// Available node types.
		enum NodeType
		{
			INVALID,
			START_ELEMENT,
			END_ELEMENT,
			CHARACTERS,
			MAX_ID
		};

			/// XML name / attribute pair.
		struct Attribute
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, Attribute);
			hkStringPtr name;
			hkStringPtr value;
		};

		struct StartElement;
		struct EndElement;
		struct Characters;

			/// Base XML node type.
		struct Node : public hkReferencedObject
		{
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			Node( NodeType t ) : type(t) { }

				/// Cast this to a StartElement if possible, otherwise return HK_NULL.
			inline StartElement* asStart();
				/// Cast this to an EndElement if possible, otherwise return HK_NULL.
			inline EndElement* asEnd();
				/// Cast to Characters if possible, otherwise return HK_NULL.
			inline Characters* asCharacters();
	
			NodeType type;
		};

			/// A start element node.
		struct StartElement : public Node
		{
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			StartElement(const char* n) : Node(START_ELEMENT), name(n) { }
				/// Get the value of attribute a or d if not found.
			const char* getAttribute( const char* a, const char* d );

			hkStringPtr name;
			hkArray<Attribute> attributes;
		};

			/// An end element node.
		struct EndElement : public Node
		{
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			EndElement(const char* n) : Node(END_ELEMENT), name(n) { }
			hkStringPtr name;
		};
		
			/// Character data.
		struct Characters : public Node
		{
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			Characters(const char* t) : Node(CHARACTERS), text(t) { }
			Characters(const char* s, int len) : Node(CHARACTERS), text(s,len) { }
			hkResult canonicalize(const char* killChars=HK_NULL);
			hkStringPtr text;
		};

		typedef hkTree< hkRefPtr<Node> > Tree;

	public:

			/// Create a parser.
		hkXmlParser();

			/// Destroy a parser. Decrement a reference to its streamreader.
		~hkXmlParser();

			/// Return the next node in the stream, or HK_NULL at the end.
			/// The caller should delete the node when done.
		virtual hkResult nextNode( Node** nodeOut, hkStreamReader* reader );

			/// Put a node back. A subsequent call to nextNode() will return 'node'.
			/// Multiple nodes may be put back and they will be returned in LIFO order.
		virtual void putBack( Node* node );

			/// Read a subtree up to the closing element of s into tree.
			/// Note that only the last node returned by nextNode() may be expanded.
			/// The tree is first clear()ed, and 's' is inserted as the root of
			/// the tree, followed by the subelements and the closing element
			/// matching 's'.
		virtual hkResult expandNode( StartElement* s, Tree& tree, hkStreamReader* reader );

			/// Parse the entire istream 'is' into a tree of XML elements.
		hkResult parse( Tree& tree, hkStreamReader* reader );

			/// Translate XML entities into their ASCII values.
			/// dst and src may overlap since dst is always smaller than src.
			/// Returns new length of dst or -1 on error (unknown entity).
		static int translateEntities(char* dst, const char* src);

			/// Replace "spaceChars" with a single space then remove duplicate whitespace.
			/// dst and src may overlap since dst is always smaller than src.
			/// Returns new length of dst.
		static int canonicalize(char* dst, const char* src, const char* spaceChars=HK_NULL);

			/// Return a null terminated description of the last error.
			/// Call this after one of the parsing methods returns HK_FAILURE.
		const char* getLastError() const;

	protected:

			// <node/> is parsed as <node></node>, remember if a closing node is pending.
		hkArray<Node*> m_pendingNodes;

			//
		hkStringPtr m_lastError;
};

#include <Common/Serialize/Util/Xml/hkXmlParser.inl>

#endif // HK_BASE_XMLPARSER_H

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
