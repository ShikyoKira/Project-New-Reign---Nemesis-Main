/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_POWERED_CHAIN_H
#define HK_DYNAMICS2_POWERED_CHAIN_H

#include <Physics/ConstraintSolver/Solve/hkpSolverResults.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>
#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainData.h>
#include <Physics/Dynamics/Action/hkpArrayAction.h>

extern const class hkClass hkpPoweredChainDataConstraintInfoClass;

extern const class hkClass hkpPoweredChainDataClass;



	/// hkPoweredChainData connects a series of bodies by 6d constraints.
	///
	/// The powered chain is essentially composed of a hkpBallSocketChainData with angular motors added on top of it.
	///
	/// hkpPoweredChainData is suited to the accurate simulation of character limbs.
	/// This constraints also allows for motors with a torque limit to model real human muscles.
class hkpPoweredChainData : public hkpConstraintChainData
{
	//+hk.ReflectedFile("hkpPoweredChainData")
	public:

		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT); 

		hkpPoweredChainData();
		
		~hkpPoweredChainData();

			/// Not implemented. Returns always true.
		hkBool isValid() const { return true; }

			/// Get type from this constraint.
 		virtual int getType() const;

			/// Returns the number of stored constraint infos.
		virtual int getNumConstraintInfos() { return m_infos.getSize(); }
	
			/// Adds a constraint info to the chain.
			/// \param pivotInA			Pivot point in bodyA's local space.
			/// \param pivotInB			Pivot point in bodyB's local space.
			/// \param aTc				Relative orientation of the two bodies to be enforced. Specifies orientation of bodyB reference direction in relation to bodyA's space.
			/// \param bTc				Relative orientation of the motors' space. Specifies orientation of the motors in bodyB's space.
			/// \param switchBodies		When set the link is 'reversed' and treats bodyB as bodyA and the other way around.
			/// \param motor			Motor that is to be used by the constraints. The motor specifies the maximum forces exerted at the constraint, and
			///                         error correction parameters.
		void addConstraintInfoInBodySpace(const hkVector4& pivotInA, const hkVector4& pivotInB, const hkQuaternion& aTc,
											class hkpConstraintMotor* xMotor, class hkpConstraintMotor* yMotor, class hkpConstraintMotor* zMotor);

			/// This is disables processing of a motor in a chain. An excluded motor is always processed by the iterative solver before other constraints in the chain.
		static void HK_CALL enableMotor(class hkpConstraintChainInstance* instance, int constraintIndex, int motorIndex);

			/// This is re-enables processing of a motor in a chain. Initially all motors are enabled.
		static void HK_CALL disableMotor(class hkpConstraintChainInstance* instance, int constraintIndex, int motorIndex);


	public:

		struct Runtime
		{
			// Stores hkpSolverResults                [6 * m_infos.getSize()]
			//        hkp3dAngularMotorSolverInfo::Status [m_infos.getSize()] // flags for angular constraints
			//        hkQuaternion                   [m_infos.getSize()] // motor's runtime data
		};

		inline const Runtime* getRuntime( hkpConstraintRuntime* runtime )     { return reinterpret_cast<Runtime*>(runtime); }

		inline hkpSolverResults* getSolverResults(hkpConstraintRuntime* runtime) { return reinterpret_cast<hkpSolverResults*>( runtime ); }

			// assuming we always want solver results
			// -- other wise swap the order of solverResults and externalRuntimeData 
		inline hkp3dAngularMotorSolverInfo::Status* getConstraintFlags(hkpConstraintRuntime* runtime)
		{
			return reinterpret_cast<hkp3dAngularMotorSolverInfo::Status*>( hkAddByteOffset(runtime, m_infos.getSize() * 6 * sizeof(hkpSolverResults) ));
		}
		inline hkReal* getMotorRuntimeQuaternions( hkpConstraintRuntime* runtime)
		{
			return reinterpret_cast<hkReal*>( hkAddByteOffset( getConstraintFlags(runtime), HK_NEXT_MULTIPLE_OF(4, m_infos.getSize() * sizeof(hkp3dAngularMotorSolverInfo::Status)) ) );
		}


	public:

		//
		// Internal functions
		//

			/// Interface implementation.
		virtual void buildJacobian( const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out );

			/// Interface implementation.
		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const;

			/// Interface implementation.
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:

			/// Serialization ctor.
		hkpPoweredChainData(hkFinishLoadedObjectFlag f);

	public:


		struct hkpBridgeAtoms m_atoms;

		struct ConstraintInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpPoweredChainData::ConstraintInfo );
			HK_DECLARE_REFLECTION();

				/// Constraint's pivot point in bodyA's space.
			hkVector4 m_pivotInA;
		
				/// Constraint's pivot point in bodyB's space.
			hkVector4 m_pivotInB;

				/// Relative orientation of the two bodies to be enforced. Specifies orientation of bodyB's reference direction in relation to bodyA's space.
			hkQuaternion m_aTc;

				/// Specifies the reference frame of the constraint in the bodyB's space (or bodyA's space when m_switchBodies ).
			hkQuaternion m_bTc; 

				/// Pointers to motors for the angular constraints.
				// Those cannot be HK_NULL because we take tau/damping from them.
			class hkpConstraintMotor* m_motors[3];


				/// When set to true, it switches the roles of bodyA and bodyB:
				/// The target m_aTb quaternion is interpreted as m_bTa; the constraint space is aligned to the body A instead of body B.
				/// Note that the pivot vectors are not swapped.
			hkBool m_switchBodies;

		};

			/// Constraint infos for the chain's constraints.
		hkArray<struct ConstraintInfo> m_infos;

			/// Solver tau, this overrides the global value from hkpSolverInfo when processing the ball-socket part of the constraint.
			/// The angular parts use the tau/dumping values from the motors.
		hkReal m_tau;

			/// Solver damping, this overrides the global value from hkpSolverInfo.
			/// The angular parts use the tau/dumping values from the motors.
		hkReal m_damping;

			/// Constraint force mixing parameter.
			///
			/// Adding this value makes the constraint softer and it behaves in a spring-like manner.
			/// This is achieved by adding this value to the diagonal of the constraint matrix for the linear velocity equations.
			///
			/// Should be zero or tiny, e.g., a fraction of HK_REAL_EPSILON. When this value is zero, then some chain configurations
			/// may result in a division by zero when solving.
		hkReal m_cfmLinAdd; //+default(0.1f*1.19209290e-07f)

			/// Constraint force mixing parameter, See m_cfmLinAdd for more details.
			/// 
			/// Multiplication factor for the diagonal value of the constraint matrix for the linear velocity equations.
			/// This parameter should be equal or greater than 1.0f. For visible effect try the value of 1.5f. You might set this value to as high as 10.0f.
			/// Note that in chains, you'd usually leave this parameter at 1.0f (as you rarely want to affect the ball-and-socket part of the chain) and
			/// increase the angular-part cfm instead.
		hkReal m_cfmLinMul; //+default(1.0f)

			/// Constraint force mixing parameter, see m_cfmLinAdd for more details.
			///
			/// Should be zero or tiny, e.g., a fraction of HK_REAL_EPSILON. When this value is zero, then some chain configurations
			/// may result in a division by zero when solving.
		hkReal m_cfmAngAdd; //+default(0.1f*1.19209290e-07F)

			/// Constraint force mixing parameter, see m_cfmLinAdd for more details.
			/// 
			/// Multiplication factor for the diagonal value of the constraint matrix for the angular velocity equations.
			/// This parameter should be equal or greater than 1.0f. For visible effect try the value of 1.5f. You might set this value to as high as 10.0f.
		hkReal m_cfmAngMul; //+default(1.0f)

			/// Specifies the maximum distance error in constraints allowed before the stabilization algorithm kicks in.
			/// The algorithm eliminates explosions in may cases, but due to its nature it yields incorrect results in certain use cases.
			/// It is disabled by default.
		hkReal m_maxErrorDistance;
};



#endif // HK_DYNAMICS2_POWERED_CHAIN_H

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
