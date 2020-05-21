/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KDTREE_AABB_CAST_UTILS_H
#define HK_KDTREE_AABB_CAST_UTILS_H

#include <Common/Internal/KdTree/hkKdTree.h>

#define KD_TREE_USE_SPU_CACHE

class hkKdTreeAabbCastUtils
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkKdTreeAabbCastUtils);
	enum {DEFAULT_STACK_SIZE = 128};

	struct NodeState
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkKdTreeAabbCastUtils::NodeState);
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

	struct hkCastAabbInput
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkKdTreeAabbCastUtils::hkCastAabbInput );

		/// The start of the AABB cast (in world space).
		hkVector4 m_from;
		/// To end of the AABB cast (in world space).
		hkVector4 m_to;
		/// The half extents of the AABB.
		hkVector4 m_halfExtents;

		hkCastAabbInput() {}
	};

	static hkBool HK_CALL castAabb( const hkKdTree* root, hkKdTreeAabbCastUtils::hkCastAabbInput& castInput, class hkKdTreeCastCollector* collector, hkReal aabbCheckTolerance);


};

#endif // HK_KDTREE_AABB_CAST_UTILS_H

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
