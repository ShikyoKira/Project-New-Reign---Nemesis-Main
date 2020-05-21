/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_TREE_H
#define HK_BASE_TREE_H

	/// Generic tree with homogeneous values.
	/// Use iterators to navigate the tree. Iterators are invalidated when
	/// the tree is modified. Compare the iterator to null to check for end
	/// of iteration.
template <typename T>
class hkTree
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_TREE, hkTree<T>);

		struct Node
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_TREE, Node);

			Node(const T& value)
				: m_firstChild(0)
				, m_nextSibling(0)
				, m_parent(0)
				, m_value(value)
			{
			}

			Node* m_firstChild;
			Node* m_nextSibling;
			Node* m_parent;
			T m_value;
		};

			/// Tree iterator.
		typedef Node* Iter;

			/// Create an empty tree.
		inline hkTree();

			/// Destroy a tree and each of its nodes.
		~hkTree();

			// 
			// Insert/remove
			//

			/// Remove all values from the tree.
		inline void clear();

			/// Add a new node at the same level after iter.
		inline Iter appendSibling(Iter iter, const T& t);

			/// Add a new node at the same level after iter.
		inline Iter appendChild(Iter iter, const T& t);

			/// Remove the subtree at iterator i. Iterator i is then invalid.
		inline void remove(Iter i);

			//
			// Value
        	//

			/// Set the value of a node.
		inline void setValue(Iter i, const T& t );

			/// Get the value at iter i.
		inline T& getValue(Iter i) const;

			/// Get the depth at iter i. Root nodes have depth 0. Returns -1 if the tree is empty;
		inline int getDepth(Iter i) const;

			/// Return the number of children of node at i.
		inline int getNumChildren(Iter i) const;

			//
			// Iterators	
			//

			/// Get the iterator at the first root or null if empty.
		inline Iter iterGetFirst() const;

			/// Next sibling.
		inline Iter iterNext(Iter i) const;

			/// Parent.
		inline Iter iterParent(Iter i) const;

			/// First child.
		inline Iter iterChildren(Iter i) const;

			/// Next node in pre-order (children before siblings).
		inline Iter iterNextPreOrder(Iter i) const;

			/// Next node in pre-order except the current nodes children are skipped.
		inline Iter iterNextPreOrderSkippingChildren(Iter i) const;

	private:

		Node* m_root;
};

#include <Common/Base/Container/Tree/hkTree.inl>

#endif //HK_BASE_TREE_H

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
