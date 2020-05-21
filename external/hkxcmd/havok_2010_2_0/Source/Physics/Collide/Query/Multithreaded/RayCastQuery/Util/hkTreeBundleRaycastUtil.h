/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TREE_BUNDLE_RAYCAST_UTIL_H
#define HK_TREE_BUNDLE_RAYCAST_UTIL_H

#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/Util/hkAabbTreeRaycastUtil.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>


struct hkpWorldRayCastOutput;
struct hkpWorldRayCastCommand;
struct hkpWorldRayCastBundleCommand;

struct hkpCollisionQueryJobHeader;
class hkpBroadPhase;
class hkKdTree;
struct hkpWorldRayCastJob;


class hkTreeBundleRayCastUtil
{
	public:
		///	this function set up jobs and start them in this function
		static void HK_CALL startWorldRayCastWithAutoBundleJobs(const hkVector4* rayFrom, const hkVector4* rayTo, int numRays, 
			hkReal angleVariance, hkReal distanceVariance2, 
			hkpWorldRayCastJob* worldRayCastJob, // size of commandArray, bundledCommandArray have to be 8*numRays
			hkJobQueue* jobQueue,
			hkJobThreadPool* jobThreadPool,
			hkpWorldRayCastOutput* resultBuffer,  // size == numRays
			bool scanAllInJobs =
			false
			);

		static void HK_CALL splitToOctant(const hkVector4* rayFrom, const hkVector4* rayTo, int numRays,
			hkVector4* rayVectorNormalized, hkSimdReal* rayLength, 
			hkArray<int>* dividedRayIdx, int* numRaysEachOctant);	// size of these have to be 8

		static void HK_CALL autoBundleFromList(int* indices, int numIndices, const hkVector4* rayFrom, const hkVector4* rayTo, int numRays, 
			hkReal angleVariance, hkReal distanceVariance2,
			hkpWorldRayCastOutput* resultBuffer,  // size == numRays
			hkpWorldRayCastCommand* singleCommandsOut, int& numSingleCommandsOut,
			hkpWorldRayCastBundleCommand* bundledCommandsOut, int& numBundledCommandsOut);

		static void HK_CALL autoBundleScanAll(int octIdx, const hkVector4* rayFrom, const hkVector4* rayTo, int numRays, 
			hkReal angleVariance, hkReal distanceVariance2, 
			hkpWorldRayCastOutput* resultBuffer,  // size == numRays
			hkpWorldRayCastCommand* singleCommandsOut, int& numSingleCommandsOut,
			hkpWorldRayCastBundleCommand* bundledCommandsOut, int& numBundledCommandsOut);

	private:
		/// check similarity of 2 lines
		static bool HK_CALL similaritiy(const hkVector4& points00, const hkVector4& points01, const hkVector4& rayVector0, 
			const hkVector4& points10, const hkVector4& points11, const hkVector4& rayVector1, 
			hkReal angleAllowance, hkReal distanceAllowance );

		HK_FORCE_INLINE
			static hkSimdReal HK_CALL pointToLineSegment2(const hkVector4& a, const hkVector4& b, const hkVector4& p);

};

#include <Physics/Collide/Query/Multithreaded/RayCastQuery/Util/hkTreeBundleRaycastUtil.inl>


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
