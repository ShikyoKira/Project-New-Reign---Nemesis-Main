/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_PRISMATIC_CONSTRAINT_H
#define HK_DYNAMICS2_PRISMATIC_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

extern const class hkClass hkpPrismaticConstraintDataAtomsClass;

class hkpRigidBody;
class hkpConstraintMotor;
extern const hkClass hkpPrismaticConstraintDataClass;

/// Prismatic constraint. Free to move along an axis.
class hkpPrismaticConstraintData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpPrismaticConstraintData();

		~hkpPrismaticConstraintData();

			/// Construct a prismatic constraint with a pivot and axis in world space.
			/// \param bodyATransform	The attached body transform.
			/// \param bodyBTransform	The reference body transform.
			/// \param pivot			The pivot point, specified in world space.
			/// \param axis				The axis, specified in world space.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform,
							const hkVector4& pivot,const hkVector4& axis);

			/// Construct a prismatic constraint with a pivot and axis in the two body spaces.
			/// \param pivotA			The pivot point, specified in bodyA space.
			/// \param pivotB			The pivot point, specified in bodyB space.
			/// \param axisA			The axis, specified in bodyA space.
			/// \param axisB			The axis, specified in bodyB space.
		void setInBodySpace( const hkVector4& pivotA, const hkVector4& pivotB,
							const hkVector4& axisA, const hkVector4& axisB,
							const hkVector4& axisAPerp, const hkVector4& axisBPerp);

			/// This allows you to change this prismatic constraint into a cylindrical constraint, where
			/// rotation around the shift axis is allowed.
		inline void allowRotationAroundAxis(bool allowRotation);

		// Methods to set and get limits and friction

			///Sets the maximum limit for the attached body's movement along the axis.
		inline void setMaxLinearLimit(hkReal mmax);

			///Sets the minimum limit for the attached body's movement along the axis.
		inline void setMinLinearLimit(hkReal mmin);

			///Sets the friction value.
		inline void setMaxFrictionForce(hkReal mag);	
		
			///Gets the maximum limit for the attached body's movement along the axis.
		inline hkReal getMaxLinearLimit() const;

			///Gets the minimum limit for the attached body's movement along the axis.
		inline hkReal getMinLinearLimit() const;

			///Gets the friction value.
		inline hkReal getMaxFrictionForce() const;

			/// Set the motor. Setting this to null will disable any motor computations.
			/// increments reference of new motor, decrements counter of replaced motor ( if any )
		void setMotor( hkpConstraintMotor* motor );

			/// Get the motor.
		inline hkpConstraintMotor* getMotor() const;

			/// Sets the target position for the motor. Only used by motors which use positions
		inline void setMotorTargetPosition( hkReal position );

			/// Gets the target angle for the motor
		inline hkReal getMotorTargetPosition();

			/// Turn the motor on or off
		void setMotorActive( hkpConstraintInstance* instance, hkBool toBeEnabled );

			/// Check consistency of constraint members
		hkBool isValid() const;

			/// Get type from this constraint
		virtual int getType() const;


		enum 
		{
			SOLVER_RESULT_MOTOR = 0,		// the motor
			SOLVER_RESULT_FRICTION = 1,		// the friction

			SOLVER_RESULT_ANG_0 = 2,		// angular constraint 0
			SOLVER_RESULT_ANG_1 = 3,		// angular constraint 1
			SOLVER_RESULT_ANG_2 = 4,		// angular constraint 2
			SOLVER_RESULT_LIN_0 = 5,		// linear constraint defined by m_constrainedAxis1B
			SOLVER_RESULT_LIN_1 = 6,		// linear constraint defined by m_constrainedAxis2B

			SOLVER_RESULT_LIMIT  = 7,		// for limits 
			SOLVER_RESULT_MAX = 8 // keep in sync with solver res array below
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpPrismaticConstraintData::Runtime );

			class hkpSolverResults m_solverResults[8/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];

				// To tell whether the previous position has been initialized.
			hkUint8 m_initialized;

				// The previous target position
			hkReal m_previousTargetPosition;
		};

		static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }


		struct Atoms
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpPrismaticConstraintData::Atoms );
			HK_DECLARE_REFLECTION();

			enum Axis
			{
				AXIS_SHAFT = 0,
				AXIS_PERP_TO_SHAFT = 1
			};

			struct hkpSetLocalTransformsConstraintAtom	m_transforms;
			struct hkpLinMotorConstraintAtom				m_motor;
			struct hkpLinFrictionConstraintAtom			m_friction;
			struct hkpAngConstraintAtom					m_ang;
			struct hkpLinConstraintAtom					m_lin0;
			struct hkpLinConstraintAtom					m_lin1;
			struct hkpLinLimitConstraintAtom				m_linLimit;

			Atoms(){}

			// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_transforms; }

			// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_linLimit+1); }

			Atoms(hkFinishLoadedObjectFlag f) : m_transforms(f), m_motor(f), m_friction(f), m_ang(f), m_lin0(f), m_lin1(f), m_linLimit(f) {}
		};

		HK_ALIGN16( struct Atoms m_atoms );

	public:

		// Internal functions

		// hkpConstraintData interface implementations
		virtual void getConstraintInfo( ConstraintInfo& infoOut ) const ;

		// hkpConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:

		hkpPrismaticConstraintData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}

};

#include <Physics/Dynamics/Constraint/Bilateral/Prismatic/hkpPrismaticConstraintData.inl>

#endif // HK_DYNAMICS2_PRISMATIC_CONSTRAINT_H

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
