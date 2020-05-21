/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMIC_AABB_TREE_INT_MEM_H
#define HK_DYNAMIC_AABB_TREE_INT_MEM_H

#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.h>


	/// Using multiple trees is deprecated in 2010-2, see the migration docs.
template<typename BOUNDING_VOLUME, typename hkIdxType = hkUint16>
class hkAabbTree
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkAabbTree );

		static const hkIdxType s_invalidIdx = hkIdxType(~0) >> 1;

		struct Node
		{
				static const hkIdxType s_leafMask = hkIdxType(~0) >> 1;
				static const hkIdxType s_leafFlag = hkIdxType(~s_leafMask);

				HK_DECLARE_PLACEMENT_ALLOCATOR();
				Node(): m_parentIdx(0){}

				BOUNDING_VOLUME m_volume;
			private:

				union
				{
					struct  
					{
						hkIdxType m_childIdx[2];
					}m_nodeData;
					struct  
					{
						void* m_userData;
					}m_leafData;
				};
				hkIdxType m_parentIdx;
				hkIdxType m_skipNodeIdx;

			public:
				// Methods to access internal data.
				hkIdxType getParentIdx() const {return m_parentIdx&s_leafMask;}
				hkIdxType getSkipNodeIdx() const {return m_skipNodeIdx;}
				hkIdxType getChildIdx(int i) const {return m_nodeData.m_childIdx[i];}
				void setSkipNodeIdx(hkIdxType idx){ m_skipNodeIdx = idx;}
				void setParentNodeIdx(hkIdxType idx){ m_parentIdx = idx | (m_parentIdx&s_leafFlag); }
				void setChildIdx(int iChild, hkIdxType idx){m_nodeData.m_childIdx[iChild] = idx;}

				// Methods to access leaf data.
				hkBool32 isLeaf() const { return m_parentIdx & s_leafFlag; }
				template<typename T>
				HK_FORCE_INLINE T getLeafData() const {HK_ASSERT(0xd95ed6f, isLeaf()); return (T)m_leafData.m_userData; }
				HK_FORCE_INLINE void setLeafData(void* userData){m_leafData.m_userData = userData; m_parentIdx |= s_leafFlag;}

				friend class hkAabbTree<BOUNDING_VOLUME, hkIdxType>;
		};


		hkAabbTree();
		hkAabbTree(const hkVector4& bitOffsetLow,  const hkVector4& bitOffsetHigh, const hkVector4& bitScale);
		virtual ~hkAabbTree(){ clear(); }


		// BUILD FUNCTIONS

		/// Build tree. Returns the indices of leaves held in this class.
		void build(hkAabb* volumes, void** userDatas, int numLeaves, hkArray<hkIdxType>& leavesOut);

		/// Rebuild tree without changing leaf indices.
		void rebuild();

		/// Check consistency of the data.
		void checkConsistency(Node* root, Node* skipNode);

		/// Delete all the nodes.
		void clear();


		// DYNAMIC UPDATE FUNCTIONS

		hkIdxType add( const hkAabb& aabb, void* userData );

		void remove( hkIdxType leafIdx );

		void update( hkIdxType leafIdx, const hkAabb& aabb );


		// ACCESS FUNCTIONS

		Node* getRootNode() const { return m_rootNode; }

		hkIdxType getRootNodeIdx() const { return m_rootIdx; }

		int getNumLeaves() const {return m_numLeaves;}

		int countLeaves(hkIdxType rootIdx) const;

		void collectLeaves(hkIdxType rootIdx, hkArray<hkIdxType>& leafIdxOut) const;

		void collectInternals(hkIdxType rootIdx, hkArray<hkIdxType>& internalIdxOut) const;

		template< typename T >
		void fetchLeavesData(hkIdxType rootIdx, hkArray<T>& leavesData);


		// ACCESS FUNCTION TO A NODE

		HK_FORCE_INLINE Node* getChild(const Node* node, int i) const;

		HK_FORCE_INLINE Node* getSkipNode(const Node* node) const;

		HK_FORCE_INLINE Node* getParent(const Node* node) const;

		HK_FORCE_INLINE Node* getNode(hkIdxType nodeIdx) const;


	private:
		// Build tree including leaves.
		// LeavesAreAllocated , use leavesOut for leaf data.
		void buildTreeInternal(BOUNDING_VOLUME* leafVolumes, void** userDatas, int numLeaves, Node* root, hkArray<hkIdxType>& leavesOut,
			bool leavesAreAllocated = false);

		// Fix the skipnodes and volume.
		void fixupTree(Node* node, hkIdxType nodeIdx, hkIdxType skipNodeIdx);

		void collectNodesInternal(hkIdxType rootIdx, hkArray<hkIdxType>* internals, hkArray<hkIdxType>* leaves) const;

		void addLeafInternal(hkIdxType insertPointIdx, hkIdxType newLeafIdx,hkIdxType scratchNodeIdx);

		Node* removeLeafInternal( hkIdxType leafIdx, hkIdxType& toDeleteNodeIdx );


		class NodeManager
		{
			public:
				NodeManager(){}
				/// Set myIdx in node and returns the idx of this node in mem location.
				hkIdxType pushBack(Node*& rootNode, hkIdxType& rootIdx, Node& node);
				/// These ptr to nodes are not valid after resized.
				Node* newNode(Node*& rootNode, hkIdxType& rootIdx, hkIdxType& newNodeIdx);
				void deleteNode(hkIdxType idx);
				Node* getNode(hkIdxType idx);
				const Node* getNode(hkIdxType idx) const;
				void removeAt(hkIdxType nodeIdx);
				void reserve(hkIdxType size, Node*& rootNode, hkIdxType& rootIdx);

				void clear();

				enum
				{
					MEM_INCREASE_SIZE = 128, 
				};

			private:
				hkArray<Node> m_nodes;
				hkArray<hkIdxType> m_freeList;
		};

	private:
		Node* m_rootNode;
		hkIdxType m_rootIdx;
		int m_numLeaves;

		NodeManager m_nodeManager;

	public:
		hkVector4 m_bitOffsetLow;
		hkVector4 m_bitOffsetHigh;
		hkVector4 m_bitScale;
		hkVector4 m_bitScaleInv;
};


#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTree.inl>

#endif

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
