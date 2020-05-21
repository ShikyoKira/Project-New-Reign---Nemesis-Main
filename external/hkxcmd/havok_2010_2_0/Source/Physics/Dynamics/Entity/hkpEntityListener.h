/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_ENTITY_LISTENER_H
#define HK_DYNAMICS2_ENTITY_LISTENER_H

#include <Common/Base/hkBase.h>

#include <Physics/Dynamics/Entity/hkpEntity.h>

/// Any class that is interested in events from an entity inherits from this class. For example, hkConstraints and hkActions are entity listeners.
class hkpEntityListener
{
	//+hk.MemoryTracker(ignore=True)
	public:
			/// Virtual destructor for derived objects
		virtual ~hkpEntityListener() {}

			/// Called when an entity is added to the hkpWorld.
		virtual void entityAddedCallback( hkpEntity* entity ) {}

			/// Called when an entity is removed from the hkpWorld.
		virtual void entityRemovedCallback( hkpEntity* entity ) {}

			/// Called when an entity changes its shape.
		virtual void entityShapeSetCallback( hkpEntity* entity)
		{
			// Most of Havok's entity listeners (VDB viewers) are only concerned about entities that are in the world.
			if (entity->getWorld())
			{
				// Since Havok's VDB viewers have do destroy old and create new display information, we can simply call:
				entityRemovedCallback(entity);
				entityAddedCallback(entity);
			}
		}

			/// Called when the motion type of an entity (rigid body) is changed
		virtual void entitySetMotionTypeCallback( hkpEntity* entity ) {}

			/// Called when an entity is deleted. hkpEntityListener subclasses \b must implement this function.
		virtual void entityDeletedCallback( hkpEntity* entity ) {}

};


#endif // HK_DYNAMICS2_ENTITY_LISTENER_H

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
