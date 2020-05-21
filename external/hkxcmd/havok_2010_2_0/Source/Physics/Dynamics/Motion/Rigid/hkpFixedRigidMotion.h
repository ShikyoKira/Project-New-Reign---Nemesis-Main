/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_FIXED_RIGID_MOTION
#define HK_DYNAMICS2_FIXED_RIGID_MOTION

#include <Physics/Dynamics/Motion/Rigid/hkpKeyframedRigidMotion.h>

extern const hkClass hkpFixedRigidMotionClass;

/// This class is contained by the hkpRigidBody class.
/// It should not be accessed directly by the user.
class hkpFixedRigidMotion : public hkpKeyframedRigidMotion
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Construct a keyframed rigid body motion object, given position and rotation.
			/// Fixed rigid bodies have infinite mass and so will not move
			/// during simulation. They can never be repositioned as their
			/// bounding volume information will not be properly updated.
		hkpFixedRigidMotion( const hkVector4& position, const hkQuaternion& rotation );

	public:

			/// This method updates the saved motion with the dynamic properties of the current keyframed motion, namely
			/// m_linearVelocity, m_angularVelocity, m_rotation, m_oldCenterOfMassInWorld, m_centerOfMassInWorld and m_localToWorld.
			/// This should always be called before a "keyframed" dynamic body is "unkeyframed" again.
		virtual void getPositionAndVelocities( hkpMotion* motionOut );

		virtual void setStepPosition( hkReal position, hkReal timestep );

			/// This function asserts if it is called.
			/// You cannot set the linear velocity of a fixed rigid motion.
		virtual void setLinearVelocity(const hkVector4& newVel);

			/// This function asserts if it is called.
			/// You cannot set the angular velocity of a fixed rigid motion.
		virtual void setAngularVelocity(const hkVector4& newVel);

	public:

		hkpFixedRigidMotion( class hkFinishLoadedObjectFlag flag ) : hkpKeyframedRigidMotion( flag ) {}
};

#endif //HK_DYNAMICS2_FIXED_RIGID_MOTION

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
