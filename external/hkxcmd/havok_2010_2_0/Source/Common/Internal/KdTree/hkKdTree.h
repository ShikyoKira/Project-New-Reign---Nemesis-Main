/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KDTREE_H
#define HK_KDTREE_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>


typedef hkUlong hkPrimitiveId;
typedef hkUint16 hkSplitType;

#if HK_POINTER_SIZE == 8
typedef hkUint32 hkHalfPointer;
#else
typedef hkUint16 hkHalfPointer;
#endif

const hkSplitType HK_KD_NODE_MIN_SPLIT = hkSplitType(0);
const hkSplitType HK_KD_NODE_MAX_SPLIT = hkSplitType(-1);


//#define  NON_VIRTUAL_TEST


class hkKdTreeNode
{

public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO, hkKdTreeNode );

	HK_FORCE_INLINE hkKdTreeNode( );

	HK_FORCE_INLINE ~hkKdTreeNode() {}

	/*
	 *  Split Data
	 */

	HK_FORCE_INLINE int getSplitAxis() const;

	HK_FORCE_INLINE hkSplitType getSplitMin() const;

	HK_FORCE_INLINE hkSplitType getSplitMax() const;

	HK_FORCE_INLINE void setSplit(int axis, hkSplitType splitMin, hkSplitType splitMax);

	/*
	 *  Leaf Data
	 */

	HK_FORCE_INLINE hkPrimitiveId getPrimitiveId() const;

	HK_FORCE_INLINE void setPrimitiveId(hkPrimitiveId id);

	HK_FORCE_INLINE hkBool isLeaf() const;

	HK_FORCE_INLINE hkBool isEmptyLeaf() const;

	HK_FORCE_INLINE hkBool32 isLeaf32() const;

	HK_FORCE_INLINE hkBool32 isEmptyLeaf32() const;

	HK_FORCE_INLINE void setEmpty(); 

	/*
	 *  Traversal
	 */

	HK_FORCE_INLINE hkKdTreeNode* getLeft() const;

	HK_FORCE_INLINE hkKdTreeNode* getRight() const;

	HK_FORCE_INLINE hkUint32 getLeftOffset() const;

	HK_FORCE_INLINE hkUint32 getRightOffset() const;

	HK_FORCE_INLINE hkBool isLeft() const;
	
	HK_FORCE_INLINE hkBool isRight() const;

	HK_FORCE_INLINE hkKdTreeNode* getSibling() const;

	HK_FORCE_INLINE void setLeftAndRightByteOffset( hkUlong offset );

	HK_FORCE_INLINE void adjustLeftAndRightByteOffset( int offsetDelta );

	/*
	 * Bound data
	 */
	HK_FORCE_INLINE void setNumPrimitivesInLeaf( hkUint32 offset );

	HK_FORCE_INLINE hkUint32 getNumPrimitivesInLeaf() const;

private:

	struct Leaf
	{
		// Type	   Bit 31
		// PrimIdx Bit 0..30
		HK_ALIGN(hkInt32 m_type_primId, 8);
		hkUint32 m_numPrimitives;
	};

	struct Node
	{
		// Type   Bit 31
		// Offset Bit 30..3 
		// Unused Bit 2
		// Split Axis Bit 1..0
		hkInt32	m_type_left_axis;
		hkSplitType m_splitMin;
		hkSplitType m_splitMax;
	};

	union {
		Leaf		m_leaf;
		Node		m_node;
	} m_data; // 8 Bytes per node 

};

struct hkKdTreeAlignedNodePair
{
	//+hk.MemoryTracker(ignore=True)
	HK_ALIGN16(hkKdTreeNode m_node0);
	hkKdTreeNode m_node1;
};

struct hkKdTreeProjectedEntry
{
	union PointerHalfUnion
	{
		hkUlong m_ulong;
		hkHalfPointer m_halfPointer[2];
	};

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO,hkKdTreeProjectedEntry);

	HK_ALIGN16(hkSplitType	m_min[3]);
	hkHalfPointer			m_primIdHigh;
	hkSplitType				m_max[3];
	hkHalfPointer			m_primIdLow;

#if (HK_POINTER_SIZE == 4) && !(defined(HK_PLATFORM_RVL) || defined(HK_PLATFORM_PSP))
	void operator=(const hkKdTreeProjectedEntry& other)
	{
		*(hkQuadReal*)this = *(const hkQuadReal*)&other;
	}
