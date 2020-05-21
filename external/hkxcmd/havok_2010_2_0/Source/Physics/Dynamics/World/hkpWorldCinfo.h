/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKDYNAMICS_WORLD_HKWORLDCINFO_XML_H
#define HKDYNAMICS_WORLD_HKWORLDCINFO_XML_H

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Physics/Dynamics/World/Memory/hkpWorldMemoryAvailableWatchDog.h>
#include <Physics/Collide/Filter/hkpConvexListFilter.h>
#include <Physics/Collide/Filter/List/hkpCollisionFilterList.h>

/// hkpWorldCinfo meta information
extern const class hkClass hkpWorldCinfoClass;

/// This structure contains all the information needed to construct a hkpWorld
/// object.
class hkpWorldCinfo : public hkReferencedObject
{
	//+version(11)
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Used to specify the speed/stiffness balance of the core solver.
			/// Havok's solver is an iterative solver and similar to a 'successive
			/// over relaxation' (SOR) method. That means that each constraint will not
			/// be solved perfectly, but rather solved like a very stiff spring,
			/// which gives the solver a 'soft' feeling. So the goal of the solver
			/// is to make the constraint to appear hard.
			/// There are 3 parameters to control the behavior of the solver:
			///  - m_solverIterations: defines how often the solver iterates over
			///    all constraints. The higher the number, the stiffer the constraints,
			///    however you need to spend more CPU. Values between 2 and 16 are
			///    reasonable, higher values could lead to instable behavior (in this
			///    case you can achieve the same results by decreasing the physics delta time).
			///  - m_tau: defines how much of the current constraint error is solved every
			///    solver iteration. High values (0.8 .. 1.2 ) make the constraints harder,
			///    however they get more unstable as well. Smaller numbers (0.3 .. 0.6) give
			///    much softer and smoother behavior, however you have to set the number of iterations
			///    pretty high to achieve a stiff behavior.
			///  - m_damping: defines how the current bodies velocity is taken into account.
			///    If you set damping to 0, you get an explicit solver, setting it to m_tau
			///    makes the solver semi-implicit. A good choice is to set it to tau.
			///
			/// This enum list allows to define some reasonable defaults: NITERS refers
			/// to the number of iterations, SOFT/MEDIUM/HARD refers to the value of tau and
			/// damp.
			/// \note There is a new experimental parameter: m_solverNumMicroSteps. This allows for
			///        further increasing (multiply) the m_solverIterations without adding extra instability
		enum SolverType
		{
				///
			SOLVER_TYPE_INVALID,
				/// 'Softest' solver type.
			SOLVER_TYPE_2ITERS_SOFT,
				/// .
			SOLVER_TYPE_2ITERS_MEDIUM,
				/// .
			SOLVER_TYPE_2ITERS_HARD,
				/// .
			SOLVER_TYPE_4ITERS_SOFT,
				/// .
			SOLVER_TYPE_4ITERS_MEDIUM,
				/// .
			SOLVER_TYPE_4ITERS_HARD,
				/// .
			SOLVER_TYPE_8ITERS_SOFT,
				/// .
			SOLVER_TYPE_8ITERS_MEDIUM,
				/// 'Hardest' solver type.
			SOLVER_TYPE_8ITERS_HARD,
				///
			SOLVER_TYPE_MAX_ID
		};

			/// Specifies simulation with discrete or continuous collision detection.
			// Note: all simulation types with enumeration values >= SIMULATION_TYPE_CONTINUOUS set m_wantContinuousAgents = true in hkpWorld's constructor.
		enum SimulationType
		{
				///
			SIMULATION_TYPE_INVALID,

				/// No continuous simulation is performed
			SIMULATION_TYPE_DISCRETE,

				/// Use this simulation if you want any continuous simulation.
				/// Depending on the hkpEntity->getQualityType(), collisions
				/// are not only performed at 'normal' physical timesteps (called PSI), but
				/// at any time when two objects collide (TOI)
			SIMULATION_TYPE_CONTINUOUS,

