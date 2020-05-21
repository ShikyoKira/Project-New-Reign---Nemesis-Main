/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_CONSTRAINT_CONSTRUCTION_KIT_H
#define HK_DYNAMICS2_CONSTRAINT_CONSTRUCTION_KIT_H

#include <Physics/Dynamics/Constraint/ConstraintKit/hkpGenericConstraintData.h>
#include <Physics/Dynamics/Constraint/ConstraintKit/hkpGenericConstraintParameters.h>
#include <Physics/Dynamics/Constraint/ConstraintKit/hkpConstraintModifier.h>

class hkpConstraintModifier;
class hkpConstraintMotor;

/// The hkpConstraintConstructionKit allows you to create your own custom Havok constraints.
class hkpConstraintConstructionKit
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintConstructionKit );
		
			/// Sets up the construction kit with the specified generic constraint.
		void begin( hkpGenericConstraintData* constraint );

		//
		// linear constraint
		//

			/// Specifies a linear degree of freedom attached to body A, and assigns it to an axis
			/// the ID is used to refer to the axis later. e.g., constrainLinearDof( id )
		hkpGenericConstraintData::hkpParameterIndex 
			setLinearDofA( const hkVector4& dofInA, int linearAxisId );

			/// Specifies a linear degree of freedom attached to body B, and assigns it to an axis.
			/// the ID is used to refer to the axis later. e.g., constrainLinearDof( id )
		hkpGenericConstraintData::hkpParameterIndex 
			setLinearDofB( const hkVector4& dof, int linearAxisId );

			/// Specifies a linear degree of freedom attached to the world, and assigns it to an axis.
			/// the ID is used to refer to the axis later. e.g., constrainLinearDof( id )
		hkpGenericConstraintData::hkpParameterIndex 
			setLinearDofWorld( const hkVector4& dofInWorld, int linearAxisId );

			/// Constrains away linear motion in specified degree of freedom
		void constrainLinearDof( int linearAxisId );

			/// Constrains away all linear degrees of freedom.
		void constrainAllLinearDof();


		//
		// angular constraint
		//

			/// Specifies a basis for the angular degrees of freedom, in body A space.
		hkpGenericConstraintData::hkpParameterIndex
			setAngularBasisA( const hkMatrix3& dofBasisInA );

			/// Specified a basis for the angular degrees of freedom, in body B space.
		hkpGenericConstraintData::hkpParameterIndex
			setAngularBasisB( const hkMatrix3& dofBasisInB );	

			/// Sets the angular constraint basis for A to its body frame.
		void setAngularBasisABodyFrame();

			/// Sets the angular constraint basis for B to its body frame.
		void setAngularBasisBBodyFrame();

			/// Constrains objects to only rotate around the axis from the angular bases of A and B.
		void constrainToAngularDof( int angularBasisAxisIndex );

			/// Constrains an object so that it cannot rotate relative to the second object.
		void constrainAllAngularDof();

			/// Sets the pivot point attached to body A from a point in A's body space.
		hkpGenericConstraintData::hkpParameterIndex
			setPivotA( const hkVector4& pivotInA );

			/// Sets the pivot point attached to body B from a point in B's body space.
		hkpGenericConstraintData::hkpParameterIndex
			setPivotB( const hkVector4& pivotInB );

			/// Sets the pivots attached to body A and body B from a common world space point.
			/// N.B. This uses the current transforms of body A and B.
		hkpGenericConstraintData::hkpParameterIndex
			setPivotsHelper( hkpRigidBody* bodyA, hkpRigidBody* bodyB, const hkVector4& pivotInWorld );

		//
		// limits, friction, motors
		//

			/// Limits linear movement on the specified axis.
		hkpGenericConstraintData::hkpParameterIndex
			setLinearLimit( int linearAxisId, hkReal min, hkReal max );

			/// Limits angular movement around the specified axis.
		hkpGenericConstraintData::hkpParameterIndex
			setAngularLimit( int angularBasisAxisIndex, hkReal min, hkReal max );

			/// Limits angular movement lie to within a cone about an axis.
		hkpGenericConstraintData::hkpParameterIndex
			setConeLimit( int angularBasisAxisIndex, hkReal angle );

			/// Limits angular movement about a twist axis. For best accuracy twist axis should stay in positive half of space
			/// defined by twist axis, plane axis and twist cross plane. Accuracy degenerates in the negative half.
		hkpGenericConstraintData::hkpParameterIndex
			setTwistLimit( int angularBasisAxisIndexTwist, int angularBasisAxisIndexPlane, hkReal min, hkReal max );

			/// Adds a motor to drive the angle about the specified axis. Returns and index into the motor
			/// array of hkpGenericConstraintDataScheme
		hkpGenericConstraintData::hkpParameterIndex
			setAngularMotor( int angularBasisAxisIndex, hkpConstraintMotor* motor );

			/// Adds a motor to drive the specified axis. Returns and index into the motor
			/// array of hkpGenericConstraintDataScheme
		hkpGenericConstraintData::hkpParameterIndex
			setLinearMotor( int angularBasisAxisIndex, hkpConstraintMotor* motor );

			/// Sets friction for angular movement around the specified axis.
		hkpGenericConstraintData::hkpParameterIndex
			setAngularFriction( int angularBasisAxisIndex, hkReal maxImpulse );

			/// Sets friction for linear movement on the specified axis.
		hkpGenericConstraintData::hkpParameterIndex
			setLinearFriction( int linearAxisId, hkReal maxImpulse );

		//
		// tau, damping
		//

			/// Sets the stiffness and damping of the subsequent constraints. Remember to restore it later.
		hkpGenericConstraintData::hkpParameterIndex
			setStrength( hkReal strength );
		
			/// restore the stiffness and damping back to solver defaults
		void restoreStrength();

		//
		// modifiers
		//

			/// Adds a callback to hkpConstraintModifier::modify to the constraint
		void addConstraintModifierCallback( hkpConstraintModifier *cm, int userData );

		//
		// commands
		//

			/// Finish construction.
		void end();

		static void HK_CALL computeConstraintInfo(const hkArray<int>& commands, hkpConstraintInfo& info);


	protected:

		hkpGenericConstraintData* m_constraint;
		hkpGenericConstraintDataScheme* m_scheme;

		int m_stiffnessReference;
		int m_dampingReference;

			// Bools to check all required data is specified, given a constraint is being requested.
		hkBool	m_linearDofSpecifiedA[3];
		hkBool	m_linearDofSpecifiedB[3];
		hkBool	m_angularBasisSpecifiedA;
		hkBool	m_angularBasisSpecifiedB;
		hkBool	m_pivotSpecifiedA;
		hkBool	m_pivotSpecifiedB;
};

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
