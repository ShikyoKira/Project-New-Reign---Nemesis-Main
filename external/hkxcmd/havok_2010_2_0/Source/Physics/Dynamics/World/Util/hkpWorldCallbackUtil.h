/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_WORLD_CALLBACK_UTIL_H
#define HK_DYNAMICS2_WORLD_CALLBACK_UTIL_H

class hkpWorld;
struct hkpConstraintBrokenEvent;
struct hkpConstraintRepairedEvent;
class hkStepInfo;
class hkpPhantom;
class hkpContactImpulseLimitBreachedListener;
class hkpContactImpulseLimitBreachedListenerInfo; 

#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

class hkpWorldCallbackUtil
{
	public:
		static void HK_CALL fireActionAdded( hkpWorld* world, hkpAction* action ) ;
		static void HK_CALL fireActionRemoved( hkpWorld* world, hkpAction* action ) ;

		static void HK_CALL fireEntityAdded( hkpWorld* world, hkpEntity* entity ) ;
		static void HK_CALL fireEntityRemoved( hkpWorld* world, hkpEntity* entity ) ;
		static void HK_CALL fireEntityShapeSet( hkpWorld* world, hkpEntity* entity ) ;
		static void HK_CALL fireEntitySetMotionType( hkpWorld* world, hkpEntity* entity ) ;

		static void HK_CALL firePhantomAdded(    hkpWorld* world, hkpPhantom* phantom );
		static void HK_CALL firePhantomRemoved(  hkpWorld* world, hkpPhantom* phantom );
		static void HK_CALL firePhantomShapeSet( hkpWorld* world, hkpPhantom* phantom ) ;

		static void HK_CALL fireConstraintAdded(   hkpWorld* world, hkpConstraintInstance* constraint ) ;
		static void HK_CALL fireConstraintRemoved( hkpWorld* world, hkpConstraintInstance* constraint ) ;
		static void HK_CALL fireConstraintViolated( hkpWorld* world, hkpConstraintInstance* constraint ) ;
		static void HK_CALL fireConstraintBroken( hkpWorld* world, const hkpConstraintBrokenEvent& event ) ;
		static void HK_CALL fireConstraintRepaired( hkpWorld* world, const hkpConstraintRepairedEvent& event ) ;

		static void HK_CALL fireContactPointCallback( hkpWorld* world, hkpContactPointEvent& event );
		static void HK_CALL fireContactConstraintAddedCallback( hkpWorld* world, hkpCollisionEvent& event );
		static void HK_CALL fireContactConstraintRemovedCallback( hkpWorld* world, hkpCollisionEvent& event );

		static void HK_CALL firePostSimulationCallback( hkpWorld* world );
		static void HK_CALL firePostIntegrateCallback( hkpWorld* world, const hkStepInfo& info );
		static void HK_CALL firePostCollideCallback( hkpWorld* world, const hkStepInfo& info );

			// This fires both island and entity activation callbacks.
		static void HK_CALL fireIslandActivated( hkpWorld* world, hkpSimulationIsland* island );
			// This fires both island and entity deactivation callbacks.
		static void HK_CALL fireIslandDeactivated( hkpWorld* world, hkpSimulationIsland* island );

		static void HK_CALL fireIslandPostIntegrateCallback( hkpWorld* world, hkpSimulationIsland* island, const hkStepInfo& info );
		static void HK_CALL fireIslandPostCollideCallback( hkpWorld* world, hkpSimulationIsland* island, const hkStepInfo& info );

		static void HK_CALL fireWorldDeleted( hkpWorld* world ) ;
		static void HK_CALL fireWorldRemoveAll( hkpWorld* world ) ;
		

		static void HK_CALL fireInactiveEntityMoved( hkpWorld* world, hkpEntity* entity);

		static void HK_CALL fireContactImpulseLimitBreached( hkpWorld* world, const hkpContactImpulseLimitBreachedListenerInfo* breachedContacts, int numBreachedContacts );

		//
		// Deprecated.
		//
		static void HK_CALL fireContactPointAdded( hkpWorld* world, hkpContactPointAddedEvent& event);
		static void HK_CALL fireContactPointRemoved( hkpWorld* world, hkpContactPointRemovedEvent& event );
		static void HK_CALL fireContactProcess( hkpWorld* world, hkpContactProcessEvent& event );
};

#endif // HK_DYNAMICS2_WORLD_CALLBACK_UTIL_H

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