				/// Multithreaded continuous simulation.
				/// You must have read the multi threading user guide.
				///
				/// To use this you should call hkpWorld::stepMultithreaded(), see
				/// the hkDefaultPhysicsDemo::stepDemo for an example.
				/// Notes:
				///   - The internal overhead for multi threaded is small and you can expect
				///     good speedups, except:
				///   - solving multiple TOI events can not be done on different threads,
				///     so TOI are solved on a single thread. However the collision detection
				///     for each TOI event can be solver multithreaded (see m_processToisMultithreaded)
			SIMULATION_TYPE_MULTITHREADED,
		};

			/// Specifies how aggressively the engine accepts new contact points
		enum ContactPointGeneration
		{
				/// Try to gather as many contact points as possible. This
				/// gives you the highest quality at the cost of some (up to 25%)
				/// extra CPU. You should use this setting if you want to stack
				/// very small objects in your game
			CONTACT_POINT_ACCEPT_ALWAYS,

				/// Accept good contact points immediately and try to
				/// to reject the rest. This is a compromise
			CONTACT_POINT_REJECT_DUBIOUS,

				/// Uses some optimistic algorithms to speed up contact point generation.
				/// This can seriously increase the performance of the engine.
				/// Note: Stacking small objects becomes very difficult with this option enabled
			CONTACT_POINT_REJECT_MANY,
		};

			/// Tell the system what should happen if an objects leaves the
			/// extents of the broadphase.
			/// If you want to do your own implementation,
			/// just derive from hkpBroadPhaseBorder, implement your own
			/// maxPositionExceededCallback and call
			/// hkpWorld::setBroadPhaseBorder( new myBroadPhaseBorder() );
		enum BroadPhaseBorderBehaviour
		{
				/// Cause an assert and set the motion type to be fixed (default).
			BROADPHASE_BORDER_ASSERT,

				/// Set the motion type to be fixed and raise a warning.
			BROADPHASE_BORDER_FIX_ENTITY,

				/// Remove the entity from the hkpWorld and raise a warning.
			BROADPHASE_BORDER_REMOVE_ENTITY,

				/// Do not do anything, just continue to work.
				/// If many (>20) objects leave the broadphase,
				/// serious memory and CPU can be wasted.
			BROADPHASE_BORDER_DO_NOTHING,
		};

			/// Default constructor
		hkpWorldCinfo();

		//
		// Methods
		//

			/// Set the parameters of the world's solver. Each solver type provides a different
			/// speed/stiffness balance for the simulation. The default solver type is
			/// SOLVER_TYPE_4ITERS_MEDIUM, which is fast and moderately soft.
		void setupSolverInfo(enum SolverType st);

			/// Sets the broadphase size to be a cube centered on the origin of side
			/// sideLength. See also m_broadPhaseWorldSize.
		void setBroadPhaseWorldSize(hkReal size);

		//
		// Members
		//
	public:

		//
		// Basic setup
		//

			/// The gravity for the world. The default is (0, -9.8, 0).
		hkVector4 m_gravity; //+default(0,-9.8f,0)

			/// This variable is used to guess at the maximum number of overlaps you will get
			/// when you add or remove an entity or phantom. It determines the size of the local
			/// array used to query the broadphase. The local array uses the hkMemory stack,
			/// The default value is 1024, which requires a local array stack area of 8K.
		hkInt32 m_broadPhaseQuerySize; //+default(1024)

			/// The velocity which defines when two objects are assumed to be
			/// in resting contact. If the approaching velocity of the two objects
			/// at a point of contact is higher than this value, a special more accurate collision
			/// restitution algorithm is called. By default this value is 1.
		hkReal m_contactRestingVelocity;

			/// Tell the system what should happen if an objects leaves the
			/// extents of the broadphase. Read BroadPhaseBorderBehaviour for more details.
			/// This value defaults to BROADPHASE_BORDER_ASSERT
		hkEnum<BroadPhaseBorderBehaviour,hkInt8> m_broadPhaseBorderBehaviour;

			/// Tell the system to sort broadphase border callbacks.
			/// In multithreaded simulation the border callbacks may be triggered in random
			/// order. Enabling this flag postpones the callbacks till the end of the simulation
			/// frame, then orders the callbacks deterministically and triggers them.
			/// This may be necessary for deterministic behavior. However also this also prevents
			/// you from getting immediate callbacks, which may be necessary e.g., when using
			/// broadphase shifting.
		hkBool m_mtPostponeAndSortBroadPhaseBorderCallbacks; //+default(false)

