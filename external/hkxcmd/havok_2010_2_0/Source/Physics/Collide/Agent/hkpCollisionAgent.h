/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_COLLISION_AGENT_H
#define HK_COLLIDE2_COLLISION_AGENT_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>

class hkpCdBody;
struct hkpCollisionInput;
struct hkpProcessCollisionInput;
struct hkpProcessCollisionOutput;
struct hkpLinearCastCollisionInput;

#ifndef hkCollisionConstraintOwner
class hkpConstraintOwner;
#	define hkCollisionConstraintOwner hkpConstraintOwner
#endif

class hkpCdPointCollector;
class hkpCdBodyPairCollector;
class hkpContactMgr;
class hkpCollisionDispatcher;

/// The base class for all collision agents, which are used for pair-wise narrowphase collision detection between shapes.
/// To create and query a collision agent you need to have two hkpCollidable objects
/// (which wrap the hkpShape objects together with associated transforms),
/// and you need to create the agent via the hkpCollisionDispatcher.
/// The collision agent provides 3 queries:
/// - getPenetations (for simply determining whether two hkpCollidable objects are overlapping)
/// - getClosestPoints (to determine the closest point(s) between two hkpCollidable object)
/// - linearCast (to determine the contact point when casting one hkpCollidable against the other)
/// Most implementations of this hkpCollisionAgent also provide static versions of these three functions.
/// This allows for querying objects without an agent, however you have to use the hkpCollisionDispatcher
/// to lookup the right static function.
/// Note that the interface takes hkpCdBody objects. However, if you are calling the hkpCollisionAgent at the user level, you must use hkpCollidable objects
/// as input (which inherit from hkpCdBody).
class hkpCollisionAgent : public hkReferencedObject	
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT);

			/// Checks whether the two objects are penetrating.
			/// The hkpCdBodyPairCollector passed in will receive a callback for each overlapping pair of shapes. You can get many callbacks
			/// from this call, if either of the shapes in bodyA or bodyB is a hkpShape collection.
		virtual void getPenetrations( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector ) = 0;

			/// Get the closest distances between two objects.
			/// For each collision between leaf shapes which is a smaller distance than the collision tolerance (specified in the "input" parameter)
			/// the collector will receive a callback. You can get many callbacks from this call, if either of the shapes in bodyA or bodyB is a hkpShape collection.
		virtual void getClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdPointCollector& collector  ) = 0;
	
			/// Report time of impact between two objects in range [0..1].
			/// For each pair of child shapes that hit in the linear cast, the "castCollector" will receive a callback
			/// You can also supply an optional "startCollector" object which, if not HK_NULL, will receive a callback for each point at the start position
			/// for the two cd bodies. (In effect this is the same functionality as calling getClosestPoints, but without the overhead of making two calls).
			/// Note that shape radius is considered - you may want to set the radius of the cast shape to zero.
		virtual void linearCast( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpLinearCastCollisionInput& input, hkpCdPointCollector& castCollector, hkpCdPointCollector* startCollector ) = 0;


	public:
		
			/// This function is used internally by hkDynamics
		virtual void processCollision( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result ) = 0;

			/// Called if the agent is no longer being used
		virtual void cleanup(hkCollisionConstraintOwner& info) = 0;

		virtual void updateShapeCollectionFilter( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkCollisionConstraintOwner& constraintOwner ) {}

		virtual void invalidateTim( const hkpCollisionInput& input);

		virtual void warpTime(hkTime oldTime, hkTime newTime, const hkpCollisionInput& input);

			//
			// Functions needed to get welding working: see hkAgent3.h file
			//
			/// remove a cached contact point. This implementation should not call any hkpContactMgr functions
		virtual void removePoint( hkContactPointId idToRemove );

			/// Convert the last invalid contact point to a real one. This implementation should not call any hkpContactMgr functions
		virtual void commitPotential( hkContactPointId newId );

			/// Flag a contact point to die at the next frame
		virtual void createZombie( hkContactPointId idTobecomeZombie );


	protected:

			/// The destructor is protected and must be called by the cleanup function.
		inline virtual ~hkpCollisionAgent();

			/// The constructor is protected and is called by a static creation function that is registered with the hkpCollisionDispatcher.
		inline hkpCollisionAgent( hkpContactMgr* contactMgr );

	protected:

			// Note: on PlayStation(R)3 spu this contact mgr points to the ppu. The hkCollisionOutput has a pointer to the local copy
		hkpContactMgr* m_contactMgr;
	
	public:

		inline hkpContactMgr* getContactMgr();

		/// Function ptr type to override heightField agent register function
		typedef void (HK_CALL *RegisterAgentFunction)(hkpCollisionDispatcher* dispatcher);

		/// Function ptr to register heightField and compound agents
		/// To enable dead-stripping of code these are set to HK_NULL
		static RegisterAgentFunction registerHeightFieldAgent;
		static RegisterAgentFunction registerCompoundAgent3;
		static RegisterAgentFunction registerCompoundCompoundAgent3;
};

#include <Physics/Collide/Agent/hkpCollisionAgent.inl>

#endif // HK_COLLIDE2_COLLISION_AGENT_H

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
