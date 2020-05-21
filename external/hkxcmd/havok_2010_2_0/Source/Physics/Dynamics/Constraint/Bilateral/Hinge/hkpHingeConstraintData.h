/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_HINGE_CONSTRAINT_H
#define HK_DYNAMICS2_HINGE_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

extern const class hkClass hkpHingeConstraintDataAtomsClass;

extern const hkClass hkpHingeConstraintDataClass;

/// Basic hinge constraint
class hkpHingeConstraintData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpHingeConstraintData();

			/// Set the data for a Hinge constraint with information given in body space.
			/// \param pivotA The constraint pivot point, specified in bodyA space.
			/// \param pivotB The constraint pivot point, specified in bodyB space.
			/// \param axisA The hinge axis, specified in bodyA space.
			/// \param axisB The hinge axis, specified in bodyB space.
		void setInBodySpace(const hkVector4& pivotA, const hkVector4& pivotB, const hkVector4& axisA, const hkVector4& axisB);
	
			/// Set the data for a Hinge constraint with information given in world space.
			/// \param bodyA The first rigid body transform
			/// \param bodyB The second rigid body transform
			/// \param pivot The pivot point, specified in world space.
			/// \param axis The hinge axis, specified in world space.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform, const hkVector4& pivot, const hkVector4& axis);
		
			/// Sets the maximum impulse that can be applied by this constraint.
			/// Set it to HK_REAL_MAX to effectively disable the limit.
		virtual void setMaxLinearImpulse(hkReal maxImpulse);

			/// Gets the maximum impulse that can be applied by this constraint.
		virtual hkReal getMaxLinearImpulse() const;

			/// Choose the body to be notified when the constraint's maximum impulse is breached.
		virtual void setBodyToNotify(int bodyIdx);

			/// Returns the index of the body that is notified when the constraint's maximum impulse limit is breached.
		virtual hkUint8 getNotifiedBodyIndex() const;

			/// Check consistency of constraint members
		virtual hkBool isValid() const;

			/// Get type from this constraint
		virtual int getType() const;

			/// Sets the solving method for this constraint. Use one of the hkpConstraintAtom::SolvingMethod as a value for method.
		virtual void setSolvingMethod(hkpConstraintAtom::SolvingMethod method);

			/// Gets the inertia stabilization factor, returns HK_FAILURE if the factor is not defined for the given constraint.
		virtual hkResult getInertiaStabilizationFactor(hkReal& inertiaStabilizationFactorOut) const;

			/// Sets the inertia stabilization factor, return HK_FAILURE if the factor is not defined for the given constraint.
		virtual hkResult setInertiaStabilizationFactor(const hkReal inertiaStabilizationFactorIn);

	public:
			//
			//	Solver interface
			//
		enum 
		{
			SOLVER_RESULT_ANG_0 = 0,		// angular constraint 0
			SOLVER_RESULT_ANG_1 = 1,		// angular constraint 1
			SOLVER_RESULT_LIN_0 = 2,		// linear constraint defined by m_constrainedAxis1B
			SOLVER_RESULT_LIN_1 = 3,		// linear constraint defined by m_constrainedAxis2B
			SOLVER_RESULT_LIN_2 = 4,		// linear constraint defined by m_constrainedAxis2B
			SOLVER_RESULT_MAX = 5 // keep this in sync with the hkSolverResult array below
		};		

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpHingeConstraintData::Runtime );

			class hkpSolverResults m_solverResults[5/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];
		};

		static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }

	public:	

		struct Atoms
		{
			// +version(1)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpHingeConstraintData::Atoms );
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalTransformsConstraintAtom	m_transforms;
			struct hkpSetupStabilizationAtom			m_setupStabilization;
			struct hkp2dAngConstraintAtom				m_2dAng;
			struct hkpBallSocketConstraintAtom			m_ballSocket;

			enum Axis
			{
				AXIS_AXLE = 0
			};

			inline Atoms(){}

				// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_transforms; }

				// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_ballSocket + 1); }

			Atoms(hkFinishLoadedObjectFlag f)
			:	m_transforms(f)
			,	m_setupStabilization(f)
			,	m_2dAng(f)
			,	m_ballSocket(f)
			{}
		};
		
		HK_ALIGN16( struct Atoms m_atoms );

	public:

			// hkpConstraintData interface implementations
		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const ;

			// hkpConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;
	public:

		hkpHingeConstraintData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}
};

#endif //HK_DYNAMICS2_HINGE_CONSTRAINT_H

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