			/// Defines the area dealt with by the collision detection broadphase. The size
			/// specified is a cuboid in world units - this cuboid defines the broadphase
			/// limits. There is a significant performance cost for objects that leave this
			/// area. The default size is a cube of side 1000, centred at the origin.
		class hkAabb m_broadPhaseWorldAabb;

			/// Enables/disables use of KdTrees. KdTrees speed up raycasting and linear casting
			/// queries performed on the world. When this is disabled the hkpWorld uses the standard broadphase.
		hkBool m_useKdTree; //+default(false)

			/// Using multiple trees is deprecated in 2010-2, see the migration docs.
			/// Enables use of Multiple Trees(KdTree for active object and AABB Tree for inactive object).
			/// Rebuild option is set in the following m_treeUpdateType
		hkBool m_useMultipleTree;	//+default(false)

		enum TreeUpdateType
		{
			REBUILD_ACTIVE,
			REBUILD_ALL,
		};

			/// This is an option for multiple trees and only works when m_useMultipleTree is enabled.
			/// When REBUILD_ACTIVE is set, it only rebuild active objects and for inactive objects, incremental update is performed.
			/// When REBUILD_ALL is set, it only uses KdTree and rebuilds everything.
		hkEnum<TreeUpdateType, hkInt8> m_treeUpdateType; 

			/// Enables/disables automatic updating of the world's KdTree after the world step.
			/// The KdTree must be in sync with the world state in order to avoid missed raycasts.
			/// Thus this should be set to false if you intend to add, move, or remove objects
			/// after the world has been stepped but before starting raycasts.
			/// If this is the case, you must call hkpWorld::updateKdTree() manually;
		hkBool m_autoUpdateKdTree; //+default(true)

			/// The collision tolerance. This is used to create and keep contact points even for
			/// non penetrating objects. This dramatically increases the stability of the
			/// system. The default collision tolerance is 0.1f.
			/// It is safe to decrease this value slightly when continuous simulation is enabled. This allows
			/// for more precise creation of contact points and better quality at the cost of generating
			/// more TOI events.
		hkReal m_collisionTolerance; //+default(.1f)

			/// The collision filter used by the hkpWorld. By default this is HK_NULL.
			/// If left as HK_NULL, a hkpNullCollisionFilter is set as the default filter in the
			/// world. This disables no collisions.
		hkRefPtr<class hkpCollisionFilter> m_collisionFilter;

			/// The filter used to decide how convex list shape collisions should be treated.
			/// By default this pointer is set to HK_NULL, in which case the default convex list filter
			/// will be created when the hkpWorld is constructed.
			/// It is not possible to change this collision filter after the world has been created.
		hkRefPtr<class hkpConvexListFilter> m_convexListFilter;

			/// The max linear velocity you expect the continuous physics to work correctly.
			/// This value is used to set internal tolerances. It defaults to 200 [meters per second]
			/// Note: This value does not limit the maximum linear velocities of rigid bodies.
			/// You have to set hkpRigidBody::m_maxLinearVelocity for each body yourself.
		hkReal m_expectedMaxLinearVelocity; //+default(200)

			/// For continuous physics this is the size of the TOI queue as used by the collision detector.
			///
			/// The collision detector exports TOIs into the hkMultithreadedSimulation::m_toiEvents array, which
			/// will have the initial size of m_sizeOfToiEventQueue. If more TOIs are generated, they will simply be
			/// dropped.
		int m_sizeOfToiEventQueue; //+default(250)

			/// The min delta time you expect between two PSI.
			/// This value is only used to set internal tolerances. It defaults to 1/30.0f second
			/// Note: This value does not limit the delta time.
		hkReal m_expectedMinPsiDeltaTime; //+default(1.0f/30.0f)


			/// The memory "watchdog" used by the world. By default this is HK_NULL, i.e., the memory
			/// for the world is not checked. If this is set, hkpWorldMemoryWatchDog::watchHeapMemory will be
			/// called every step, which gives the hkpWorldMemoryWatchDog a chance to prevent the memory
			/// exceeding a desired threshold.
		hkRefPtr<class hkWorldMemoryAvailableWatchDog> m_memoryWatchDog;


		//
		// Optimizations
		//

