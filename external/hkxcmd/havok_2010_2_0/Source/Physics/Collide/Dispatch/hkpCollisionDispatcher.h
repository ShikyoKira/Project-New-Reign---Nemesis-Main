/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_COLLIDE2_COLLISION_DISPATCHER_H
#define HK_COLLIDE2_COLLISION_DISPATCHER_H

#include <Physics/Collide/Agent/Collidable/hkpCdBody.h>
#include <Physics/Collide/Shape/hkpShape.h>
#include <Physics/Collide/Agent3/hkpAgent3.h>
#include <Physics/Collide/Agent/Collidable/hkpCollidableQualityType.h>
#include <Physics/Collide/Agent/hkpCollisionQualityInfo.h>
#include <Physics/Collide/Agent/hkpCollisionInput.h>


class hkpCollidable;
class hkpCollisionAgent;
class hkpContactMgr;
struct hkpCollisionInput;
class hkpContactMgrFactory;
class hkpCdPointCollector;



class hkpCdPointCollector;
class hkpCdBodyPairCollector;
struct hkpLinearCastCollisionInput;

enum hkpCollisionDispatcherAgentType
{
	HK_AGENT_TYPE_NULL = 0,
	HK_AGENT_TYPE_BRIDGE = 1
};


	/// This class is used to find the correct function which performs collision detection between pairs of shapes.
	/// It's based on the following rules:
	///  - Each shapes has a type
	///  - For each pair of types, there must be a function registered to handle this pair of types
	///
	/// However implementing all combinations would be quite complex. Therefore we allow for:
	///  - shapes to actually subclass other more basic shapes (e.g., a box is a convex shape)
	///    and then reuse a common base function
	///  - functions, which work on a pair of shapes, where only one shape type is used (e.g., hkpTransformShape, hkpBvTreeShape)
	///    and the other ignored (HK_SHAPE_ALL).
	///    in this case we have to register a function with a pair of types ( HK_SHAPE_MY_TYPE, HK_SHAPE_ALL )
	/// 
	/// To implement this behavior, the user has to
	///  - first declare the shape inheritance hierarchy by calling registerAlternateShapeType for each inheritance link
	///  - register all functions which handle collisions between two shape types.
	/// 
	/// Note: You still cannot call registerAlternateShapeType after you registered your first agent
	///
	/// Also this class can dispatch hkpContactMgrFactory s. This dispatch is quite simple as there is no inheritance.
	/// Also this class combines two hkCollidableQualityTypes into a hkCdInteractionQualtityType
	///
	/// In addition to that, this class also dispatches a pair of hkpCollidableQualityType into a hkpCollisionQualityInfo structure
class hkpCollisionDispatcher : public hkReferencedObject
{
	public:
		enum {
			HK_MAX_RESPONSE_TYPE = 8,
			HK_MAX_SHAPE_TYPE    = 32,
			HK_MAX_AGENT2_TYPES  = 64,
			HK_MAX_AGENT3_TYPES = 18,
			HK_MAX_COLLISION_QUALITIES = 8
		};
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

			/// A function to create a collision agent
		typedef hkpCollisionAgent* (HK_CALL *CreateFunc)(const hkpCdBody& collA, const hkpCdBody& collB, const hkpCollisionInput& env, hkpContactMgr* mgr);

			/// A function to check whether two objects are penetrating
		typedef void (HK_CALL *GetPenetrationsFunc)     (const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			/// A function to find the closest points
		typedef void (HK_CALL *GetClosestPointsFunc)   (const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdPointCollector& output );

			/// A function to do linear casts
		typedef void (HK_CALL *LinearCastFunc)         (const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpLinearCastCollisionInput& input, hkpCdPointCollector& castCollector, hkpCdPointCollector* startCollector );

			/// For backward compatibility
		typedef CreateFunc CollisionAgentCreationFunction;

		typedef hkChar CollisionQualityIndex;

			/// A helper struct, which is used as an input to registerCollisionAgent
		struct AgentFuncs
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, AgentFuncs );

				/// The agent creation functions
			CreateFunc           m_createFunc;

				/// A static penetration function
			GetPenetrationsFunc  m_getPenetrationsFunc;

				/// A static get closest distance functions
			GetClosestPointsFunc m_getClosestPointFunc;

				/// A static linear cast function
			LinearCastFunc       m_linearCastFunc;

				/// A flag telling the system, whether a symmetric agent is involved.
				/// If the collision detector sees the flag to be set to true, it tries
				/// to swap objectA with objectB
			hkBool				 m_isFlipped;

				/// A flag indicating whether the object can handle continuous collision
			hkBool				 m_isPredictive;

