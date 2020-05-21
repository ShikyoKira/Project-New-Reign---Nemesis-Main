/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_HKA_TWO_JOINTS_IK_SOLVER_H
#define INC_HKA_TWO_JOINTS_IK_SOLVER_H

#include <Common/Base/hkBase.h>

class hkaPose;

/// This IK solver operates on the two specified joints in order for a third bone/joint to reach
/// a desired position. The first joint is considered to be a ball-socket, while the second joint is considered to be
/// a hinge joint (and therefore an axis must be provided).
/// The joints/bones do not need to be immediate parents of each other (but they must be in the same chain).
class hkaTwoJointsIkSolver : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME );

		/// This setup data is passed to every call of solve()
		struct Setup
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaTwoJointsIkSolver::Setup );

				/// The id of the first joint (ball-socket : thigh, shoulder)
			hkInt16 m_firstJointIdx;

				/// The id of the second joint (hinge : knee, elbow)
			hkInt16 m_secondJointIdx;

				/// The id of the end bone (usually heel, hand, toe)
			hkInt16 m_endBoneIdx;

				/// The id of the twist bone corresponding to the first joint (optional)
			hkInt16 m_firstJointTwistIdx;

				/// The id of the twist bone corresponding to the second joint (optional)
			hkInt16 m_secondJointTwistIdx;

				/// The hinge axis for the second joint, in local space. Positive rotations (using right hand rule) around this
				/// axis should extend the limb.
			hkVector4 m_hingeAxisLS;

				/// Limit the hinge angle (to avoid knee/elbow artifacts). Default is -1 (180 deg).
			hkReal m_cosineMaxHingeAngle;

				/// Limit the hinge angle (to avoid knee/elbow artifacts). Default is 1 (0 deg).
			hkReal m_cosineMinHingeAngle;

				/// Gain of the IK applied to the first joint (from 0 to 1). You can use this to transition smoothly from/to IK-fixed poses
			hkReal m_firstJointIkGain;

				/// Gain of the IK applied to the second joint (from 0 to 1). You can use this to transition smoothly from/to IK-fixed poses
			hkReal m_secondJointIkGain;

				/// Gain of the IK applied to the end joint (from 0 to 1). You can use this to transition smoothly from/to IK-fixed poses
				/// Only has an effect if m_enforceEndRotation is true
			hkReal m_endJointIkGain;

				/// The target position for the end bone, in model space
			hkVector4 m_endTargetMS;

				/// The target rotation for the end bone in model space
			hkQuaternion m_endTargetRotationMS;

				/// The offset of the end effector in the local space of the end bone
			hkVector4 m_endBoneOffsetLS;
			
				/// The rotation offset of the end effector in the local space of the end bone
			hkQuaternion m_endBoneRotationOffsetLS;
			
				/// Set to true if the position of the end effector is to be solved for
			hkBool m_enforceEndPosition;

				/// Set to true if the rotation of the end effector is to be solved for
			hkBool m_enforceEndRotation;

				// Sets defaults
			Setup() :
				m_firstJointIdx (-1),
				m_secondJointIdx(-1),
				m_endBoneIdx(-1),
				m_firstJointTwistIdx(-1),
				m_secondJointTwistIdx(-1),
				m_hingeAxisLS(hkVector4::getZero()),
				m_cosineMaxHingeAngle (-1.0f),
				m_cosineMinHingeAngle(1.0f),
				m_firstJointIkGain(1.0f),
				m_secondJointIkGain(1.0f),
				m_endJointIkGain(1.0f),
				m_endTargetMS(hkVector4::getZero()),
				m_endTargetRotationMS(hkQuaternion::getIdentity()),
				m_endBoneOffsetLS(hkVector4::getZero()),
				m_endBoneRotationOffsetLS(hkQuaternion::getIdentity()),
				m_enforceEndPosition(true),
				m_enforceEndRotation(false)
			{
			}

		};

			/// Modifies the given pose in order to solve the two two-joint system described in the setup parameter.
		static hkBool HK_CALL solve ( const Setup& setup, hkaPose& poseInOut );

};


#endif // INC_HKA_TWO_JOINTS_IK_SOLVER_H

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
