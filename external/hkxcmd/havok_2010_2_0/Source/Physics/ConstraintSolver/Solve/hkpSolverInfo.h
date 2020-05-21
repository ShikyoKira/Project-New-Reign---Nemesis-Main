/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONSTRAINTSOLVER2_SOLVER_INFO_H
#define HK_CONSTRAINTSOLVER2_SOLVER_INFO_H

#include <Common/Base/hkBase.h>

	/// Parameters for the solver.
struct hkpSolverInfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpSolverInfo );

	// Class to decide when an object is slow and what to do<br>
	//  - An object is recognized as a slow object if
	//     - all components of normalizedVelocity are less than 1
	//		- normalizedVelocity = m_linearVelocityThresholdInv * linearVelocity + m_angularVelocityThresholdInv * angularVelocity

	enum DeactivationClass
	{
		DEACTIVATION_CLASS_INVALID, // same as off
		DEACTIVATION_CLASS_OFF,
		DEACTIVATION_CLASS_LOW,
		DEACTIVATION_CLASS_MEDIUM,
		DEACTIVATION_CLASS_HIGH,
		DEACTIVATION_CLASS_AGGRESSIVE,
		DEACTIVATION_CLASSES_END
	};

	struct DeactivationInfo
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpSolverInfo::DeactivationInfo );

		//
		//	The next values are used for solver deactivation.
		//  This type of deactivation just stabilizes slow objects
		//  it does not save CPU work
		//
		float m_linearVelocityThresholdInv;
		float m_angularVelocityThresholdInv;
		float m_slowObjectVelocityMultiplier;
		float m_relativeSleepVelocityThreshold;

		//
		//	The next values are used for a real deactivation.
		//  They do save CPU work
		//

			/// the maximum distance sqrd a deactivation candidate is allowed to move
			/// There are 2 settings: The first is the normal one and will trigger a deactivation
			/// after 20 frames, the second one is an emergency deactivation and will trigger after 80 frames
		float m_maxDistSqrd[2];

			/// Same as m_maxDistSqrd, but instead of using the distance traveled by the mass center
			/// this is the distance traveled by any point on the object caused by rotation only
		hkHalf m_maxRotSqrd[2];
	};


	//
	//	Static members
	//
	HK_ALIGN16( float		m_one );
	float		m_tau;
	float		m_damping;			// [ 0.0f .. 1.0f ]
	float		m_frictionTau;

	hkVector4	m_globalAccelerationPerSubStep;
	hkVector4	m_globalAccelerationPerStep;

	hkVector4	m_integrateVelocityFactor;
	hkVector4	m_invIntegrateVelocityFactor;

		// tau/damping
	float		m_dampDivTau;
	float		m_tauDivDamp;
	float		m_dampDivFrictionTau;
	float		m_frictionTauDivDamp;



	inline void setTauAndDamping( float tau, float damping )
	{
		m_tau = tau;
		m_frictionTau = 0.5f * tau;
		m_damping = damping;
		m_dampDivTau = damping / tau;
		m_dampDivFrictionTau = damping / m_frictionTau;
		m_tauDivDamp = tau/damping;
		m_frictionTauDivDamp = m_frictionTau/damping;
		m_integrateVelocityFactor.setAll3( m_tauDivDamp );
		m_invIntegrateVelocityFactor.setAll3( m_dampDivTau );
	}


	float		m_contactRestingVelocity;	// [0.0 .. 0.5f if the contact velocity is less, restitution will be set to 0
	DeactivationInfo m_deactivationInfo[ DEACTIVATION_CLASSES_END ];

	//
	//	Time step information
	//
	float		m_deltaTime;
	float		m_invDeltaTime;
	int			m_numSteps;
	int  		m_numMicroSteps;
	float		m_invNumMicroSteps;
	float		m_invNumSteps;

	hkBool		m_forceCoherentConstraintOrderingInSolver;


	// <os.todo.a> change those counters to be more intuitive.
	inline void incrementDeactivationFlags() 
	{
		m_deactivationIntegrateCounter++;
		// we need each consecutive bi-flag to be not a negation of the previous one
		if (((m_deactivationIntegrateCounter+4) & 7) == 0)
		{
			m_deactivationNumInactiveFramesSelectFlag[0] ^= 0x01; // flip high-freq flag
		}
		if ((m_deactivationIntegrateCounter & 7) == 0)
		{
			m_deactivationNumInactiveFramesSelectFlag[0] ^= 0x02; // flip high-freq flag
		}

		if ((m_deactivationIntegrateCounter & 15) == 0)
		{
			m_deactivationNumInactiveFramesSelectFlag[1] = 1 - m_deactivationNumInactiveFramesSelectFlag[1]; // flip low-freq flag
			m_deactivationIntegrateCounter = 0; // loop counter
		}
	}

	hkUint8			m_deactivationNumInactiveFramesSelectFlag[2];
	hkUint8			m_deactivationIntegrateCounter;

	float			m_maxConstraintViolationSqrd;
};

