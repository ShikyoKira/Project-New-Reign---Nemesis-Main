/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_1D_ANGULAR_FOLLOW_COM_CINFO
#define HK_1D_ANGULAR_FOLLOW_COM_CINFO


	/// All the parameters needed to construct a hkp1dAngularFollowCam
class hkp1dAngularFollowCamCinfo 
{
	public:
		
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CAMERA, hkp1dAngularFollowCamCinfo);

		hkp1dAngularFollowCamCinfo();

		struct CameraSet
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkp1dAngularFollowCamCinfo::CameraSet );

			/// The resting position of the camera in unrotated space. Default: ( 0.0f, 12.0f, 5.5f)
			hkVector4 m_positionUS;

			/// The position in unrotated space. Default: (0, 0, 0)
			hkVector4 m_lookAtUS;

			/// The field of view (f.o.v.) in radians. Default: 1.0
			hkReal m_fov;

			/// m_velocity [m/sec] defines the velocity for which this parameter set gets a weight of 1.0f. Default: 0.0
			/// Normally the hkReal parameters are interpolated from two parameter sets.
			/// The m_velocity parameter of set 1 defines the velocity
			/// for which set 1 gets a weight of 1.0f.
			/// Example: Set0.m_velocity = 10.0f and Set1.m_velocity = 20.0f.
			///
			/// In this case for:
			///   - velocities < 10 Set0 is used.
			///   - velocities 10-20 Set0 and Set1 are interpolated
			///   - velocities >20 Set1 is used
			hkReal m_velocity;
			

			/// This parameter defines the magnitude of the influence of the velocity for the camera direction. Default 0.01
			/// Normally the camera direction is based on the direction the car.
			/// However in some situations the camera should also use the velocity
			/// direction. So this parameter is a factor to the current velocity
			/// to be added to the direction of the car to calculate the ideal
			/// camera direction.
			hkReal m_speedInfluenceOnCameraDirection;
		
			/// The angular speed the camera follows the car. Default 4.0
			/// m_angularRelaxation is used to make the yaw angle of the camera follow
			/// the direction of the car. Values from 1.0f to 10.0f are a good choice
			hkReal m_angularRelaxation;

			/// Construct with default values
			CameraSet();
		};

	public:

		/// An extra angular add-on to the yaw angle (needed if the internal angle calculation has an offset)
		hkReal m_yawCorrection;

		/// This member can be set to -1.0f (instead of 1.0f) to reverse the camera yaw angle
		hkReal m_yawSignCorrection;

		/// A normalized vector pointing up in world space.
		hkVector4 m_upDirWS;

		/// A normalized vector pointing forward in the local space of the rigid body
		hkVector4 m_rigidBodyForwardDir;

		/// Two parameters sets for the car values.
		/// Depending on the velocity of the car, the parameters actually
		/// used are calculated by interpolating between these two sets
		/// \verbatim
		/// using the following formula:
		/// hkReal f1 = min (1.0f, current_velocity / set[1].m_velocity);
		/// hkReal f0 = 1.0f - f1;
		/// set = f0 * m_set[0] + f1 * m_set[1]
		/// \endverbatim
		CameraSet m_set[2];
		

};

#endif /* HK_1D_ANGULAR_FOLLOW_COM_CINFO */

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
