/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLISION_LIFETIME_EVENT_H
#define HK_COLLISION_LIFETIME_EVENT_H

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/World/hkpSimulationIsland.h>
class hkpSimpleConstraintContactMgr;

/// An event recording the start or end of a collision between two bodies.
class hkpCollisionEvent
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpCollisionEvent);
			/// Determines the participant to which a listener was attached.
		enum CallbackSource
		{
			SOURCE_A = 0,
			SOURCE_B = 1,
			SOURCE_WORLD
		};

	public:
			/// Constructor.
		inline hkpCollisionEvent( CallbackSource source, hkpRigidBody* bodyA, hkpRigidBody* bodyB, hkpSimpleConstraintContactMgr* mgr );

			/// Returns the first or second rigid body.
		inline hkpRigidBody* getBody( int bodyIdx ) const;

			/// Returns the simulation island (i.e., the constraint owner) containing this contact.
		inline hkpSimulationIsland* getSimulationIsland() const;

	public:

		CallbackSource m_source;

			/// The rigid bodies.
		hkpRigidBody* m_bodies[2];
	
			/// The contact manager.
		hkpSimpleConstraintContactMgr* m_contactMgr;
};

#include <Physics/Dynamics/Collide/ContactListener/hkpCollisionEvent.inl>

#endif //HK_COLLISION_LIFETIME_EVENT_H

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
