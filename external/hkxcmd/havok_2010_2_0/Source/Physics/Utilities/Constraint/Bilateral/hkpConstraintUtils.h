/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_UTILITIES_CONSTRAINTUTILS_H
#define INC_UTILITIES_CONSTRAINTUTILS_H

#include <Common/Base/hkBase.h>

class hkpConstraintInstance;
class hkpConstraintMotor;
class hkpPositionConstraintMotor;
class hkpConstraintChainInstance;
class hkpBallSocketChainData;
class hkpEntity;
class hkVector4;


class hkpConstraintInstance;
class hkpConstraintMotor;
class hkpPositionConstraintMotor;
class hkpConstraintChainInstance;
class hkpBallSocketChainData;
class hkpEntity;
class hkVector4;
class hkpConstraintData;

	/// Constraint utility functions.
class hkpConstraintUtils
{
	//+hk.MemoryTracker(ignore=True)
public:

		/// Used by collectConstraints to filter out which constraints are of interest.
		/// See collectConstraints.
	struct CollectConstraintsFilter {
		virtual ~CollectConstraintsFilter() {}
		virtual hkBool collectConstraint(hkpConstraintInstance* constraint) { return true; }
	};

		/// Converts the given constraint to its powered counterpart, using the given motor. If the constraint cannot be powered or it's powered already it returns HK_NULL
		/// and raises a warning.
	static hkpConstraintInstance* HK_CALL convertToPowered (const hkpConstraintInstance* originalConstraint, hkpConstraintMotor* constraintMotor, hkBool enableMotors);

		/// Converts the given constraint to its corresponding 'limits' counterpart. This only works for powered/non-powered hinge and ragdoll constraints.
		/// It returns the original constraint if it already is of a 'limits' type.
		/// The function returns HK_NULL and asserts for all other constraints.
	static hkpConstraintInstance* HK_CALL convertToLimits (hkpConstraintInstance* originalConstraint);

		/// Checks whether the linear component of a constraint is valid. If large errors (distance between pivots > maxAllowedError) are detected,
		/// the constraint's child object (A) is moved towards the position of the parent object (B) by the amount specified by "relativeDisplacementTowardsParent".
		/// This can be called for the constraints of a rag doll to avoid the situation of the ragdoll getting stuck with an arm / leg in the landscape (by moving the child
		/// rigid body towards the parent, the rag doll is actually "pushed" away from the landscape.
		/// Notice that this method expects the pivots of rigid bodies to match the constraints representing the joints.
		/// The method return true if any fix up is done, false if no fix up is done.
	static hkBool HK_CALL checkAndFixConstraint (const hkpConstraintInstance* constraint, hkReal maxAllowedError = 0.05f, hkReal relativeDisplacementTowardsParent = 0.8f);

		/// This functions modifies the constraint's data. It moves the pivot point to a place between the bodies, where the constraint is most stable.
		/// Note that this only looks at a single constraint in isolation, so this won't be so helpful for systems of constraint bodies, and may even make things
		/// worse if the constraints were fine tuned earlier.
		/// This function may be notsafe to use if the hkpConstraintData is shared among multiple hkpConstraintInstances.
	static void HK_CALL setHingePivotToOptimalPosition(hkpConstraintInstance* constraint);

		/// Extracts pivot vectors from the constraint.
	static hkResult HK_CALL getConstraintPivots(const hkpConstraintInstance* constraint, hkVector4& pivotA, hkVector4& pivotB);

		/// Extracts motors from the constraints.
	static hkResult HK_CALL getConstraintMotors(const hkpConstraintData* constraintData, hkpConstraintMotor*& motor0, hkpConstraintMotor*& motor1, hkpConstraintMotor*& motor2 );

		/// Collect all the constraints from the given entities.
		/// Can optionally provide a CollectContraintsFilter to only collect certain constraints.
	static void HK_CALL collectConstraints(const hkArray<hkpEntity*>& entities, hkArray<hkpConstraintInstance*>& constraintsOut, CollectConstraintsFilter* collectionFilter = HK_NULL );

		/// Loosen the constraint limits so that it's constrained bodies are no longer violating them.
		///
		/// NOTE: This class only operates on the limits of CONSTRAINT_TYPE_RAGDOLL and CONSTRAINT_TYPE_LIMITEDHINGE constraints.
		///		  Other constraint types are currently not supported. Additionally, this cannot loosen constrained axes which
		///		  are not controlled by a limit (i.e., the non-principle axes of the limited hinge constraint, positional constraint, etc.).
	static hkResult HK_CALL loosenConstraintLimits(hkpConstraintInstance* constraint);

};

#endif // INC_UTILITIES_CONSTRAINTUTILS_H

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
