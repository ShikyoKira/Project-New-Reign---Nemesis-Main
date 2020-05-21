/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_RAGDOLL_LIMITS_H
#define HK_DYNAMICS2_RAGDOLL_LIMITS_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

extern const class hkClass hkpRagdollLimitsDataAtomsClass;

class hkpRigidBody;

extern const hkClass hkpRagdollLimitsDataClass;

/// It is a version of a hkRagdollConstraint with removed linear constraints.
/// This constraint is meant to be used with hkConstraintChains. (that
/// correspond to the ball-and-socket part). Once a chain has been defined,
/// this constraint can be simply overlayed on top of it to impose additional
/// angular constraints. When this constraint is used together with
/// hkpPoweredChainData, care should be taken not to set the target in the
/// powered chain data to lay outside the limits defined here.
class hkpRagdollLimitsData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpRagdollLimitsData();

			/// Setup a ragdoll constraint based on information in world space
			/// Use the set angle limit methods to add in the extra data.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform, 
						const hkVector4& twistAxisW, const hkVector4& planeAxisW);

			/// Setup a ragdoll constraint based on information in body space.
			/// Use the set angle limit methods to add in the extra data.
		void setInBodySpace( const hkVector4& planeAxisA, const hkVector4& planeAxisB,
						const hkVector4& twistAxisA, const hkVector4& twistAxisB);

			/// Gets the constraint frame. column 0 = twist axis, 1 = plane, 2 = twist cross plane
		void getConstraintFrameA( hkMatrix3& constraintFrameA ) const;

			/// Gets the constraint frame. column 0 = twist axis, 1 = plane, 2 = twist cross plane
		void getConstraintFrameB( hkMatrix3& constraintFrameB ) const;

			///Sets the minimum twist limit (in radians).
		inline void setTwistMinAngularLimit(hkReal rad);

			///Sets the maximum twist limit (in radians).
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
			/// Angles should be specified in radians.
		inline void setAsymmetricConeAngle(hkReal cone_min, hkReal cone_max );

			/// sets the m_angularLimitsTauFactor. This is a factor in the range between 0 and 1
			/// which controls the stiffness of the angular limits. If you slowly increase
			/// this value from 0 to 1 for a newly created constraint,
			/// you can nicely blend in the limits.
		inline void setAngularLimitsTauFactor( hkReal mag );
			
			/// get the m_angularLimitsTauFactor;
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

			/// Check consistency of constraint
		hkBool isValid() const;
 
			/// Get type from this constraint
		virtual int getType() const;


	protected:

		hkpRagdollLimitsData( hkpRigidBody* rbA, hkpRigidBody* rbB );


	public:

		enum 
		{
			SOLVER_RESULT_TWIST = 0,			// angular limits defined around m_freeAxisA 
			SOLVER_RESULT_CONE  = 1,	    	// angular constraint defined by m_constrainedAxis1A
			SOLVER_RESULT_PLANE = 2,	    	// angular constraint defined by m_constrainedAxis2A

			SOLVER_RESULT_MAX = 3 // keep in sync with solver res array below
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpRagdollLimitsData::Runtime );

			class hkpSolverResults m_solverResults[3/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];

			/// Soft recovery parameter for the cone limit.
			hkReal m_coneAngleOffset;
		};

		static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }

		// hkpConstraintData interface implementations
		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const;

			// hkpConstraintData interface implementation
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:

		struct Atoms
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpRagdollLimitsData::Atoms );
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalRotationsConstraintAtom	m_rotations;
			struct hkpTwistLimitConstraintAtom			m_twistLimit;
			struct hkpConeLimitConstraintAtom			m_coneLimit;
			struct hkpConeLimitConstraintAtom			m_planesLimit;

			enum Axis
			{
				AXIS_TWIST = 0,
				AXIS_PLANES = 1,
				AXIS_CROSS_PRODUCT = 2
			};

			Atoms(){}

			// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_rotations; }

			// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
			int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_planesLimit+1); }

			Atoms(hkFinishLoadedObjectFlag f) : m_rotations(f), m_twistLimit(f), m_coneLimit(f), m_planesLimit(f) {}
		};

		HK_ALIGN16( struct Atoms m_atoms );


	public:

		inline hkpRagdollLimitsData(hkFinishLoadedObjectFlag f);
};

//////////////////////////////////

#include <Physics/Dynamics/Constraint/Chain/RagdollLimits/hkpRagdollLimitsData.inl>

#endif // HK_DYNAMICS2_RAGDOLL_LIMITS_H

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