#endif

	HK_FORCE_INLINE hkPrimitiveId getPrimitiveId() const
	{
		PointerHalfUnion u;
		u.m_halfPointer[0] = m_primIdHigh;
		u.m_halfPointer[1] = m_primIdLow;
		return u.m_ulong;
	}

	HK_FORCE_INLINE void setPrimitiveId(hkPrimitiveId primId)
	{
		PointerHalfUnion u;
		u.m_ulong = primId;
		m_primIdHigh = u.m_halfPointer[0];
		m_primIdLow  = u.m_halfPointer[1];
	}

	HK_FORCE_INLINE void invalidate()
	{
		m_min[0] = m_min[1] = m_min[2] = HK_KD_NODE_MAX_SPLIT;
		m_max[0] = m_max[1] = m_max[2] = HK_KD_NODE_MIN_SPLIT;
	}

	HK_FORCE_INLINE void reset()
	{
		m_min[0] = m_min[1] = m_min[2] = HK_KD_NODE_MIN_SPLIT;
		m_max[0] = m_max[1] = m_max[2] = HK_KD_NODE_MAX_SPLIT;
	}

};

struct hkKdTreeCinfo
{
	enum EmptyNodeAllocation
	{
		NO_EMPTY_NODES = 0,
		FEW_EMTPY_NODES = 8,
		STANDARD_EMPTY_NODES = 2,
		LOTS_OF_EMPTY_NODES = 1
	};

	int m_numPrimitives;
	hkEnum<EmptyNodeAllocation, hkUint8> m_emptyNodeAllocation;
	hkBool m_isDistributedBuild;
	hkAabb m_aabb;

	hkKdTreeCinfo();
};

class hkKdTree
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, hkKdTree);

	hkKdTree(int prealloc); // deprecated
	hkKdTree(const hkKdTreeCinfo& cinfo);
	~hkKdTree();
	void reset(const hkKdTreeCinfo& cinfo);

	hkArray<hkKdTreeNode>& getNodes() { return m_nodes; }
	const hkArray<hkKdTreeNode>& getNodes() const { return m_nodes; }
	hkKdTreeNode* getRoot() { return m_nodes.begin(); }
	const hkKdTreeNode* getRoot() const { return m_nodes.begin(); }

	HK_FORCE_INLINE int getNumPrimitives() const { return m_numPrimitives; }
	HK_FORCE_INLINE int getNumReservedEmptyNodes() const { return m_numReservedEmptyNodes; }
	HK_FORCE_INLINE int getTotalNumNodes() const { return m_numReservedEmptyNodes + m_numRegularNodes; }

	HK_FORCE_INLINE hkAabb& getAabb() { return m_aabb; }
	HK_FORCE_INLINE const hkAabb& getAabb() const { return m_aabb; }

	HK_FORCE_INLINE hkKdTreeProjectedEntry* getProjectedEntries() { return m_projectedEntries.begin(); }
	HK_FORCE_INLINE const hkKdTreeProjectedEntry* getProjectedEntries() const { return m_projectedEntries.begin(); }

	enum
	{
		DISTRIBUTED_BUILD_SUBTREE_OFFSET = 8,
	};

	enum
	{
		SCALE_MULTIPLIER = 65532,
	};

	// Summary of memory constraints and assumptions
	// Fast builds:
	//		1) The smallest tree (1 primitive) needs 2 nodes - 1 root, and one for 1 padding
	//		2) For larger trees, for N primitives, we have N leaf nodes, N-1 tree nodes, and 1 extra node at the root.
	//			This doesn't count reserved empty nodes, and the actual count can be less because of writing multiple primitives per leaf.
	static HK_FORCE_INLINE int HK_CALL getSizeForFastBuild(int numPrimitives) {	return 2 * numPrimitives;	}
	

	// Summary of memory constraints and assumptions
	// Distributed builds:
	//		1) We set up 4 sub-trees, so need to be able to handle the worst-case distribution
	//		2) The size of each subtree is hkKdTree::getSizeForFastBuild(numPrimsInSubTree)
	//		3) The single-threaded version of tree building starts the subtrees at node 8 (since it needs to reassemble the tree later
	static HK_FORCE_INLINE int HK_CALL getSizeForDistributedBuild(int numPrimitives);

	static HK_FORCE_INLINE int HK_CALL getNumExtraNodesForFastBuild(int numPrimitives, hkKdTreeCinfo::EmptyNodeAllocation ena);

		/// Prevent a node from being "hit" by raycasts
	void invalidatePrimitive(hkPrimitiveId id);


	HK_ALIGN16(hkArray<hkKdTreeNode> m_nodes);
	int m_maxDepth;



protected:
	void initNodeArray(int numNodes);

	int m_numPrimitives;
	int m_numRegularNodes;
	int m_numReservedEmptyNodes;
	hkArray<struct hkKdTreeProjectedEntry> m_projectedEntries;

	hkAabb m_aabb;

	HK_FORCE_INLINE void setNumPrimitives(int n);
};

#include <Common/Internal/KdTree/hkKdTree.inl>

#endif // HK_KDTREE_H

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
