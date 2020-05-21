/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CONSTRAINTSOLVER2_SOLVE_H
#define HK_CONSTRAINTSOLVER2_SOLVE_H

#include <Common/Base/hkBase.h>
#include <Physics/ConstraintSolver/Solve/hkpSolverInfo.h>

//#define HK_COMPRESS_SCHEMAS
#if defined HK_COMPRESS_SCHEMAS
#	define HK_COMPRESS_SCHEMAS_FLOAT16
#endif

class hkpJacobianSchema;
class hkpVelocityAccumulator;
class hkSpuDmaAccessor;
class hkSpuDmaReader;
class hkpConstraintInstance;
class hkpSolverResults;

struct hkpSolverElemTemp
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpSolverElemTemp );

	union
	{
		hkReal m_impulseApplied;
		hkReal m_usedImpulseFactor;
		hkReal m_deltaRhs;
	};
};

struct hkpImpulseLimitBreachedElem
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpImpulseLimitBreachedElem );

	HK_ALIGN16(hkUint16	  m_type);				// set to 0
	hkUint16 m_isContact;
	hkpConstraintInstance* m_constraintInstance;	// this is the value set in hkpConstraintQueryIn.m_constraintInstance
	hkpSolverResults*	  m_solverResult;		// a pointer to the solver results
};

	// this class is followed by an array of hkpImpulseLimitBreachedElem
struct hkpImpulseLimitBreachedHeader
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT_SOLVER, hkpImpulseLimitBreachedHeader );

	HK_ALIGN16( int m_numBreached );

		// for multi threading this points to the next array
	hkpImpulseLimitBreachedHeader* m_next;

	inline hkpImpulseLimitBreachedElem& getElem(int index){ return *(hkpImpulseLimitBreachedElem*)hkAddByteOffset(this,index*sizeof(hkpImpulseLimitBreachedElem) + sizeof(hkpImpulseLimitBreachedHeader) ); }
};

class hkpConstraintInstance;

extern "C"  
{
	// info pretty much read only, but cannot be shared between threads
	hkBool32 HK_CALL hkSolveConstraints( const hkpSolverInfo& info, hkpJacobianSchema *schemas,  hkpVelocityAccumulator * accumulators, hkpSolverElemTemp* temp );

	void HK_CALL hkSolveApplyGravityByTheSteps( const hkpSolverInfo& info, hkpVelocityAccumulator* accumulators, hkpVelocityAccumulator* accumulatorsEnd );

	hkBool32 HK_CALL hkSolveConstraintsByTheSteps( const hkpSolverInfo& info, int currentSolverStep, int currentSolverMicroStep, const hkpJacobianSchema *schemas,  hkpVelocityAccumulator * accumulators, hkpSolverElemTemp* temp );

	int HK_CALL hkSolveIntegrateVelocitiesByTheSteps( const hkpSolverInfo& info, int currentSolverStep, hkpVelocityAccumulator* accumulators, hkpVelocityAccumulator* accumulatorsEnd, hkBool& outSolveFinished );

	void HK_CALL hkSolveUpload();

	void HK_CALL hkSolveStepJacobians ( const hkpSolverInfo& i, const hkpJacobianSchema *schemas, hkpVelocityAccumulator* accumulators, hkpSolverElemTemp* tmp );

	// this function exports the result from the solver into the solver results.
	// it also exports the impulse breached constraints within the schemas memory;
	void HK_CALL hkExportImpulsesAndRhs( const hkpSolverInfo& i, const hkpSolverElemTemp* temp, const hkpJacobianSchema *schemas, const hkpVelocityAccumulator* accums );


//#if ! defined (HK_PLATFORM_SPU)
//	// This function exports the impulse breached constraints within the schemas memory. No solver results per se are exported.
//	void HK_CALL hkExportBreachedImpulses( const hkpSolverInfo& i, const hkpSolverElemTemp* temp, const hkpJacobianSchema *schemas, const hkpVelocityAccumulator* accums );
//#endif


		/// Get the violating velocities for some constraints.
		/// Contact constraint - returns min( 0.0f, seperatingVelocity ).
		/// Bilateral constraint - returns the violating velocity.
		/// All the rest return 0.0f.
		///
		/// Continues until:
		///  - the constraint is finished by finding the end mark or a new header schema.
		///  - we reach the friction part a contact constraint.
		///  - we reach maxNumVelocities.
		///
		/// Returns the number of velocities calculated.
	int HK_CALL hkSolveGetToiViolatingConstraintVelocity( hkpSolverInfo& i,	const hkpJacobianSchema *schemas, const hkpVelocityAccumulator* accums, int maxNumVelocities, hkReal* velocitiesOut );
}


#if defined HK_COMPRESS_SCHEMAS

	#define HK_SIZE_OF_JACOBIAN_END_SCHEMA 16
	#define HK_SIZE_OF_JACOBIAN_HEADER_SCHEMA 16

	#define HK_SIZE_OF_JACOBIAN_GOTO_SCHEMA 16
	#define HK_SIZE_OF_JACOBIAN_SHIFT_SOLVER_RESULTS_SCHEMA 16

#if defined HK_COMPRESS_SCHEMAS_FLOAT16
#	define HK_SIZE_OF_JACOBIAN_1D_BILATERAL_SCHEMA 32
#	define HK_SIZE_OF_JACOBIAN_1D_ANGULAR_SCHEMA 16
	#define HK_SIZE_OF_JACOBIAN_SINGLE_CONTACT_SCHEMA 32
	#define HK_SIZE_OF_JACOBIAN_PAIR_CONTACT_SCHEMA 80 // and can get rid of one more - 64