			/// NOTE: There is currently a known issue HVK-2553 associated with markers which
			/// can cause a possible crash during simulation. You should leave this value at 0 in the meantime.
			/// This variable is used to subdivide the x-axis in the 3-axisSweepAndPrune space
			/// into regular intervals. Setting this value slows down the normal operations by a
			/// very small amount, but can speed up asynchronous broadphase queries by up to 10
			/// times. Values have to be of power of 2. A typical good value would be 64.
			/// The default is 0, i.e., markers are not enabled by default.
		hkInt32 m_broadPhaseNumMarkers;


			/// Specifies how aggressively the engine accepts new contact points
			/// Defaults to CONTACT_POINT_REJECT_MANY
		hkEnum<ContactPointGeneration, hkInt8> m_contactPointGeneration;

			/// On PlayStation(R)3 contact point confirmed callbacks can be omitted when bodies have zero restitution.
			/// The default false value ensures all callbacks are triggered properly.
		hkBool m_allowToSkipConfirmedCallbacks; //+default(false)

			/// Use an hkpHybridBroadPhase to accelerate collision queries and providing additional collisions related facilities (see hkpHybridBroadPhase).
			/// NOTE: This will increase memory consumption by about 64 bytes per objects as well as increase broadphase CPU usage by about 10%.
		hkBool	m_useHybridBroadphase; //+default(false)

		//
		// Solver settings
		//

			/// Parameter of the world's constraint solver. Default sets of values can be
			/// automatically set using the setupSolverInfo() method by passing the required
			/// hkSolverType. See SolverType or documentation for details on customizing these values.
		hkReal m_solverTau;

			/// Parameter of the world's constraint solver. Default sets of values can be
			/// automatically set using the setupSolverInfo() method by passing the required
			/// hkSolverType. See SolverType or documentation for details on customizing these values.
			/// The default is 0.6f
		hkReal m_solverDamp; //+default(.6f)

			/// Parameter of the world's constraint solver. Default sets of values can be
			/// automatically set using the setupSolverInfo() method by passing the required
			/// hkSolverType. See SolverType or documentation for details on customizing these values.
			/// The default is 4.
		hkInt32 m_solverIterations; //+default(4)

			/// This is the number of Gauss Seidel steps the solver performs during each Havok step.
			/// Basically this allows for virtually increasing the number of solver steps without
			/// adding extra instability. However the microsteps are not as powerful as the main
			/// solver steps.
			/// So by default this value should be 1. Only if you wish to increase the number
			/// of solver steps to be higher than 8 and you experience stability problems,
			/// you can increase this value and leave the number of solver steps to 4 or 8.
		hkInt32 m_solverMicrosteps; //+default(1)

			/// This is the maximum constraint positional error allowed before
			/// hkpConstraintListener::constraintViolatedCallback() gets triggered.
			/// The default is disable.
			/// Check out ConstraintProjectionDemo
		hkReal m_maxConstraintViolation; //+default(HK_REAL_MAX)

			/// When using multithreading, this forces the solver to process the constraints in the same
			/// order as it is done in single-treaded mode. As the result, a big part of solving is performed
			/// by a single processing unit. This will cause a noticable/significant slowdown when
			/// processing a single large island.
			///
			/// Leaving this option off (default) may cause some random jitter.
		hkBool m_forceCoherentConstraintOrderingInSolver; //+default(false)

		//
		// Internal algorithm settings
		//

			/// This value determines when collision normals will be welded. See the user guide for details.
			/// By default this is 30 degrees. This value is only used for two sided welding, if you build
			/// one sided welding information this value is not used.
		hkReal m_snapCollisionToConvexEdgeThreshold; //+default(.524f)

			/// This value determines when collision normals will be welded. See the user guide for details.
			/// This value is only used for two sided welding, if you build one sided welding information this value is not used.
			/// By default this is 40 degrees.
		hkReal m_snapCollisionToConcaveEdgeThreshold; //+default(0.698f)

			/// This flag determines whether TOIs will be rejected by welding. See the user guide for details.
			/// By default this is set to false, meaning that TOIs will always be accepted, even if
			/// they will cause a welding artifact. If you set this to true, TOIs will never produce welding artifacts,
			/// however it means that objects will be able to penetrate landscapes more easily even in continuous mode.
			/// Note that this is not an issue if you have build one sided welding information for your landscapes; in
			/// this case TOIs are never rejected by setting this variable.
		hkBool m_enableToiWeldRejection; //+default(false)

