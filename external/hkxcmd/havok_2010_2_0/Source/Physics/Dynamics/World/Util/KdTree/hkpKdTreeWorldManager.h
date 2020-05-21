/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KdTreeWorldManager_H
#define HK_KdTreeWorldManager_H

#include <Physics/Dynamics/World/Util/hkpTreeWorldManager.h>

#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/World/Listener/hkpIslandActivationListener.h>
#include <Physics/Dynamics/Phantom/hkpPhantomListener.h>

#include <Common/Base/Thread/JobQueue/hkJobQueue.h>


class hkJobThreadPool;
class hkJobQueue;
class hkpEntity;
class hkKdTree;

struct hkpWorldRayCastInput;
struct hkpWorldRayCastOutput;
class hkpRayHitCollector;
struct hkpLinearCastInput;
class hkpCdPointCollector;
class hkpCollisionFilter;

void HK_CALL hkpKdTreeWorldManager_registerSelf();

class hkpKdTreeWorldManager :  public hkpTreeWorldManager, 
								public hkpEntityListener, 
								public hkpPhantomListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

		hkpKdTreeWorldManager(hkpWorld* world, ObjectRemovedBehavior behav = MARK_TREE_AS_DIRTY);
		virtual ~hkpKdTreeWorldManager();

		static hkpTreeWorldManager* HK_CALL create( hkpWorld* world, ObjectRemovedBehavior behav );

		//
		// We need to receive callbacks to know when the world is dirty
		//

		//hkpEntityListener Interface
		virtual void entityAddedCallback( hkpEntity* entity );
		virtual void entityRemovedCallback( hkpEntity* entity );
		virtual void entitySetMotionTypeCallback( hkpEntity* entity );

		// hkpPhantomListener Interface
		virtual void phantomAddedCallback( hkpPhantom* phantom );
		virtual void phantomRemovedCallback( hkpPhantom* phantom );

		//
		// Maintenance
		//
		virtual void updateFromWorld(hkJobQueue* jobQueue = HK_NULL, hkJobThreadPool* jobThreadPool = HK_NULL);

		virtual bool isUpToDate() const { return !m_mustUpdateFromWorld; }
		virtual void setUpToDate(bool upToDate) { m_mustUpdateFromWorld = !upToDate; }

		//
		// Raycasting and linearcasting interface.
		// The methods are analogical to those in hkpWorld.
		//

		/// Cast a ray into the world and get the closest hit.
		virtual void castRay(const hkpWorldRayCastInput& input, const hkpCollisionFilter* filter, hkpWorldRayCastOutput& output ) const;

		/// Cast a ray into the world and do a callback for every hit.
		virtual void castRay(const hkpWorldRayCastInput& input, const hkpCollisionFilter* filter, hkpRayHitCollector& collector ) const;

		/// Cast a shape within the world.
		/// The castCollector collects all potential hits.
		virtual void linearCast(const hkpCollidable* collA, 
			const struct hkpLinearCastInput& input, const class hkpCollidableCollidableFilter* filter,
			const struct hkpCollisionInput& collInput, struct hkpCollisionAgentConfig* config,
			class hkpCdPointCollector& castCollector, class hkpCdPointCollector* startPointCollector );


	public:
	
		void buildTree(hkKdTree*& tree, const hkArrayBase<const hkpCollidable*>& collidables, hkJobQueue* jobQueue = HK_NULL, hkJobThreadPool* jobThreadPool = HK_NULL);

		hkKdTree* getTree() { return m_kdTree; }

		const hkKdTree* getTree() const { return m_kdTree; }

		// Must be public as it is accessed by some demos
		void gatherCollidablesFromWorld(hkArray<const hkpCollidable*>::Temp& collidables);

	protected:

		void buildTreeSingleThreaded(hkKdTree*& tree, const hkArrayBase<const hkpCollidable*>& collidables);
		void buildTreeDistributed(hkKdTree*& tree, const hkArrayBase<const hkpCollidable*>& collidables, hkJobQueue* jobQueue, hkJobThreadPool* jobThreadPool);
		void removeObjectFromTree(const hkpWorldObject* object);

	protected:
		hkpWorld* m_world;
		hkBool m_mustUpdateFromWorld;
		hkEnum<ObjectRemovedBehavior, hkUint8> m_objectRemovedBehavior;

		hkSemaphoreBusyWait* m_semaphore;
};

#endif	// HK_KdTreeWorldManager_H

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
