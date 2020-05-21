/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#define DynamicTree hkAabbTree<BOUNDING_VOLUME, hkIdxType>

template<typename BOUNDING_VOLUME, typename hkIdxType>
HK_FORCE_INLINE
typename DynamicTree::Node* DynamicTree::getChild(const Node* node, int i) const
{
	return const_cast<Node*>(m_nodeManager.getNode(node->m_nodeData.m_childIdx[i]));
}

template<typename BOUNDING_VOLUME, typename hkIdxType>
HK_FORCE_INLINE
typename DynamicTree::Node* DynamicTree::getSkipNode(const Node* node) const
{
	return (node->m_skipNodeIdx != s_invalidIdx)? 
		const_cast<Node*>(m_nodeManager.getNode(node->m_skipNodeIdx)) : HK_NULL;
}

template<typename BOUNDING_VOLUME, typename hkIdxType>
HK_FORCE_INLINE
typename DynamicTree::Node* DynamicTree::getParent(const Node* node) const
{
	return ((node->m_parentIdx & Node::s_leafMask) != s_invalidIdx)? 
		const_cast<Node*>(m_nodeManager.getNode(node->m_parentIdx  & Node::s_leafMask) ): HK_NULL; 
}

template<typename BOUNDING_VOLUME, typename hkIdxType>
HK_FORCE_INLINE
typename DynamicTree::Node* DynamicTree::getNode(hkIdxType nodeIdx) const
{
	return const_cast<Node*>(m_nodeManager.getNode( nodeIdx ));
}


template<typename BOUNDING_VOLUME, typename hkIdxType>
typename DynamicTree::Node* DynamicTree::NodeManager::getNode( hkIdxType idx )
{
	HK_ASSERT(0x17e484b, idx < hkIdxType(m_nodes.getSize()) );
	return &m_nodes[idx];
}

template<typename BOUNDING_VOLUME, typename hkIdxType>
const typename DynamicTree::Node* DynamicTree::NodeManager::getNode( hkIdxType idx ) const
{
	HK_ASSERT(0x1396e0f5, idx < hkIdxType(m_nodes.getSize()) );
	return &m_nodes[idx];
}

template<typename BOUNDING_VOLUME, typename hkIdxType>
template< typename T >
void DynamicTree::fetchLeavesData(hkIdxType rootIdx, hkArray<T>& leavesData)
{
	hkArray<hkIdxType> leafIdx;
	collectLeaves(rootIdx, leafIdx);

	leavesData.setSize( leafIdx.getSize() );
	for(int i=0; i<leafIdx.getSize(); i++)
	{
		leavesData[i] = getNode( leafIdx[i] )->template getLeafData<T>();
	}
}

#undef DynamicTree

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