			AgentFuncs() : m_isFlipped(false), m_isPredictive(false) {}
		};

			/// A helper struct, which is used as an input to registerCollisionAgent
			/// for streamed agents. Read hkAgent3 for details
		struct Agent3Funcs
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, Agent3Funcs );
			Agent3Funcs() : m_isPredictive(false), m_ignoreSymmetricVersion(false), m_reusePreviousEntry(false)
			{
				m_updateFilterFunc	= HK_NULL;
				m_calcStatisticsFunc= HK_NULL;
				m_invalidateTimFunc = HK_NULL;
				m_warpTimeFunc		= HK_NULL;
				m_sepNormalFunc		= HK_NULL;
				m_processMidphaseFunc = HK_NULL;
				m_processPostMidphaseFunc = HK_NULL;
			}

			HK_ALIGN16(hkAgent3::CreateFunc      m_createFunc);
				// Called when the agent is destoyed
			hkAgent3::DestroyFunc     m_destroyFunc;
				// Called when the agent has all points removed.
				// Info: This is only used when you have Tim's -- and there fore m_sepNormalFunc assigned.
			hkAgent3::CleanupFunc     m_cleanupFunc;
				// Point remove/commit/zombie functions used for welding
			hkAgent3::RemovePointFunc m_removePointFunc;
			hkAgent3::CommitPotentialFunc m_commitPotentialFunc;
			hkAgent3::CreateZombieFunc  m_createZombieFunc;
				// Should be implemented for all agents
			hkAgent3::UpdateFilterFunc  m_updateFilterFunc;
			hkAgent3::CalcStatisticsFunc  m_calcStatisticsFunc;
			hkAgent3::InvalidateTimFunc m_invalidateTimFunc;
			hkAgent3::WarpTimeFunc      m_warpTimeFunc;
				// Only needed if the agent uses Tim's
			hkAgent3::SepNormalFunc     m_sepNormalFunc;
			hkAgent3::ProcessFunc       m_processFunc;
			hkAgent3::ProcessMidphaseFunc m_processMidphaseFunc;
			hkAgent3::ProcessPostMidphaseFunc m_processPostMidphaseFunc;
			hkBool						m_isPredictive;
			hkBool						m_ignoreSymmetricVersion;
			hkBool						m_reusePreviousEntry;
		};

		struct InitCollisionQualityInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, InitCollisionQualityInfo );

			hkReal m_gravityLength;
			hkReal m_collisionTolerance;
			hkReal m_minDeltaTime;
			hkReal m_maxLinearVelocity;
			hkReal m_numToisTillAllowedPenetrationSimplifiedToi;
			hkReal m_numToisTillAllowedPenetrationToi;
			hkReal m_numToisTillAllowedPenetrationToiHigher;
			hkReal m_numToisTillAllowedPenetrationToiForced;
			hkUint16 m_defaultConstraintPriority;
			hkUint16 m_toiConstraintPriority;
			hkUint16 m_toiHigherConstraintPriority;
			hkUint16 m_toiForcedConstraintPriority;
			hkBool m_wantContinuousCollisionDetection;
			hkBool m_enableNegativeManifoldTims;
			hkBool m_enableNegativeToleranceToCreateNon4dContacts;
			hkBool m_enableToiWeldRejection;
		};

		enum IsAgentPredictive
		{
			IS_NOT_PREDICTIVE,
			IS_PREDICTIVE
		};

	public:

			/// create the collision dispatcher
		hkpCollisionDispatcher( CreateFunc defaultCreationFunction, hkpContactMgrFactory* defaultContactMgrFactory );

			/// delete it
		~hkpCollisionDispatcher();


			//
			//	Shape Types
			//


			/// register a shape type and a possible alternative shape type, which
			/// is used if the first type does not give a valid answer
			/// Note: you should register all alternate shapes BEFORE you can register the collision
			/// agent creation functions (otherwise it gets very CPU intensive).
			///
			/// Background:
			///
			/// Since havok2.2, the inheritance of shapes is defined in the collision dispatcher,
			/// not in the shapes. That means before registering agents, all shape types and their
			/// alternate(=superClass) type needs to be registered.
			/// - Example: A triangle is a convex shape, so in order for the convex agent to
			///   pick up a triangle the following lines must precede the registration of the convex
			///   agent: dispatcher->registerAlternateShapeTypes( HK_SHAPE_TRIANGLE, HK_SHAPE_CONVEX );
		void registerAlternateShapeType( hkpShapeType primaryType, hkpShapeType alternateType );


			/// returns true, if type has a path to alternateType or type == alternateType
			/// Note: This function is really fast
		inline hkBool32 hasAlternateType( hkpShapeType type, hkpShapeType alternateType );
			

			//
			//	Agents
			//


			/// Register an agent for a given pair of shape types, use HK_SHAPE_ALL for one type if you want to register
			/// the other type with all agents.
			/// Later calls to this functions override existing values, that means later calls automatically have a higher priority.
			///  - Note: You should register all shape alternate Types before you call registerCollisionAgent()
			///  - Note: A call to this function is slow
			///  - Note: Agent using the streaming technology must be registered just after the non
			///    streaming version is registered
			/// 
			/// Build in checks
			///
			/// Example: Say you register a fast sphere-sphere agent first and than you register a slow convex-convex agent.
			/// As later registrations overrides earlier, the convex-convex agent will actually override the
			/// sphere-sphere one (given that there is a rule that a sphere is a convex object). Bad :-(
			///
			/// Unfortunately this wrong registration is usually difficult to find.
			///
			/// Solution:
			///  - Currently the program checks, if a more general agent overrides an more specialized agent and asserts.
			///    However in some cases the check does not work, so you can disable it by calling setEnableChecks(false).
			///  - Call debugPrintTable() to get a text file which shows you check the contents of the table by hand
		void registerCollisionAgent( AgentFuncs& f, hkpShapeType typeA, hkpShapeType typeB );

			/// Register an agent with the collision dispatcher. Does not register bridge agent.
			/// This means this agent will only be used
			///  - if static collision queries are used or
			///  - if agent is used by a phantom or
			///  - if the agent is owned by a transform agent
		void registerCollisionAgent2( AgentFuncs& f, hkpShapeType typeA, hkpShapeType typeB );


			/// disable checks if you want to force override existing entries when calling registerCollisionAgent.
			/// Don't forget to re-enabling the checks, once you have done your unchecked operations.
		void setEnableChecks( hkBool checkEnabled);

			/// Debug print of the current table entries
		void debugPrintTable();

			/// Disables debugging. This should be done at the end of registering all agents to save some memory.
			/// Note: Checking is only enabled in debug
		void disableDebugging();


			/// returns a new collision agent for a collidable pair
		HK_FORCE_INLINE hkpCollisionAgent* getNewCollisionAgent(	const hkpCdBody& a, const hkpCdBody& b, const hkpCollisionInput& env, hkpContactMgr* mgr) const;

			/// get the agent creation function registered for a given pair.
		HK_FORCE_INLINE CreateFunc getCollisionAgentCreationFunction( hkpShapeType typeA, hkpShapeType typeB, IsAgentPredictive predictive ) const;

			/// get the functions, which allows you to query two collision collidables for penetration cases
		HK_FORCE_INLINE GetPenetrationsFunc getGetPenetrationsFunc( hkpShapeType typeA, hkpShapeType typeB ) const;

			/// get the functions, which allows you to query two collision collidables for penetration cases
		HK_FORCE_INLINE GetClosestPointsFunc getGetClosestPointsFunc( hkpShapeType typeA, hkpShapeType typeB ) const;

			/// get the functions, which allows you to query two collision collidables for penetration cases
		HK_FORCE_INLINE LinearCastFunc getLinearCastFunc( hkpShapeType typeA, hkpShapeType typeB ) const;

		HK_FORCE_INLINE hkBool getIsFlipped( hkpShapeType typeA, hkpShapeType typeB ) const;


			/// Same as registerCollisionAgent, but for the hkAgent3 technology
			/// if reusePreviousEntry is set to true, than f is ignored and the f of previous call to registerAgent3 is used
		int registerAgent3( Agent3Funcs& f, hkpShapeType typeA, hkpShapeType typeB );

		HK_FORCE_INLINE hkAgent3::AgentType getAgent3Type( hkpShapeType typeA, hkpShapeType typeB, hkBool32 predicitve ) const;

		HK_FORCE_INLINE hkAgent3::CreateFunc				getAgent3CreateFunc  ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::DestroyFunc				getAgent3DestroyFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::CleanupFunc				getAgent3CleanupFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::RemovePointFunc			getAgent3RemovePointFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::CommitPotentialFunc		getAgent3CommitPotentialFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::CreateZombieFunc			getAgent3CreateZombieFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::UpdateFilterFunc			getAgent3UpdateFilterFunc ( hkAgent3::AgentType type ) const;
		HK_FORCE_INLINE hkAgent3::CalcStatisticsFunc		getAgent3CalcStatisticsFunc ( hkAgent3::AgentType type ) const;
		HK_FORCE_INLINE hkAgent3::InvalidateTimFunc			getAgent3InvalidateTimFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::WarpTimeFunc				getAgent3WarpTimeFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::ProcessFunc				getAgent3ProcessFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::ProcessMidphaseFunc		getAgent3ProcessMidphaseFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::ProcessPostMidphaseFunc	getAgent3ProcessPostMidphaseFunc ( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::SepNormalFunc				getAgent3SepNormalFunc( hkAgent3::AgentType type );
		HK_FORCE_INLINE hkAgent3::Symmetric					getAgent3Symmetric( hkAgent3::AgentType type );
			//
			//	Contact Mgrs
			//

			/// register a contact factory
		void registerContactMgrFactory( hkpContactMgrFactory*, int responseA, int responseB );


			/// Get a contact manager factory - used to create a contact manager.
		HK_FORCE_INLINE hkpContactMgrFactory* getContactMgrFactory(int responseA, int responseB) const;

			/// register a contact factory with all other types: Note overrides existing entries
		void registerContactMgrFactoryWithAll( hkpContactMgrFactory*, int responseA );


			//
			//	hkpCollisionQualityInfo
			//
		HK_FORCE_INLINE CollisionQualityIndex getCollisionQualityIndex( hkpCollidableQualityType a, hkpCollidableQualityType b);

		HK_FORCE_INLINE hkpCollisionQualityInfo* getCollisionQualityInfo( CollisionQualityIndex index );
		
		void initCollisionQualityInfo( InitCollisionQualityInfo& input );
		
			// hkReferencedObject interface implementation
		void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const;

			//
			//	Some simple convenience functions
			//

			/// Get the closest point between 2 shapes. Warning the hit.m_rootCollidable is not used (points to unused memory)
		void getClosestPoints( const hkpShape* shapeA, const hkTransform& transformA, const hkpShape* shapeB, const hkTransform& transformB, hkReal collisionTolerance, hkpCdPointCollector& collector );

			/// Get the penetration status of 2 shapes.
		void getPenetrations( const hkpShape* shapeA, const hkTransform& transformA, const hkpShape* shapeB, const hkTransform& transformB, hkReal collisionTolerance, hkpCdBodyPairCollector& collector );

	public:

		struct ShapeInheritance
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, ShapeInheritance );

			hkpShapeType m_primaryType;
			hkpShapeType m_alternateType;
		};

		struct AgentEntry
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, AgentEntry );

			hkpShapeType m_typeA;
			hkpShapeType m_typeB;
			hkBool m_checkEnabled;
			AgentFuncs  m_createFunc;
		};

		struct Agent3Entry
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, Agent3Entry );

			hkpShapeType m_typeA;
			hkpShapeType m_typeB;
			hkBool m_checkEnabled;
			Agent3Funcs  m_funcs;
		};

			// A helper struct, which is used as an input to registerFullAgent
		struct Agent3FuncsIntern: public Agent3Funcs
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, Agent3FuncsIntern );

			hkAgent3::Symmetric       m_symmetric;
		};

		struct DebugEntry
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, DebugEntry );

				/// Shape Type A
			char m_typeA;
				/// Shape Type B
			char m_typeB;
				
				/// The sum of all shape rules which have to be applied to get to this entry.
				/// E.g. we have two spheres (m_typeAB = HK_SHAPE_SPHERE)
				///  - if we register a hkSphereSphere Agent, no indirections are needed and the priority is 0
				///  - if we register a hkSphereConvex Agent, 1 indirections is needed and the priority is 1
				///  - if we register a hkConvexConvex Agent, 2 indirections are needed and the priority is 2
				///  - if we register a hkSphere-All Agent, 1 indirections is needed and the priority is 1
			char m_priority;
		};

		typedef DebugEntry	DebugTable[HK_MAX_SHAPE_TYPE][HK_MAX_SHAPE_TYPE];


	protected:
		void internalRegisterCollisionAgent( hkUchar agentTypesTable[HK_MAX_SHAPE_TYPE][HK_MAX_SHAPE_TYPE], int agentType, hkpShapeType typeA, hkpShapeType typeB, hkpShapeType origA, hkpShapeType origB, DebugTable *debugTable, int recursionDepth );

		void updateHasAlternateType( hkpShapeType primaryType, hkpShapeType alternateType, int depth );

		void resetCreationFunctions();


	public:

			/// Collision quality setting for a collision between a pair of objects.
			/// Values must correspond to hkpConstraintInstance::ConstraintPriority
		enum CollisionQualityLevel
		{
				/// Invalid.
			COLLISION_QUALITY_INVALID,

				/// Use this to get discrete stepping behavior or for fixed objects.
			COLLISION_QUALITY_PSI,

				/// Use this to avoid fast-moving debris object penetrating through the ground and other static objects.
				/// This only backsteps the debris objects.
			COLLISION_QUALITY_SIMPLIFIED_TOI,

				/// Application: Ensure non-penetration between pairs of non-fixed objects
				/// Use this if you want to prevent bullet through paper
			COLLISION_QUALITY_TOI,

				/// Same as above.
				/// Application: Ensure non-penetration between moving objects & fixed objects
				/// Tech. Info: This ensures constraints to be treated as 'higher-priority' in PSI solver
			COLLISION_QUALITY_TOI_HIGHER,

				/// Application: Ensure and monitor non-penetration between moving objects & fixed objects.
				///              You receive callbacks when penetration occurs.
				///
				/// Tech. Info: This ensures constraints to be treated as 'higher-priority' in PSI & TOI solvers
				///             plus 'forced' during TOI handling
			COLLISION_QUALITY_TOI_FORCED,

				/// This quality is the same as COLLISION_QUALITY_TOI_FORCED with some additional tolerances
				/// set to enable more accurate generation of contact points.
			COLLISION_QUALITY_CHARACTER,

				/// Only used internally for expanding contact manifold in hkpContinuousSimulation::updateManifold().
				/// Will try to collect as many contact points as possible.
			COLLISION_QUALITY_TMP_EXPAND_MANIFOLD

		};


		CreateFunc	m_defaultCollisionAgent;

		hkpContactMgrFactory*	m_contactMgrFactory[HK_MAX_RESPONSE_TYPE][HK_MAX_RESPONSE_TYPE];

			/// for each shapeType T, m_hasAlternateType[T] has a bit set for each superclass
		HK_ALIGN16(hkUint32				m_hasAlternateType[ HK_MAX_SHAPE_TYPE ]);

			/// CollidableType x collidableType x {0, Pred} -> agentType dispatching for agent2 & agent3 technology.
			///  - m_agent2Types hold the proper agent's index for a given collision pair for non predictive collision detection
			///  - m_agent2TypesPred for predictive collision detection
		int						m_numAgent2Types;
		HK_ALIGN16(hkUchar	    m_agent2Types    [HK_MAX_SHAPE_TYPE][HK_MAX_SHAPE_TYPE]);
		hkUchar				    m_agent2TypesPred[HK_MAX_SHAPE_TYPE][HK_MAX_SHAPE_TYPE];
		AgentFuncs				m_agent2Func     [ HK_MAX_AGENT2_TYPES ];

		int						m_numAgent3Types;
		HK_ALIGN16(hkUchar		m_agent3Types    [HK_MAX_SHAPE_TYPE][HK_MAX_SHAPE_TYPE]);
		HK_ALIGN16(hkUchar		m_agent3TypesPred[HK_MAX_SHAPE_TYPE][HK_MAX_SHAPE_TYPE]);
		Agent3FuncsIntern       m_agent3Func     [ HK_MAX_AGENT3_TYPES ];

			/// CollidableQuality x collidableQuality -> collisionQuality dispatching
		HK_ALIGN16(hkChar				m_collisionQualityTable[ HK_COLLIDABLE_QUALITY_MAX][HK_COLLIDABLE_QUALITY_MAX]);
		HK_ALIGN16(hkpCollisionQualityInfo  m_collisionQualityInfo[ HK_MAX_COLLISION_QUALITIES ]);


			/// This is set to true if there is at least one call to registerCollisionAgent() made
		hkBool m_collisionAgentRegistered;
		hkBool m_agent3Registered;

		hkBool m_midphaseAgent3Registered;

			/// If set, produces an Assert whenever a more-general agent overrides a more-specific agent in dispatch tables.
		hkBool m_checkEnabled;

			/// All alternate shape rules (a simple collection of all registerAlternateShapeType() calls )
		hkArray<ShapeInheritance> m_shapeInheritance;

			/// Additional information to the main tables. This allows for debugging
			/// a wrong order of registering agents, see DebugEntry
		DebugTable	*m_debugAgent2Table;
		DebugTable	*m_debugAgent2TablePred;
		DebugTable	*m_debugAgent3Table;
		DebugTable	*m_debugAgent3TablePred;

	public:
		//
		// The following two members are only used when you call hkpWorld::getCinfo(...)
		// They're set upon dispatcher initialization.
		//
		hkReal m_expectedMaxLinearVelocity;
		hkReal m_expectedMinPsiDeltaTime;
};



#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.inl>

#endif // HK_COLLIDE2_COLLISION_DISPATCHER_H

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