			/// The recommended welding solution of versions 4.5 and later involves building welding info and storing
			/// additional data with the mesh. However, if you have not built this info, or wish to weld non-triangle
			/// based meshes, you may wish to enable the old, deprecated welding. To do so, set this flag to true.
		hkBool m_enableDeprecatedWelding; //+default(false)

			/// (Collision config) The early out distance default for Linear casts. Defaults to
			/// 0.01 units, usually about a 10th of collision tolerance so change this if you
			/// change the collision tolerance value.
		hkReal m_iterativeLinearCastEarlyOutDistance; //+default(.01f)

			/// (Collision config) The max number of iterations the Linear casts will make.
			/// Defaults to 20.
		hkInt32 m_iterativeLinearCastMaxIterations; //+default(20)

			/// Internal deactivation flag, leave at default.
		hkUint8 m_deactivationNumInactiveFramesSelectFlag0; //+default(0)

			/// Internal deactivation flag, leave at default.
		hkUint8 m_deactivationNumInactiveFramesSelectFlag1; //+default(0)

			/// Internal deactivation counter, leave at default.
		hkUint8 m_deactivationIntegrateCounter; //+default(0)



			/// This says whether bodies are to be activated when they get moved around with any
			/// of the setPositon/setRotation/setTransform calls. By default this is true.
		hkBool m_shouldActivateOnRigidBodyTransformChange; //+default(true)

			/// This is the base distance used for the deactivation check.
			/// The basic idea is that we deactivate an object if its position stays
			/// within a sphere (radius=m_deactivationReferenceDistance) for about 20 frames (high frequency deactivation).
			/// Or within a sphere (radius = 4*m_deactivationReferenceDistance) for about 80 frames (low frequency deactivation).
			/// If the object breaches this deactivation sphere, the center of the sphere is set to the current position
			/// of the object. Also the same rules apply to angular movement.
			/// Some details:
			///  - Every object is only checked every 4th frame for high frequency deactivation
			///  - Every object is only checked every 16th frame for low frequency deactivation
			///  - Just before deactivating we check the velocity again. If the velocity has significantly increased
			///    (is significantly (0.1f) bigger than 2 times the maximum velocity measured over the last 20/80 frames)
			///    we do not deactivate this object.
			///
			/// You can set some of those values in the hkpSolverInfo (hkpWorld::getSolverInfo()) for each deactivation class after the world is constructed.
		hkReal m_deactivationReferenceDistance; //+default(0.02f)

			/// Advanced Parameter:
			/// Rotates the normal of the contact point for TOI contact point resolution.
			/// The range of this parameter should be between 0 and 1 (0.2f by default).
			/// Setting this parameter bigger than 0 simulates an extra surface roughness,
			/// thereby allowing sliding objects to get an extra separating velocity and
			/// therefore reducing the frequency of subsequent TOI.
			/// In short: Setting this parameter to >0 adds extra bounce and can greatly
			/// reduce CPU.
		hkReal m_toiCollisionResponseRotateNormal; //+default(0.2f)

		
		//	// Advanced Parameter:
		//	// This enables breached force limit events to be triggered from the TOI solver. 
		//	// This is NOT to enable the TOI's triggering limit-breached events at all. 
		//	// This is enable 'secondary' events to be triggered during TOI's localized solving.
		//	// Note: that when this enabled you're very likely to receive multiple events
		//	// relating to a single piece being broken off. This has to be handled and filtered 
		//	// appropriately in your listener logic.
		//hkBool m_enableForceLimitBreachedSecondaryEventsFromToiSolver; //+default(false)

			/// The maximum number of midphase agent sectors that will be processed by each task
			/// split-off from an hkpAgentSectorJob_Midphase (or hkpAgentSectorJob if the split
			/// collision pipeline is not being used).
			/// Note that midphase sectors hold twice as many entries as narrowphase sectors.
		int m_maxSectorsPerMidphaseCollideTask; //+default(2)

			/// The maximum number of midphase agent sectors that will be processed by each task
			/// split-off from an hkpAgentSectorJob_Narrowphase (or hkpAgentSectorJob if the split
			/// collision pipeline is not being used).
			/// Note that narrowphase sectors hold half as many entries as midphase sectors.
		int m_maxSectorsPerNarrowphaseCollideTask; //+default(4)

