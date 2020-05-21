/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_LIMITED_HINGE_CONSTRAINT_H
#define HK_LIMITED_HINGE_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

extern const class hkClass hkpLimitedHingeConstraintDataAtomsClass;

extern const hkClass hkpLimitedHingeConstraintDataClass;

/// Full hinge constraint with limits and motor.
/// 
/// By default the motor is disabled. When the motor is enabled friction is automatically disabled.
class hkpLimitedHingeConstraintData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpLimitedHingeConstraintData();

		~hkpLimitedHingeConstraintData();

			/// Set the data for a Limited Hinge constraint with information given in body space.
			/// \param pivotA The constraint pivot point, specified in bodyA space.
			/// \param pivotB The constraint pivot point, specified in bodyB space.
			/// \param axisA The hinge axis, specified in bodyA space.
			/// \param axisB The hinge axis, specified in bodyB space.
			/// \param axisAPerp Axis perpendicular to the hinge axis, specified in bodyA space.
			/// \param axisBPerp Axis perpendicular to the hinge axis, specified in bodyB space.
		void setInBodySpace(const hkVector4& pivotA, const hkVector4& pivotB, 
								const hkVector4& axisA, const hkVector4& axisB, 
								const hkVector4& axisAPerp, const hkVector4& axisBPerp);

			/// Set the data for a Limited Hinge constraint with information given in world space.
			/// \param bodyA The first rigid body transform.
			/// \param bodyB The second rigid body transform.
			/// \param pivot The pivot point, specified in world space.
			/// \param axis	The hinge axis, specified in world space.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform, 
								const hkVector4& pivot, const hkVector4& axis);
			
			/// Sets the maximum impulse that can be applied by this constraint.
			/// Set it to HK_REAL_MAX to not limit the maximum impulse.
		virtual void setMaxLinearImpulse(hkReal maxImpulse);

			/// Gets the maximum impulse that can be applied by this constraint.
		virtual hkReal getMaxLinearImpulse() const;

			/// Choose the body to be notified when the constraint's maximum impulse is breached.
		virtual void setBodyToNotify(int bodyIdx);

			/// Returns the index of the body that is notified when the constraint's maximum impulse limit is breached.
		virtual hkUint8 getNotifiedBodyIndex() const;

			/// Check consistency of constraint members
		virtual hkBool isValid() const;
	
		/* Methods to set and get angle limits and friction */

			/// Sets the friction value. Set this before adding to the system.
			/// Note that this value is an absolute torque value and is therefore dependent on the masses of constrained bodies
			/// and not limited between 0.0f and 1.0f.
			/// If trying to stiffen up hinge constraints, try setting this value sufficiently high so that constraints are completely stiff and then
			/// reduce until the desired behaviour has been achieved.
		inline void setMaxFrictionTorque(hkReal tmag);

			/// Gets the friction value.
		inline hkReal getMaxFrictionTorque() const;

			/// Sets the maximum angular limit.
		inline void setMaxAngularLimit(hkReal rad);

			/// Sets the minimum angular limit.
		inline void setMinAngularLimit(hkReal rad);

			/// Gets the maximum angular limit.
		inline hkReal getMaxAngularLimit() const;

			/// Gets the minimum angular limit.
		inline hkReal getMinAngularLimit() const;

			/// Sets the m_angularLimitsTauFactor. This is a factor in the range between 0 and 1
			/// which controls the stiffness of the angular limits. If you slowly increase
			/// this value from 0 to 1 for a newly created constraint,
			/// you can nicely blend in the limits.
		inline void setAngularLimitsTauFactor( hkReal mag );

			/// Get the m_angularLimitsTauFactor;
 		inline hkReal getAngularLimitsTauFactor() const;

			/// This is the preferable way to disable angular limits when motor is active.
		inline void disableLimits();

		//
		// Motor-related methods
		//

			/// Get the motor. Default is HK_NULL
		inline hkpConstraintMotor* getMotor() const;

			/// Is this motor on ?
		inline hkBool isMotorActive() const;

			/// Turn the motor on or off
		void setMotorActive( hkpConstraintInstance* instance, hkBool toBeEnabled );

			/// Set the motor. Setting this to null will disable any motor computations.
			/// The angle passed to the hkpConstraintMotor::motor() callback will be the relative angle
			/// between the attached and reference body. You need to set the desired target angle of
			/// your motor each step.
			/// increments reference of new motor, decrements counter of replaced motor ( if any )
		void setMotor( hkpConstraintMotor* motor );

			/// Sets the target angle for the motor. Only used by motors which use positions
		inline void setMotorTargetAngle( hkReal angle );

			/// Gets the target angle for the motor
		inline hkReal getMotorTargetAngle() const;

			/// Sets the solving method for this constraint. Use one of the hkpConstraintAtom::SolvingMethod as a value for method.
		virtual void setSolvingMethod(hkpConstraintAtom::SolvingMethod method);

			/// Gets the inertia stabilization factor, returns HK_FAILURE if the factor is not defined for the given constraint.
		virtual hkResult getInertiaStabilizationFactor(hkReal& inertiaStabilizationFactorOut) const;

			/// Sets the inertia stabilization factor, return HK_FAILURE if the factor is not defined for the given constraint.
		virtual hkResult setInertiaStabilizationFactor(const hkReal inertiaStabilizationFactorIn);

		//
		//
		//


			/// Get type from this constraint
		virtual int getType() const;

		enum 
		{
			SOLVER_RESULT_MOTOR = 0,		// the motor
			SOLVER_RESULT_FRICTION = 1,		// or friction 
			SOLVER_RESULT_LIMIT = 2,		// limits defined around m_freeAxisA 
			SOLVER_RESULT_ANG_0 = 3,		// angular constraint 0
			SOLVER_RESULT_ANG_1 = 4,		// angular constraint 1

			SOLVER_RESULT_LIN_0 = 5,		// linear constraint 
			SOLVER_RESULT_LIN_1 = 6,		// linear constraint 
			SOLVER_RESULT_LIN_2 = 7,		// linear constraint 
			SOLVER_RESULT_MAX = 8
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpLimitedHingeConstraintData::Runtime );

				// Solver results must always be in the first position
			HK_ALIGN16( class hkpSolverResults m_solverResults[8/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/] );

				// To tell whether the previous angle has been initialized.
			hkUint8 m_initialized;
	
				// The previous target angle
			hkReal m_previousTargetAngle;

				/// Returns current angle position
			inline hkReal getCurrentPos() const;
		};

		static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }


	public:

		struct Atoms
		{
			// +version(1)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpLimitedHingeConstraintData::Atoms );
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalTransformsConstraintAtom	m_transforms;
			struct hkpSetupStabilizationAtom			m_setupStabilization;
			struct hkpAngMotorConstraintAtom           	m_angMotor;
			struct hkpAngFrictionConstraintAtom        	m_angFriction;
			struct hkpAngLimitConstraintAtom           	m_angLimit; 
			struct hkp2dAngConstraintAtom			  	m_2dAng;
			struct hkpBallSocketConstraintAtom			m_ballSocket;
		
			enum Axis
			{
				AXIS_AXLE = 0,
				AXIS_PERP_TO_AXLE_1 = 1,
				AXIS_PERP_TO_AXLE_2 = 2
			};

			Atoms(){}

			// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_transforms; }

			// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_ballSocket + 1); }

			Atoms(hkFinishLoadedObjectFlag f)
			:	m_transforms(f)
			,	m_setupStabilization(f)
			,	m_angMotor(f)
			,	m_angFriction(f)
			,	m_angLimit(f)
			,	m_2dAng(f)
			,	m_ballSocket(f)
			{}
		};
		
		HK_ALIGN16( struct Atoms m_atoms );


	public:

			// hkpConstraintData interface implementations
		virtual void getConstraintInfo( ConstraintInfo& infoOut ) const ;

			// hkpConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:

		hkpLimitedHingeConstraintData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}

};

#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.inl>

#endif

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
