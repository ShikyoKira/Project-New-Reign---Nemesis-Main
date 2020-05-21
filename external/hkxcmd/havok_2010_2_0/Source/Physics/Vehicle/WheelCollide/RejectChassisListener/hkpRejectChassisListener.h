/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_REJECT_RAY_CHASSIS_LISTENER_H
#define HK_REJECT_RAY_CHASSIS_LISTENER_H

#include <Physics/Dynamics/Phantom/hkpPhantomOverlapListener.h>

extern const class hkClass hkpRejectChassisListenerClass;

/// An hkpPhantomOverlapListener is used to ignore the chassis and other phantoms
/// when doing collision detection.
/// (This inherits from base object so that it can be serialized without
/// the listener interface declared as reflected.)
class hkpRejectChassisListener : public hkReferencedObject, public hkpPhantomOverlapListener
{
	public:
		// +version(1)
	
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

		hkpRejectChassisListener() { }

		virtual ~hkpRejectChassisListener();
		
		//
		// Methods
		//
		
		void collidableAddedCallback(const hkpCollidableAddedEvent& event);
		
		void collidableRemovedCallback(const hkpCollidableRemovedEvent& event);
		
		//
		// Members
		//
	public:
		
			/// The collidable of the owning vehicle's chassis which should be ignored.
		const hkpCollidable* m_chassis; //+nosave

	public:
		hkpRejectChassisListener(hkFinishLoadedObjectFlag f) : hkReferencedObject(f) { }
};

#endif // HK_REJECT_RAY_CHASSIS_LISTENER_H

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
