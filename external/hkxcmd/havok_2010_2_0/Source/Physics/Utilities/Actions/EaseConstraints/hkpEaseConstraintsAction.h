/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_EASE_CONSTRAINTS_ACTION_H
#define HK_EASE_CONSTRAINTS_ACTION_H

#include <Physics/Dynamics/Action/hkpArrayAction.h>

// Utilities includes
#include <Physics/Utilities/Constraint/Bilateral/hkpConstraintUtils.h>

/// You can use this action to avoid constraint violation.
///
/// In some violation cases, especially in which the violations are only momentary, it may be desirable to loosen the constraint
/// limits and then restore them over time. The most common use case for this would be setting a ragdoll to a violating animation
/// pose and then switching to "full ragdoll". This operation can often cause significant popping as the ragdoll is forced into
/// a non-violating state. You can use this class to avoid that initial popping. See EaseConstraintsActionDemo.
///
/// This action wraps an array of constraints. Upon creation, it stores the limits of the constraints. When loosenConstraints
/// is called, the constrained rigid bodies are checked for violation and constraint limits are loosened if needed.
///
/// To begin restoring the original constraints, restoreConstraints must be called with a duration and the action must be added
/// to an hkpWorld. You may also simply call restoreConstraints(0) to immediately restore the constraints.
///
/// CAVEATS: This class only operates on the limits of CONSTRAINT_TYPE_RAGDOLL and CONSTRAINT_TYPE_LIMITEDHINGE constraints.
///          Other constraint types are currently not supported. Additionally, this cannot loosen constrained axes which
///          are not controlled by a limit (i.e., the non-principle axes of the limited hinge constraint, positional constraint, etc.).
///
///          This class operates on the hkpConstraintData of the hkpConstraintInstances directly. So any instances sharing that
///          data will also be affected by this action. If this is undesirable (as it will be in most cases), you'll want to make
///          a clone of the hkpConstraintInstances first.
///
class hkpEaseConstraintsAction : public hkpArrayAction
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
	/// Collect constraints from the list of entities using hkpConstraintUtils::collectConstraints.
	/// And store the original limits of the constraints.
	hkpEaseConstraintsAction( const hkArray<hkpEntity*>& entities, hkUlong userData = 0 );

	virtual ~hkpEaseConstraintsAction() {}

	/// Call hkpConstraintUtils::loosenConstraintLimits on all constraints.
	/// restoreConstraints must be called for action to be automatically removed from world.
	void loosenConstraints();

	/// Restore the constraints over the given duration. When duration is zero constraints are
	/// restored and this action is removed from the world.
	void restoreConstraints(hkReal duration = 0);

	/// hkpAction implementation.
	/// This slowly brings the constraint limits back to their original values.
	/// This occurs at a speed determined by the duration specified by restoreConstraints().
	virtual void applyAction( const hkStepInfo& stepInfo );

	/// Overridden to assert, currently not supported.
	virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const ;

	/// Gets the duration of the action
	hkReal getDuration() const;

protected:

	struct CollectSupportedConstraints : public hkpConstraintUtils::CollectConstraintsFilter
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpEaseConstraintsAction::CollectSupportedConstraints);
		virtual hkBool collectConstraint(hkpConstraintInstance* constraint)
		{
			HK_ASSERT2(0x71f1eed1, constraint->getData() != HK_NULL, "Internal error: constraint data is null.");

#ifdef HK_DEBUG
			const hkpConstraintData* data = constraint->getData();
			if ( data->getReferenceCount() >= 2 )
			{
				HK_WARN(0x54ddb624, "hkpConstraintData found which has a reference count >= 2. This may indicate shared constraint data.  hkpEaseConstraintsAction operates on the constraint data directly, so any constraint instances which share this data will be affected.  This problem commenly arises when you shallow clone constraints/ragdolls.");
			}
#endif

			return constraint->isEnabled() && (constraint->getData()->getType() == hkpConstraintData::CONSTRAINT_TYPE_RAGDOLL || constraint->getData()->getType() == hkpConstraintData::CONSTRAINT_TYPE_LIMITEDHINGE);
		}
	};

	static void _saveLimits(hkArray<hkReal>& dst, const hkArray<hkpConstraintInstance*>& src);

	static void _restoreLimits(const hkArray<hkpConstraintInstance*>& dst, const hkArray<hkReal>& src);

	static void _loosenConstraints(const hkArray<hkpConstraintInstance*>& constraints);

	static int _partiallyRestoreConstraint(hkpConstraintInstance* runtimeConstraint, hkReal* originalLimitsPtr, hkReal restorationWeight);

	/// Duration of this action.
	hkReal m_duration;

	/// Time passed since the action was added to the world.
	hkReal m_timePassed;

	/// Initial constraints
	hkArray<hkpConstraintInstance*> m_originalConstraints;

	/// Copy of initial constraints limits (later restored)
	hkArray<hkReal> m_originalLimits;

};

#endif // HK_EASE_CONSTRAINTS_ACTION_H

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
