/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_BROAD_PHASE_BORDER_H
#define HK_UTILITIES2_BROAD_PHASE_BORDER_H

#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/World/hkpWorldCinfo.h>
#include <Physics/Dynamics/Phantom/hkpPhantomOverlapListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldDeletionListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>

class hkpWorld;
class hkpEntity;

/// This class monitors objects leaving the broadphase.
class hkpBroadPhaseBorder : public hkReferencedObject, protected hkpWorldDeletionListener, protected hkpPhantomOverlapListener, hkpWorldPostSimulationListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES);

			/// Creates an instance and attaches it to the world. It also adds a second reference to it that gets removed when the world gets deleted
			/// The positions of the border phantoms are derived from the broadphase extents of the hkpWorld
		hkpBroadPhaseBorder( hkpWorld* world, hkpWorldCinfo::BroadPhaseBorderBehaviour type  = hkpWorldCinfo::BROADPHASE_BORDER_ASSERT, hkBool postponeAndSortCallbacks = false );
			
		virtual ~hkpBroadPhaseBorder();

			/// This function is called when an object goes beyond the scope of the broadphase.
			/// By default, it removes the entity from the world, you may implement your own version.
		virtual void maxPositionExceededCallback( hkpEntity* body );

			/// Deactivate this class
		virtual void deactivate();

		virtual void collidableAddedCallback( const hkpCollidableAddedEvent& event	);

			/// Callback implementation
		virtual void collidableRemovedCallback( const hkpCollidableRemovedEvent& event );

			/// Called at the end of the hkpWorld::simulate call.
			/// Handles entities exiting broad phase in the last frame.
		virtual void postSimulationCallback( hkpWorld* world );

	public:

		virtual void worldDeletedCallback( hkpWorld* world );

		hkpWorld* m_world;

		class hkpPhantom* m_phantoms[6];

		hkpWorldCinfo::BroadPhaseBorderBehaviour m_type;

		hkBool m_postponeAndSortCallbacks;

		hkArray<hkpEntity*> m_entitiesExitingBroadPhase;

};

#endif // HK_UTILITIES2_BROAD_PHASE_BORDER_H

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
