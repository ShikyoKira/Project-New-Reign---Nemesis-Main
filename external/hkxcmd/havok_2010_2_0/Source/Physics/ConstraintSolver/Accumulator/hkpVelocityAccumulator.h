
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_CONSTRAINTSOLVER2_VELOCITY_ACCUMULATOR_H
#define HK_CONSTRAINTSOLVER2_VELOCITY_ACCUMULATOR_H

	/// This is a simplified Rigid body as it is used by the
	/// constraint solver.
	/// Note: This class has different purposes, whether it is used for setting
	/// up the constraint solver input (hkConstraintDate::buildJacobian()), or when it is
	/// used as a solver output.
class hkpVelocityAccumulator 
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpVelocityAccumulator );

	    /// A list of possible types
	    enum hkpAccumulatorType
	    {
		    /// A rigid body
		    HK_RIGID_BODY,

				/// A fixed rigid body, which ignores gravity
			HK_KEYFRAMED_RIGID_BODY,

				/// A rigid body which has no gravity applied (used for character controller)
			HK_NO_GRAVITY_RIGID_BODY,

			HK_END
		};

		/// A list of possible contexts the accumulator can be used in
		enum hkpAccumulatorContext
		{
				/// Accumulator is used in the solver setup stage, where the Jacobians / schemas are created
			ACCUMULATOR_CONTEXT_BUILD_JACOBIANS,

				/// Accumulator is used inside the solver iterations
			ACCUMULATOR_CONTEXT_SOLVER,
		};

			/// The valid context the accumulator can be used in
		hkEnum<hkpAccumulatorType,hkUint8> m_type;

		hkEnum<hkpAccumulatorContext, hkUint8> m_context;

			/// An index into hkpSolverInfo::m_deactivationInfo which is taken from
			/// hkpRigidBodyCinfo::m_solverDeactivation
		hkUint32 m_deactivationClass;

			/// Scaling factor for gravity
		hkReal m_gravityFactor;


			/// the linear velocity
		hkVector4 m_linearVel;

			/// the angular velocity in principle axis space
		hkVector4 m_angularVel;

			/// the inverted diagonal of the inertia tensor in local space.
			/// plus the w component is the inverted mass
		hkVector4 m_invMasses;

		//
		//	Solver input accessors
		// 

			/// Sets the center of mass in world space. Works in both contexts!
		void setCenterOfMassInWorld(const hkVector4& worldCenterOfMass)
		{
			m_scratch3 = worldCenterOfMass;
		}

			/// Get center of mass in world space. Works in both contexts!
		const hkVector4& getCenterOfMassInWorld() const
		{
			return m_scratch3;
		}

			/// Set the matrix converting from world to principle axis space
			/// Note: This only works before the constraint solver is called (e.g., in hkConstraintDate::buildJacobian())
		void setCoreFromWorldMatrix(const hkMatrix3& rot)
		{
			// We need to be in build Jacobians mode!
			HK_ASSERT(0x6aede27a, m_context == ACCUMULATOR_CONTEXT_BUILD_JACOBIANS);
			*(hkRotation*)&m_scratch0 = *(const hkRotation*)&rot;
		}

			/// Sets the matrix converting from world to principle axis space to identity
			/// Note: This only works before the constraint solver is called (e.g., in hkConstraintDate::buildJacobian())
		HK_FORCE_INLINE void setCoreFromWorldMatrixToIdentity()
		{
			// We need to be in build Jacobians mode!
			HK_ASSERT(0x6aede27a, m_context == ACCUMULATOR_CONTEXT_BUILD_JACOBIANS);
			m_scratch0 = hkVector4::getConstant<HK_QUADREAL_1000>();
			m_scratch1 = hkVector4::getConstant<HK_QUADREAL_0100>();
			m_scratch2 = hkVector4::getConstant<HK_QUADREAL_0010>();
		}

			/// Get The matrix converting from world to principle axis space
			/// Note: This only works before the constraint solver is called (e.g., in hkConstraintDate::buildJacobian())
		const hkRotation& getCoreFromWorldMatrix() const
		{
			// We need to be in build Jacobians mode!
			HK_ASSERT(0x6aede27a, m_context == ACCUMULATOR_CONTEXT_BUILD_JACOBIANS);
			return *(const hkRotation*)&m_scratch0;
		}

			/// Get the average velocity during constraint solving multiplied by the number of substeps
			/// Note: this function only works after the constraint solver is called
		hkVector4& getSumLinearVel()
		{
			// We need to be in solver mode!
			HK_ASSERT(0x5d9b1d6a, m_context == ACCUMULATOR_CONTEXT_SOLVER);
			return m_scratch0;
		}

			/// Get the average velocity in principle axis space during constraint solving multiplied by the number of substeps
			/// Note: this function only works after the constraint solver is called
		hkVector4& getSumAngularVel()
		{
			// We need to be in solver mode!
			HK_ASSERT(0x5d9b1d6a, m_context == ACCUMULATOR_CONTEXT_SOLVER);
			return m_scratch1;
		}

			/// Get the average velocity during constraint solving multiplied by the number of substeps
			/// Note: this function only works after the constraint solver is called
		const hkVector4& getSumLinearVel() const
		{
			// We need to be in solver mode!
			HK_ASSERT(0x5d9b1d6a, m_context == ACCUMULATOR_CONTEXT_SOLVER);
			return m_scratch0;
		}

			/// Get the average velocity during constraint solving multiplied by the number of substeps
			/// Note: this function only works after the constraint solver is called
		const hkVector4& getSumAngularVel() const
		{
			// We need to be in solver mode!
			HK_ASSERT(0x5d9b1d6a, m_context == ACCUMULATOR_CONTEXT_SOLVER);
			return m_scratch1;
		}

			/// Note: this function only works after the constraint solver is called
		hkQuaternion& getCoreFromWorldQuaternion()
		{
			// We need to be in solver mode!
			HK_ASSERT(0x5d9b1d6a, m_context == ACCUMULATOR_CONTEXT_SOLVER);
			return (hkQuaternion&)m_scratch2;
		}

		const hkQuaternion& getCoreFromWorldQuaternion() const
		{
			// We need to be in solver mode!
			HK_ASSERT(0x5d9b1d6a, m_context == ACCUMULATOR_CONTEXT_SOLVER);
			return (const hkQuaternion&)m_scratch2;
		}
        
			// Convert the accumulator for use in a solver
		HK_FORCE_INLINE void convertToSolverType()
		{
			// We need to be in build Jacobians mode in order for the transition to make sense!
			// This is checked by getCoreFromWorldMatrix
			const hkRotation& m = getCoreFromWorldMatrix();
			m_context = ACCUMULATOR_CONTEXT_SOLVER;
			getCoreFromWorldQuaternion()._set(m);
			getSumLinearVel().setZero4();
			getSumAngularVel().setZero4();
		}

		

	public:		// internal, public needed for HK_COMPILE_TIME_ASSERT
		hkVector4  m_scratch0;
		hkVector4  m_scratch1;
		hkVector4  m_scratch2;
		hkVector4  m_scratch3;

	public:
			/// Set this velocity accumulator to the one of a fixed object
		HK_FORCE_INLINE void setFixed()
		{
			m_context = ACCUMULATOR_CONTEXT_BUILD_JACOBIANS;
			m_type = hkpVelocityAccumulator::HK_KEYFRAMED_RIGID_BODY;
			m_gravityFactor = 0.0f;
			this->m_invMasses.setZero4();
			this->m_linearVel.setZero4();
			this->m_angularVel.setZero4();
			this->m_scratch3.setZero4();	// setCenterOfMassInWorld(hkVector4::getConstant<HK_QUADREAL_0>());
			setCoreFromWorldMatrixToIdentity();
		}
};

#endif // HK_CONSTRAINTSOLVER2_VELOCITY_ACCUMULATOR_H

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
