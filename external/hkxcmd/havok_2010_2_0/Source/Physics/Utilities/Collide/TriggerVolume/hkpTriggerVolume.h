/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TRIGGER_VOLUME_H
#define HK_TRIGGER_VOLUME_H

#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Common/Base/Types/Color/hkColor.h>

class hkpCharacterProxy;

static const int HK_TRIGGER_VOLUME_DEBUG_COLOR = hkColor::rgbFromChars( 0xff, 0xff, 0xff, 0x70 );

/// This is a super-class for trigger volumes, which are rigid bodies that record collision
/// events, but have no physical effect.
///
/// Passing a rigid body into the constructor of a new TriggerVolume converts it into a trigger
/// volume and no further work need be done. The object manages its own memory and keeps itself
/// alive so long as its triggerBody is alive. The trigger volume sets the world to fire
/// collisionAdded/Removed callbacks for its duration.
///
/// Notes:
///     - The events are usually fired at the end of the physics step by a postSimulationCallback.
///       They can also be fired when the triggerVolume is removed from the world, or explicitly
///       by calling updateOverlaps.
///     - The events are fired in deterministic order, and the m_overlappingBodies array is kept in
///       deterministic order.
///     - Although the trigger volume can detect a body which entered and left the volume during
///       a frame, it cannot detect a body which left and re-entered the volume during a frame.
///       In this case, no events are issued.
///     - You should not assume that the body in a leaving event is still in the world.
class hkpTriggerVolume : public hkReferencedObject, public hkpContactListener, public hkpWorldPostSimulationListener, public hkpEntityListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES );
		HK_DECLARE_REFLECTION();

			/// Convert the given rigidBody into a triggerVolume.
			/// The body will store a pointer to this hkpTriggerVolume object in its
			/// HK_PROPERTY_TRIGGER_VOLUME property.
		hkpTriggerVolume( hkpRigidBody* triggerBody );
		hkpTriggerVolume(hkFinishLoadedObjectFlag f);

			/// The type of events which are recorded by the trigger volume.
		enum EventType
		{
			ENTERED_EVENT = 1,
			LEFT_EVENT = 2,
			ENTERED_AND_LEFT_EVENT = 1 | 2,
				/// Fired for any overlapping body when the trigger body leaves the world or is
				/// deleted.
			TRIGGER_BODY_LEFT_EVENT = 4 | 2
		};

			/// Subclasses should implement this function to be informed of events involving
			/// rigid bodies (including the bodies of hkpCharacterRigidBodies).
		virtual void triggerEventCallback( hkpRigidBody* body, EventType type ) {}
			
			/// Character proxies process their own collisions, so they call this method to inform
			/// the trigger volume that they touched it during the frame.
		virtual void triggerEventCallback( hkpCharacterProxy* proxy, EventType type ) {}
		
			/// Return the rigid body underlying this triggerVolume.
		inline hkpRigidBody* getTriggerBody() const;

			/// Returns the triggerVolume of a rigidBody or HK_NULL if the
			/// rigidBody is not the triggerBody of a triggerVolume.
		inline static hkpTriggerVolume* getTriggerVolume( hkpRigidBody* body );

			/// Return the set of currently overlapping bodies.
		inline const hkArrayBase<hkpRigidBody*>& getOverlappingBodies() const;

			/// Call this to update the triggerVolume outside the physics step.
			/// triggerEventCallbacks may be fired with types ENTERED_EVENT and LEFT_EVENT.
			/// You may want to re-collide the entities before calling this.
		void updateOverlaps();

			/// Destructor.
		virtual ~hkpTriggerVolume();

	public:
		// hkpContactListener interface.
		virtual void contactPointCallback( const hkpContactPointEvent& event );
		virtual void collisionAddedCallback( const hkpCollisionEvent& event );
		virtual void collisionRemovedCallback( const hkpCollisionEvent& event );

		// hkpWorldPostSimulationListener interface.
		virtual void postSimulationCallback( hkpWorld* world );

		// hkpEntityListener interface.
		virtual void entityDeletedCallback( hkpEntity* entity );
		virtual void entityAddedCallback( hkpEntity* entity );
		virtual void entityRemovedCallback( hkpEntity* entity );

			/// Called when the triggerBody has been added to the world.
		void triggerBodyEnteredWorld( hkpWorld* );
			/// Called when the triggerBody has been removed from the world or deleted.
		void triggerBodyLeftWorld();

			/// The array of bodies in contact with the triggerBody after callbacks have been
			/// fired. It is kept in deterministic order. It is not updated until after the
			/// triggerEventCallbacks have been fired. This object adds a reference to each
			/// body in this array.
		hkArray<hkpRigidBody*> m_overlappingBodies;

			/// The four types of event recorded for a body.
		enum Operation
		{
				/// A collision added event.
			ADDED_OP,
				/// A collision removed event.
			REMOVED_OP,
				/// A contactPointEvent of TYPE_EXPAND_MANIFOLD
			CONTACT_OP,
				/// A contactPointEvent of TYPE_TOI
			TOI_OP
		};

			/// The information stored for each event.
		struct EventInfo {
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpTriggerVolume::EventInfo);
			HK_DECLARE_REFLECTION();
				/// Ensures that when the event queue is sorted for determinism, the sort is stable.
			hkUint64 m_sortValue;
				/// A reference is added to the body for every occurrence of the body in event
				/// queue. These redundant references are tidied up in the postSimulationCallback.
			hkpRigidBody* m_body;
			hkEnum<Operation, int> m_operation;
		};

			/// The queue of events for this frame.
		hkArray<EventInfo> m_eventQueue;

			/// The trigger body.
		hkpRigidBody* m_triggerBody;

			/// Used to set the sort values of individual events. Reset to 0 each frame.
		hkUint32 m_sequenceNumber;

	protected:
			/// The order used to sort the event queue.
		static hkBool HK_CALL bodyOrderForQueues( const hkpTriggerVolume::EventInfo& infoA, const hkpTriggerVolume::EventInfo& infoB );

			/// The order used to sort arrays of bodies.
		static hkBool HK_CALL bodyOrderForArrays( const hkpRigidBody* bodyA, const hkpRigidBody* bodyB );

			/// Calculates a sortValue and adds the event to the event queue.
		inline void addEvent( hkpRigidBody* body, Operation m_operation );

			/// Free references to any bodies in the event queue and clear the array.
		inline void abandonEventQueue();
};

#include <Physics/Utilities/Collide/TriggerVolume/hkpTriggerVolume.inl>

#endif // HK_TRIGGER_VOLUME_H

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
