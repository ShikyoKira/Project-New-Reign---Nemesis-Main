/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_PHANTOM_OVERLAP_LISTENER_H
#define HK_DYNAMICS2_PHANTOM_OVERLAP_LISTENER_H

class hkpPhantom;
class hkpCollidable;

	/// This is a variable of hkpCollidableAddedEvent.
enum hkpCollidableAccept
{
		/// The collidable should be added to the hkpPhantom.
	HK_COLLIDABLE_ACCEPT = 0,
		/// The collidable should not be added to the hkpPhantom.
	HK_COLLIDABLE_REJECT = 1
};

	/// This structure is the input to hkpPhantomOverlapListener::collidableAddedCallback
struct hkpCollidableAddedEvent
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpCollidableAddedEvent );

		/// The hkpPhantom.
	const hkpPhantom* m_phantom;

		/// The hkpCollidable.
	const hkpCollidable* m_collidable;

		/// An export flag: if any callback sets this flag to HK_COLLIDABLE_REJECT
		/// than the collidable won't be added to the phantom.
		/// This allows you to:
		///  - add a special filter
		///  - or redirect certain overlaps to a different location
		///
		/// By default this is set to HK_COLLIDABLE_ACCEPT
	mutable hkpCollidableAccept m_collidableAccept;
};

	/// This structure is the input to hkpPhantomOverlapListener::collidableRemovedCallback
struct hkpCollidableRemovedEvent
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpCollidableRemovedEvent );

		/// The hkpPhantom.
	const hkpPhantom* m_phantom;
	
		/// The hkpCollidable.
	const hkpCollidable* m_collidable;

		/// The next value is set to true, if the collidable was actually
		/// added to the hkpPhantom.
		/// It might be set to false if:
		///  - it was previously rejected by the hkpCollidableAddedEvent
		///  - it was filtered out by the hkpPhantom's filter upon addition.
	hkBool m_collidableWasAdded;
};

	/// Implement this class to receive notifications when objects enter the phantom's AABB area.
	/// You can also reject collidables in the collidableAddedCallback, to prevent them
	/// from being added to the hkpPhantom.
class hkpPhantomOverlapListener
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpPhantomOverlapListener);
			/// This callback is triggered when a new collidable overlaps with the AABB of the phantom.
			/// It will be called after it has passed the hkpCollisionFilter.
			/// A callback can set m_collidableAccept to HK_COLLIDABLE_REJECT, which means that the
			/// phantom will not add the collidable to its internal list, however
			/// all the events will still be raised.
		virtual void collidableAddedCallback(   const hkpCollidableAddedEvent& event ) = 0;

			/// The callback will be called whenever the objects leave the AABB of the phantom.
			/// i.e., it is called whenever hkpPhantom::removeOverlappingCollidable() is called.
			/// collidableRemovedCallback() is not symmetric to collidableAddedCallback because
			/// it is called regardless of whether the collidable has been added to the callback or not.
			/// Even if the collidable had been filtered out by the collision filter or by
			/// a hkpPhantomOverlapListener, this event will still be called when upon removal.
		virtual void collidableRemovedCallback( const hkpCollidableRemovedEvent& event ) = 0;

			/// Virtual destructor for derived objects
		virtual ~hkpPhantomOverlapListener() {}
};

#endif //HK_DYNAMICS2_PHANTOM_OVERLAP_LISTENER_H

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
