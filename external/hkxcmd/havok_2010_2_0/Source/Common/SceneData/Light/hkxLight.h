/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_LIGHT_HKXLIGHT_HKCLASS_H
#define HKSCENEDATA_LIGHT_HKXLIGHT_HKCLASS_H


/// hkxLight meta information
extern const class hkClass hkxLightClass;

/// The information needed to construct a simple light.
class hkxLight : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
		HK_DECLARE_REFLECTION();
	
		hkxLight() { }
		hkxLight(hkFinishLoadedObjectFlag f) : hkReferencedObject(f) { }

			/// Used to specify the type of the light.
		enum LightType
		{
				/// 
			POINT_LIGHT,
				/// 
			DIRECTIONAL_LIGHT,
				/// 
			SPOT_LIGHT
		};
		
		//
		// Members
		//
	public:
		
			/// The type of the light.
		hkEnum<LightType, hkInt8> m_type;
		
			/// The position (used for point and spot lights).
		hkVector4 m_position;
		
			/// The direction(used for directional and spot lights).
		hkVector4 m_direction;
		
			/// The RGB value of the light color.
		hkUint32 m_color;
		
			/// The angle, in radians of the spot light (full extent).
		hkReal m_angle;
};

#endif // HKSCENEDATA_LIGHT_HKXLIGHT_HKCLASS_H

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
