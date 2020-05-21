/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_AABB_TREE_RAYCAST_UTIL_H
#define HK_AABB_TREE_RAYCAST_UTIL_H

#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>



class hkCpuWorldRayCastCollector;
typedef hkCpuWorldRayCastCollector hkTreeWorldRayCastCollector;

struct hkpWorldRayCastCommand;
class hkpFixedBufferRayHitCollector;


template<typename BOUNDING_VOLUME, typename hkIdxType>
class hkAabbTree;

template<typename BOUNDING_VOLUME, typename hkIdxType>
class hkAabbTreeQueryUtils;

template<typename BOUNDING_VOLUME, typename hkIdxType = hkUint16>
class hkAabbTreeRayCastUtil
{
	public:

		typedef hkAabbTree<BOUNDING_VOLUME, hkIdxType> Tree;

		enum
		{
			ACTIVESTACK_MAX = 250,
			TASK_MAX = 10, 
		};

		static void HK_CALL castRayFiltering( const Tree* tree, const hkAabbTreeData::Ray* rays, 
			hkpWorldRayCastCommand* commandArray, const hkpCollisionFilter* filter,
			hkTreeWorldRayCastCollector collectors[4], hkpFixedBufferRayHitCollector* fixedBufferCollectors/*[4]*/,int numRays );

	private:
		static void HK_CALL filterRays(const Tree* tree, 
			const  typename Tree::Node* node, const  typename Tree::Node* skipNode, 
			const hkAabbTreeData::Ray* rays, hkTreeWorldRayCastCollector collectors[4], hkpFixedBufferRayHitCollector* fixedBufferCollectors/*[4]*/, 
			hkpWorldRayCastCommand* commandArray, const hkpCollisionFilter* filter,
			hkReal* earlyHitFractionArray,
			int* raysIdx, int numRays, 
			int taskCap, int raysIdxOutCap0, int raysIdxOutCap1, 
			int* raysIdxOut0, int& numRaysOut0, int* raysIdxOut1, int& numRaysOut1,
			typename Tree::Node& child0Out,
			typename Tree::Node& child1Out);

};

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
