/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TYPE_TREE_CACHE_H
#define HK_TYPE_TREE_CACHE_H

#include <Common/Base/Reflection/TypeTree/hkTrackerTypeTreeNode.h>

#include <Common/Base/Container/StringMap/hkStorageStringMap.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>

#include <Common/Base/Memory/Allocator/FreeList/hkFreeList.h>

	/// Used for looking up type node trees. Also handles the memory scope of type tree nodes.
	/// 
	/// Provides some support simple forms of sharing - built in types (int, char etc), and named types
	/// Types can be looked up from a sting (if held as setTypeTree), or for 'named' types (for example class enum)
	/// can be looked up directly from the type name.
	/// 
	/// Note the setTypeTree name - is the fully qualified name. I.e., a struct would be 'struct Blah'.
	/// The type name would just be 'Blah'.
class hkTrackerTypeTreeCache: public hkReferencedObject 
{
    public:
		HK_DECLARE_CLASS_ALLOCATOR(hkTrackerTypeTreeCache);

		typedef hkTrackerTypeTreeNode Node;
		typedef hkTrackerTypeTreeNode::Type Type;

			/// Gets the type tree by name. Returns true if found
		hkBool getTypeExpressionTree(const char* typeExpr, const Node**nodeOut) const;
			/// Gets a type tree - return HK_NULL if it doesn't find it
		const Node* getTypeExpressionTree(const char* typeExpr) const;
			/// Set a type tree to be associated with a type expression
			/// It's assumed that the node was allocated with newNode from this cache
			/// NOTE that the name is assumed to be persistent - i.e., it stays in scope as long as the hkTrackerTypeTreeCache does
			/// A string can be made this way using newText - which will share text if the exact text already exists.
		void setTypeExpressionTree(const char* typeText, const Node* node);

			/// New node
		Node* newNode(Type type);
			/// New named node
			/// Can only be used for ENUM/NAMED/CLASS types
			/// NOTE - may mutate a NAMED type if the type is different.
		const Node* newNamedNode(Type type, const hkSubString& name, hkBool allocName = true);
		const Node* newNamedNode(Type type, const char* name, hkBool allocName = true);
			/// Look up if a named node already exists
		const Node* getNamedNode(const char* name) const;

			/// New text - will return the same pointer if the exact text is already in the cache
		const char* newText(const char* text);

			/// Deallocate everything
		void clear();

			/// Get a built in type
		const Node* getBuiltInType(Type type) const { return m_builtInTypes[type]; }

			/// Ctor
		hkTrackerTypeTreeCache();

    protected:
		typedef hkStorageStringMap<int> TextMap;
		typedef hkStringMap<const Node*> ExpressionTypeMap;			///< Maps the full text type expansion to the type
		typedef hkStringMap<const Node*> NamedTypeMap;

		const Node* m_builtInTypes[hkTrackerTypeTreeNode::TYPE_END];

		hkFreeList m_nodeFreeList;							///< Holds the nodes
		TextMap m_textMap;									///< Holds the text - created with newText

		NamedTypeMap m_namedTypeMap;						///< Maps only named types (enum class to their type). Cannot hold templates.
		ExpressionTypeMap m_expressionTypeMap;				///< Maps the complete type text (i.e., including struct etc) to a type
};


#endif // HKBASE_RTTI_TYPE_CACHE_H

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
