/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONSTRAINTSOLVER2_CONSTRAINT_QUERY_IN_H
#define HK_CONSTRAINTSOLVER2_CONSTRAINT_QUERY_IN_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Physics/hkStepInfo.h>
#include <Physics/ConstraintSolver/Solve/hkpSolverInfo.h>

class hkpVelocityAccumulator;

	/// The time information, the constraints get access to
class hkpConstraintQueryStepInfo
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpConstraintQueryStepInfo );

			/// The delta time of each solvers substep
		HK_ALIGN16( hkPadSpu<float>		m_substepDeltaTime );

		hkPadSpu<float>		m_microStepDeltaTime;

			/// The 1.0f/m_substepDeltaTime
		hkPadSpu<float>		m_substepInvDeltaTime;


		hkPadSpu<float>		m_frameDeltaTime;
		hkPadSpu<float>		m_frameInvDeltaTime;
		hkPadSpu<float>		m_invNumSteps;
		hkPadSpu<float>		m_invNumStepsTimesMicroSteps;	// = 1.0f/(numSubsteps*numMicroSteps)
	
		hkPadSpu<float>		m_maxConstraintViolationSqrd;

			// a factor all rhs should be multiplied
		hkPadSpu<float>		m_rhsFactor;

			// a factor all invMasses should be multiplied
		hkPadSpu<float>		m_virtMassFactor;

			// a factor for all rhs used for friction calculations
		hkPadSpu<float> m_frictionRhsFactor;
};

#define HK_ACCUMULATOR_OFFSET_TO_INDEX(offset) hkObjectIndex(unsigned(offset) / sizeof(hkpVelocityAccumulator))


	/// The input structure to hkConstraints::buildJacobian.
class hkpConstraintQueryIn: public hkpConstraintQueryStepInfo
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpConstraintQueryIn );
	
		hkpConstraintQueryIn() { }

			/// The velocity accumulator of bodyA. Typically this is a hkpVelocityAccumulator
		HK_ALIGN16( hkPadSpu<const hkpVelocityAccumulator*>	m_bodyA );

			/// The velocity accumulator of bodyB. Typically this is a hkpVelocityAccumulator
		hkPadSpu<const hkpVelocityAccumulator*>	m_bodyB;

			/// the transform of rigid body A, identity transform if not available
		hkPadSpu<const hkTransform*>	m_transformA;

			/// the transform of rigid body B, identity transform if not available
		hkPadSpu<const hkTransform*>	m_transformB;

			/// the current global solver settings
		hkPadSpu<hkReal> m_tau;

			/// the current global solver settings
		hkPadSpu<hkReal> m_damping;

			/// the current constraint violation settings
		hkSimdReal m_maxConstraintViolationSqrd;

			/// if this class is used with the hkDynamics library, this points to an hkpConstraintInstance
		hkPadSpu<class hkpConstraintInstance*>	m_constraintInstance;

			/// if this class is used with the hkDynamics library, this points to an hkpConstraintRuntime
		hkPadSpu<void*>	m_constraintRuntime;

			/// listeners
		hkPadSpu<hkpViolatedConstraintArray*> m_violatedConstraints;
		
			// Data to be written to the header schema
		hkPadSpu<hkUint32> m_accumulatorAIndex;
		hkPadSpu<hkUint32> m_accumulatorBIndex;

		hkPadSpu<HK_CPU_PTR(void*)> m_constraintRuntimeInMainMemory;

		inline void set(const hkpSolverInfo& si, const hkStepInfo& stepInfo, hkpViolatedConstraintArray* violatedConstraintsBuffer)
		{
			m_substepDeltaTime    = si.m_deltaTime;
			m_microStepDeltaTime  = si.m_deltaTime * si.m_invNumMicroSteps;
			m_substepInvDeltaTime = si.m_invDeltaTime;
			m_invNumSteps		  = si.m_invNumSteps;
			m_invNumStepsTimesMicroSteps	  = si.m_invNumSteps * si.m_invNumMicroSteps;
			m_tau			      = si.m_tau;
			m_damping		      = si.m_damping;
			
			m_violatedConstraints = violatedConstraintsBuffer;

			m_rhsFactor			  = si.m_tauDivDamp * si.m_invDeltaTime;
			m_virtMassFactor      = si.m_damping;
			m_frictionRhsFactor   = si.m_frictionTauDivDamp * si.m_invDeltaTime;

			m_frameDeltaTime      = stepInfo.m_deltaTime;
			m_frameInvDeltaTime   = stepInfo.m_invDeltaTime;
			m_maxConstraintViolationSqrd = hkSimdReal(si.m_maxConstraintViolationSqrd);
		}
};

#endif // HK_CONSTRAINTSOLVER2_CONSTRAINT_QUERY_IN_H

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
