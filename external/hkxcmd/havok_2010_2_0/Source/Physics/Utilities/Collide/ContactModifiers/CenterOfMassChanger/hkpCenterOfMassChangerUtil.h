/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_CENTER_OF_MASS_CHANGER_UTIL
#define HK_UTILITIES2_CENTER_OF_MASS_CHANGER_UTIL


#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

#include <Physics/Dynamics/Entity/hkpEntity.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

class hkpRigidBody;

/// This listener class can be used to virtually change the center of mass of objects during collision.
///
/// By calling setCenterOfMassForContact() in hkpResponseModifier this allows an object to
/// interact differently with one object than with another.
/// This utility is an entity listener and uses 2 rigid bodies to decide when to
/// change the center of mass and how to change it. You may implement a world listener
/// and a look-up table to change the response of any object in contact with any
/// other object.
class hkpCenterOfMassChangerUtil: public hkReferencedObject, private hkpContactListener, private hkpEntityListener
{
	public:	
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES);

			/// Adds the hkpCenterOfMassChangerUtil as a listener to bodyA.
			/// If a contact point is added between bodyA and bodyB, the center of mass is displaced for each body.
			/// The displacement vector specifies displacement in the body's local space.
		hkpCenterOfMassChangerUtil( hkpRigidBody* bodyA, hkpRigidBody* bodyB, const hkVector4& displacementA, const hkVector4& displacementB );

		~hkpCenterOfMassChangerUtil();

	protected:

		void contactPointCallback( const hkpContactPointEvent& event );

			// The hkpEntityListener interface implementation
		void entityDeletedCallback( hkpEntity* entity );


	protected:

		hkpRigidBody* m_bodyA;
		hkpRigidBody* m_bodyB;
		hkVector4 m_displacementA;
		hkVector4 m_displacementB;
};

#endif		// HK_UTILITIES2_CENTER_OF_MASS_CHANGER_UTIL

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
