/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_SIMULATION_H
#define HK_DYNAMICS2_SIMULATION_H

#include <Common/Base/hkBase.h>
#include <Common/Base/DebugUtil/DeterminismUtil/hkCheckDeterminismUtil.h>
#include <Physics/Dynamics/World/hkpWorld.h>	

class hkpConstraintQueryIn;
class hkpSimulationIsland;
struct hkpWorldDynamicsStepInfo;
struct hkpProcessCollisionInput;
struct hkpProcessCollisionOutput;
class hkpBroadPhase;
class hkpWorld;
class hkStepInfo;
class hkpEntity;
struct hkpAgentNnEntry;
struct hkpToiEvent;

extern const hkClass hkpSimulationClass;

void HK_CALL hkpSimulation_registerSelf();

	// Base class for simulations, performs simulation at discrete physical time steps.
class hkpSimulation: public hkReferencedObject
{
	//+serializable(false)
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpSimulation( hkpWorld* world );
		hkpSimulation( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) {}
		~hkpSimulation();

		// Register methods to create simulations s.th. code can be stripped when not in use
		typedef hkpSimulation* (HK_CALL *CreateSimulationFunction)( hkpWorld* );
		static CreateSimulationFunction createDiscrete;
		static CreateSimulationFunction createContinuous;
		static CreateSimulationFunction createMultithreaded;

		static hkpSimulation* HK_CALL create( hkpWorld* world );

		//
		// Synchronous interface
		//

			// Implemented method of hkpSimulation
		virtual hkpStepResult stepDeltaTime( hkReal physicsDeltaTime );


			// Advance the state of bodies in time.
		virtual hkpStepResult integrate( hkReal physicsDeltaTime );

			// Perform collision detection.
		virtual hkpStepResult collide();

			// Advance the current time
		virtual hkpStepResult advanceTime();


		//
		// Asynchronous interface (used in conjunction with the methods above)
		//

			// Schedule an asynchronous step, given a frame delta time
		void setFrameTimeMarker( hkReal frameDeltaTime );

			// Returns true if m_currentTime is earlier than the frame time
		bool isSimulationAtMarker();

			// Returns true if m_currentTime is equal to m_currentPsiTime
		bool isSimulationAtPsi() const;

		//
		// Additional multithreading interface.
		//

		virtual hkpStepResult stepBeginSt( hkJobQueue* queue, hkReal physicsDeltaTime ){ return HK_STEP_RESULT_SUCCESS; }

		virtual hkpStepResult finishMtStep( hkJobQueue* queue, hkJobThreadPool* threadPool ){return HK_STEP_RESULT_SUCCESS; }

		virtual void getMultithreadConfig( hkpMultithreadConfig& config ){}

		virtual void setMultithreadConfig( const hkpMultithreadConfig& config, hkJobQueue* queue ){}

		//
		// Time accessors
		//

		inline hkReal getCurrentTime() { return m_currentTime; }
		inline hkReal getCurrentPsiTime() { return m_currentPsiTime; }
		inline hkReal getSimulateUntilTime() { return m_simulateUntilTime; }
		inline hkReal getPhysicsDeltaTime() { return m_physicsDeltaTime; }
		inline hkReal getFrameMarkerPsiSnap() { return m_frameMarkerPsiSnap; }

		//
		// Utility methods
		//

		enum FindContacts
		{
			FIND_CONTACTS_DEFAULT = 0,
			FIND_CONTACTS_EXTRA
		};
			/// Calculates contact information by performing discrete collision detection.
			/// It uses hkTransform from hkMotionState and ignores hkSweptTransform, therefore by default it will
			/// perform collision detection at m_timeOfNextPsi, unless the user changes hkTransforms of bodies.
			/// Note: in case of hkPredGsk, final position will be used always.
			/// This should be only performed for bodies that have just been added, activated, or moved in the world,
			/// and have a 'stationary' sweptTransform (with the same start & end positions).
		virtual void collideEntitiesDiscrete( hkpEntity** entities, int numEntities, hkpWorld* world, const hkStepInfo& stepInfo, FindContacts findExtraContacts  );

			// Update the broadphase for a set of entities
		static void collideEntitiesBroadPhaseDiscrete( hkpEntity** entities, int numEntities, hkpWorld* world );

			// Just calls processAgentsOfEntities
		void collideEntitiesNarrowPhaseDiscrete( hkpEntity** entities, int numEntities, const hkpProcessCollisionInput& input, FindContacts findExtraContacts );


		enum ResetCollisionInformation
		{
			RESET_TOI = 1,
			RESET_TIM = 2,
			RESET_AABB = 4,
			RESET_ALL = 7
		};

			// Invalidates TIMs and removes contact points from manifolds.
		virtual void resetCollisionInformationForEntities( hkpEntity** entities, int numEntities, hkpWorld* world, enum ResetCollisionInformation resetInfo = RESET_ALL );

			// Checks that there is no simulation-scope information relating to the entities.
			// Checks for TOI Events in hkpContinuousSimulation.
		virtual void assertThereIsNoCollisionInformationForEntities( hkpEntity** entities, int numEntities, hkpWorld* world ) {}