#else
#	define HK_SIZE_OF_JACOBIAN_1D_BILATERAL_SCHEMA 48
#	define HK_SIZE_OF_JACOBIAN_1D_ANGULAR_SCHEMA 32
#	define HK_SIZE_OF_JACOBIAN_SINGLE_CONTACT_SCHEMA 48
#	define HK_SIZE_OF_JACOBIAN_PAIR_CONTACT_SCHEMA 96 //112
#endif

	#define HK_SIZE_OF_JACOBIAN_1D_BILATERAL_USER_TAU_SCHEMA 48 //-16
	#define HK_SIZE_OF_JACOBIAN_1D_LINEAR_LIMIT_SCHEMA 48 //-16
	#define HK_SIZE_OF_JACOBIAN_1D_FRICTION_SCHEMA 48 // -16
	#define HK_SIZE_OF_JACOBIAN_1D_LINEAR_MOTOR_SCHEMA 80

	#define HK_SIZE_OF_JACOBIAN_1D_PULLEY_SCHEMA 64

	#define HK_SIZE_OF_JACOBIAN_1D_ANGULAR_LIMITS_SCHEMA 48
	#define HK_SIZE_OF_JACOBIAN_1D_ANGULAR_FRICTION_SCHEMA 48
	#define HK_SIZE_OF_JACOBIAN_1D_ANGULAR_MOTOR_SCHEMA 64

#if HK_POINTER_SIZE == 4
	#define HK_SIZE_OF_JACOBIAN_2D_FRICTION_SCHEMA 112
#else
	#define HK_SIZE_OF_JACOBIAN_2D_FRICTION_SCHEMA 128
#endif
	#define HK_SIZE_OF_JACOBIAN_3D_FRICTION_SCHEMA 160

#if HK_POINTER_SIZE == 4
#define HK_SIZE_OF_JACOBIAN_2D_ROLLING_FRICTION_SCHEMA 80
#else
#define HK_SIZE_OF_JACOBIAN_2D_ROLLING_FRICTION_SCHEMA 96
#endif

	#define HK_SIZE_OF_JACOBIAN_SET_MASS_SCHEMA 32
	#define HK_SIZE_OF_JACOBIAN_ADD_VELOCITY_SCHEMA 32
	#define HK_SIZE_OF_JACOBIAN_SET_CENTER_OF_MASS_SCHEMA 96

#else // 

	#define HK_SIZE_OF_JACOBIAN_END_SCHEMA 16
#if HK_POINTER_SIZE == 4
	#define HK_SIZE_OF_JACOBIAN_HEADER_SCHEMA 16
#else
#	define HK_SIZE_OF_JACOBIAN_HEADER_SCHEMA 32
#endif
#	define HK_SIZE_OF_JACOBIAN_GOTO_SCHEMA 16
	#define HK_SIZE_OF_JACOBIAN_SHIFT_SOLVER_RESULTS_SCHEMA 16

	#define HK_SIZE_OF_JACOBIAN_1D_BILATERAL_SCHEMA 48
	#define HK_SIZE_OF_JACOBIAN_1D_BILATERAL_USER_TAU_SCHEMA 64
	#define HK_SIZE_OF_JACOBIAN_1D_LINEAR_LIMIT_SCHEMA 64
	#define HK_SIZE_OF_JACOBIAN_1D_FRICTION_SCHEMA 64
	#define HK_SIZE_OF_JACOBIAN_1D_LINEAR_MOTOR_SCHEMA 80

	#define HK_SIZE_OF_STABLE_BALL_SOCKET_SCHEMA			64
	#define HK_SIZE_OF_JACOBIAN_1D_PULLEY_SCHEMA 64

	#define HK_SIZE_OF_JACOBIAN_1D_ANGULAR_SCHEMA 32
	#define HK_SIZE_OF_JACOBIAN_1D_ANGULAR_LIMITS_SCHEMA 48
	#define HK_SIZE_OF_JACOBIAN_1D_ANGULAR_FRICTION_SCHEMA 48
	#define HK_SIZE_OF_JACOBIAN_1D_ANGULAR_MOTOR_SCHEMA 64

	#define HK_SIZE_OF_JACOBIAN_SINGLE_CONTACT_SCHEMA 48
	#define HK_SIZE_OF_JACOBIAN_PAIR_CONTACT_SCHEMA 112
	#define HK_SIZE_OF_JACOBIAN_2D_FRICTION_SCHEMA 112
#if HK_POINTER_SIZE == 4
	#define HK_SIZE_OF_JACOBIAN_3D_FRICTION_SCHEMA 144
#else
	#define HK_SIZE_OF_JACOBIAN_3D_FRICTION_SCHEMA 160
#endif

#if HK_POINTER_SIZE == 4
#define HK_SIZE_OF_JACOBIAN_2D_ROLLING_FRICTION_SCHEMA 80
#else
#define HK_SIZE_OF_JACOBIAN_2D_ROLLING_FRICTION_SCHEMA 96
#endif

	#define HK_SIZE_OF_JACOBIAN_SET_MASS_SCHEMA 32
	#define HK_SIZE_OF_JACOBIAN_ADD_VELOCITY_SCHEMA 32
	#define HK_SIZE_OF_JACOBIAN_SET_CENTER_OF_MASS_SCHEMA 96

#endif

#define HK_SIZE_OF_JACOBIAN_STIFF_SPRING_CHAIN_SCHEMA 16
#define HK_SIZE_OF_JACOBIAN_BALL_SOCKET_CHAIN_SCHEMA 16
#define HK_SIZE_OF_JACOBIAN_POWERED_CHAIN_SCHEMA  32

#endif // HK_CONSTRAINTSOLVER2_SOLVE_H

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
