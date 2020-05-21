/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES_KEYFRAME_UTILITY_H
#define HK_UTILITIES_KEYFRAME_UTILITY_H

#include <Common/Base/hkBase.h>

class hkpRigidBody;

	/// This class contains a set of utility functions to help calculate and apply velocities to rigid bodies
	/// in order to make them follow a user-defined position/orientation and velocity. Thus is can be used to make
	/// rigid bodies follow "keyframes", either exactly, or in an approximate "blended" manner.
	/// NOTE: You must be careful when switching from using hard to soft keyframes:
	/// All keyframes eventually require "centre-of-mass" positions in order to correctly compute the velocities
	/// to set. The applyHardKeyFrame() method automatically converts Local positions to "centre-of-mass" positions, however
	/// applySoftKeyFrame() takes a KeyFrameInfo which must have "centre-of-mass" positions set explicitly.
class hkpKeyFrameUtility
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpKeyFrameUtility );

		///You use an AccelerationInfo to specify acceleration details for hkpKeyFrameUtility::applySoftKeyFrame
		struct AccelerationInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpKeyFrameUtility::AccelerationInfo );

			AccelerationInfo();

				/// This parameter controls the linear stiffness of applySoftKeyFrame, its units are 1/Time.
				/// E.g. If this value is 2, then the applySoftKeyFrame function will set the body's velocity such
				/// that it would achieve the position of the next keyframe in 0.5 = 1/2 seconds.
				/// Valid values range between 0 and 1/deltaTime
			hkVector4 m_linearPositionFactor;

				/// This parameter controls the angular stiffness of applySoftKeyFrame, its units are 1/Time.
				/// E.g. If this value is 2, then the applySoftKeyFrame function will set the body's velocity such
				/// that it would achieve the orientation of the next keyframe in 0.5 = 1/2 seconds.
				/// Valid values range between 0 and 1/deltaTime
			hkVector4 m_angularPositionFactor;

				/// This parameter controls the velocity control of applySoftKeyFrame, its units are 1/Time.
				/// E.g. If this value is 2, then the applySoftKeyFrame function will set the body's velocity such
				/// that it would achieve the linear velocity of the next keyframe in 0.5 = 1/2 seconds.
				/// Valid values range between 0 and 1/deltaTime
			hkVector4 m_linearVelocityFactor;

				/// This parameter controls the velocity control of applySoftKeyFrame, its units are 1/Time.
				/// E.g. If this value is 2, then the applySoftKeyFrame function will set the body's velocity such
				/// that it would achieve the angular velocity of the next keyframe in 0.5 = 1/2 seconds.
				/// Valid values range between 0 and 1/deltaTime
			hkVector4 m_angularVelocityFactor;

				/// Maximum linear acceleration. If the velocity changes required by the applySoftKeyFrame
				/// function would exceed this acceleration, they are clipped against this.
			hkReal m_maxLinearAcceleration;
				/// The maximum angular accelerations (m/sec2). If the velocity changes required by the applySoftKeyFrame
				/// function would exceed this acceleration, they are clipped against this.
			hkReal m_maxAngularAcceleration; 

				/// If the distance between object and keyframe gets bigger than the max allowed distance, the object is
				/// immediately "warped" to the correct keyframe position/orientation, and corresponding velocities.
			hkReal m_maxAllowedDistance;

		};


			/// You use a KeyFrameInfo to specify keyframe details for hkpKeyFrameUtility::applySoftKeyFrame.
			/// NOTE: All the positions for SOFT keyframes must be "centre-of-mass" positions. To convert a Local position
			/// to a "centre-of-mass" position, you must add on rb->getCentreOfMassLocal() rotated by m_orientation.
			/// It contains the current keyframe position, orientation, angular velocity, and linear velocity.
			/// It also provides functions for calculating the angular and linear velocities if you only have the current and target positions and orientations.
		struct KeyFrameInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpKeyFrameUtility::KeyFrameInfo );

				/// The current "centre-of-mass" position of a keyframe
			hkVector4 m_position;
				/// The current orientation
			hkQuaternion m_orientation;

				/// The current velocity
			hkVector4 m_linearVelocity;

				/// The current angular velocity
			hkVector4 m_angularVelocity;

				/// Convenience function for setting velocities if you only have "centre-of-mass" positions and orientations
			void setUsingPositionOrientationPair(  const hkVector4& currentPosition, const hkQuaternion& currentOrientation, const hkVector4& nextPosition, const hkQuaternion& nextOrientation, hkReal invDeltaTime );

				/// Convenience function for setting velocities if you only have "centre-of-mass" positions and orientations. Uses approximation of angular velocity.
			void fastSetUsingPositionOrientationPair(  const hkVector4& currentPosition, const hkQuaternion& currentOrientation, const hkVector4& nextPosition, const hkQuaternion& nextOrientation, hkReal invDeltaTime );
		};


			/// Change velocities of body to move towards keyframe given acceleration guidelines, in time deltaTime.
			/// Uses several fast approximations for angular calculations and exponential falloff.
		static void HK_CALL applySoftKeyFrame( const KeyFrameInfo& keyFrameInfo, AccelerationInfo& accelInfo, hkReal deltaTime, hkReal invDeltaTime, hkpRigidBody* body );

			/// Change velocities of body to move to future (position, orientation) in time deltaTime
			/// Note: You must pass in INVERSE deltaTime
			/// NOTE: All the positions for HARD keyframes are automatically converted to be "centre-of-mass" positions
			/// using the information in the body supplied.
		static void HK_CALL applyHardKeyFrame( const hkVector4& nextPosition, const hkQuaternion& nextOrientation, hkReal invDeltaTime, hkpRigidBody* body);


			/// This function should be used instead of hkpKeyFrameUtility::applyHardKeyFrame when stepping asynchronously
		static void HK_CALL applyHardKeyFrameAsynchronously( const hkVector4& nextPosition, const hkQuaternion& nextOrientation, hkReal invDeltaTime, hkpRigidBody* body);


};

#endif // HK_UTILITIES_KEYFRAME_UTILITY_H

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
