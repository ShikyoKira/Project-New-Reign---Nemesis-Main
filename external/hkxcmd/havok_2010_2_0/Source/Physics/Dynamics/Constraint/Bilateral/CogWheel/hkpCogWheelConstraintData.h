/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_COG_WHEEL_CONSTRAINT_H
#define HK_DYNAMICS2_COG_WHEEL_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>
#include <Physics/ConstraintSolver/Solve/hkpSolverResults.h>

/// This constraint represents interaction between two cogwheels.
///
/// This is a simple one-dimensional constraint only. In order to create a meaningful mechanism, the elements linked
/// must be connected to other external constraints with additional hinge constraints. This constraintData has a helper
/// functions that will allow you to easily create those.
///
/// Note that the axes of the cog wheels don't have to be fixed at one relative position. The wheel axes are free
/// to move in a (physically sensible) range. And the constraints should properly track the contact point of the wheels.
/// It will also work, when the cogs are physically disjoint at times.
///
/// If you experience stability problems in bigger setups, the external prismatic and hinge constraints should
/// have higher priority (hkpConstraintInstance::PRIORITY_TOI_HIGHER or higher), or the rack-and-pinion constraint
/// should be added as first.
///
class hkpCogWheelConstraintData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

		HK_DECLARE_REFLECTION();
		hkpCogWheelConstraintData();

			/// Sets the cog wheels up with world space information.
		void setInWorldSpace(	const hkTransform& bodyATransform, const hkTransform& bodyBTransform,
								const hkVector4& rotationPivotA, const hkVector4& rotationAxisA, hkReal radiusA,
								const hkVector4& rotationPivotB, const hkVector4& rotationAxisB, hkReal radiusB);

								
			/// Sets the cog wheels up with body space information.
			/// The parameters are analogical to setInWorldSpace().
		void setInBodySpace( const hkVector4& rotationPivotAInA, const hkVector4& rotationAxisAInA, hkReal radiusA,
							 const hkVector4& rotationPivotBInB, const hkVector4& rotationAxisBInB, hkReal radiusB);

			// This creates a simple hinge constraint between one of the constraint's bodies and any other body.
			// The hinge axis and pivots are the same as the chosen bodies' axis & pivot in this cog-wheel constraint.
			// This function uses setInWorldSpace() method on the new constraint, so the bodies (including the external
			// body) must have their transforms properly initialized. They don't need to be added to the world however.
			// \param constraint The instance that we want to extend with a matching hinge constraint.
			// \param bodyIndex  Index of the body, in this constraint, to be used as bodyA of the new hinge constraint.
			// \param anotherBody Any body to be used as bodyB of the new hinge constraint.
		hkpConstraintInstance* createMatchingHingeConstraint(hkpConstraintInstance* constraint, int bodyIndex, 
													class hkpRigidBody* anotherBody, bool createLimitedHinge = false);

			/// Gets the radius of a cog wheel.
		inline hkReal getCogWheelRadius(int wheelIdx);

			/// Sets the radius of a cog wheel.
		void setCogWheelRadius(int wheelIdx, hkReal radius, hkpConstraintInstance** allInstances, int numInstances);


			/// Check consistency of constraint
		hkBool isValid() const;

			/// Get type from this constraint
 		virtual int getType() const;

		
	public:

		enum 
		{
			SOLVER_RESULT_COG_WHEEL = 0,	// 
			SOLVER_RESULT_MAX = 1			// keep in sync with solver res array below
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpCogWheelConstraintData::Runtime );

			class hkpSolverResults m_solverResults[1/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];

			// This stores the initial relative angular orientation of the bodies. 0 if uninitialized (triggers 
			// initialization of m_revolutionCounter too). Non-zero when initialized (set to a tiny number 
			// (< HK_REAL_EPSILON) for zero values).
			hkReal m_initialAngleOffset[2];

			// Previously calculated relative angle.
			hkReal m_prevAngle[2];

			// This counts revolutions of the cog wheel to properly calculate the Rhs. Auto-initialized to 0.
			hkInt32	m_revolutionCounter[2];
		};


		inline Runtime* getRuntime( hkpConstraintRuntime* runtime ){ return reinterpret_cast<Runtime*>(runtime); }

		struct Atoms
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpCogWheelConstraintData::Atoms );
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalTransformsConstraintAtom		m_transforms;
			struct hkpCogWheelConstraintAtom				m_cogWheels;
			
			Atoms(){}

			// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_transforms; }

			// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_cogWheels+1); }

			Atoms(hkFinishLoadedObjectFlag f) : m_transforms(f), m_cogWheels(f) {}
		};

		HK_ALIGN16( struct Atoms m_atoms );

	public:

		// hkpConstraintData interface implementations
		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const;

		// hkpConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;
	
	public:

		hkpCogWheelConstraintData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}
};


#include <Physics/Dynamics/Constraint/Bilateral/CogWheel/hkpCogWheelConstraintData.inl>

#endif // HK_DYNAMICS2_COG_WHEEL_CONSTRAINT_H

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
