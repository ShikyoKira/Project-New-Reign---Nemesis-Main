/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_ANDERSSON_TREE_H
#define HK_ANDERSSON_TREE_H

// Tallest allowable tree, ie. 2^64 branches
#ifndef HK_AA_TREE_HEIGHT_LIMIT
#define HK_AA_TREE_HEIGHT_LIMIT 64
#endif

/// An Arne Andersson tree (AA tree) is a balanced red-black tree with one additional rule.
/// Unlike red-black trees, RED nodes on an AA tree can only be added as a right subchild. In other words,
/// no RED node can be a left subchild. This results in the simulation of a 2-3 tree instead of a 2-3-4 tree,
/// which greatly simplifies the maintenance operations. The maintenance algorithms for a red-black tree
/// need to consider seven different shapes to properly balance the tree. An AA tree on the other hand only
/// needs to consider two shapes due to the strict requirement that only right links can be red.
/// See http://en.wikipedia.org/w/index.php?title=AA_tree&oldid=148001972
class hkAATree
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_TREE, hkAATree);

		// user-defined item handling
		typedef int   (HK_CALL *compareFunc) ( const void *p1, const void *p2 );
		typedef void* (HK_CALL *cloneFunc)   ( void *p );
		typedef void  (HK_CALL *destroyFunc) ( void *p );

		// default item handling
		static int   HK_CALL defaultCompare(const void* p1, const void* p2);
		static void* HK_CALL defaultClone(void* p);
		static void  HK_CALL defaultDestroy(void* p);

		hkAATree( compareFunc cmp=defaultCompare, cloneFunc dup=defaultClone, destroyFunc rel=defaultDestroy );
		~hkAATree();

		// tree functions

		void* find( void* data ) const;
		hkBool insert( void* data );
		hkBool erase( void *data );
		HK_FORCE_INLINE hkUint32 getSize() const;
		void clear();

	protected:

		struct Node
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_TREE, hkAATree::Node);

			int          m_level;   // Horizontal level for balance
			void *       m_data;    // User-defined content
			struct Node* m_link[2]; // Left (0) and right (1) links
		};

		// tree traversal

	public:

		struct Iterator
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_TREE, hkAATree::Iterator);

			hkAATree* m_tree;						   // Paired tree
			Node*     m_it;                            // Current node
			Node*     m_path[HK_AA_TREE_HEIGHT_LIMIT]; // Traversal path
			hkUint32  m_top;                           // Top of stack

			void* start( hkAATree* tree, int dir );
			void* move( int dir );
			HK_FORCE_INLINE void* first( hkAATree* tree );
			HK_FORCE_INLINE void* last( hkAATree* tree );
			HK_FORCE_INLINE void* next();
			HK_FORCE_INLINE void* prev();
		};


		Node *       m_root; // Top of the tree
		Node *       m_nil;  // End of tree sentinel

	protected:

		HK_FORCE_INLINE Node* newNode( void *data );
		HK_FORCE_INLINE void skew(Node** t);
		HK_FORCE_INLINE void split(Node** t);

		compareFunc  m_cmp;  // Compare two items (user-defined)
		cloneFunc    m_dup;  // Clone an item (user-defined)
		destroyFunc  m_rel;  // Destroy an item (user-defined)
		hkUint32     m_size; // Number of items in the tree

};

#include <Common/Base/Container/Tree/hkAnderssonTree.inl>

#endif //HK_ANDERSSON_TREE_H

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
