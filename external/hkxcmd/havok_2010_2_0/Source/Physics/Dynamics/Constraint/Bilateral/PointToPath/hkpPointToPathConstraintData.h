/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_DYNAMICS2_POINT_TO_PATH_CONSTRAINT_H
#define HK_DYNAMICS2_POINT_TO_PATH_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>
#include <Physics/Dynamics/Constraint/Bilateral/PointToPath/hkpParametricCurve.h>

extern const hkClass hkpPointToPathConstraintDataClass;

	/// A constraint where one body is free to move along a specified path relative to the other.
	/// Body A is the attached body, while the path is on body B.
class hkpPointToPathConstraintData : public hkpConstraintData
{
	public:  
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		enum OrientationConstraintType
		{
			/// 
			CONSTRAIN_ORIENTATION_INVALID,
			/// 
			CONSTRAIN_ORIENTATION_NONE,
			/// 
			CONSTRAIN_ORIENTATION_ALLOW_SPIN,
			/// 
			CONSTRAIN_ORIENTATION_TO_PATH,
			/// 
			CONSTRAIN_ORIENTATION_MAX_ID
		};

		hkpPointToPathConstraintData();

			/// Destructor.
		virtual ~hkpPointToPathConstraintData();

			/// Sets the construction information with world space information.
			/// pathToRefBodyTransform is the transform of the path in reference body space
		void setInWorldSpace( const hkTransform& slidingBodyTransform, const hkTransform& referenceBodyTransform, const hkVector4& pivotWs, hkpParametricCurve* path, const hkTransform& pathToRefBodyTransform = hkTransform::getIdentity() );

			/// Sets the pivot and path with body space information.
		void setInBodySpace( const hkVector4& pivotA, const hkVector4& pivotB, hkpParametricCurve* path);

			/// Set orientation constraint type. This must be one of:
			///     - CONSTRAIN_ORIENTATION_NONE : No angular constraints
			///     - CONSTRAIN_ORIENTATION_ALLOW_SPIN : Attached body constrained so that 'X' axis
			///       of pivot Transform (constraint space) is aligned "along" the path.
			///     - CONSTRAIN_ORIENTATION_TO_PATH : Attached body completely "aligned" with path
			///       using path direction and binormal, with 'X' axis of pivot Transform
			///       (constraint space) is aligned "along" the path and 'Z' axis of the Pivot
			///       Transform aligned with the "binormal" of the path.
		inline void setOrientationType(enum OrientationConstraintType otype);

			/// Set the transformation of the attached body's pivot, in the attached body's
			/// Local space. This is the "Constraint" space for the attached body. This both
			/// sets the pivot position, and pivot orientation in this space.
			/// See setAttachedBodyPivotOrientation() for how the orientation may be used.
			/// The default is the Identity.
		inline void setAttachedBodyPivotTransform(const hkTransform& t);

			/// Set the orientation of the attached body's pivot, in the attached body's Local
			/// space. This is the "Constraint" space for the attached body. The does not
			/// affect the "linear"component of the constraint (keeping the pivot of the
			/// attached boy on the path), but affects the "angular" component of the
			/// constraint (which aligns the attached body with the path, if required). If
			/// the OrientationType is set to CONSTRAIN_ORIENTATION_ALLOW_SPIN or
			/// CONSTRAIN_ORIENTATION_TO_PATH the 'X' axis of the Pivot Transform will
			/// align itself "along" the path. If OrientationType is set to
			/// CONSTRAIN_ORIENTATION_TO_PATH then, additionally, the 'Z' axis of the Pivot
			/// Transform will align itself with the "binormal" of the path. The default is
			/// the Identity.
		inline void setAttachedBodyPivotOrientation(const hkRotation& r);

			/// Set the "Constraint" space for the reference body, in the attached body's
			/// Local space. This is the space in which the path is stored (i.e., a "path Space
			/// to Local space" Transform. The default is the Identity ( implying the path
			/// should be specified in the reference body's Local space).
		inline void setReferenceBodyPathTransform(const hkTransform& t);

			/// Set the friction magnitude.
		inline void setMaxFrictionForce(hkReal friction);

			/// Get the friction magnitude.
		inline hkReal getMaxFrictionForce();

			/// Check consistency of constraint members.
		virtual hkBool isValid() const;

			/// Set the path.
		void setPath(hkpParametricCurve* path);

			/// Get the path.
		inline hkpParametricCurve* getPath() const;

			/// Get the "Constraint" space for the i'th body.
			/// getConstraintToLocalTransform(0) returns the Transform of the attached body's
			/// pivot, in the attached body's Local space.
			/// getConstraintToLocalTransform(1) returns the Transform of the reference body's
			/// path, in the reference body's Local space.
		inline const hkTransform& getConstraintToLocalTransform(int i) const;

			/// Get type from this constraint.
		virtual int getType() const;

		enum 
		{
			SOLVER_RESULT_FRICTION = 0,	// the friction.
			SOLVER_RESULT_1 = 1,		// constraint 0, depends on the type.
			SOLVER_RESULT_2 = 2,		// constraint 0, depends on the type.
			SOLVER_RESULT_3 = 3,		// constraint 0, depends on the type.
			SOLVER_RESULT_4 = 4,		// constraint 0, depends on the type.
			SOLVER_RESULT_5 = 5,		// constraint 0, depends on the type.
			SOLVER_RESULT_6 = 6,		// constraint 0, depends on the type.
			SOLVER_RESULT_MAX = 7 // keep this in sync with the hkSolverResult array below
		};


		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpPointToPathConstraintData::Runtime );

			class hkpSolverResults m_solverResults[7/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];

				/// Current parametric position.
			hkReal m_parametricPosition;
		};

		static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }

			// hkpConstraintData interface implementation
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:


		struct hkpBridgeAtoms m_atoms;

	protected:

			/// The parametric path followed by bodyA.
		hkpParametricCurve* m_path;
		
	public:

		/// Max linear friction impulse.
		hkReal m_maxFrictionForce;
		
			/// Angular constraining options.
		hkEnum<OrientationConstraintType, hkInt8> m_angularConstrainedDOF;

			/// The transforms from constraint space to object space
		hkTransform m_transform_OS_KS[2];

	public:
			//
			// Internal functions
			//
		void calcPivot( const hkTransform& transformBodyA, hkVector4& pivotOut ) const;

		virtual void buildJacobian( const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out );

		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& info ) const;

	public:

		hkpPointToPathConstraintData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}

};

#include <Physics/Dynamics/Constraint/Bilateral/PointToPath/hkpPointToPathConstraintData.inl>

#endif	// HK_DYNAMICS2_POINT_TO_PATH_CONSTRAINT_H

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
