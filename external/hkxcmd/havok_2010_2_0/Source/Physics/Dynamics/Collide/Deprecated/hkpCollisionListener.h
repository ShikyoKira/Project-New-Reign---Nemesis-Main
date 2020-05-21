/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_COLLISION_LISTENER_H
#define HK_DYNAMICS2_COLLISION_LISTENER_H

#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>
#include <Physics/Dynamics/Collide/Deprecated/hkpCollisionEvents.h>

	/// The base interface for listeners that respond to collision events.
	/// There are two types of contacts:
	///     - Normal contacts, which are persistent between two objects and are part of the contact manifold
	///     - TOI contacts, which are just created if two objects hit with high velocity and the interaction
	///       of both objects is flagged as continuous.
class hkpCollisionListener : public hkpContactListener
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpCollisionListener);
			/// Called after a contact point is created. This contact point is still a potential one.
			/// You can use this callback to:
			///     - Override the friction and restitution values in the contact point's hkContactPointMaterial.
			///     - Reject the contact point.
			/// In this call you get access to the full information of the new contact point.
		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event) {}

			/// Called before a contact point gets removed.
		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event ) {}

			/// Called just after all collision detection between 2 rigid bodies has been executed.
			/// NB: Callback frequency is related to hkpEntity::m_processContactCallbackDelay.
			/// This callback allows you to change any information in the result.
			/// You can and should use this callback to:
			///     - do custom welding
			///     - trigger sound effects, especially scratching and rolling
		virtual void contactProcessCallback( hkpContactProcessEvent& event ) {}


			/// Called when a new contact point is used for the very first time.
			/// At this stage the contact point no longer is a potential one.
			/// So this callback can be used to trigger high quality game events.
			/// This callback is fired:
			///     - If it is a normal (manifold) contact: by the constraint solver.
			///     - If it is a TOI contact: by the TOI handler in hkpContinuousSimulation.
		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event) {}

	public:
		// Internal

			/// This issues a contactPointConfirmedCallback when the contact point is new.
		virtual void contactPointCallback( const hkpContactPointEvent& event );


			/// Virtual destructor for derived classes.
		virtual ~hkpCollisionListener() {}
};

#endif // HK_DYNAMICS2_COLLISION_LISTENER_H

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
