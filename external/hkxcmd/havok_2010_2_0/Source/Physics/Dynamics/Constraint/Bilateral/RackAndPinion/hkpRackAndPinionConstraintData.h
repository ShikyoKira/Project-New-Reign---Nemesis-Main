/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_RACK_AND_PINION_CONSTRAINT_H
#define HK_DYNAMICS2_RACK_AND_PINION_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>
#include <Physics/ConstraintSolver/Solve/hkpSolverResults.h>

/// This constraint represents interaction between a pinion and a rack elements; or between a screw and its nut.
///
/// This is a simple one-dimensional constraint only. In order to create a meaningful mechanism, the elements linked
/// must be connected to other external constraints -- most probably a prismatic constraint for the rack, and a hinge,
/// or limited-hinge for the pinion. This constraintData has helper functions that will allow you to easily create
/// those.
///
/// Note that the axes of the rack and pinion don't have to be fixed at one relative position. The axes are free
/// to move in a (physically sensible) range. And the constraints should properly track the contact point of the parts.
/// It will also work, when the parts are physically disjoint at times. Note that the interaction force is always
/// determined by the direction of the bodyB (and it's shiftAxis, which is stored in bodyB's local space).
/// 
/// If you experience stability problems in bigger setups, the external prismatic and hinge constraints should
/// have higher priority (hkpConstraintInstance::PRIORITY_TOI_HIGHER or higher), or the rack-and-pinion constraint
/// should be added as first.
///
class hkpRackAndPinionConstraintData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

		HK_DECLARE_REFLECTION();
		hkpRackAndPinionConstraintData();

		enum Type
		{
			TYPE_RACK_AND_PINION,
			TYPE_SCREW
		};

			/// Sets the rack-and-pinion up with world space information.
			/// \param pinionARotationPivot Is also the reference 'zero-point' for the linear limits along rackShiftAxis
			/// \param rackBShiftAxis Shift axis of bodyB. Note that this cannot be parallel to pinionRotationAxis for
			///                       rack-and-pinion type, and it cannot be perpendicular to pinionRotationAxis for
			///                       screw-type constraints.
			/// \param pinionRadiusOrScrewPitch Is equal to linear displacement caused by one full rotation of elements.
			///                                 The sign can be negative to reverse the shift direction.
		void setInWorldSpace(	const hkTransform& bodyATransform, const hkTransform& bodyBTransform,
								const hkVector4& pinionARotationPivot, const hkVector4& pinionARotationAxis, 
								const hkVector4& rackBhiftAxis, hkReal pinionRadiusOrScrewPitch,
								Type type = TYPE_RACK_AND_PINION);

								
			/// Sets the rack-and-pinion up with body space information.
			/// The parameters are analogical to setInWorldSpace().
		void setInBodySpace(	const hkVector4& pinionARotationPivotInA, const hkVector4& pinionARotationPivotInB,
								const hkVector4& pinionARotationAxisInA, const hkVector4& rackBShiftAxisInB,
								hkReal pinionRadiusOrScrewPitch, Type type = TYPE_RACK_AND_PINION);

			// This creates a prismatic constraint between bodyB (rack) of this constraint and any other body.
			// The prismatic axis and pivot are the same as in this rack&pinion/screw constraint.
			// This function uses setInWorldSpace() method on the new constraint, so the bodies (including the external
			// body) must have their transforms properly initialized. They don't need to be added to the world however.
			// \param constraint The instance that we want to extend with a matching hinge constraint.
			// \param anotherBody Any body to be used as bodyB of the new hinge constraint. This can also be the bodyA
		    //                    of this rack&pinion/screw constraint.
		hkpConstraintInstance* createMatchingPrismaticConstraint(hkpConstraintInstance* constraint, 
																 class hkpRigidBody* anotherBody);

			// This creates a  hinge constraint between bodyA (pinion) of this constraint and any other body.
			// The hinge axis and pivot are the same as in this rack&pinion/screw constraint.
			// This function uses setInWorldSpace() method on the new constraint, so the bodies (including the external
			// body) must have their transforms properly initialized. They don't need to be added to the world however.
			// \param constraint The instance that we want to extend with a matching hinge constraint.
			// \param anotherBody Any body to be used as bodyB of the new hinge constraint. This can also be the bodyB
			//                    of this rack&pinion/screw constraint.
		hkpConstraintInstance* createMatchingHingeConstraint(hkpConstraintInstance* constraint, 
													class hkpRigidBody* anotherBody, bool createLimitedHinge = false);

			/// Gets the pinion radius or screw pitch, depending on the constraint's usage.
		inline hkReal getPinionRadiusOrScrewPitch();

			/// Sets the pinion radius or screw pitch, depending on the constraint's usage.
		void setPinionRadiusOrScrewPitch(hkReal ratio, hkpConstraintInstance** allInstances, int numInstances);


			/// Check consistency of constraint
		hkBool isValid() const;

			/// Get type from this constraint
 		virtual int getType() const;

		
	public:

		enum 
		{
			SOLVER_RESULT_RACK_AND_PINION = 0,	// 
			SOLVER_RESULT_MAX = 1			// keep in sync with solver res array below
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpRackAndPinionConstraintData::Runtime );

			class hkpSolverResults m_solverResults[1/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];

			// This stores the initial relative angular orientation of the bodies. 0 if uninitialized (triggers 
			// initialization of m_revolutionCounter too). Non-zero when initialized (set to a tiny number 
			// (< HK_REAL_EPSILON) for zero values).
			hkReal m_initialAngleOffset;

			// Previously calculated relative angle.
			hkReal m_prevAngle;

			// This counts revolutions of the cog wheel to properly calculate the Rhs. Auto-initialized to 0.
			hkInt32	m_revolutionCounter;


		};


		inline Runtime* getRuntime( hkpConstraintRuntime* runtime ){ return reinterpret_cast<Runtime*>(runtime); }

		struct Atoms
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpRackAndPinionConstraintData::Atoms );
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalTransformsConstraintAtom		m_transforms;
			struct hkpRackAndPinionConstraintAtom			m_rackAndPinion;
			
			Atoms(){}

			// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_transforms; }

			// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_rackAndPinion+1); }

			Atoms(hkFinishLoadedObjectFlag f) : m_transforms(f), m_rackAndPinion(f) {}
		};

		HK_ALIGN16( struct Atoms m_atoms );

	public:

		// hkpConstraintData interface implementations
		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const;

		// hkpConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;
	
	public:

		hkpRackAndPinionConstraintData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}
};


#include <Physics/Dynamics/Constraint/Bilateral/RackAndPinion/hkpRackAndPinionConstraintData.inl>

#endif // HK_DYNAMICS2_RACK_AND_PINION_CONSTRAINT_H

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
