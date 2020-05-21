/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_RIGID_BODY_RESET_UTIL_H
#define HKP_RIGID_BODY_RESET_UTIL_H

#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Physics/Dynamics/Motion/hkpMotion.h>

class hkpRigidBody;
class hkpWorld;

/// Reset a breakable body to its unbroken state,
/// This includes physics and graphics as well.
///
/// \sa hkdBreakableBody
class hkpRigidBodyResetUtil : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DYNAMICS);

			/// Create an instance which will track the specified unbroken breakable body.
			/// Stores the original transform if supplied, otherwise uses the breakable body's current position
			/// as the original transform.
		hkpRigidBodyResetUtil(hkpRigidBody* rigidBody);

			/// Destructor.
		virtual ~hkpRigidBodyResetUtil();


			/// Returns a pointer to the unbroken breakable body we are watching
		hkpRigidBody* getRigidBody() const { return m_mainRB;}


			/// Resets the main rigid body's transform and velocity
		virtual void resetRB(hkpWorld* world);

		bool wasOriginalBodyActive() const { return m_originalBodyWasActive; }

	private:

			// Do not copy.
		hkpRigidBodyResetUtil(const hkpRigidBodyResetUtil& a);

			// Do not copy.
		hkpRigidBodyResetUtil& operator=(const hkpRigidBodyResetUtil& rhs);

		// These member variables can be overridden by the user
	public:
			/// original transform of the main rigid body. Used when the body is reset.
		hkTransform			m_originalTransform;
			/// original linear velocity of the main rigid body. Used when the body is reset.
		hkVector4			m_originalLinearVelocity;
			/// original angular velocity of the main rigid body. Used when the body is reset.
		hkVector4			m_originalAngularVelocity;
			/// original angular velocity of the main rigid body. Used when the body is reset.
		hkpMotion::MotionType m_originalMotionType;

	protected:
			/// The rigid body to reset
		hkpRigidBody*		m_mainRB;

		hkBool				m_originalBodyWasActive;

};

#endif // HKP_RIGID_BODY_RESET_UTIL_H

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
