/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONTACT_MANIFOLD_UTIL_H
#define HK_CONTACT_MANIFOLD_UTIL_H

#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldDeletionListener.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

class hkpSimpleConstraintContactMgr;

/// For those collisions registered, this utility issues TYPE_MANIFOLD_AT_END_OF_STEP
/// contactPointCallbacks, a version of the TYPE_MANIFOLD contactPointCallback issued at the end
/// of the physics step. (The ordinary versions are fired at the beginning of the following step.)
///
/// The callbacks fired by this utility can be optionally fired in deterministic order. To achieve
/// this, the utility assumes that the listeners are deterministically ordered at each entity and
/// the world. If you intend to register listeners with this utility, and have their callbacks
/// fired in deterministic order, you should ensure that they are added to their entity or the
/// world in a deterministic order.
///
/// (Implementation note: It could be implemented as a hkpWorldExtension but it
/// is intended solely for use with the world's hkpCollisionCallbackUtil, so we
/// put it inside that object.)
class hkpEndOfStepCallbackUtil : public hkReferencedObject, public hkpWorldPostSimulationListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS );
		
			/// Add a collision to the util.
		void registerCollision( hkpSimpleConstraintContactMgr* mgr, hkpContactListener* listener, hkpCollisionEvent::CallbackSource source );

			/// Remove a collision from the util.
		void unregisterCollision( hkpSimpleConstraintContactMgr* mgr, hkpContactListener* listener, hkpCollisionEvent::CallbackSource source );

		hkpEndOfStepCallbackUtil();

	public:
			/// Used by the util to fire individual contact point events for the contactListener.
		static void fireContactPointEventsForCollision( hkpSimpleConstraintContactMgr* mgr, hkpContactListener* listener, hkpCollisionEvent::CallbackSource source );

		// hkpWorldPostSimulationListener interface
		virtual void postSimulationCallback( hkpWorld* world );

	public:
			/// Used to make the partial order on new collisions a total order.
		int m_sequenceNumber;

			/// Should we guarantee that callbacks are fired in deterministic order. This defaults
			/// to false as callbacks are not expected to be in deterministic order.
		hkBool m_deterministicOrder;

			/// The data stored about the collision and the listener.
		struct Collision
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpEndOfStepCallbackUtil::Collision);
			hkpSimpleConstraintContactMgr* m_mgr;
			hkpContactListener* m_listener;
			hkpCollisionEvent::CallbackSource m_source;

				/// Equality.
			hkBool operator == ( const Collision& other ) const;

				/// Return a unique id which can be used to partially sort Collisions.
			hkUint64 getUid() const;

				/// Partially orders Collisions by UID.
			inline hkBool operator < ( const Collision& other ) const;
		};

			/// We add a sequence number.
		struct NewCollision : public Collision
		{
				/// Used to make the partial order on collisions a total order. (NewCollisions
				/// with the same UID will arrive in a deterministic order, which we remember
				/// using this value.)
			int m_sequenceNumber;

				/// Totally orders NewCollisions.
			inline hkBool operator < ( const NewCollision& other ) const;
		};

			/// The array of collisions for which this util is issuing endOfStepCallbacks.
		hkArray<Collision> m_collisions;

			/// Collisions added since the last frame.
		hkArray<NewCollision> m_newCollisions;

			/// Collisions removed since the last frame.
		hkArray<Collision> m_removedCollisions;
};

#endif // HK_CONTACT_MANIFOLD_UTIL_H

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
