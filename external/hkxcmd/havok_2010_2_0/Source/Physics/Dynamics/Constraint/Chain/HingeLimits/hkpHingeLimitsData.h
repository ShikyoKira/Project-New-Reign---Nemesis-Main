/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_HINGE_LIMITS_H
#define HK_HINGE_LIMITS_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

extern const class hkClass hkpHingeLimitsDataAtomsClass;

extern const hkClass hkpHingeLimitsDataClass;

	/// This is a version of a hkLimitedHingeConstraint with removed linear constraints.
	/// This constraint is meant to be used with hkConstraintChains.
	/// (that correspond to the ball-and-socket part). Once a chain has been defined, this constraint can be simply overlayed on top of it to impose
	/// additional angular constraints.
	/// When this constraint is used together with hkpPoweredChainData,
	/// care should be taken not to set the target in the powered chain data to lay
	/// outside the limits defined here.
class hkpHingeLimitsData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpHingeLimitsData();

			/// Set the data for a Limited Hinge constraint with information given in body space.
		void setInBodySpace(const hkVector4& axisA, const hkVector4& axisB, 
								const hkVector4& axisAPerp, const hkVector4& axisBPerp);

			/// Set the data for a Limited Hinge constraint with information given in world space.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform, const hkVector4& axis);
			
			/// Check consistency of constraint members
		virtual hkBool isValid() const;
	
		/* Methods to set and get angle limits */

			/// Sets the maximum angular limit.
		inline void setMaxAngularLimit(hkReal rad);

			/// Sets the minimum angular limit.
		inline void setMinAngularLimit(hkReal rad);

			/// Gets the maximum angular limit.
		inline hkReal getMaxAngularLimit() const;

			/// Gets the minimum angular limit.
		inline hkReal getMinAngularLimit() const;

			/// sets the m_angularLimitsTauFactor. This is a factor in the range between 0 and 1
			/// which controls the stiffness of the angular limits. If you slowly increase
			/// this value from 0 to 1 for a newly created constraint,
			/// you can nicely blend in the limits.
		inline void setAngularLimitsTauFactor( hkReal mag );
			
			/// get the m_angularLimitsTauFactor;
 		inline hkReal getAngularLimitsTauFactor() const;

			/// Get type from this constraint
		virtual int getType() const;

		enum 
		{
			SOLVER_RESULT_LIMITS = 0,			// angular limits defined around m_freeAxisA 
			SOLVER_RESULT_ANG_AXIS_1 = 1,		// angular constraint defined by m_constrainedAxis1A
			SOLVER_RESULT_ANG_AXIS_2 = 2,		// angular constraint defined by m_constrainedAxis2A

			SOLVER_RESULT_MAX = 3				// keep this in sync with the hkSolverResult array below
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpHingeLimitsData::Runtime );

			class hkpSolverResults m_solverResults[3/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];
			// used by limits
			hkReal m_lastAngle; 
		};

		static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }


	public:

		struct Atoms
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpHingeLimitsData::Atoms );
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalRotationsConstraintAtom  m_rotations;
			struct hkpAngLimitConstraintAtom           m_angLimit; 
			struct hkp2dAngConstraintAtom			  m_2dAng;

			enum Axis
			{
				AXIS_AXLE = 0,
				AXIS_PERP_TO_AXLE_1 = 1,
				AXIS_PERP_TO_AXLE_2 = 2
			};

			Atoms(){}

			// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_rotations; }

			// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_2dAng+1); }

			Atoms(hkFinishLoadedObjectFlag f) :m_rotations(f), m_angLimit(f), m_2dAng(f) {}
		};

		HK_ALIGN16( struct Atoms m_atoms );

	public:

		// hkpConstraintData interface implementations
		virtual void getConstraintInfo( ConstraintInfo& infoOut ) const;

		// hkpConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:

		hkpHingeLimitsData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}

};

#include <Physics/Dynamics/Constraint/Chain/HingeLimits/hkpHingeLimitsData.inl>

#endif // HK_HINGE_LIMITS_H

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
