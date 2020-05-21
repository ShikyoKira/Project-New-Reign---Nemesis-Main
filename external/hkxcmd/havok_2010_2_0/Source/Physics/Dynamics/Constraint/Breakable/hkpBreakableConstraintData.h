/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_BREAKABLE_CONSTRAINT_H
#define HK_BREAKABLE_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/Dynamics/Constraint/hkpConstraintInstance.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

class hkpWorld;

extern const hkClass hkpBreakableConstraintDataClass;

/// Definition of the breakable constraint class which is essentially a wrapper around a hkpConstraintInstance.
/// The constraint will "break" i.e., cease to apply forces/impulse to maintain itself
/// whenever the "threshold" is exceeded.
/// N.B. Currently, "threshold" is an empirical value
/// based on the magnitude of the force(s) required to maintain the constraint and
/// must be hand-tweaked, but larger values produce "harder-to-break" constraints.
/// N.B This constraint can be shared. In this case all constraints will break at the very same time
class hkpBreakableConstraintData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Construct the constraint with a pointer to the constraint to be broken
			/// and a link to the hkpWorld object (needed for the constraint to perform its logic).
			/// The constraint to be 'breakable' should be passed in along with the world.
			///
			/// The breakable constraint essentially wraps another constraint. A reference
			/// is added to the constraint passed in (and removed upon destruction). The
			/// world pointer is need in case you want the constraint to be automatically
			/// removed from the world when breakage occurs. NOTE: Instead of adding the
			/// original constraint to the world, you should add this breakable constraint.
			/// Do not add both constraints.
		hkpBreakableConstraintData( hkpConstraintData* constraintData );	
		
			/// The reference to the original constraint is removed.
		~hkpBreakableConstraintData();

			/// Set the current threshold.
			/// N.B. Currently, "threshold" is an empirical value
			/// based on the magnitude of the force(s) required to maintain the constraint and
			/// must be hand-tweaked, but larger values produce "harder-to-break" constraints.
		inline void setThreshold(hkReal thresh);

			/// Remove Constraint from world when broken
		inline void setRemoveWhenBroken(hkBool tf);

			/// Set this to true if you want to undo all impulses
			/// applied between now and the time of breakage.
			///
			/// As the constraint is not broken by the solver, the
			/// solver might apply a huge impulse before the constraint breaks.
			/// So even if a constraint door brakes, it might be able to stop
			/// a fast car hitting it.
			/// Setting this flag to true enables a workaround, which reverts
			/// back the velocity of the two bodies involved before the time
			/// of breakage. This workaround can have some artifacts, especially
			/// when used with keyframed objects, but might work very well in
			/// other cases.
		inline void setRevertBackVelocityOnBreak( hkBool b );

			/// Returns whether constraint will be removed when it breaks
		inline hkBool getRemoveWhenBroken() const;

			/// read setRevertBackVelocityOnBreak() for details
		inline hkBool getRevertBackVelocityOnBreak() const;
		
			/// Returns whether the constraint is in broken state
		inline hkBool getIsBroken( const hkpConstraintInstance* instance );

			/// Set the broken state of the given constraint instance
		void setBroken ( hkpConstraintInstance* instance, hkBool broken, hkReal currentForce = 0.0f );

			/// Returns the breaking threshold
		inline hkReal getThreshold() const;

			/// Check consistency of constraint members
		virtual hkBool isValid() const;
		
			// hkpConstraintData interface implementation
		virtual int getType() const;

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpBreakableConstraintData::Runtime );

			hkBool m_isBroken;
			hkReal m_linearVelcityA[3];
			hkReal m_linearVelcityB[3];
			hkReal m_angularVelcityA[3];
			hkReal m_angularVelcityB[3];
		};

		inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime );

			// hkpConstraintData interface implementation
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:
			/// Gets the wrapped constraint
		inline hkpConstraintData* getWrappedConstraintData();

			/// Gets the wrapped constraint
		inline const hkpConstraintData* getWrappedConstraintData() const;

	//protected:
	public: // as they are reset in hkpWorld::addConstraint.


		struct hkpBridgeAtoms m_atoms;
		

			/// The wrapped constraint
		hkpConstraintData* m_constraintData;

			// the size of the runtime of the child constraint
		hkUint16 m_childRuntimeSize;

			// the number of solver results of the child constraint
		hkUint16 m_childNumSolverResults;

	public:

			/// The threshold of breakage
		hkReal m_solverResultLimit;

			/// Remove breakable constraint from world when broken?
		hkBool  m_removeWhenBroken;

			/// See setRevertBackVelocityOnBreak()
		hkBool  m_revertBackVelocityOnBreak;

			/// Needed to build an Jacobian, which is doing nothing
		void buildNopJacobian( const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out );

	public:

			/// This is where the constraint is either solved for or broken
		void buildJacobian( const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out );

			// Does the constraint need a single-threaded callback before jacobians are built from atoms?
		virtual hkBool isBuildJacobianCallbackRequired() const { return true; }

			// The callback. Note that all callbacks are performed in single threaded mode.
		virtual void buildJacobianCallback( const hkpConstraintQueryIn &in );


		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& info ) const;

	public:

			// This is to allow loading of pre-3.3 assets.
			//
			// In 3.3 we have two problematic members: m_childRuntimeSize and m_childNumSolverResults.
			//
			// Initialization of those members depends on a call to a virtual method of a different object, 
			// and we cannot do that safely in our current serialization framework neither at the time 
			// of converting assets nor in the finish-up constructor.
			//
			// Therefore we're doing that in hkpWorld::addConstraint().
			//
		hkpBreakableConstraintData(hkFinishLoadedObjectFlag f);
};

#include <Physics/Dynamics/Constraint/Breakable/hkpBreakableConstraintData.inl>

#endif

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
