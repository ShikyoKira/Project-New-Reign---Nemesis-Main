/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_AABBTREEWORLDMANAGER_H
#define HKP_AABBTREEWORLDMANAGER_H


#include <Physics/Dynamics/World/Util/hkpTreeWorldManager.h>


#include <Physics/Dynamics/World/hkpWorldCinfo.h>
#include <Physics/Dynamics/World/Listener/hkpIslandActivationListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Phantom/hkpPhantomListener.h>

#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>
#include <Common/Internal/KdTree/Build/hkKdTreeBuildInput.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTree.h>



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
struct hkpCollisionQueryJobHeader;

void HK_CALL hkpAabbTreeWorldManager_registerSelf();

class hkpAabbTreeWorldManager : public hkpTreeWorldManager, 
	public hkpEntityListener, 
	public hkpPhantomListener, 
	public hkpIslandActivationListener, 
	public hkpWorldPostSimulationListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

		hkpAabbTreeWorldManager(hkpWorld* world, hkpWorldCinfo::TreeUpdateType updateType = hkpWorldCinfo::REBUILD_ACTIVE, hkReal aabbExpandDt = 0.0f );
		virtual ~hkpAabbTreeWorldManager();

		static hkpTreeWorldManager* HK_CALL create( hkpWorld* world, hkpWorldCinfo::TreeUpdateType updateType, hkReal aabbExpandDt );

		void reset();

		//	Callback interfaces
		//hkpEntityListener Interface
		virtual void entityAddedCallback( hkpEntity* entity );
		virtual void entityRemovedCallback( hkpEntity* entity );
		virtual void entitySetMotionTypeCallback( hkpEntity* entity );
		virtual void entityDeletedCallback( hkpEntity* entity );

		// hkpPhantomListener Interface
		virtual void phantomAddedCallback( hkpPhantom* phantom );
		virtual void phantomRemovedCallback( hkpPhantom* phantom );

		//	hkpEntityActivationListener Interface
		virtual void islandActivatedCallback( hkpSimulationIsland* island );
		virtual void islandDeactivatedCallback( hkpSimulationIsland* island );

		//	hkpWorldPostSimulationListener Interface
		virtual void postSimulationCallback( hkpWorld* world );
		virtual void inactiveEntityMovedCallback( hkpEntity* entity );


		//	Maintenance
		virtual void updateFromWorld(hkJobQueue* jobQueue = HK_NULL, hkJobThreadPool* jobThreadPool = HK_NULL);

		virtual bool isUpToDate() const { return m_treeIsClean;}
		
		virtual void setUpToDate( bool upToDate ) { m_treeIsClean = upToDate;}

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

		virtual int calcWorkingBufferSize(int maxObjects);

		virtual void setWorkingBuffer(void* ptr);

		const hkArray<const hkpCollidable*>& getActiveList() const{return m_activeList;}

		static void HK_CALL gatherCollidablesFromWorld(hkArray<const hkpCollidable*>& collidables, const hkpWorld* world);

		static void HK_CALL gatherActiveCollidablesFromWorld(hkArray<const hkpCollidable*>& collidables, const hkpWorld* world);

		static void HK_CALL gatherInactiveCollidablesFromWorld(hkArray<const hkpCollidable*>& collidables, const hkpWorld* world);

		void checkConsistency();

	protected:

		void removeObjectFromTree(const hkpWorldObject* object);

		//	clear the tree before build
		//	setNode( HK_NULL ) is called in buildActiveTree
		//	setNode( leaf ) is called in buildInactiveTree
		void buildActiveTree(hkArray<const hkpCollidable*>& collidables, hkJobQueue* jobQueue = HK_NULL, hkJobThreadPool* jobThreadPool = HK_NULL);

		void buildInactiveTree(hkArray<const hkpCollidable*>& collidables, hkJobQueue* jobQueue = HK_NULL, hkJobThreadPool* jobThreadPool = HK_NULL);

		void buildKdTree(hkKdTree*& tree, hkArray<const hkpCollidable*>& collidables, hkpWorld* world, hkJobQueue* jobQueue, hkJobThreadPool* jobThreadPool);

		void setTreeIsClean();

		void clearActiveTree();


		hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>::Node* getNode(const hkpCollidable* collidable);
		
		hkUint16 getNodeIdx(const hkpCollidable* collidable) const;

		static void setNode(const hkpCollidable* collidable, hkUint16 nodeIdx);

		static const hkUint16 s_invalidIdx = 0xffff;


	protected:

		void incrementalBuild(hkJobQueue *jobQueue, hkJobThreadPool *jobThreadPool);

		void fullBuild(hkJobQueue *jobQueue, hkJobThreadPool *jobThreadPool);

		//	returns true if all the objects are scanned
		bool scanActiveListAndGetInactivated(hkArray<const hkpCollidable*>& inactivatedOut);

		void pushBackToActiveList( const hkpCollidable* collidable );
	
	protected:

		void objActivated(int& value) const { value++;}
		void objInactivated(int& value) const { value--;}
		bool objIsActive(int& value, const hkpCollidable* c) const 
		{
			hkUint16 idx = getNodeIdx(c);
			return (value == 1 && idx != s_invalidIdx);
		}
		bool objIsInactive(int& value, const hkpCollidable* c) const {return (value == -1);}

		void queueActivated( const hkpCollidable* c );
		void queueInactivated( const hkpCollidable* c );

	public:
		bool m_useInactiveTree;
	
		enum 
		{
			NUM_DISTRIBUTED_BUILD_THRESHOLD = 32, 
		};

	protected:
		hkpWorld* m_world;
		bool m_doneFirstBuild;//have to full build at the first time in incremental update

		//	kd tree datas
		hkKdTreeBuildInput::WorkingSet m_kdTreeWorkingSet;
		// max objects for kd tree
		int m_maxObjects;
		hkpCollisionQueryJobHeader* m_kdTreeJobpHeader;

		

		//	general datas
		hkReal m_aabbExpandDt;

		hkArray<const hkpCollidable*> m_activeList;

		//	map collidable to transition data index
		hkPointerMap<const hkpCollidable*, int> m_transitionQueue;

		//	for inactivated check
		bool m_enableFullScan;
		int m_activeListCheckStartIdx;
		int m_numObjToCheckInAFrame;


		bool m_treeIsClean;

		//	in order to update only there are some bodies changing state
		//	switch these flags in callback functions
		bool m_mustUpdateActiveList;

		hkSemaphoreBusyWait* m_kdTreeSemaphore;
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
