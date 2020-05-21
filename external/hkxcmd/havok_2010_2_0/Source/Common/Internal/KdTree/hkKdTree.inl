/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


HK_FORCE_INLINE hkSplitType hkConvertRealToSplitType( hkReal in, hkReal offset, hkReal scale )
{
	//replace with proper int to float correction
	return hkSplitType( (in - offset) / scale * hkReal(hkKdTree::SCALE_MULTIPLIER) );
}

HK_FORCE_INLINE hkReal hkConvertSplitTypeToReal( hkSplitType in, hkReal offset, hkReal scale )
{
	return  (hkReal(in) / hkReal(hkKdTree::SCALE_MULTIPLIER) ) * scale  + offset;
}


hkKdTreeNode::hkKdTreeNode( )  
{ 
	m_data.m_leaf.m_type_primId =0x7FFFFFFF; /* Empty leaf */ 
	m_data.m_leaf.m_numPrimitives = 0;
}

int hkKdTreeNode::getSplitAxis() const
{
	HK_ASSERT(0x1d97fecf, !isLeaf());
	return m_data.m_node.m_type_left_axis & 0x00000003;
}

hkSplitType hkKdTreeNode::getSplitMin() const
{
	HK_ASSERT(0x1d97fecf, !isLeaf());
	return m_data.m_node.m_splitMin;
}

hkSplitType hkKdTreeNode::getSplitMax() const
{
	HK_ASSERT(0x1d97fecf, !isLeaf());
	return m_data.m_node.m_splitMax;
}

hkPrimitiveId hkKdTreeNode::getPrimitiveId() const
{
	HK_ASSERT(0x1d97fecf, isLeaf());
	return m_data.m_leaf.m_type_primId & 0x7FFFFFFF;
}

hkKdTreeNode* hkKdTreeNode::getLeft() const
{
	HK_ASSERT(0x1d97fecf, !isLeaf());
	
	hkKdTreeNode* left = reinterpret_cast<hkKdTreeNode*> ((hkUlong)this + (m_data.m_node.m_type_left_axis & 0x7FFFFFFC) );
	
	// Alignment check
	HK_ASSERT(0x206ee6f2, (((hkUlong) left) & 0xF) == 0);

	return left;
}

hkKdTreeNode* hkKdTreeNode::getRight() const
{
	hkKdTreeNode* right = getLeft()+1;

	HK_ASSERT(0x206ee6f3, (((hkUlong) right) & 0xF) == 8);

	return right;
}

hkUint32 hkKdTreeNode::getLeftOffset() const
{
	return m_data.m_node.m_type_left_axis & 0x7FFFFFFC;
}

hkUint32 hkKdTreeNode::getRightOffset() const
{
	return getLeftOffset() + sizeof(hkKdTreeNode);
}

void hkKdTreeNode::setSplit(int axis, hkSplitType splitMin, hkSplitType splitMax)
{
	HK_ASSERT(0x430fd315, (axis >=0) && (axis <=3) );

	m_data.m_node.m_splitMin = splitMin;
	m_data.m_node.m_splitMax = splitMax;
	m_data.m_node.m_type_left_axis = 0x80000000 | (m_data.m_node.m_type_left_axis & 0xFFFFFFFC) | axis;
}

hkKdTreeNode* hkKdTreeNode::getSibling() const
{
	hkKdTreeNode* sibling = reinterpret_cast<hkKdTreeNode*> ((hkUlong)this ^ 0x8 );
	HK_ASSERT(0x6115931c, (isLeft() && sibling->isRight()) || (isRight() && sibling->isLeft()) );

	return sibling;
}

void hkKdTreeNode::setNumPrimitivesInLeaf( hkUint32 offset )
{
	m_data.m_leaf.m_numPrimitives = offset;
}

hkUint32 hkKdTreeNode::getNumPrimitivesInLeaf() const
{
	HK_ASSERT(0x1d97fecf, isLeaf());
	return m_data.m_leaf.m_numPrimitives;
}

hkBool32 hkKdTreeNode::isLeaf32() const
{
	return ~(m_data.m_leaf.m_type_primId >> 31);
}

hkBool32 hkKdTreeNode::isEmptyLeaf32() const
{
	return ~(m_data.m_leaf.m_type_primId ^ 0x7FFFFFFF);
}

hkBool hkKdTreeNode::isLeaf() const
{
	return m_data.m_leaf.m_type_primId >= 0;
}

hkBool hkKdTreeNode::isEmptyLeaf() const
{
	return m_data.m_leaf.m_type_primId == 0x7FFFFFFF;
}

hkBool hkKdTreeNode::isLeft() const
{
	return ( ((hkUint32) (hkUlong) this) & 0xF) == 0x0;
}

hkBool hkKdTreeNode::isRight() const
{
	return ( ((hkUint32) (hkUlong) this) & 0xF) == 0x8;
}

void hkKdTreeNode::setPrimitiveId(hkPrimitiveId id)
{
	HK_ASSERT(0x4097d487, id < 0x7FFFFFFF );
	m_data.m_leaf.m_type_primId = (int)id & 0x7FFFFFFF;
}


void hkKdTreeNode::setLeftAndRightByteOffset( hkUlong offset )
{
	HK_ASSERT(0x7006becc, (offset & 0x3) == 0);
	HK_ASSERT(0x77fc899b, (offset & 0x80000000) == 0);

	m_data.m_node.m_type_left_axis =  (m_data.m_node.m_type_left_axis & 0x80000003) | ((hkUint32)offset);
}

void hkKdTreeNode::adjustLeftAndRightByteOffset( int offsetDelta )
{
	hkUlong offset = (m_data.m_node.m_type_left_axis & ~0x80000003);
	offset += offsetDelta;
	m_data.m_node.m_type_left_axis =  (m_data.m_node.m_type_left_axis & 0x80000003) | ((hkUint32)offset);
}


void hkKdTreeNode::setEmpty()
{
	m_data.m_node.m_type_left_axis =0x7FFFFFFF; /* Empty leaf */ 
}

void hkKdTree::setNumPrimitives(int n)
{
	m_numPrimitives = n; 
	m_projectedEntries.setSize(n);
}

inline int HK_CALL hkKdTree::getSizeForDistributedBuild(int numPrimitives)
{
	// Worst distribution is 3 subtrees of size 1 and 1 of size N-3
	const int smallSubTreeSize = getSizeForFastBuild(1);
	const int bigSubTreeSize = getSizeForFastBuild(numPrimitives - 3);
	const int singleThreadedOverhead = DISTRIBUTED_BUILD_SUBTREE_OFFSET;

	return singleThreadedOverhead + bigSubTreeSize + 3*smallSubTreeSize;
}
inline int HK_CALL hkKdTree::getNumExtraNodesForFastBuild(int numPrimitives, hkKdTreeCinfo::EmptyNodeAllocation ena)
{
	if (ena == hkKdTreeCinfo::NO_EMPTY_NODES)
		return 0;
	else
		return numPrimitives / (int) ena;
}

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
