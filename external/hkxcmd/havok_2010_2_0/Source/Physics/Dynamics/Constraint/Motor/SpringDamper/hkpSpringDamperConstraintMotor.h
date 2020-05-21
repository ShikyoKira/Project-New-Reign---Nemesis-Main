/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_SPRING_DAMPER_CONSTRAINT_MOTOR_H
#define HK_DYNAMICS2_SPRING_DAMPER_CONSTRAINT_MOTOR_H

#include <Physics/Dynamics/Constraint/Motor/hkpLimitedForceConstraintMotor.h>

extern const hkClass hkpSpringDamperConstraintMotorClass;

	/// this motor works like a spring. It tries to reach a given target position
	/// using an angular spring which has a spring constant.
	/// Due to the implementation, the maximum spring constant is limited and
	/// will be clipped. Its stiffness can never exceed 1/numberOfSolverSteps
	/// of the stiffness of the hkpPositionConstraintMotor (when using a tau of 1).
class hkpSpringDamperConstraintMotor : public hkpLimitedForceConstraintMotor
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Will zero all values except maxForce, which is set to a very high value.
		hkpSpringDamperConstraintMotor();

			/// Construct a motor with the given properties.
		hkpSpringDamperConstraintMotor( hkReal springConstant, hkReal springDamping, hkReal maxForce  = HK_REAL_MAX);

			/// Motor clone method.
		virtual hkpConstraintMotor* clone() const;

	public:
			/// The spring constant in N/m.
		hkReal m_springConstant;

			/// The spring damping in Nsec/m.
		hkReal m_springDamping;

	public:

		hkpSpringDamperConstraintMotor(hkFinishLoadedObjectFlag f) : hkpLimitedForceConstraintMotor(f)  {}
};


#endif // HK_DYNAMICS2_SPRING_DAMPER_CONSTRAINT_MOTOR_H

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
