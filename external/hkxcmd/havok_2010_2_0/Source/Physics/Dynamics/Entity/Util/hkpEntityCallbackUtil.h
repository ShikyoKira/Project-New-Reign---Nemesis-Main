/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_ENTITY_CALLBACK_UTIL_H
#define HK_DYNAMICS2_ENTITY_CALLBACK_UTIL_H

class hkpEntity;

#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

class hkpEntityCallbackUtil
{
	public:
			/// go through both, the entity and the collision listeners
		static void HK_CALL fireEntityAdded( hkpEntity* entity );
		static void HK_CALL fireEntityRemoved( hkpEntity* entity );
		static void HK_CALL fireEntityShapeSet( hkpEntity* entity );
		static void HK_CALL fireEntityDeleted( hkpEntity* entity );
		static void HK_CALL fireEntitySetMotionType( hkpEntity* entity ) ;

			// fast inline functions calling the internal functions
		static HK_FORCE_INLINE void HK_CALL fireContactPointCallback( hkpEntity* entity, hkpContactPointEvent& event );

		static void HK_CALL fireContactConstraintAddedCallback( hkpEntity* entity, hkpCollisionEvent& event );
		static void HK_CALL fireContactConstraintRemovedCallback( hkpEntity* entity, hkpCollisionEvent& event );

	protected:
		static void HK_CALL fireContactPointCallbackInternal( hkpEntity* entity, hkpContactPointEvent& event );

	public:
		//
		// Deprecated.
		//

		static HK_FORCE_INLINE void HK_CALL fireContactPointAdded( hkpEntity* entity, hkpContactPointAddedEvent& event);
		static HK_FORCE_INLINE void HK_CALL fireContactPointRemoved( hkpEntity* entity, hkpContactPointRemovedEvent& event );
		static HK_FORCE_INLINE void HK_CALL fireContactProcess( hkpEntity* entity, hkpContactProcessEvent& event );

	protected:
		//
		// Deprecated.
		//

		static void HK_CALL fireContactPointAddedInternal( hkpEntity* entity, hkpContactPointAddedEvent& event);
		static void HK_CALL fireContactPointRemovedInternal( hkpEntity* entity, hkpContactPointRemovedEvent& event );
		static void HK_CALL fireContactProcessInternal( hkpEntity* entity, hkpContactProcessEvent& event );

};


void HK_CALL hkpEntityCallbackUtil::fireContactPointCallback( hkpEntity* entity, hkpContactPointEvent& event )
{
	if ( entity->m_contactListeners.getSize())
	{
		fireContactPointCallbackInternal( entity, event );
	}
}

// Deprecated.
void HK_CALL hkpEntityCallbackUtil::fireContactPointAdded( hkpEntity* entity, hkpContactPointAddedEvent& event)
{
	if ( entity->m_contactListeners.getSize())
	{
		fireContactPointAddedInternal(entity, event);
	}
}

// Deprecated.
void HK_CALL hkpEntityCallbackUtil::fireContactPointRemoved( hkpEntity* entity, hkpContactPointRemovedEvent& event )
{
	if ( entity->m_contactListeners.getSize())
	{
		fireContactPointRemovedInternal( entity, event );
	}
}

// Deprecated.
void HK_CALL hkpEntityCallbackUtil::fireContactProcess( hkpEntity* entity, hkpContactProcessEvent& event )
{
	if ( entity->m_contactListeners.getSize())
	{
		fireContactProcessInternal( entity, event );
	}
}
#endif // HK_DYNAMICS2_ENTITY_CALLBACK_UTIL_H

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
