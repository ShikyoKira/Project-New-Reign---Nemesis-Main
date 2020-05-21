/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KD_TREE_JOB_UTILS
#define HK_KD_TREE_JOB_UTILS

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Common/Internal/KdTree/QueryUtils/hkKdTreeUtils.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTree.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>

#define hkpWorldRayCastOutputPpu hkpWorldRayCastOutput

struct hkpKdTreeBuildSetupJob;

class hkKdTreeRayCastJobUtil
{
	public:
		// Returns a pointer to the next 'free' hkpWorldRayCastOutput.
		// This can either be:
		// - the next truly free entry (if there's room left in the array)
		// - the furthest hit in the array (if the array is already full and we need to drop entries)
		// - HK_NULL if the array is full and the current hit is further away than the furthest hit in the entry

		static void HK_CALL initializeBuildSetupJobs(const hkpCollidable* const* collidablesPtr, int numCollidables, hkKdTreeProjectedEntry* entries, hkSemaphoreBusyWait* semaphore, int numJobs, hkpKdTreeBuildSetupJob* jobsOut);

		static void HK_CALL makeBundleFromInputs(hkpWorldRayCastInput* inputs, hkKdTreeUtils::RayBundle& fromBundle, hkKdTreeUtils::RayBundle& toBundle);

};

struct hkpWorldRayCastCommand;
class hkpCollisionFilter;
class hkKdTree;
struct hkpWorldRayCastBundleCommand;
class hkSpu4WayCache;
class hkpFixedBufferRayHitCollector;

class hkTreeRayCastJobUtil
{
	public:
		static void HK_CALL castRayKdTree(const hkKdTree* kdTree, hkpWorldRayCastCommand* command, const hkpCollisionFilter* filter, hkpFixedBufferRayHitCollector* fixedBufferHitCollector,
			hkSpu4WayCache* collidableCache=HK_NULL, hkSpu4WayCache* shapeCache=HK_NULL, hkSpu4WayCache* transformCache=HK_NULL,
			hkpWorldRayCastOutputPpu* resultLocalArray = HK_NULL);

		template<typename BOUNDING_VOLUME>
		static void HK_CALL castRayBundleAabbTree(const hkAabbTree<BOUNDING_VOLUME>* aabbTree, hkpWorldRayCastBundleCommand* command, const hkpCollisionFilter* filter,
			hkVector4& earlyHitFractionInOut,
			hkSpu4WayCache* collidableCache=HK_NULL, hkSpu4WayCache* shapeCache=HK_NULL, hkSpu4WayCache* transformCache=HK_NULL,
			hkpWorldRayCastOutputPpu* resultLocalArray = HK_NULL, int* numResultsOut = HK_NULL);

		static void HK_CALL castRayBundleKdTree(const hkKdTree* kdTrees, hkpWorldRayCastBundleCommand* command, const hkpCollisionFilter* filter,
			hkVector4& earlyHitFractionInOut,
			hkSpu4WayCache* collidableCache=HK_NULL, hkSpu4WayCache* shapeCache=HK_NULL, hkSpu4WayCache* transformCache=HK_NULL,
			hkpWorldRayCastOutputPpu* resultLocalArray = HK_NULL, int* numResultsInOut = HK_NULL);


		static void HK_CALL castRayByCommands(hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16,hkUint16>* aabbTreeNewInt,
			hkKdTree* kdTree, hkpWorldRayCastCommand* commandArrayOnPpu, int numCommands, hkpCollisionFilter* filter,
			hkSpu4WayCache* collidableCache=HK_NULL, hkSpu4WayCache* shapeCache=HK_NULL, hkSpu4WayCache* transformCache=HK_NULL);

		enum
		{
			RAY_FILTERING_STREAM_SIZE = 128,
		};

		static void HK_CALL castRayBundleByCommands(hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16,hkUint16>* aabbTreeNewInt,
			hkKdTree* kdTree, hkpWorldRayCastBundleCommand* commandArrayOnPpu, int numCommands, hkpCollisionFilter* filter,
			hkSpu4WayCache* collidableCache=HK_NULL, hkSpu4WayCache* shapeCache=HK_NULL, hkSpu4WayCache* transformCache=HK_NULL);
};

#undef hkpWorldRayCastOutputPpu

#endif // HK_COLLISION_JOB_UTILS

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
