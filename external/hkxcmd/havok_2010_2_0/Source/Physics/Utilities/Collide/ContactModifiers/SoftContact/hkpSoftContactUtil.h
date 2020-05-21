/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_SOFT_CONTACT_UTIL
#define HK_UTILITIES2_SOFT_CONTACT_UTIL


#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

#include <Physics/Dynamics/Entity/hkpEntity.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

class hkpRigidBody;

	/// A listener class used to soften collisions.
	///
	/// The utility scales the forces applied between bodies during collision resolution. This results in softened collisions and contact.
	/// Note that this utility should not be used for pairs of bodies between which continuous collision detection and response is performed,
	/// as this might significantly impact simulations performance.
	/// This feature is currently beta - use it with care.
class hkpSoftContactUtil: public hkReferencedObject, private hkpContactListener, private hkpEntityListener
{
	public:	
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES);

			/// Adds the soft contact util as a listener to bodyA.
			///
			/// If a contact point is added between bodyA and bodyB, the forces applied between the bodies are changed.
			/// This may be used to achieve soft contacts.
			/// If bodyB is set to NULL, than bodyA will get soft contacts with all bodies
		hkpSoftContactUtil( hkpRigidBody* bodyA, hkpRigidBody* optionalBodyB, hkReal forceScale, hkReal maxAccel );

		~hkpSoftContactUtil();

	protected:

		void contactPointCallback( const hkpContactPointEvent& event );

		void entityDeletedCallback( hkpEntity* entity );

		//
		//	Internal public section
		//
	protected:
		hkpRigidBody* m_bodyA;
		hkpRigidBody* m_bodyB;

	public:
		hkReal m_forceScale;
		hkReal m_maxAcceleration;
};

#endif		// HK_UTILITIES2_SOFT_CONTACT_UTIL

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
