/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_WHEEL_CONSTRAINT_H
#define HK_DYNAMICS2_WHEEL_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

extern const class hkClass hkpWheelConstraintDataAtomsClass;

extern const hkClass hkpWheelConstraintDataClass;

/// A wheel constraint. The constraint's body A is the wheel, while its body B is the chassis.
class hkpWheelConstraintData : public hkpConstraintData
{
	public: 
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpWheelConstraintData();
			
			/// Sets the wheel up with world space information. The Wheel is A, the Chassis is B.
			/// \param bodyATransform		The transform of the attached body, i.e., the wheel.
			/// \param bodyBTransform		The transform of the reference body, i.e., the chassis.
			/// \param pivot				The pivot point, specified in world space.
			/// \param axle					The axle, specified in world space.
			/// \param suspensionAxis		The suspension axis, in world space.
			/// \param steeringAxis			The steering axis, in world space.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform, 
							const hkVector4& pivot, const hkVector4& axle, 
							const hkVector4& suspensionAxis, const hkVector4& steeringAxis);

			/// Sets the wheel up with body space information. The Wheel is A, the Chassis is B.
			/// \param pivotA				The pivot point of the wheel, specified in the wheel body space.
			/// \param pivotB				The pivot point of the chassis, specified in chassis body space.
			/// \param axleA				The axle, specified in bodyA space.
			/// \param axleB				The axle, specified in bodyB space.
			/// \param suspensionAxisB		The suspension axis, in bodyB space.
			/// \param steeringAxisB		The steering axis, in bodyB space.
		void setInBodySpace(const hkVector4& pivotA, const hkVector4& pivotB, 
							const hkVector4& axleA, const hkVector4& axleB, 
							const hkVector4& suspensionAxisB, const hkVector4& steeringAxisB);

			/// Check consistency of constraint members
		virtual hkBool isValid() const;

		    /// Sets the steering angle.
		void setSteeringAngle( hkReal angle );

			/// Get type from this constraint
		virtual int getType() const;

			/// Sets maximum limit of the suspension (0.5f by default).
		HK_FORCE_INLINE void setSuspensionMaxLimit(hkReal maxLimit);
			
			/// Sets minimum limit of the suspension (-0.5f by default).
		HK_FORCE_INLINE void setSuspensionMinLimit(hkReal minLimit);

			/// Sets suspension strength factor (0 by default).
		HK_FORCE_INLINE void setSuspensionStrength(hkReal tau);

			/// Sets suspension damping factor (0 by default).
		HK_FORCE_INLINE void setSuspensionDamping(hkReal damping);


	public:

		struct Atoms
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpWheelConstraintData::Atoms );
			HK_DECLARE_REFLECTION();

			enum Axis
			{
				AXIS_SUSPENSION = 0,
				AXIS_PERP_SUSPENSION = 1,

				AXIS_AXLE = 0,
				AXIS_STEERING = 1
			};

			struct hkpSetLocalTransformsConstraintAtom	m_suspensionBase;
			struct hkpLinLimitConstraintAtom				m_lin0Limit;
			struct hkpLinSoftConstraintAtom				m_lin0Soft;
			struct hkpLinConstraintAtom					m_lin1;
			struct hkpLinConstraintAtom					m_lin2;

			struct hkpSetLocalRotationsConstraintAtom	m_steeringBase;
			struct hkp2dAngConstraintAtom                m_2dAng;


			Atoms()
			{
				HK_CONSTRAINT_FILL_PADDING_WITH_ZERO(m_lin2,   m_steeringBase);
			}

			// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_suspensionBase; }

			// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_2dAng+1); }

			Atoms(hkFinishLoadedObjectFlag f) : m_suspensionBase(f), m_lin0Limit(f), m_lin0Soft(f), m_lin1(f), m_lin2(f), m_steeringBase(f), m_2dAng(f) {}
		};
		
		HK_ALIGN16( struct Atoms m_atoms );

		// Used for modifying the localRotation when settingSteeringAngle
		hkVector4 m_initialAxleInB;
		hkVector4 m_initialSteeringAxisInB;


		enum 
		{
			SOLVER_RESULT_ANG_0 = 0,		// angular constraint 0
			SOLVER_RESULT_ANG_1 = 1,		// angular constraint 1
			SOLVER_RESULT_LIMIT = 2,        // linear limit 2
			SOLVER_RESULT_LIN_0 = 3,		// linear constraint 
			SOLVER_RESULT_LIN_1 = 4,		// linear constraint 
			SOLVER_RESULT_TAU_AND_DAMP = 5,	// tau and damping
			SOLVER_RESULT_MAX = 6 // keep in sync with solver res array below.
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpWheelConstraintData::Runtime );

			class hkpSolverResults m_solverResults[6/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];
		};

		static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }

	public:

		// Internal functions

		// hkpConstraintData interface implementations
		virtual void getConstraintInfo( ConstraintInfo& infoOut ) const;

		// hkpConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;
	public:

		hkpWheelConstraintData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}
};

void hkpWheelConstraintData::setSuspensionMaxLimit(hkReal maxLimit)
{
	m_atoms.m_lin0Limit.m_max = maxLimit;
}

void hkpWheelConstraintData::setSuspensionMinLimit(hkReal minLimit)
{
	m_atoms.m_lin0Limit.m_min = minLimit;
}

void hkpWheelConstraintData::setSuspensionStrength(hkReal tau)
{
	m_atoms.m_lin0Soft.m_tau = tau;
}

void hkpWheelConstraintData::setSuspensionDamping(hkReal damping)
{
	m_atoms.m_lin0Soft.m_damping = damping;
}

#endif //HK_DYNAMICS2_WHEEL_CONSTRAINT_H

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
