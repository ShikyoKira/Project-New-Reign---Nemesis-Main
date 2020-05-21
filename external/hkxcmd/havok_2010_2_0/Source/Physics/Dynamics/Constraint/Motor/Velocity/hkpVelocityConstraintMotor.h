/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_VELOCITY_CONSTRAINT_MOTOR_H
#define HK_DYNAMICS2_VELOCITY_CONSTRAINT_MOTOR_H

#include <Physics/Dynamics/Constraint/Motor/hkpLimitedForceConstraintMotor.h>

extern const hkClass hkpVelocityConstraintMotorClass;

	/// this motor simply tries to reach and keep a desired target velocity
class hkpVelocityConstraintMotor : public hkpLimitedForceConstraintMotor
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Sets all values to reasonable defaults.
		inline hkpVelocityConstraintMotor();

			/// Construct a motor with the given properties.
		inline hkpVelocityConstraintMotor( hkReal velocityTarget, hkReal maxForce );

			/// Motor clone method.
		virtual hkpConstraintMotor* clone() const;


	public:

			/// The stiffness between 0 and 1.
		hkReal m_tau;

			/// Velocity target.
		hkReal m_velocityTarget;
			
			/// Tells the motor to ignore the stored velocity target.
			/// When set to true, the motor calculates the requested velocity from the target orientations
			/// supplied by the constraint owning the motor.
		hkBool m_useVelocityTargetFromConstraintTargets;

	public:

		hkpVelocityConstraintMotor(hkFinishLoadedObjectFlag f) : hkpLimitedForceConstraintMotor(f) {}
};

#include <Physics/Dynamics/Constraint/Motor/Velocity/hkpVelocityConstraintMotor.inl>

#endif // HK_DYNAMICS2_VELOCITY_CONSTRAINT_MOTOR_H

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
