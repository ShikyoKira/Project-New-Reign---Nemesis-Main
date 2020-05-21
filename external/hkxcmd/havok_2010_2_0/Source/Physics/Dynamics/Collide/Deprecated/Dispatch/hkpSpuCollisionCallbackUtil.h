/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_SPU_COLLISION_CALLBACK_UTIL_H
#define HK_SPU_COLLISION_CALLBACK_UTIL_H


#include <Common/Base/Spu/Dma/Utils/hkSpuDmaUtils.h>
#include <Physics/Dynamics/Collide/Deprecated/hkpCollisionListener.h>

class hkpBvTreeShape;

	/// This is an utility class, which helps to export callback information from the SPU to the PPU. This is just an example how to do this.
	/// Please copy the files for PPU and SPU and create your own version.
	/// For future Havok physics versions this class is subject to change.
	///
	/// The SPU is severely memory-limited, so to be able to properly handle collision events (without any restrictions) as soon as the collision
	/// detection is running on the SPU we need to forward those events to the PPU. This utility handles the forwarding of the events. Simply
	/// create an instance of this utility using the static createSpuCollisionCallbackUtil() function. Pass in the total amount of memory you want
	/// the utility to have. Note that this includes the buffer for the events, so don't make it too small. You then have to attach this utility
	/// to each entity you want the events forwarded for. During collision detection this utility will now collect all 3 types of collision events
	/// (added, processed and removed), repack them, add them to the utility's event queue and write this event data back to the PPU. On the PPU
	/// you now only have to call fireCallbacks() with a pointer to your user-implemented callback functions. This will fire the appropriate
	/// callback function for each collision event and pass in the corresponding event data. Make sure to reset the event queue on your last call
	/// to fireCallbacks() (in case you should call it several times) or otherwise the queue will fill up pretty fast, preventing any further events
	/// to be forwarded from SPU to PPU.
