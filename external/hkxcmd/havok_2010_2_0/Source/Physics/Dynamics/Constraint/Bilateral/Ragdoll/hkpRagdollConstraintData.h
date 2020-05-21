/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_RAGDOLL_CONSTRAINT_H
#define HK_DYNAMICS2_RAGDOLL_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

extern const class hkClass hkpRagdollConstraintDataAtomsClass;

class hkpRigidBody;

extern const hkClass hkpRagdollConstraintDataClass;

/// The ragdoll constraint. You can use this constraint to realistically
/// simulate the behavior of body joints, such as hips, shoulders, and knees.
/// Ragdoll constraint allows you to activate motors to drive the 3 angles. If
/// you are using a motor with a target position to set, set it to 0. During
/// the hkpConstraintMotor::motor() callback the difference between the current
/// angle the desired angle will be passed in. This will result in your motor
/// driving the difference in the angle to zero. If the motor is enabled, all
/// friction will be disabled, however once you disable the motor, friction
/// will be enabled again.
class hkpRagdollConstraintData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpRagdollConstraintData();

			/// Will dereference any motors held.
		~hkpRagdollConstraintData();


			/// Setup a ragdoll constraint based on information in world space.
			/// Use the set angle limit methods to add in the extra data.
			/// Use the set motors and set angle limit methods to add in the extra data.
			/// Will set the target frame B to match the current setup frame for B
			/// \param bodyA			The attached body transform
			/// \param bodyB			The reference body transform
			/// \param pivot			The pivot point, specified in world space.
			/// \param twistAxisW		The twist axis, specified in world space.
			/// \param planeAxisW		The plane axis, specified in world space.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform,
						const hkVector4& pivot, const hkVector4& twistAxisW,
						const hkVector4& planeAxisW);

			/// Setup a ragdoll constraint based on information in body space.
			/// Use the set angle limit methods to add in the extra data.
			/// Use the set motors and set angle limit methods to add in the extra data.
			/// Will set the target frame B to match the current setup frame for B
			/// \param pivotA			The pivot point, specified in bodyA space.
			/// \param pivotB			The pivot point, specified in bodyB space.
			/// \param twistAxisA		The twist axis, specified in bodyA space.
			/// \param twistAxisB		The twist axis, specified in bodyB space.
			/// \param planeAxisA		The plane axis, specified in bodyA space.
			/// \param planeAxisB		The plane axis, specified in bodyB space.
		void setInBodySpace( const hkVector4& pivotA, const hkVector4& pivotB,
						const hkVector4& planeAxisA, const hkVector4& planeAxisB,
						const hkVector4& twistAxisA, const hkVector4& twistAxisB);

			/// Gets the constraint frame.
			/// \param constraintFrameA Column 0 = twist axis, Column 1 = plane, Column 2 = twist cross plane.
		void getConstraintFrameA( hkMatrix3& constraintFrameA ) const;

			/// Gets the constraint frame.
			/// \param constraintFrameB Column 0 = twist axis, Column 1 = plane, Column 2 = twist cross plane.
		void getConstraintFrameB( hkMatrix3& constraintFrameB ) const;

			/// Sets the minimum twist limit (in radians).
		inline void setTwistMinAngularLimit(hkReal rad);

			/// Sets the maximum twist limit (in radians).
		inline void setTwistMaxAngularLimit(hkReal rad);

			/// Sets the maximum plane limit (in radians).
		inline void setPlaneMaxAngularLimit(hkReal rad);

			/// Sets the minimum plane limit (in radians).
		inline void setPlaneMinAngularLimit(hkReal rad);

			/// Sets the half angle used to define the cone of possible movement (in radians).
		inline void setConeAngularLimit(hkReal rad);

			/// Enables/disables cone limit stabilization.
		void setConeLimitStabilization(hkBool enable);

			/// Sets the angle used to define the cone of possible movement in terms of
			/// a min and max angle which it bisects and then calls setConeAngularLimit()
			/// and also adjusts the twist axis in body B accordingly.
			/// The angles are specified in radians.
		void setAsymmetricConeAngle(hkReal cone_min, hkReal cone_max );

			/// Sets the friction value. Set this before adding to the system.
			/// Note that this value is an absolute torque value and is therefore
			/// dependent on the masses of constrained bodies and not limited
			/// between 0.0f and 1.0f. If trying to stiffen up ragdoll constraints,
			/// try setting this value sufficiently high so that constraints are
			/// completely stiff and then reduce until the desired behavior has
			/// been achieved.
		void setMaxFrictionTorque(hkReal tmag);

			/// Sets the m_angularLimitsTauFactor. This is a factor in the range
			/// between 0 and 1 which controls the stiffness of the angular limits.
			/// If you slowly increase this value from 0 to 1 for a newly created
			/// constraint, you can nicely blend in the limits.
		inline void setAngularLimitsTauFactor( hkReal mag );

			/// Gets the angular limits tau factor for this constraint.
 		inline hkReal getAngularLimitsTauFactor() const;

			/// Gets the minimum twist limit (in radians).
		inline hkReal getTwistMinAngularLimit() const;

			/// Gets the maximum twist limit (in radians).
		inline hkReal getTwistMaxAngularLimit() const;

			/// Gets the maximum plane limit (in radians).
		inline hkReal getPlaneMaxAngularLimit() const;

			/// Gets the minimum plane limit (in radians).
		inline hkReal getPlaneMinAngularLimit() const;

			/// Gets the cone angle limit (in radians).
		inline hkReal getConeAngularLimit() const;

			/// Gets the state of cone limit stabilization.
		inline hkBool getConeLimitStabilization() const;

			/// Gets the friction value.
		inline hkReal getMaxFrictionTorque() const;

			/// Sets the maximum impulse that can be applied by this constraint.
			/// Set it to HK_REAL_MAX to effectively disable the limit.
		virtual void setMaxLinearImpulse(hkReal maxImpulse);

			/// Gets the maximum impulse that can be applied by this constraint.
		virtual hkReal getMaxLinearImpulse() const;

			/// Choose the body to be notified when the constraint's maximum impulse is breached.
		virtual void setBodyToNotify(int bodyIdx);

			/// Returns the index of the body that is notified when the constraint's maximum impulse limit is breached.
		virtual hkUint8 getNotifiedBodyIndex() const;

			/// Sets the solving method for this constraint. Use one of the hkpConstraintAtom::SolvingMethod as a value for method.
		virtual void setSolvingMethod(hkpConstraintAtom::SolvingMethod method);

			/// Gets the inertia stabilization factor, returns HK_FAILURE if the factor is not defined for the given constraint.
		virtual hkResult getInertiaStabilizationFactor(hkReal& inertiaStabilizationFactorOut) const;

			/// Sets the inertia stabilization factor, return HK_FAILURE if the factor is not defined for the given constraint.
		virtual hkResult setInertiaStabilizationFactor(const hkReal inertiaStabilizationFactorIn);

		//
		// Motor-related methods.
		//

			/// Sets target relative orientation between the constraints reference frames attached to
			/// each of the bodies.
			/// See also: setTargetRelativeOrientationOfBodies()
		void setTarget( const hkMatrix3& target_cbRca );

			/// Sets target relative orientation between bodies.
			/// This bypasses and ignores actual orientation of the constraint space in relation to each of the bodies.
			/// See also: setTarget()
		void setTargetRelativeOrientationOfBodies( const hkRotation& bRa );

			/// Get the target frame
		void getTarget(hkMatrix3& target_cbRca );

			/// Return the motor used to drive the twist axis angle.
		hkpConstraintMotor* getTwistMotor() const;

			/// Set the motor used to drive the twist axis angle.
			/// The measured angle passed in to the motor will be a relative angle
			/// between the current deflection around the twist axis and the
			/// desired deflection. i.e., the target angle that the motor should
			/// be trying to achieve is zero.
			/// Increments reference of new motor, decrements counter of replaced
			/// motor ( if any, can be null )
		void setTwistMotor( hkpConstraintMotor* motor );

			/// Return the motor used to drive the cone axis angle.
		hkpConstraintMotor* getConeMotor() const;

			/// Set the motor used to drive the cone axis angle.
			/// The measured angle passed in to the motor will be a relative
			/// angle between the current deflection around the cone axis and
			/// the desired deflection. i.e., the target angle that the motor
			/// should be trying to achieve is zero.
			/// Increments reference of new motor, decrements counter of
			/// replaced motor (if any, can be null).
		void setConeMotor( hkpConstraintMotor* motor );

			/// Return the motor used to drive the plane axis angle.
		hkpConstraintMotor* getPlaneMotor() const;

			/// Set the motor used to drive the plane axis angle.
			/// The measured angle passed in to the motor will be a relative angle
			/// between the current deflection around the plane axis and the desired
			/// deflection. i.e., the target angle that the motor should be trying
			/// to achieve is zero.
			/// Increments reference of new motor, decrements counter of replaced
			/// motor (if any, can be null).
		void setPlaneMotor( hkpConstraintMotor* motor );

			/// Check is the motors are active.
		hkBool getMotorsActive() const;

			/// Turn the motors on or off
			/// \param instance		The constraint instance to modify.
			/// \param toBeEnabled	The desired state of the motors.
		void setMotorsActive( hkpConstraintInstance* instance, hkBool toBeEnabled );

			/// Check consistency of constraint
		hkBool isValid() const;

			/// Get type from this constraint
		virtual int getType() const;

		enum
		{
			SOLVER_RESULT_MOTOR_0 = 0,			// the angular motor value
			SOLVER_RESULT_MOTOR_1 = 1,			// the angular motor value
			SOLVER_RESULT_MOTOR_2 = 2,			// the angular motor value

			SOLVER_RESULT_FRICTION_0 = 3,		// the angular friction value
			SOLVER_RESULT_FRICTION_1 = 4,		// the angular friction value
			SOLVER_RESULT_FRICTION_2 = 5,		// the angular friction value

			SOLVER_RESULT_TWIST = 6,			// angular limits defined around m_freeAxisA
			SOLVER_RESULT_CONE  = 7,	    	// angular constraint defined by m_constrainedAxis1A
			SOLVER_RESULT_PLANE = 8,	    	// angular constraint defined by m_constrainedAxis2A

			SOLVER_RESULT_LIN_0 = 9,			// linear constraint defined by (1,0,0) in object space A
			SOLVER_RESULT_LIN_1 = 10,			// linear constraint defined by (0,1,0) in object space A
			SOLVER_RESULT_LIN_2 = 11,			// linear constraint defined by (0,0,1) in object space A

			SOLVER_RESULT_MAX = 12 // keep in sync with solver res array below
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpRagdollConstraintData::Runtime );

			HK_ALIGN16( class hkpSolverResults m_solverResults[12/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/] );

			/// Tells whether the previous target angles have been initialized.
			/// This is zeroed when created.
			hkUint8 m_initialized[3];

			/// Target angles for the individual motors
			hkReal m_previousTargetAngles[3];

			/// Soft recovery parameter for the cone limit.
			hkReal m_coneAngleOffset;
		};


		static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }

			/// hkpConstraintData interface implementations
		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const;

			/// hkpConstraintData interface implementation
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

		struct Atoms
		{
			// +version(1)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpRagdollConstraintData::Atoms );
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalTransformsConstraintAtom	m_transforms;
			struct hkpSetupStabilizationAtom			m_setupStabilization;
			struct hkpRagdollMotorConstraintAtom		m_ragdollMotors;
			struct hkpAngFrictionConstraintAtom			m_angFriction;
			struct hkpTwistLimitConstraintAtom			m_twistLimit;
			struct hkpConeLimitConstraintAtom			m_coneLimit;
			struct hkpConeLimitConstraintAtom			m_planesLimit;
			struct hkpBallSocketConstraintAtom			m_ballSocket;

			enum Axis
			{
				AXIS_TWIST = 0,
				AXIS_PLANES = 1,
				AXIS_CROSS_PRODUCT = 2
			};

			Atoms(){}

			/// Get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_transforms; }

			/// Get the size of all atoms
			/// Note: we can't use sizeof(*this) because of align16 padding.
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_ballSocket + 1); }

			Atoms(hkFinishLoadedObjectFlag f)
			:	m_transforms(f)
			,	m_setupStabilization(f)
			,	m_ragdollMotors(f)
			,	m_angFriction(f)
			,	m_twistLimit(f)
			,	m_coneLimit(f)
			,	m_planesLimit(f)
			,	m_ballSocket(f)
			{}
		};

		HK_ALIGN16( struct Atoms m_atoms );

		enum MotorIndex
		{
			MOTOR_TWIST = 0,
			MOTOR_PLANE = 1,
			MOTOR_CONE  = 2
		};

		/// Sets the motor specified by the index.
		/// Increments reference of new motor, decrements counter of
		/// replaced motor (if any, can be null).
		void setMotor(MotorIndex index, hkpConstraintMotor* newMotor);

	public:

		inline hkpRagdollConstraintData(hkFinishLoadedObjectFlag f);
};


#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.inl>

#endif // HK_DYNAMICS2_RAGDOLL_H

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
