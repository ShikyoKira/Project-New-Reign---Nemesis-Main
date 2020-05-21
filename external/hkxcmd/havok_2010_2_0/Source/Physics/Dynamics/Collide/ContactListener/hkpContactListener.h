/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONTACT_POINT_LISTENER_H
#define HK_CONTACT_POINT_LISTENER_H

#include <Physics/Dynamics/Collide/ContactListener/hkpContactPointEvent.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpCollisionEvent.h>

#include <Physics/Dynamics/Collide/Deprecated/hkpCollisionEvents.h>

	/// The interface for listeners that respond to contact events.
class hkpContactListener
{
	//+hk.MemoryTracker(ignore=True)
	public:

		//
		// Contact point events.
		//

			/// This is called for each new contact point between two bodies. It will
			/// also be called for all contact points in the contact manifold when the
			/// hkpSimpleConstraintContactMgr's m_contactPointCallbackDelay value
			/// reaches zero.
		virtual void contactPointCallback( const hkpContactPointEvent& event ) {}

		//
		// Collision events.
		//
		// If hkpWorldCinfo.m_fireCollisionCallbacks is set to true, then these are fired when
		// the constraint representing the collision is added or removed from the world.
		//
		// These callbacks are fired single-threaded but not necessarily in deterministic order.
		//
		// Note that a collisionAddedCallback does not guarantee that a collision has occurred if a
		// collisionRemovedCallback is also fired in the same frame. (Of course, contactPointEvents
		// will be fired if a collision did occur during the frame).
		//

			/// Called for a new collision between two bodies.
		virtual void collisionAddedCallback( const hkpCollisionEvent& event ) {}

			/// Called when two bodies are no longer colliding.
		virtual void collisionRemovedCallback( const hkpCollisionEvent& event ) {}

		//
		// If you want to receive contactPointCallbacks of TYPE_MANIFOLD_AT_END_OF_STEP, you should
		// use these methods to register and unregister the collision.
		//

			/// Call this from a collisionAddedCallback to receive the extra contactPointCallbacks.
		void registerForEndOfStepContactPointCallbacks( const hkpCollisionEvent& event );

			/// Call this from a collisionRemovedCallback to unregister.
		void unregisterForEndOfStepContactPointCallbacks( const hkpCollisionEvent& event );


			/// Destructor.
		virtual ~hkpContactListener() {}

	public:

		//
		// Deprecated callbacks which support the old hkpCollisionListener interface.
		//

		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event) {}
		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event ) {}
		virtual void contactProcessCallback( hkpContactProcessEvent& event ) {}
};

#endif // HK_CONTACT_POINT_LISTENER_H

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