HK_CLASSALIGN16(class) hkSpuCollisionCallbackUtil : public hkReferencedObject
{
	public:

			/// register its SPU functions
		static void HK_CALL HK_INIT_FUNCTION(registerSpuFunctions)();

	public:

			/// Event base class.
			///
			/// This is the base class for all 3 collision events. It has a type that can be used
			/// to identify the actual event. It also holds the total size for the event (used for
			/// advancing to the next event in the queue) and PPU pointers to the colliding entities.
		HK_ALIGN16(struct) Event
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkSpuCollisionCallbackUtil::Event );
			public:

				enum EventType
				{
					CONTACT_POINT_ADDED,
					CONTACT_POINT_PROCESS,
					CONTACT_POINT_REMOVED
				};

			public:
				/// Safely get the contact mgr, returns zero if the collision agent has been removed in the meantime
				hkpDynamicsContactMgr* getContactManager();

				/// Gets the next event in the list
				/// If there isn't an entry after this one, will return rubbish
				/// Use the getTerminator method to give you the first invalid entry
				Event* getNext() { return hkAddByteOffset(this, m_size); }

				hkUint16					m_size;
				hkEnum<EventType, hkUint16>	m_type;
				hkpEntity*					m_entityA;
				hkpEntity*					m_entityB;
				hkpDynamicsContactMgr*		m_contactMgr;
		};

			/// This callback event is fired whenever a new contact point is added.
			///
			/// It stores the contact point information.
		struct ContactPointAddedEvent : public hkSpuCollisionCallbackUtil::Event
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkSpuCollisionCallbackUtil::ContactPointAddedEvent );

			hkpShapeKey					m_shapeKeyA;
			hkpShapeKey					m_shapeKeyB;
			hkContactPointId			m_contactId;
			hkReal						m_projectedVelocity;
			hkContactPoint				m_contactPoint;
		};

			/// This callback event is fired for a colliding pair of entities during collision detection.
			///
			/// It holds all existing contact points between the two entities. This array of contact points
			/// is dynamically expanded and the total number of contact points is stored separately.
			/// \note If you want to add custom members to this event make sure to only add them BEFORE
			///        m_contactPoints or otherwise you risk them to be overwritten!
		struct ContactPointProcessEvent : public hkSpuCollisionCallbackUtil::Event
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkSpuCollisionCallbackUtil::ContactPointProcessEvent );

				struct ContactPoint: public hkContactPoint
				{
					HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MATH, hkSpuCollisionCallbackUtil::ContactPointProcessEvent::ContactPoint );
					HK_FORCE_INLINE hkUint32 getContactPointId() { return m_position.getInt24W(); }
				};

			public:

				int				m_numContactPoints;

				//
				// Add any additional custom members here... 
				// Note: never append them after m_contactPoints!
				// Keep in mind that m_contactPoints is 16byte aligned!
				//

				// this array is dynamically allocated when creating the utility
				ContactPoint	m_contactPoints[1];
		};

			/// This callback event is fired whenever an existing contact point is removed.
			///
			/// It stores the contact point's id.
		struct ContactPointRemovedEvent : public hkSpuCollisionCallbackUtil::Event
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkSpuCollisionCallbackUtil::ContactPointRemovedEvent );

			hkContactPointId	m_contactPointId;
		};

			/// This structure serves as a kind of interface to user-implemented callback functions.
			///
			/// Create a dedicated object of this type (or derive an existing one from it) and implement
			/// the callback functions to your likings. Each function is passed its individual event
			/// structure holding all relevant data.
		struct Callbacks
		{
			public:

				virtual ~Callbacks() {}

				virtual void contactPointAddedCallbackFromSpu  (ContactPointAddedEvent*   event) {}
				virtual void contactPointProcessCallbackFromSpu(ContactPointProcessEvent* event) {}
				virtual void contactPointRemovedCallbackFromSpu(ContactPointRemovedEvent* event) {}
		};

			/// Use these flags to decide whether a call to fireCallbacks() should reset the event queue or not.
			///
			/// Resetting the event queue will delete all existing events. So if you want to process these events a second time
			/// (e.g., first for some visualization, then for actual processing) you can call fireCallbacks() twice, once without
			/// resetting the queue, the second time with resetting the queue.
			/// \note You should make sure to reset the queue once you are done with it or otherwise it will fill up pretty fast
			///        and no further events will be able to be forwarded from SPU to PPU
		enum ResetEventQueue
		{
			RESET_EVENT_QUEUE,
			DO_NOT_RESET_EVENT_QUEUE,
		};

			/// Empties out the list, resetting it
		void reset() { m_nextFreeEvent = m_events; }

			/// Get the first in the list
		Event* getFirst() { return m_events; }

			/// Get one past the end element in the list. An event pointer must be less then this to be valid
		Event* getTerminator();


	public:

			/// Call this function to create a new collision callback utility.
			///
			/// The totalSizeOfUtil parameter specifies the total(!) size of the utility, not the event buffer size. The event
			/// buffer size is 16 bytes less.
			// This is done to have more control over the amount of memory that might be wasted when allocating a buffer size that
			// is exactly the same size as one pool memory page. In such a case the whole utility would be just 16 bytes too large
			// and would allocate one whole additional memory page just for 16 bytes.
		static hkSpuCollisionCallbackUtil* HK_CALL createSpuCollisionCallbackUtil(int totalSizeOfUtil);

			/// Call this function to fire your user-implemented event callback functions.
			///
			/// See ResetEventQueue for some information on the resetFlag parameter.
			/// See Callbacks fr some information on the user-implemented callback functions.
		void fireCallbacks(hkpWorld* world, Callbacks* m_callbacks, ResetEventQueue resetFlag = RESET_EVENT_QUEUE);

			// internal functions
		static void contactPointAddedEvent  (hkpEntity* entityA, hkpEntity* entityB, hkpContactPointAddedEvent&   event);
		static void contactPointProcessEvent(hkpEntity* entityA, hkpEntity* entityB, hkpContactProcessEvent&      event);
		static void contactPointRemovedEvent(hkpEntity* entityA, hkpEntity* entityB, hkpContactPointRemovedEvent& event);
		static hkBool shapeContainerIsCollisionEnabled(const hkpProcessCollisionInput* input, const hkpCdBody* bodyA, const hkpCdBody* bodyB, const HK_SHAPE_CONTAINER* shapeCollectionB, hkpShapeKey shapeKeyB);
		static hkBool shapeContainer2IsCollisionEnabled(const hkpProcessCollisionInput* input, const hkpCdBody* bodyA, const hkpCdBody* bodyB, const HK_SHAPE_CONTAINER* shapeCollectionA, const HK_SHAPE_CONTAINER* shapeCollectionB, hkpShapeKey shapeKeyA, hkpShapeKey shapeKeyB);
		static int numShapeKeyHitsLimitBrached( const hkpProcessCollisionInput* input, 
														const hkpCdBody* bodyA, const hkpCdBody* bodyB, 
														const hkpBvTreeShape* bvTreeShapeB, hkAabb& aabb,
														hkpShapeKey* shapeKeysInOut,
														int shapeKeysCapacity);

	protected:

		static void writeBackEvent(hkpEntity* entity, const Event& event);

	public:

		// sizeof(hkReferencedObject) : 8 bytes

		hkUint32	m_capacity; // number of 16byte blocks, i.e. actual size in bytes = m_capacity * 16
		void*		m_nextFreeEvent;

		// this array is dynamically allocated when creating the utility
		Event		m_events[1];
};


#endif // HK_SPU_COLLISION_CALLBACK_UTIL_H

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