			/// If set to true (default), the continuous TOI solver will run collision detection
			/// multithreaded (used in hkMultithreadedSimulation only). If set to false, the
			/// old-style single threaded variant will be used.
			/// TOIs in general have to be solved sequentially and therefore cannot be solved
			/// multithreaded. However the collision detection part of each TOI can be parallelized
			/// and solved by different threads. As a result you should see a TOI solving speedup
			/// if a fast complicated object (e.g., listshape) hits a pile of other complicated
			/// objects. However you will not get any speedup (but rather a slow-down) if a simple
			/// object (e.g., ragdoll bone) hits another single object (e.g., landscape). You should
			/// try to experiment with that parameter to see which value suits your scenario best.
			/// \note If you enable this flag the Havok worker threads will not be released until
			/// the stepDeltaTime() call is finished, i.e., you cannot use them for different
			/// purposes even if they are idle.
		hkBool m_processToisMultithreaded;	//+default(true)

			/// The maximum number of hkpAgentNnEntry elements that will be processed by each task
			/// split-off from an hkAgentNnEntryJob_Midphase (or hkpAgentSectorJob if the split
			/// collision is not being used for TOIs).
			/// This value is only used when m_processToisMultithreaded is enabled.
			/// Note that setting this value appropriately for a given simulation case may
			/// significantly improve the CPU performance.
			/// This now defaults to 8 on non-PlayStation(R)3 platforms and 4 on PlayStation(R)3 (see hkpWorldCinfo constructor).
			/// This value should be set lower when we expect collisions between complex shapes
			/// (e.g., between a list shape and a complex landscape object). However, when only
			/// simple objects are to be collided, setting this value higher will provide better
			/// performance.
			/// Generally, we need to balance the gain from offloading computations to other
			/// threads against the costs of creating and managing MT jobs. If you are experiencing
			/// inadequate multi-threaded performance, experiment with some progressively higher
			/// values to see if performance is improved.
		int m_maxEntriesPerToiMidphaseCollideTask; //+default(-1)

			/// The maximum number of hkpAgentNnEntry elements that will be processed by each task
			/// split-off from an hkAgentNnEntryJob_Midphase (or hkpAgentSectorJob if the split
			/// collision is not being used for TOIs).
			/// This now defaults to 12 on non-PlayStation(R)3 platforms and 6 on PlayStation(R)3
			/// (see hkpWorldCinfo constructor).
		int m_maxEntriesPerToiNarrowphaseCollideTask; //+default(-1)

			/// The maximum number of post-TOI collision pairs processed single-threadedly. The default value is 0 which means
			/// that all collision jobs are processed in a different thread (SPU).
		int m_maxNumToiCollisionPairsSinglethreaded; //+default(0)

			/// When checking for the allowed penetration depth, Havok's collision detection will initially allow for a fixed
			/// percentage of that depth during the first TOI. The remaining penetration depth will then be subdivided into a
			/// customizable number of TOIs. This allows for tweaking 'competing' collision agents to be resolved in a way the
			/// user prefers, e.g., if the player is standing beneath a downward bound elevator platform he should preferably
			/// tunnel through the platform than through the floor (and by this possibly getting pushed out of the world).
			/// This value defines the number of TOIs to be triggered for collisions between fast-moving, non critical objects
			/// and the landscape (COLLISION_QUALITY_SIMPLIFIED_TOI) and has to be > 1.0.
			/// The default is 3.0.
		hkReal m_numToisTillAllowedPenetrationSimplifiedToi; //+default(3.0f)

			/// This value defines the number of TOIs to be triggered for collisions between two moving objects (COLLISION_QUALITY_TOI)
			/// and has to be > 1.0.
			/// For details see m_numToisTillAllowedPenetrationSimplifiedToi.
			/// The default is 3.0.
		hkReal m_numToisTillAllowedPenetrationToi; //+default(3.0f)

			/// This value defines the number of TOIs to be triggered for collisions between a (non-critical) moving object and
			/// the landscape (COLLISION_QUALITY_TOI_HIGHER) and has to be > 1.0.
			/// For details see m_numToisTillAllowedPenetrationSimplifiedToi.
			/// The default is 4.0.
		hkReal m_numToisTillAllowedPenetrationToiHigher; //+default(4.0f)

