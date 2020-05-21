/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


HK_ALWAYS_INLINE const hkKdTreeNode* HK_CALL hkGetNodeFromMainMemory(const hkKdTreeNode* ppuNode, hkBool unused )
{
	return ppuNode;
}

HK_ALWAYS_INLINE const hkKdTreeNode* HK_CALL hkGetLeftMainMemoryAddr(const hkKdTreeNode& spuNode, const hkKdTreeNode* ppuNode)
{
	return ppuNode->getLeft();
}

HK_ALWAYS_INLINE const hkKdTreeNode* HK_CALL hkGetRightMainMemoryAddr(const hkKdTreeNode& spuNode, const hkKdTreeNode* ppuNode)
{
	return ppuNode->getRight();
}

HK_ALWAYS_INLINE const hkKdTreeProjectedEntry* HK_CALL hkGetProjectedEntry(const hkKdTreeNode& spuNode, const hkKdTreeProjectedEntry* base, int primIdx)
{
	const int aabbIdx = (int) spuNode.getPrimitiveId();
	const hkKdTreeProjectedEntry* const pe = base + aabbIdx + primIdx;
	return pe;
}

HK_ALWAYS_INLINE void HK_CALL hkWaitForDma() {}

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
