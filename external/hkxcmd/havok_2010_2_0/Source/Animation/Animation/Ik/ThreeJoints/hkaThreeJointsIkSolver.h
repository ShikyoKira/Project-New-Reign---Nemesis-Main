/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_THREEJOINTSIKSOLVER_H
#define HK_THREEJOINTSIKSOLVER_H

#include <Common/Base/hkBase.h>

class hkaPose;
class hkInternal3JointsIkIterSolver;

/// This plane IK solver operates on the three specified joints in
/// order for a third bone (end point/effector) to reach a desired
/// position. All bones must lie in the plane. All three joints are
/// considered hinges with an axis normal to this plane.
/// Additionally the first joint is allowed to rotate about a
/// specified axis parallel to the plane. The joints/bones do not
/// need to be immediate parents of each other (but they must be in
/// the same chain).
class hkaThreeJointsIkSolver : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME );

		/// This setup data is passed to constructor
		struct Setup
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaThreeJointsIkSolver::Setup );

				/// The id of the first joint
			hkInt16 m_firstJointIdx;

				/// The id of the second joint
			hkInt16 m_secondJointIdx;

				/// The id of the third joint
			hkInt16 m_thirdJointIdx;

				/// The id of the end bone
			hkInt16 m_endBoneIdx;
	
				/// Rotation axis of the first joint in model space
			hkVector4 m_hingeAxisMS;				
				
		};

		/// Constructor input is setup structure and init pose
		hkaThreeJointsIkSolver (const Setup& setupIn, const hkaPose& poseIn);
		/// Destructor
		~hkaThreeJointsIkSolver();

		/// Modifies the given pose in order to reach the target position in model space.
		hkResult HK_CALL solve ( const hkVector4& targetPositionMS, hkaPose& poseInOut );

	private:
		
		Setup m_setup;

		hkInternal3JointsIkIterSolver* m_intSolver;
					
};


#endif // HK_THREEJOINTSIKSOLVER_H

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
