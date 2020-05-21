/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS_ADD_MODIFIER_UTIL_H
#define HK_DYNAMICS_ADD_MODIFIER_UTIL_H

#include <Common/Base/Container/Array/hkArray.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

/// This utility contains static functions useful to contactListeners.
class hkpAddModifierUtil
{
	public:
		//
		// Response modifiers functions which can be used from contact point callbacks.
		//
		// To use these, one or both of the involved hkpEntities must have the appropriate flag set in its m_responseModifierFlags member.

		static void HK_CALL setInvMassScalingForContact( const hkpContactPointEvent& event, hkpRigidBody* bodyA, hkpRigidBody* bodyB, const hkVector4& factorA, const hkVector4& factorB );
		static void HK_CALL setInvMassScalingForContact( const hkpContactPointEvent& event, hkpRigidBody* body, const hkVector4& factor );
		static void HK_CALL setCenterOfMassDisplacementForContact( const hkpContactPointEvent& event, hkpRigidBody* bodyA, hkpRigidBody* bodyB, const hkVector4& displacementA, const hkVector4& displacementB );
		static void HK_CALL setImpulseScalingForContact( const hkpContactPointEvent& event, hkpRigidBody* bodyA, hkpRigidBody* bodyB, hkReal usedImpulseFraction, hkReal maxAcceleration );
		static void HK_CALL setSurfaceVelocity( const hkpContactPointEvent& event, hkpRigidBody* body, const hkVector4& velWorld );
		static void HK_CALL clearSurfaceVelocity( const hkpContactPointEvent& event, hkpRigidBody* body );
		static void HK_CALL setLowSurfaceViscosity( const hkpContactPointEvent& event );
};

#endif // HK_DYNAMICS_ADD_MODIFIER_UTIL_H

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
