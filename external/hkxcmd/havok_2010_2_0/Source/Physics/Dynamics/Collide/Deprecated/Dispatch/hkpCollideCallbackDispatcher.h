/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE_CALLBACK_DISPATCHER
#define HK_COLLIDE_CALLBACK_DISPATCHER

#include <Physics/Dynamics/Collide/Deprecated/hkpCollisionListener.h>
#include <Physics/Collide/Shape/hkpShape.h>

# define HK_SHAPE_CONTAINER hkpShapeContainer

		/// This is the main entry point for any 'contact point added' callback.
	HK_FORCE_INLINE void HK_CALL hkFireContactPointAddedCallback   (hkpWorld*  world,  hkpEntity* entityA, hkpEntity* entityB, hkpContactPointAddedEvent& event);

		/// This is the main entry point for any 'contact point processed' callback.
	HK_FORCE_INLINE void HK_CALL hkFireContactProcessCallback      (hkpWorld*  world,  hkpEntity* entityA, hkpEntity* entityB, hkpContactProcessEvent& event);

		/// This is the main entry point for any 'contact point removed' callback.
	HK_FORCE_INLINE void HK_CALL hkFireContactPointRemovedCallback (hkpWorld*  world,  hkpEntity* entityA, hkpEntity* entityB, hkpContactPointRemovedEvent& event);


#include <Physics/Dynamics/Collide/Deprecated/Dispatch/hkpCollideCallbackDispatcher.inl>


#endif // HK_COLLIDE_CALLBACK_DISPATCHER

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