			/// This value defines the number of TOIs to be triggered for collisions between a (critical) moving object and the
			/// landscape (COLLISION_QUALITY_TOI_FORCED) and has to be > 1.0.
			/// For details see m_numToisTillAllowedPenetrationSimplifiedToi.
			/// The default is 20.0.
		hkReal m_numToisTillAllowedPenetrationToiForced; //+default(20.0f)

		//
		// Debugging flags
		//

			/// Set to false if you want deactivation to be disabled. See the frequency settings
			/// for deactivation too.
		hkBool m_enableDeactivation; //+default(true)

			/// Simulation type. By default this is SIMULATION_TYPE_CONTINUOUS. See hkpWorld::SimulationType for more details.
		hkEnum<SimulationType, hkInt8> m_simulationType;

			/// Sets whether the world is automatically segmented into separate islands each step.
			/// This should always be enabled. The default is enabled.
		hkBool m_enableSimulationIslands; //+default(true)

			/// This parameter allows to keep the islands size reasonable large to avoid performance overhead in multi threaded simulations.
			///
			/// Having lots of very small islands can cause some unnecessary overhead in the multithreaded simulation.
			/// (Its like rendering single triangles using a modern graphics card).
			/// Therefore we have the concept of sparse islands.
			/// These islands could in theory be split into smaller islands,
			/// however its not done and those islands are flagged with hkpSimulationIsland::m_isSparse = true.
			/// But whenever an island is deactivated, it will be checked for splits and only inactive sub-islands will be deactivated.
			///
			/// This parameter only works in multithreading mode, otherwise it will not be used and will be set to 0.
			/// 
			/// Technically, this parameter specifies the maximum size of sparse islands.
			/// (Size, being the maximum of the number of entities and the number of constraints; and sparse, meaning having hkpEntities
			/// not fully interconnected.)
		hkUint32 m_minDesiredIslandSize; //+default(64)

			/// Forces actions to be processed in a single thread. If you have a multi threaded simulation and
			/// you are using actions which influence objects other then the action's entities (listed in hkpAction::getEntities(), this flag must
			/// be set to true (e.g., hkpVehicleInstance applies forces on objects laying on the ground and uses a phantom).
		hkBool m_processActionsInSingleThread; //+default(true)

			/// Concerns multithreaded integration of islands where there are no constraints; this concerns PlayStation(R)3 platform mainly.
			/// When this flag is enabled, body integration is performed in a separate job that can be run on PlayStation(R)3's SPU to save PPU time.
			/// This is not used by default on other platforms.
			///
			/// Note that as soon as there are any constraints added to the island, this flag is not used. You might consider setting a low or zero value for
			/// hkpMultithreadConfig::m_maxNumConstraintsSolvedSingleThreaded too. However, when constraints are present, multithreaded solving on SPU requires
			/// more than 10 separate jobs. Alternatively you can keep the islands size small with m_minDesiredIslandSize, to ensure that no big island
			/// takes too much of PPU time.
		hkBool m_allowIntegrationOfIslandsWithoutConstraintsInASeparateJob; //+default(false)

			/// Snaps the frameTimeMarker to the currentPsiTime when those times are close together.
			/// By default they must be less than .0001 seconds apart to be snapped together.
			/// This synchronization of frameTime and physicsTime is performed by default, and is useful to control numerical drift when e.g.
			/// running simulation at half the frequency of the frame rate. A side effect is such that the total time of simulation may
			/// slightly vary from the sum of frameDeltaTime parameters passed to world->setFrameTimeMarker(frameDeltaTime).
			/// If you use your hkpWorld for testing and precise measurements you may prefer to disable synchronization.
			/// To do this set this variable to 0.
		hkReal m_frameMarkerPsiSnap; //+default(.0001f)

			/// Should collisionAdded/RemovedCallbacks be fired to hkpContactListeners.
			/// For finer grained use of this facility, see the hkpCollisionCallbackUtil.
		hkBool m_fireCollisionCallbacks; //+default(false)

		virtual ~hkpWorldCinfo() {}
		explicit hkpWorldCinfo( hkFinishLoadedObjectFlag flag );

};

#endif // HKDYNAMICS_WORLD_HKWORLDCINFO_XML_H

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
