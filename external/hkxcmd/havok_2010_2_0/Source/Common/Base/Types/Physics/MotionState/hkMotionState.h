/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MATH_MOTION_STATE_H
#define HK_MATH_MOTION_STATE_H

#include <Common/Base/hkBase.h>

extern const class hkClass hkMotionStateClass;

	/// An hkMotionState basically is the dynamic state of
	/// a rigid body, which is used for collision detection and basic
	/// integration. It includes basically an hkSweptTransform.
	/// It also holds a transform which caches the position and orientation
	/// of the body at time t1.
class hkMotionState
{
	// +version(1)
	public:
		
		HK_DECLARE_REFLECTION();

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkMotionState);

			/// Empty constructor, does not initialize anything
		inline hkMotionState();

			/// Initialize with position and rotation
		void initMotionState( const hkVector4& position, const hkQuaternion& rotation );

			/// Get a non-const reference to the transform.
		inline hkTransform& getTransform();

			/// Get a const reference to the transform.
		inline const hkTransform& getTransform() const;


			/// Get a const reference to the swept transform.
		inline const hkSweptTransform& getSweptTransform() const;

			/// Get a non const reference to the transform.
		inline hkSweptTransform& getSweptTransform();

	protected:

		hkTransform m_transform;

			// a swept transform
		class hkSweptTransform m_sweptTransform;

	public:
		
			/// The angular velocity * dt which was used in the last integration step.
			/// Note: .w = m_deltaAngle.length3()
		hkVector4 m_deltaAngle; 

			/// A sphere around the center of mass which completely encapsulates the object
		hkReal m_objectRadius;

			/// linear damping
		hkHalf	m_linearDamping;

			/// angular damping
		hkHalf	m_angularDamping;

			/// time factor.
		hkHalf	m_timeFactor;

			/// The maximum linear velocity
		hkUFloat8 m_maxLinearVelocity;

			/// The maximum angular velocity. The units used here are radians per second.
			/// Note: there is also a hard limit in the engine which is 170 degrees/timestep.
		hkUFloat8 m_maxAngularVelocity;

			// Note: the next variables actually belong to hkpMotion, however they can cheaply put here (padding!)
			// The deactivation class of the motion state as used internally
		hkUint8	m_deactivationClass;

};

#include <Common/Base/Types/Physics/MotionState/hkMotionState.inl>

#endif // HK_MATH_MOTION_STATE_H

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