struct hkp1dMotorSolverInfo 
{
    HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkp1dMotorSolverInfo );
    
    /// max impulse that can be applied forward
    hkReal				m_maxImpulsePerSubstep;
    
    /// max impulse that can be applied backwards. This is a negative value. This has not been tested for positive values!
    hkReal				m_minImpulsePerSubstep;
    
    /// the velocity target
    hkReal				m_velocity;
    
    /// The strength of the constraint between 0 and 1
    hkReal				m_tau; // Consider changing this to a hkHalf
    
    /// The damping of the motor between 0 and 1
    hkReal				m_damping; // Consider changing this to a hkHalf
};

struct hkp3dAngularMotorSolverInfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkp3dAngularMotorSolverInfo );

	typedef hkUint8 Status;

	enum Mask
	{

		ANGULAR_1ST = 0x03,
		ANGULAR_2ND = 0x0c,
		ANGULAR_3RD = 0x30,
	};

	enum MotorState
	{
			// Values:

			// 0b00 -- ok, not broken
		MOTOR_NOT_BROKEN          = 0x00,

			// 0b01 -- broken, max breached
		MOTOR_BROKEN_MAX_BREACHED = 0x01,

			// 0b11 -- broken, min breached
		MOTOR_BROKEN_MIN_BREACHED = 0x03,

			// 0b10 -- illegal, (might be used for marking excluded/disabled motors)
		MOTOR_DISABLED = 0x02
	};


	// Flags that mark individual angular constraints as broken.
	Status m_broken;
	
	inline int getState(int idx) const 
	{
		HK_ASSERT2(0xad67d8d8, idx >= 0  && idx <= 2, "Idx must be in {1,2,3}.");
		return (m_broken >> (2 * idx)) & ANGULAR_1ST; 
	}

	inline void setState(int idx, int value)
	{
		HK_ASSERT2(0xad67d8d8, idx >= 0  && idx <= 2, "Idx must be in {1,2,3}.");
		const int shiftedValue  = (value & ANGULAR_1ST) << (2 * idx);
		const int shiftedMask   = ANGULAR_1ST << (2 * idx);

		m_broken &= hkUint8(~shiftedMask);
		m_broken |= hkUint8(shiftedValue);
	}

	// Holds motor information for each of the angular constraints.
	hkp1dMotorSolverInfo m_motorInfos[3];
};

	/// A small helper class to collect violated constraints (including SPU)
	/// Note that this has a unchangeable maximum capacity.
	/// Check out hkpWorldCinfo::m_maxConstraintViolation
struct hkpViolatedConstraintArray
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpViolatedConstraintArray );
		enum { CAPACITY = 128 };

		hkpViolatedConstraintArray(){ reset(); }

		void reset(){ m_nextFreeElement = 0; }

	public:
		hkUint32						m_nextFreeElement;			///< the next free element, might be greater than capacity.
		class hkpConstraintInstance*	m_constraints[CAPACITY];	///< A buffer holding up to CAPACITY violated constraints, the overflow will be ignored.
};


#endif // HK_CONSTRAINTSOLVER2_SOLVER_INFO_H

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
