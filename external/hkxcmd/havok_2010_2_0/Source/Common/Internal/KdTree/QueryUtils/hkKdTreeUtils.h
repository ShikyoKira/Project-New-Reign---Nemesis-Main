/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KDTREE_UTILS_H
#define HK_KDTREE_UTILS_H

#include <Common/Internal/KdTree/hkKdTree.h>

struct hkpShapeRayCastInput;
struct hkpShapeRayCastOutput;
struct hkpWorldRayCastInput;

class hkKdTreeCastCollector;
class hkKdTreeBundleCastCollector;

#define KD_TREE_USE_SPU_CACHE

class hkKdTreeUtils
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkKdTreeUtils);
	enum {DEFAULT_STACK_SIZE = 128};

	static hkBool HK_CALL castRayMultiTree(int numTrees, hkKdTree* const* trees, const hkVector4& start, const hkVector4& end, hkKdTreeCastCollector* collector);
	static hkBool HK_CALL castRayIterative( const hkKdTree*, const hkVector4& start, const hkVector4& end, hkKdTreeCastCollector* collector);
	static void HK_CALL castRayRecursive( const hkKdTree*, const hkVector4& orig, const hkVector4& dir, hkReal tNear, hkReal tFar, hkReal originalLength, hkKdTreeCastCollector* collector);

	struct RayBundle
	{
		hkVector4 m_vec[3];
	};

	static hkBool HK_CALL isValidRayBundle(const RayBundle& fromBundle, const RayBundle& toBundle, hkVector4Comparison::Mask activeRayMask);
	static hkBool HK_CALL isValidRayBundle(const hkVector4* fromBundle, const hkVector4* toBundle, hkVector4Comparison::Mask activeRayMask);

	// Ray bundle versions
	static hkBool HK_CALL castRayBundleIterative( const hkKdTree* tree, const RayBundle& start, const RayBundle& end, hkKdTreeBundleCastCollector* collector, hkVector4Comparison::Mask rayMask = hkVector4Comparison::MASK_XYZW);

private:
	static void HK_CALL _castRayRecursive( const hkKdTreeNode* root, const hkKdTreeNode* node, const hkKdTreeProjectedEntry* projectedEntries, const hkVector4& scale, const hkVector4& offset, const hkVector4& orig, const hkVector4& dir, hkReal tNear, hkReal tFar, hkReal originalLength, hkKdTreeCastCollector* collector);

public:

		/// Stack structure for use in castRayIterative
	struct NodeState
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkKdTreeUtils::NodeState);
		HK_ALIGN16(const hkKdTreeNode* node);
		hkReal tNear;
		hkReal tFar;
		
		hkUint32 m_padding;

		void set(const hkKdTreeNode* n, hkReal tn, hkReal tf) 
		{
			node = n;
			tNear = tn;
			tFar = tf;
		}
	};

		/// Stack structure for use in castRayBundleIterative
	struct BundleState
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkKdTreeUtils::BundleState);
		hkPadSpu<const hkKdTreeNode*> node;
		hkVector4 tNear;
		hkVector4 tFar;
		hkVector4Comparison activeMask; 

		void set(const hkKdTreeNode* n, hkVector4Parameter tn, hkVector4Parameter tf, hkVector4Comparison& mask) 
		{
			node = n;
			tNear = tn;
			tFar = tf;
			activeMask = mask;
		}
	};

};

//
// Tree node access methods
// These reduce to simple pointer access on PPU/CPU, but use a DMA cache on SPU
//

HK_ALWAYS_INLINE const hkKdTreeNode* HK_CALL hkGetNodeFromMainMemory(const hkKdTreeNode* ppuNode, hkBool waitForCompletion = true);
HK_ALWAYS_INLINE const hkKdTreeNode* HK_CALL hkGetLeftMainMemoryAddr(const hkKdTreeNode& spuNode, const hkKdTreeNode* ppuNode);
HK_ALWAYS_INLINE const hkKdTreeNode* HK_CALL hkGetRightMainMemoryAddr(const hkKdTreeNode& spuNode, const hkKdTreeNode* ppuNode);
HK_ALWAYS_INLINE const hkKdTreeProjectedEntry* HK_CALL hkGetProjectedEntry(const hkKdTreeNode& spuNode, const hkKdTreeProjectedEntry* base, int primIdx = 0);
HK_ALWAYS_INLINE void HK_CALL hkWaitForDma();

#include <Common/Internal/KdTree/QueryUtils/hkKdTreeUtils.inl>

#endif // HK_KDTREE_UTILS_H

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
