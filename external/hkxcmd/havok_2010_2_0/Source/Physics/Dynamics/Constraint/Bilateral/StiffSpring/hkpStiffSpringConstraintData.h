/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_STIFF_SPRING_CONSTRAINT_H
#define HK_DYNAMICS2_STIFF_SPRING_CONSTRAINT_H

#include <Physics/ConstraintSolver/Solve/hkpSolverResults.h>
#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

extern const class hkClass hkpStiffSpringConstraintDataAtomsClass;

extern const hkClass hkpStiffSpringConstraintDataClass;

/// A stiff spring constraint. It holds the constrained bodies apart at a specified distance,
/// as if they were attached at each end of an invisible rod.
class hkpStiffSpringConstraintData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();
			
		hkpStiffSpringConstraintData();

			/// Sets the spring up with world space information.
			/// Will compute a rest length too (so call setlength after this if needed)
			/// \param pivotA			bodyA's pivot point, specified in world space.
			/// \param pivotB			bodyB's pivot point, specified in world space.
		inline void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform, 
								const hkVector4& pivotAW, const hkVector4& pivotBW);

			/// Sets the spring up with body space information.
			/// \param pivotA			bodyA's pivot point, specified in bodyA's space.
			/// \param pivotB			bodyB's pivot point, specified in bodyB's space.
			/// \param restLength		The length of the stiff spring when at rest
		inline void setInBodySpace(const hkVector4& pivotA, const hkVector4& pivotB, hkReal restLength);

			/// Gets the length of the stiff spring.
		inline hkReal getSpringLength() const;

			/// Sets the length of the stiff spring when at rest.
		inline void setSpringLength( hkReal length );

			/// Check consistency of constraint.
		hkBool isValid() const;

			/// Get type from this constraint.
 		virtual int getType() const;

		
	public:

		enum 
		{
			SOLVER_RESULT_LIN_0 = 0,		// linear constraint 
			SOLVER_RESULT_MAX = 1
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpStiffSpringConstraintData::Runtime );

			class hkpSolverResults m_solverResults[1/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];
		};

		inline const Runtime* getRuntime( hkpConstraintRuntime* runtime ){ return reinterpret_cast<Runtime*>(runtime); }


		struct Atoms
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpStiffSpringConstraintData::Atoms );
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalTranslationsConstraintAtom m_pivots;
			struct hkpStiffSpringConstraintAtom			m_spring;

			Atoms(){}

			// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_pivots; }

			// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_spring+1); }

			Atoms(hkFinishLoadedObjectFlag f) : m_pivots(f), m_spring(f) {}
		};

		HK_ALIGN16( struct Atoms m_atoms );
			


	public:

		// Internal functions

		// hkpConstraintData interface implementations
		virtual void getConstraintInfo( ConstraintInfo& infoOut ) const;

		// hkpConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;
	
	public:

		hkpStiffSpringConstraintData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}
};


#include <Physics/Dynamics/Constraint/Bilateral/StiffSpring/hkpStiffSpringConstraintData.inl>

#endif // HK_DYNAMICS2_STIFF_SPRING_CONSTRAINT_H

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
