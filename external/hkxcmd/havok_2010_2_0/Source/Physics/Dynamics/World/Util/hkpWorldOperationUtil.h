/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_WORLD_UTIL_H
#define HK_DYNAMICS2_WORLD_UTIL_H

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Motion/hkpMotion.h>

class hkpConstraintInstance;
class hkpEntity;
struct hkpAgentNnTrack;
class hkpContinuousSimulation;

/// Class hkpWorldOperationUtil
class hkpWorldOperationUtil
{
	public://private:
		enum FireCallbacks
		{
				// this is only used in addEntity and removeEntity, when they're called from hkpRigidBody::setMotionType()
			DO_NOT_FIRE_CALLBACKS_AND_SUPPRESS_EXECUTION_OF_PENDING_OPERATIONS,
			DO_FIRE_CALLBACKS
		};

	public:

			//
			// WARNING: Those are internal functions and should never be called directly.
			//
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [entity,HK_ACCESS_RW] );
		static void HK_CALL    addEntityBP ( hkpWorld* world, hkpEntity* entity );
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [entity,HK_ACCESS_RW] );
		static void HK_CALL removeEntityBP ( hkpWorld* world, hkpEntity* entity );
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [entity,HK_ACCESS_RW] );
		static void HK_CALL updateEntityBP ( hkpWorld* world, hkpEntity* entity );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [phantom,HK_ACCESS_RW] );
		static void HK_CALL    addPhantomBP( hkpWorld* world, hkpPhantom* phantom);
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [phantom,HK_ACCESS_RW] );
		static void HK_CALL removePhantomBP( hkpWorld* world, hkpPhantom* phantom);


			/// When islands are enabled, creates an activated/deactivated island for the entity.
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [entity,HK_ACCESS_RW] );
		static void HK_CALL addEntitySI ( hkpWorld* world, hkpEntity* entity, hkpEntityActivation initialActivationState );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [entity,HK_ACCESS_RW] );
		static void HK_CALL removeEntitySI ( hkpWorld* world, hkpEntity* entity );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RO] [constraint->getEntityA()->getSimulationIsland(),HK_ACCESS_RW] [constraint->getEntityB()->getSimulationIsland(),HK_ACCESS_RW] );
		static void HK_CALL addConstraintToCriticalLockedIsland( hkpWorld* world, hkpConstraintInstance* constraint  );
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RO] [constraint->getEntityA()->getSimulationIsland(),HK_ACCESS_RW] [constraint->getEntityB()->getSimulationIsland(),HK_ACCESS_RW] );
		static void HK_CALL removeConstraintFromCriticalLockedIsland( hkpWorld* world, hkpConstraintInstance* constraint  );

		static hkpConstraintInstance*
						/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
					HK_CALL addConstraintImmediately   ( hkpWorld* world, hkpConstraintInstance* constraint, FireCallbacks fireCallbacks = DO_FIRE_CALLBACKS );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RO] [world,HK_ACCESS_RW] );
		static void HK_CALL removeConstraintImmediately( hkpWorld* world, hkpConstraintInstance* constraint, FireCallbacks fireCallbacks = DO_FIRE_CALLBACKS );


			//
			// Island handling
			//

			/// activate an island.
			/// Note that as the result of callback, the island may be destroyed before the function returns.
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL internalActivateIsland  ( hkpWorld* world, hkpSimulationIsland* island, hkBool allowIslandMerges = false );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL internalDeactivateIsland( hkpWorld* world, hkpSimulationIsland* island );

		static HK_FORCE_INLINE void
					/// ###ACCESS_CHECKS###( [entityA->getWorld(),HK_ACCESS_RO] );
				HK_CALL mergeIslandsIfNeeded(                 hkpEntity* entityA, hkpEntity* entityB);

			// Ask for two simulation islands to be merged (merge will be delayed if necessary)
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL mergeIslands    ( hkpWorld* world, hkpEntity* entityA, hkpEntity* entityB);

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void	HK_CALL internalMergeTwoIslands ( hkpWorld* world, hkpSimulationIsland* islandA, hkpSimulationIsland* islandB );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL removeIsland    ( hkpWorld* world, hkpSimulationIsland* island );

		// split a single island, internal function use splitSimulationIslands() instead,
		// if oldEntitiesOut is set, than the original entity array will be stored into oldEntitiesOut;
		static void HK_CALL splitSimulationIsland( hkpSimulationIsland* currentIsland, hkpWorld* world, hkArray<hkpSimulationIsland*>& newIslandsOut, hkArray<hkpEntity*>* oldEntitiesOut = HK_NULL );

		// split any possible active or inactive simulation islands
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL splitSimulationIslands( hkpWorld* world );

			// split a single island
		static void HK_CALL splitSimulationIsland( hkpWorld* world, hkpSimulationIsland* currentIsland );

		static void HK_CALL validateIsland  ( hkpWorld* world, hkpSimulationIsland* island );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RO] );
		static void HK_CALL validateWorld   ( hkpWorld* world );


			//
			// Internal, only used by hkpRigidBody::setMotionType
			//
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL removeAttachedActionsFromFixedIsland  ( hkpWorld* world, hkpEntity* entity, hkArray<hkpAction*>& actionsToBeMoved );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL removeAttachedActionsFromDynamicIsland( hkpWorld* world, hkpEntity* entity, hkArray<hkpAction*>& actionsToBeMoved );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL addActionsToEntitysIsland             ( hkpWorld* world, hkpEntity* entity, hkArray<hkpAction*>& actionsToBeMoved );


			//
			//
			//

			/// ###ACCESS_CHECKS###( [body->getWorld(),HK_ACCESS_RW] );
		static void HK_CALL setRigidBodyMotionType( hkpRigidBody* body, hkpMotion::MotionType newState, hkpEntityActivation initialActivationState, hkpUpdateCollisionFilterOnEntityMode queryBroadPhaseForNewPairs);

	private:
		// used for setEntityMotionType

			// Used by setMotionType
			// This stores dynamic motion and collidable's quality type when switching to fixed or keyframed motion.
			/// ###ACCESS_CHECKS###( [body->getWorld(),HK_ACCESS_RW] );
		static void HK_CALL replaceMotionObject(hkpRigidBody* body, hkpMotion::MotionType newState, hkBool newStateNeedsInertia, hkBool oldStateNeedsInertia, hkpWorld* world );

		static HK_FORCE_INLINE void HK_CALL sortBigIslandToFront( hkpWorld* world, hkpSimulationIsland* island );

			// Used by setMotionType
			// Function name ends with Plus, because it also does sth else.
			//  - if the agent connects entities in two separeate dynamic islands it
			//    moves the agentEntries to the dynamic island, which is not going to be changed to fixed. .. blah
			/// ###ACCESS_CHECKS###( [entity->getWorld(),HK_ACCESS_RW] );
		static void HK_CALL removeAttachedAgentsConnectingTheEntityAndAFixedPartnerEntityPlus( hkpAgentNnTrack& trackToScan, hkpEntity* entity, hkpAgentNnTrack& agentsRemoved, hkpMotion::MotionType newMotionType);

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL removeAttachedConstraints( hkpEntity* entity, hkArray<hkpConstraintInstance*>& constraintsToBeMoved );


	public:
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL cleanupDirtyIslands( hkpWorld* world );


		// marks an island for deactivation. Only call this when you have single threaded rw access to the world
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL markIslandInactive( hkpWorld* world, hkpSimulationIsland* island );

		// marks an island for deactivation. Only call this when you have multithreaded read only access to the world
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RO] );
		static void HK_CALL markIslandInactiveMt( hkpWorld* world, hkpSimulationIsland* island );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RO] [island,HK_ACCESS_RW] );
		static void HK_CALL markIslandActive( hkpWorld* world, hkpSimulationIsland* island );
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] );
		static void HK_CALL removeIslandFromDirtyList( hkpWorld* world, hkpSimulationIsland* island);


			// calculates a size information which can be used for canIslandBeSparse(world,size)
		HK_FORCE_INLINE static int  HK_CALL estimateIslandSize( int numEntities, int numConstraints );

			// returns true if the island defined by the given size (by estimateIslandSize) would be a candidate for a sparseEnabled island.
			// These are islands which hold lots of independent objects. See hkpWorldCinfo::m_minDesiredIslandSize for details
		HK_FORCE_INLINE static bool HK_CALL	canIslandBeSparse( hkpWorld* world, int islandSize );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [island,HK_ACCESS_RW] );
		HK_FORCE_INLINE static void HK_CALL putIslandOnDirtyList( hkpWorld* world, hkpSimulationIsland* island);

};




#include <Physics/Dynamics/World/Util/hkpWorldOperationUtil.inl>

#endif // HK_DYNAMICS2_WORLD_UTIL_H

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
