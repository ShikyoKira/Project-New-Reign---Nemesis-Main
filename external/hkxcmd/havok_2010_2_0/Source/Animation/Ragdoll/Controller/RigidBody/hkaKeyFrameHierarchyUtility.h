/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KEYFRAME_HIERARCHY_UTILITY_H
#define HK_KEYFRAME_HIERARCHY_UTILITY_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Reflection/Attributes/hkAttributes.h>

extern const class hkClass hkaKeyFrameHierarchyUtilityControlDataClass;

extern const class hkClass hkaKeyFrameHierarchyUtilityClass;

class hkpRigidBody;

	/// The hkaKeyFrameHierarchyUtility implements a controller that applies linear and angular velocities to the
	/// rigid bodies in a constraint hierarchy e.g., a rag doll.
	/// This controller is useful when you are attempting to drive the system to a desired pose / configuration.
class hkaKeyFrameHierarchyUtility 
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaKeyFrameHierarchyUtility);


		HK_DECLARE_REFLECTION();

			/// A structure containing the control data for the utility
			/// The controller works by first matching acceleration, then velocity and finally position.
			/// At each stage the controller examines the difference between the keyframe and the body.
			/// This difference is scaled by the gain and applied it to the body by changing its velocity.
			/// Differences in position are clamped before being applied.
			/// When choosing values for the gains it is common to have higher values in the higher order controllers
			/// e.g., acceleration gain > velocity gain > position gain
			///
			/// Once the three controllers have been applied we perform a final pass which
			/// snaps the bodies to the keyframes when they are very close. This controller called the snap controller
			/// works similarly to the position controller. It usually has a higher gain than the position controller.
			/// It differs slightly in that its strength is proportional to the error and only uses its full strength
			/// if the current error is smaller than m_snapMaxLinearDistance/m_snapMaxAngularDistance.
		struct ControlData
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_RIGID_BODY, hkaKeyFrameHierarchyUtility::ControlData );

				/// This parameter blends the desired target for a bone between model space (0.0) or local space(1.0)
				/// Usually the controller will be much stiffer and more stable when driving to model space.
				/// However local space targeting can look more natural.
				/// It is similar to the deprecated bone controller hierarchyGain parameter
			hkReal m_hierarchyGain;	//+default(0.17f)
									//+hk.RangeReal(absmin=0.0,absmax=1.0)
									//+hk.Description("This parameter blends the desired target for a bone between model space: zero, or local space: one. Usually the controller will be much stiffer and more stable when driving to model space. However local space targeting can look more natural.")

				/// This gain dampens the velocities of the bodies. The current velocity of the body is
				/// scaled by this parameter on every frame before the controller is applied. It is
				/// applied every step and is generally more aggressive than standard linear or angular damping.
				/// A value of 0 means no damping.
			hkReal m_velocityDamping;	//+default(0.0f)
										//+hk.RangeReal(absmin=0.0,absmax=1.0)
										//+hk.Description("This gain dampens the velocities of the bodies. The current velocity of the body is scaled by this parameter on every frame before the controller is applied. It is applied every step and is generally more aggressive than standard linear or angular damping. A value of 0 means no damping.")

				/// This gain controls the proportion of the difference in acceleration that is
				/// applied to the bodies. It dampens the effects of the velocity control.
			hkReal m_accelerationGain;	//+default(1.0f)
										//+hk.RangeReal(absmin=0.0,absmax=1.0)
										//+hk.Description("This gain controls the proportion of the difference in acceleration that is applied to the bodies. It dampens the effects of the velocity control.")

				/// This gain controls the proportion of the difference in velocity that is
				/// applied to the bodies. It dampens the effects of the position control.
			hkReal m_velocityGain;	//+default(0.6f)
									//+hk.RangeReal(absmin=0.0,absmax=1.0)
									//+hk.Description("This gain controls the proportion of the difference in velocity that is applied to the bodies. It dampens the effects of the position control.")

				/// This gain controls the proportion of the difference in position that is
				/// applied to the bodies. It has the most immediate effect. High gain
				/// values make the controller very stiff. Once the controller is too stiff
				/// it will tend to overshoot. The velocity gain can help control this.
			hkReal m_positionGain;	//+default(0.05f)
									//+hk.RangeReal(absmin=0.0,absmax=1.0)	
									//+hk.Description("This gain controls the proportion of the difference in position that is applied to the bodies. It has the most immediate effect. High gain values make the controller very stiff. Once the controller is too stiff it will tend to overshoot. The velocity gain can help control this.")

				/// The position difference is scaled by the inverse delta time to compute a
				/// velocity to be applied to the rigid body. The velocity is first clamped to this
				/// limit before it is applied.
			hkReal m_positionMaxLinearVelocity;	//+default(1.4f)
												//+hk.RangeReal(absmin=0.0,absmax=100.0,softmax=10.0)
												//+hk.Description("The position difference is scaled by the inverse delta time to compute a velocity to be applied to the rigid body. The velocity is first clamped to this limit before it is applied.")

				/// The orientation difference is scaled by the inverse delta time to compute an angular
				/// velocity to be applied to the rigid body. The velocity is first clamped to this
				/// limit before it is applied.
			hkReal m_positionMaxAngularVelocity;	//+default(1.8f)
													//+hk.RangeReal(absmin=0.0,absmax=100.0,softmax=10.0)
													//+hk.Description("The orientation difference is scaled by the inverse delta time to compute an angular velocity to be applied to the rigid body. The velocity is first clamped to this limit before it is applied.")

				/// This gain allows for precise matching between keyframes and the current position.
				/// It works like the m_positionGain: it calculates an optimal
				/// \code deltaVelocity = (keyFramePosition - currentPosition) / deltaTime \endcode
				/// scales it by m_snapGain, clips it against m_snapMaxXXXVelocity and scales it down
				/// \code if (keyFramePosition - currentPosition) > m_snapMaxXXXXDistance \endcode
			hkReal m_snapGain;	//+default(0.1f)
								//+hk.RangeReal(absmin=0.0,absmax=1.0) 
								//+hk.Description("This gain allows for precise matching between keyframes and the current position. It works like the m_positionGain: it calculates an optimal deltaVelocity, scales it by m_snapGain, clips it against m_snapMaxXXXVelocity and scales it down if the implied positional difference is larger than m_snapMaxXXXXDistance.")

				/// See m_snapGain. The linear velocity calculated from the snapGain is clamped to this
				/// limit before being applied.
			hkReal m_snapMaxLinearVelocity;	//+default(0.3f)
											//+hk.RangeReal(absmin=0.0,absmax=100.0,softmax=10.0)
											//+hk.Description("See m_snapGain. The linear velocity calculated from the snapGain is clamped to this limit before being applied.")

				/// See m_snapGain. The angular velocity calculated from the snapGain is clamped to this
				/// limit before being applied.
			hkReal m_snapMaxAngularVelocity;	//+default(0.3f)
												//+hk.RangeReal(absmin=0.0,absmax=100.0,softmax=10.0)
												//+hk.Description("See m_snapGain. The angular velocity calculated from the snapGain is clamped to this limit before being applied.")

				/// This sets the max linear distance for the snap gain to work at full strength.
				/// The strength of the controller peaks at this distance.
				/// If the current distance is bigger than m_snapMaxLinearDistance, the snap velocity will be scaled
				/// by sqrt( maxDistane/currentDistance ).
			hkReal m_snapMaxLinearDistance;	//+default(0.03f)
											//+hk.RangeReal(absmin=0.0,absmax=10.0,softmax=1.0)
											//+hk.Description("This sets the max linear distance for the snap gain to work at full strength. The strength of the controller peaks at this distance.")

				/// This sets the max angular distance for the snap gain to work at full strength.
				/// The strength of the controller peaks at this distance.
				/// If the current distance is bigger than m_snapMaxAngularDistance, the snap velocity will be scaled
				/// by sqrt( maxDistane/currentDistance ).
			hkReal m_snapMaxAngularDistance;	//+default(0.1f)
												//+hk.RangeReal(absmin=0.0,absmax=10.0,softmax=1.0)
												//+hk.Description("This sets the max angular distance for the snap gain to work at full strength. The strength of the controller peaks at this distance.")

				// These useful defaults are appropriate in most circumstances
			ControlData() :
				m_hierarchyGain(0.17f),
				m_velocityDamping(0.0f),
				m_accelerationGain(1.0f),
				m_velocityGain(0.6f),
				m_positionGain(0.05f),
				m_positionMaxLinearVelocity(1.4f),
				m_positionMaxAngularVelocity(1.8f),
				m_snapGain(0.1f),
				m_snapMaxLinearVelocity(0.3f),
				m_snapMaxAngularVelocity(0.3f),
				m_snapMaxLinearDistance(0.03f),
				m_snapMaxAngularDistance(0.1f) {}

			hkBool isValid() const;

			ControlData( hkFinishLoadedObjectFlag flag ) {}

		};

		/// A structure holding data which used by this utility and has to be persistent
		struct WorkElem
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaKeyFrameHierarchyUtility::WorkElem );

			hkVector4    m_prevPosition;
			hkQuaternion m_prevRotation;
			hkVector4    m_prevLinearVelocity;
			hkVector4    m_prevAngularVelocity;
		};

		/// A structure holding the data representing the pose to drive to and the persistent data
		struct KeyFrameData
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaKeyFrameHierarchyUtility::KeyFrameData );

			/// The transformation from the ragdoll root to world space
			hkQsTransform			m_worldFromRoot;

			/// The desired pose in local space, one transform for each rigid body
			const hkQsTransform *	m_desiredPoseLocalSpace;

			/// Internal array of work data. Has to be persistent and should be initialized with the initialize() method
			struct WorkElem*		m_internalReferencePose;
		};

		/// A structure holding the rigid bodies and information about how they are linked / parented
		/// This parent indices are usually derived by looking at how the bodies are constrained; see hkaRagdollUtils::constructSkeletonForRagdoll
		struct BodyData
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaKeyFrameHierarchyUtility::BodyData );

			/// The number of rigid bodies
			int				     m_numRigidBodies;

			/// An array of rigid bodies. The velocities of these rigid bodies will be modified to match the input pose
			hkpRigidBody*const*  m_rigidBodies;

			/// The hierarchy of rigid bodies. A rigid body which has no parent has an index of-1
			const hkInt16*       m_parentIndices;

			/// An index one per body indicating which control data to use for this body
			/// If this is HK_NULL all bodies use the first element of the control data array
			int*				 m_controlDataIndices;

			/// A weight for each bone. The gains are multiplied by these weights.
			/// If this is HK_NULL then all weights are treated as 1.
			/// If this is not HK_NULL it must point to m_numRigidBodies elements.
			const hkReal* m_boneWeights;

			BodyData() : m_controlDataIndices(HK_NULL), m_boneWeights(HK_NULL) {}
		};

			/// The output
		struct Output
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaKeyFrameHierarchyUtility::Output );

				/// This output parameter is related to velocity and is calculated
				/// as a byproduct of computing the snap gain. It is a reasonable
				/// measure of the error or stress in the controller
			hkReal m_stressSquared;
		};

			/// Initialize the internal WorkElem array
		static void HK_CALL initialize(const BodyData& bodies, WorkElem* internalReferencePose);

			/// applies the required velocities in order to drive the rigid bodies of this
			/// ragdoll to the desired pose.
			/// If the output array can be HK_NULL if you wish to ignore the output
		static void HK_CALL applyKeyFrame( hkReal m_deltaTime, const KeyFrameData& pose, const BodyData& body, const ControlData* controlPalette, Output* outputArray = HK_NULL );
};


#endif // HK_KEYFRAME_HIERARCHY_UTILITY_H

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
