/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_HKA_CCD_IKSOLVER_H
#define INC_HKA_CCD_IKSOLVER_H

#include <Common/Base/hkBase.h>

class hkaPose;

/// The CCD (cyclical coordinate descent) IK Solver is an iterative that solves an arbitrary number of constraints. Each constraint
/// is defined as a chain of bones (defined with an start and end bone) + a target position in model space; the solver will rotate the
/// bones in the chain to try to make then end bone match the target position. This is done over a number of iterations over the bones
/// in the chain, incrementally reaching closer to the target.
class hkaCcdIkSolver : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME );

		/// A constraint to be solved by the hkIkSolver. A constraint specified a chain of bones
		/// and a desired world transformation for the end bone (end effector)
		struct IkConstraint
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaCcdIkSolver::IkConstraint );

				/// The start bone in this chain
			hkInt16 m_startBone;

				/// The end bone in this chain
			hkInt16 m_endBone;

				/// The target position for the end bone, in model space
			hkVector4 m_targetMS;

		};

			/// Constructor, it takes the number of iterations and gain used by the solver.
		hkaCcdIkSolver( hkInt32 iterations = 8, hkReal gain = 0.5f );

			/// The main method in the class, it performs the IK on the given pose.
		hkBool solve ( const hkArrayBase<IkConstraint>& constraints, hkaPose& poseInOut );

			/// Sets the number of iterations
		inline void setIterations( hkInt32 number );

			/// Gets the number of iterations
		inline hkInt32 getIterations() const;

			/// Sets the gain factor
		inline void setGain( hkReal gain );

			/// Gets the gain factor
		inline hkReal getGain() const;

	protected:

		// The number of iterations of the IK solver
		hkInt32 m_iterations;

		// The gain factor to use in the IK solver
		hkReal m_gain;

};

#include <Animation/Animation/Ik/Ccd/hkaCcdIkSolver.inl>

#endif // INC_HKA_CCD_IKSOLVER_H

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
