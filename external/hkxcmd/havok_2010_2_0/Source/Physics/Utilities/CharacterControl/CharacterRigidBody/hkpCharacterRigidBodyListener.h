/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CHARACTER_RIGIDBODY_LISTENER__H
#define HK_CHARACTER_RIGIDBODY_LISTENER__H

#include <Physics/Dynamics/Entity/hkpEntity.h>
#include <Physics/Dynamics/World/hkpWorld.h>

class hkpCharacterRigidBody;
class hkpCapsuleShape;
class hkpWorldPostSimulationListener;
class hkpSimpleConstraintContactMgr;

/// A listener which modifies the interaction of a hkpCharacterRigidBody's rigid body for
/// maxSlope, maxForce and welding.
class hkpCharacterRigidBodyListener : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Destructor.
		virtual ~hkpCharacterRigidBodyListener() { }

	public:

			/// Called after every simulation step.
			/// Calls discardVerticalPoints and then processActualPoints.
		virtual void characterCallback( hkpWorld* world, hkpCharacterRigidBody* characterRB );

	public:

			/// Remove all additional vertical points we added in the last step.
		void discardVerticalPoints( hkpCharacterRigidBody* characterRB );

			/// Applies processActualPoints to each of the character's collision entries.
		void processActualPoints( hkpWorld* world, hkpCharacterRigidBody* characterRB );

			/// Process a collision entry for contact points for unwelding, maxSlope and maxForce.
			/// Calls unweldContactPoints, considerCollisionEntryForSlope and considerCollisionEntryForMassModification.
		virtual void processActualPoints( const hkpWorld* world, hkpCharacterRigidBody* characterRB, const hkpLinkedCollidable::CollisionEntry& entry, hkpSimpleConstraintContactMgr* mgr, hkArray<hkContactPointId>& contactPointIds );

	public:
			/// Within the specified range, reorient welded contact point normals.
		virtual void unweldContactPoints( hkpCharacterRigidBody* characterRB, const hkpLinkedCollidable::CollisionEntry& entry, hkpSimpleConstraintContactMgr* mgr, const hkArray<hkContactPointId>& contactPointIds );

			/// Traverse all the contact points in a collision entry, adding vertical planes as necessary.
		virtual void considerCollisionEntryForSlope( const hkpWorld* world, hkpCharacterRigidBody* characterRB, const hkpLinkedCollidable::CollisionEntry& entry, hkpSimpleConstraintContactMgr* mgr, hkArray<hkContactPointId>& contactPointIds );

			/// For each collision entry, limit the responses involved based on the character's m_maxForce value.
		virtual void considerCollisionEntryForMassModification( const hkpWorld* world, hkpCharacterRigidBody* characterRB, const hkpLinkedCollidable::CollisionEntry& entry, hkpSimpleConstraintContactMgr* mgr, const hkArray<hkContactPointId>& contactPointIds );		
};

#endif //HK_CHARACTER_RIGIDBODY_LISTENER__H

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