			// Removes simulation's collision information related to the agent. This is empty here, as no simulation-scope information
			// is stored in hkpSimulation. (This is overridden for hkpContinuousSimulation.)
		virtual void removeCollisionInformationForAgent( hkpAgentNnEntry* agent ) {}

			// Asserts when collision information related to the agent is found.
		virtual void assertThereIsNoCollisionInformationForAgent( hkpAgentNnEntry* agent ) {}

	protected:
		friend class hkpWorld;
		friend class hkpWorldOperationQueue;
		friend class hkpWorldMemoryUtil;
			// See hkpWorld::reintegrateAndRecollideEntities
		virtual void reintegrateAndRecollideEntities( hkpEntity** entityBatch, int numEntities, hkpWorld* world, int reintegrateRecollideMode );

	protected:

		//
		// Helper step methods
		//
			// Asserts if the time step changes dramatically from step to step
		void checkDeltaTimeIsOk( hkReal deltaTime );

			// Utility function
		hkpStepResult reCollideAfterStepFailure();

			// Collide discrete
		virtual void collideInternal( const hkStepInfo& stepInfoIn );

			// Integrate discrete
		hkpStepResult integrateInternal( const hkStepInfo& stepInfoIn );

			// Apply all actions of the world
		void applyActions();

			// Called from integrate()
		static HK_FORCE_INLINE void integrateIsland( hkpSimulationIsland* isle, const hkpWorldDynamicsStepInfo& stepInfo, hkpConstraintQueryIn& constraintQueryIn );

			// Called from collide()
		static HK_FORCE_INLINE void collideIslandNarrowPhaseDiscrete( hkpSimulationIsland* isle, const hkpProcessCollisionInput& input);


			// Helper called from advanceTime()
		hkReal snapSimulateTimeAndGetTimeToAdvanceTo();

		//
		// Agent processing sub methods
		//
			// Defines a callback function type to be called by processAgentsOfEntities (below)
		typedef void (hkpSimulation::*AgentEntryProcessFunction)(hkpAgentNnEntry*, const hkpProcessCollisionInput&, hkpProcessCollisionOutput&);

			// Iterates through agents, processes each only once, and calls processingFunction for it.
		void processAgentsOfEntities( hkpEntity** entities, int numEntities, const hkpProcessCollisionInput& input, AgentEntryProcessFunction processingFunction, FindContacts findExtraContacts );


			// The standard implementation of the callback function for hkpSimulation. Runs discrete collision detection for an agent entry.
		void processAgentCollideDiscrete(hkpAgentNnEntry* entry, const hkpProcessCollisionInput& processInput, hkpProcessCollisionOutput& processOutput);

			// Callback function which invalidates TIM's and contact points in the contact manfold.
		void processAgentResetCollisionInformation(hkpAgentNnEntry* entry, const hkpProcessCollisionInput& processInput, hkpProcessCollisionOutput& processOutput);

	public:

			/// Adds deltaTime's value to whatever time-related variables are stored in hkpSimulation.
		virtual void warpTime( hkReal deltaTime ) {}

	public:

			// Internal use only
		void setCurrentTime( hkTime time ) { m_currentTime = time; }

			// Internal use only
		void setCurrentPsiTime( hkTime time ) { m_currentPsiTime = time; }

			// Internal use only
		void setSimulateUntilTime ( hkTime time ) { m_simulateUntilTime = time; }

		hkpWorld* getWorld(){ return m_world; }

	public:

			// helper counter to check for determinism;
		hkUint32 m_determinismCheckFrameCounter;

	protected:

		class hkpWorld* m_world;

		enum LastProcessingStep
		{
			INTEGRATE,
			COLLIDE
		};

			// Used for debug checks that integrate and collide are called consecutively
        hkEnum<LastProcessingStep,hkUint8> m_lastProcessingStep;

			// Current time of the simulation. (Not the same as the time of the last simulation step.)
			// All TOIs up to this time have been processed.
			// Note: that the simulation's global time is not the absolute time from the beginning of the simulation. The time is
			// reset in the whole simulation every once in a while to avoid floating-point precision problems.
		hkTime m_currentTime;

			// Current "PSI" time of the simulation. This is always greater than or equal to m_currentTime.
			// PSIs are stepped "ahead" and then verified by solving TOIs up to that time.
		hkTime m_currentPsiTime;

			// Current delta time being simulated
		hkReal m_physicsDeltaTime;

			// Current asynchronous marker (-1 if not set)
		hkTime m_simulateUntilTime;

		hkReal m_frameMarkerPsiSnap;

        hkUint32 m_previousStepResult;	// should be hkEnum<hkpStepResult,hkUint8>, but serialization breaks

};

void HK_CALL hkLs_doSimpleCollisionResponse( hkpWorld* world, const hkpToiEvent& event, hkReal rotateNormal, hkArray<hkpEntity*>& toBeActivated );

#endif // HK_DYNAMICS2_SIMULATION_H

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
