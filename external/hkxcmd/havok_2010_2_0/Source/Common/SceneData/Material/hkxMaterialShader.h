/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_MATERIAL_HKXMATERIALSHADER_HKCLASS_H
#define HKSCENEDATA_MATERIAL_HKXMATERIALSHADER_HKCLASS_H

extern const class hkClass hkxMaterialShaderClass;

class hkxMaterialShader : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
		HK_DECLARE_REFLECTION();
	
		hkxMaterialShader() { }
		hkxMaterialShader(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_name(f), m_vertexEntryName(f), m_geomEntryName(f), m_pixelEntryName(f), m_data(f) {}

			/// Used by textures to hint at their usage.
		enum ShaderType
		{
			EFFECT_TYPE_INVALID,
			EFFECT_TYPE_UNKNOWN,
			EFFECT_TYPE_HLSL_INLINE, // As a source text file, built-in
			EFFECT_TYPE_CG_INLINE, // As a source text file, built-in
			EFFECT_TYPE_HLSL_FILENAME, // As a source text filename
			EFFECT_TYPE_CG_FILENAME, // As a source text filename
			EFFECT_TYPE_MAX_ID
		};
				
		//
		// Members
		//
	public:
		
			/// The name of the material shader
		hkStringPtr m_name;

			/// What type of shader is this?
		hkEnum<ShaderType, hkUint8> m_type;

			/// Entry points
		hkStringPtr m_vertexEntryName;
		hkStringPtr m_geomEntryName;
		hkStringPtr m_pixelEntryName;

		/// The data for the effect (source, or filename)
		hkArray<hkUint8> m_data;
};

#endif // HKSCENEDATA_MATERIAL_HKXMATERIALSHADER_HKCLASS_H

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
