/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CPU_COLLIDE_QUERY_WORLD_RAYCAST_JOB_H
#define HK_CPU_COLLIDE_QUERY_WORLD_RAYCAST_JOB_H


#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>
#include <Physics/Collide/BroadPhase/hkpBroadPhaseCastCollector.h>
#include <Common/Internal/KdTree/QueryUtils/hkKdTreeCastCollector.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.h>
#include <Common/Internal/Collide/Tree/AabbTree/QueryUtils/hkAabbTreeQueryUtils.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>


class hkpFixedBufferRayHitCollector;
struct hkpWorldRayCastCommand;
struct hkpWorldRayCastOutput;
struct hkpWorldRayCastInput;

class hkCpuWorldRayCastCollector : public hkpBroadPhaseCastCollector,
	public hkKdTreeCastCollector,
	public hkAabbTreeQueryUtils<hkAabbTreeData::hkAabbTreeAabb, hkUint16>::Collector,
	public hkAabbTreeQueryUtils<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>::Collector
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkCpuWorldRayCastCollector );
		hkCpuWorldRayCastCollector() : m_fixedBufferRayHitCollector(HK_NULL){ resetHitInfo();}
		void resetHitInfo(){m_hitFraction = 1.0f; m_hit = false;}

		//	interface for aabbTree cast collector
		virtual hkReal addLeaf( const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb, hkUint16>::Node* leaf );
		virtual hkReal addLeaf( const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>::Node* leaf );
	protected:

		//	interface for broadphase cast collector
		virtual hkReal addBroadPhaseHandle( const hkpBroadPhaseHandle* broadPhaseHandle, int castIndex );

		//	interface for kdtree cast collector
		virtual	hkBool addMidPhaseHit( hkPrimitiveId primId );
		virtual hkBool hasHit() {return m_hit; }
		virtual hkReal getHitFraction() {return m_hitFraction;}

		hkBool testCollidable(const hkpCollidable* collidable);

	public:

		// global, command-independent data
		const hkpCollisionFilter*			m_filter;
		hkReal						m_hitFraction;
		hkBool						m_hit;

		// command-dependent data
		hkpWorldRayCastCommand*			m_command;
		const hkpWorldRayCastInput*		m_originalInput;
		hkpWorldRayCastOutput*			m_result;
		hkpWorldRayCastOutput*			m_nextFreeResult;

		hkpShapeRayCastInput			m_workInput;

		hkpFixedBufferRayHitCollector*	m_fixedBufferRayHitCollector;

		hkVector4 m_earlyOutHitFraction;
};

hkJobQueue::JobStatus HK_CALL hkCpuWorldRayCastJob( hkJobQueue& jobQueue, hkJobQueue::JobQueueEntry& nextJobOut );

#endif // HK_CPU_COLLIDE_QUERY_WORLD_RAYCAST_JOB_H

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
