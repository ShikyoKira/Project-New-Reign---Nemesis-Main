/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONSTRAINTSOLVER2_MOTOR_CONSTRAINT_PUBLIC_H
#define HK_CONSTRAINTSOLVER2_MOTOR_CONSTRAINT_PUBLIC_H

#include <Physics/ConstraintSolver/Solve/hkpSolverResults.h>

class hkpConstraintQueryIn;
class hkpConstraintQueryOut;
class hkpConstraintMotor;
class hkp2AngJacobian;
class hkp1Lin2AngJacobian;

/// A structure which is used for building Jacobian elements
class hkp1dBilateralConstraintStatus
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkp1dBilateralConstraintStatus );

	    /// The relative mass of the two objects in constraint space
	    hkPadSpu<hkReal>			m_virtualMass;
};

/// The low level input structure to set up linear and angular motors for the constraint solver
class hkp1dConstraintMotorInfo
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkp1dConstraintMotorInfo );

	    // <todo> not used in position motor
	    /// position target for the current frame.
	    hkPadSpu<hkReal>				m_targetPosition;  
    
	    /// The target velocity
	    hkPadSpu<hkReal>				m_targetVelocity;
    
	    /// max force that can be applied
	    hkPadSpu<hkReal>				m_maxForce;
    
	    /// max force that can be applied in the reverse direction
	    hkPadSpu<hkReal>				m_minForce;
    
	    /// The relative stiffness of the motor between 0..1
	    hkPadSpu<hkReal>				m_tau;
    
	    /// The relative damping of the motor between 0..1
	    hkPadSpu<hkReal>				m_damping;
};

/// The inputs for the hkpConstraintMotor::hkCalcMotorData method
class hkpConstraintMotorInput: public hkp1dBilateralConstraintStatus
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpConstraintMotorInput );

	    /// delta time information about the solver (
	    hkPadSpu<const class hkpConstraintQueryStepInfo*> m_stepInfo;
    
	    /// information from the last step
	    class hkpSolverResults m_lastResults;
    
	    /// new target minus old target
	    hkPadSpu<hkReal> m_deltaTarget;
    
	    /// old target minus current position
	    hkPadSpu<hkReal> m_positionError;
};

/// The outputs for the hkpConstraintMotor::hkCalcMotorData method
class hkpConstraintMotorOutput: public hkp1dConstraintMotorInfo
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpConstraintMotorOutput );
};

extern "C"
{
		/// Calculate the desired position and the forces we can apply to get there according
		/// to the implemented force law.
	void HK_CALL hkCalcMotorData(const hkpConstraintMotor* someMotor, const hkpConstraintMotorInput* input, hkpConstraintMotorOutput* output);

	// build jacobian element
	void HK_CALL hk1dAngularVelocityMotorBeginJacobian( hkVector4Parameter directionOfConstraint, const hkpConstraintQueryIn &in, hkp2AngJacobian& jac, hkp1dBilateralConstraintStatus& statusOut );

	// must call hk1dAngularVelocityMotorBeginJacobian first to build jacobian in hkpConstraintQueryOut
	void HK_CALL hk1dAngularVelocityMotorCommitJacobian( hkp1dConstraintMotorInfo& info, const hkpConstraintQueryIn &in, hkp2AngJacobian& jac, hkpConstraintQueryOut &out );

	// build jacobian element
	void HK_CALL hk1dLinearVelocityMotorBeginJacobian( hkVector4Parameter directionOfConstraint, hkVector4Parameter pivot, const hkpConstraintQueryIn &in, hkp1Lin2AngJacobian& jac, hkp1dBilateralConstraintStatus& statusOut );

	// must call hk1dLinearVelocityMotorBeginJacobian first to build jacobian in hkpConstraintQueryOut
	void HK_CALL hk1dLinearVelocityMotorCommitJacobian( hkp1dConstraintMotorInfo& info, const hkpConstraintQueryIn &in, hkp1Lin2AngJacobian& jac, hkpConstraintQueryOut &out );
}

#endif // HK_CONSTRAINTSOLVER_MOTOR_CONSTRAINT_PUBLIC_H

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
