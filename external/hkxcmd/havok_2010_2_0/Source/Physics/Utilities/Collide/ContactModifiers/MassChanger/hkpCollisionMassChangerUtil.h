/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_MASS_CHANGER_UTIL
#define HK_UTILITIES2_MASS_CHANGER_UTIL


#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

#include <Physics/Dynamics/Entity/hkpEntity.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

class hkpRigidBody;

/// This listener class can be used to virtually change the masses of objects during collision.
///
/// By calling setInvMassScalingForContact() in hkpResponseModifier this allows an object to
/// interact differently with one object than with another (i.e., it may appear to be fixed to one
/// object, and not to another).
/// This utility is an entity listener and uses 2 rigid bodies to decide when to
/// change the mass and what to change it to. You may implement a world listener
/// and a look-up table to change the response of any object in contact with any
/// other object.
class hkpCollisionMassChangerUtil: public hkReferencedObject, private hkpContactListener, private hkpEntityListener
{
	public:	
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES);

			/// Adds the hkpCollisionMassChangerUtil as a listener to bodyA.
			/// If a contact point is added between bodyA and bodyB, the relative mass of the collision is changed.
			/// You specify this using the inverseMassA for the apparent inverse mass of body A in the collision, and
			/// inverseMassB for the apparent inverse mass of body B in the collision.
			/// By default inverseMassA is 0, and inverseMassB is 1, i.e., bodyA will appear to be immovable from bodyB's perspective.
		hkpCollisionMassChangerUtil( hkpRigidBody* bodyA, hkpRigidBody* bodyB, float inverseMassA = 0, float inverseMassB = 1 );

			/// Adds the hkpCollisionMassChangerUtil as a listener to bodyA.
			/// If a contact point is added between bodyA and bodyB, the relative mass and inertia of the collision is changed.
			/// You specify this using the inverseMassesA for the apparent inverse mass and inertia components of body A in the collision, and
			/// inverseMassB for the apparent inverse mass and inertia components of body B in the collision.
		hkpCollisionMassChangerUtil( hkpRigidBody* bodyA, hkpRigidBody* bodyB, const hkVector4& inverseMassesA, const hkVector4& inverseMassesB );

		~hkpCollisionMassChangerUtil();

	protected:

		// The contact listener interface.
		virtual void contactPointCallback( const hkpContactPointEvent& event );

		// The hkpEntityListener interface implementation
		
		void entityDeletedCallback( hkpEntity* entity );

	protected:

		hkpRigidBody* m_bodyA;
		hkpRigidBody* m_bodyB;
		hkVector4 m_inverseMassesA;
		hkVector4 m_inverseMassesB;
};

#endif		// HK_UTILITIES2_MASS_CHANGER_UTIL

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
