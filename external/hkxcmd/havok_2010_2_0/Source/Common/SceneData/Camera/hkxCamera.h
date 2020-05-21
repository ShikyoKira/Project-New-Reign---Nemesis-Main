/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_CAMERA_HKXCAMERA_HKCLASS_H
#define HKSCENEDATA_CAMERA_HKXCAMERA_HKCLASS_H

/// hkxCamera meta information
extern const class hkClass hkxCameraClass;

/// The information needed to construct a simple camera.
class hkxCamera : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
public: 

	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
	HK_DECLARE_REFLECTION();

	hkxCamera() { }
	hkxCamera(hkFinishLoadedObjectFlag f) : hkReferencedObject(f) { }

		//
		// Members
		//
	public:
		
			/// The position of the viewer in world space
		hkVector4 m_from;
		
			/// The focal point of the camera.
		hkVector4 m_focus;
		
			/// Up wrt the viewer.
		hkVector4 m_up;
		
			/// The angle, in radians, of the vertical field of view.
		hkReal m_fov;
		
			/// Distance along the direction to the far clipping plane.
		hkReal m_far;
		
			/// Distance along the direction to the near clipping plane.
		hkReal m_near;

			/// True if the camera is left handed
		hkBool m_leftHanded;
};

#endif // HKSCENEDATA_CAMERA_HKXCAMERA_HKCLASS_H

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
