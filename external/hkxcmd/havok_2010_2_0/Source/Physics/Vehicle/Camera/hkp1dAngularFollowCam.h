/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_1D_ANGULAR_FOLLOW_COM
#define HK_1D_ANGULAR_FOLLOW_COM

#include <Common/Base/hkBase.h>
#include <Physics/Vehicle/Camera/hkp1dAngularFollowCamCinfo.h>


/// The hkp1dAngularFollowCam attaches a camera to a vehicle to aid rendering.
/// The position of the camera rotates around the object using a single axle
/// (normally the up axis).
/// The camera tries to slowly move to a certain point (m_positionUS) defined in object space
/// and always looks at a given point in object space.
/// See hkp1dAngularFollowCamCinfo for parameter description
class hkp1dAngularFollowCam : public  hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Default constructor
		hkp1dAngularFollowCam ();

			/// Constructor.
		hkp1dAngularFollowCam (const hkp1dAngularFollowCamCinfo &bp);

			/// Destructor.
		virtual ~hkp1dAngularFollowCam();

			/// Reset all values except pointer values.
		void reinitialize( const hkp1dAngularFollowCamCinfo &bp );

		struct CameraInput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CAMERA, CameraInput);
			hkVector4		m_linearVelocity;
			hkVector4		m_angularVelocity;
			hkTransform		m_fromTrans;
			hkReal			m_deltaTime;
		};

		// Output of the camera calculations
		struct CameraOutput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CAMERA, CameraInput);
			hkVector4  m_positionWS;
			hkVector4  m_lookAtWS;
			hkVector4  m_upDirWS;
			hkReal m_fov;
			hkReal m_pad[3];
		};

			/// Immediately jump to the ideal yaw angle.
		virtual void resetCamera( const hkTransform& trans, const hkVector4& linearVelocity, const hkVector4& angularVelocity);

		virtual void calculateCamera ( const CameraInput &in, CameraOutput &out );
		
	protected:


		hkReal m_cameraYawAngle;

		hkReal m_yawCorrection;
		hkReal m_yawSignCorrection;

		hkVector4 m_upDirWS;
		hkVector4 m_rigidBodyForwardDir;


		hkVector4 m_flat0DirWS;  // an orthogonal to m_upDirWS
		hkVector4 m_flat1DirWS;	 // an orthogonal to m_upDirWS and m_flat0DirWS

		hkp1dAngularFollowCamCinfo::CameraSet m_set[2];

	protected:
			/// Internal methods for calculating camera position
		HK_FORCE_INLINE hkReal calcYawAngle(const hkReal factor1, const hkTransform& trans, const hkVector4& linearVelocity);
		HK_FORCE_INLINE hkReal calcVelocityFactor(const hkVector4& bodyVelocity);
};



#endif //HK_1D_ANGULAR_FOLLOW_COM

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
