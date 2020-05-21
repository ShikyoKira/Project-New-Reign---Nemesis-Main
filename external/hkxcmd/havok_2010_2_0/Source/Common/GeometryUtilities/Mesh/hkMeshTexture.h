/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MESH_TEXTURE_H
#define HK_MESH_TEXTURE_H


extern const class hkClass hkMeshTextureClass;

	/// A texture used by a hkMeshMaterial
class hkMeshTexture: public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Default Ctor
		hkMeshTexture() {}
			/// Serialization Ctor
		hkMeshTexture( hkFinishLoadedObjectFlag flag ): hkReferencedObject(flag) {}

			/// Format of the texture data
		enum Format
		{
			Unknown,
			PNG,
			TGA,
			BMP,
			DDS
		};	

			/// Filtering mode used by the texture
		enum FilterMode
		{
			POINT,
			LINEAR,
			ANISOTROPIC
		};

		enum TextureUsageType
		{
			UNKNOWN,			
			DIFFUSE,
			REFLECTION,			
			BUMP,		
			NORMAL,			
			DISPLACEMENT,			
			SPECULAR, // Specular Level map			
			SPECULARANDGLOSS, // Specular Level map with the Gloss (power) in the Alpha channel			
			OPACITY, // Opacity (transparency) map. Normally not used, just use the alpha channel in one of the diffuse maps instead.			
			EMISSIVE, // Emissive (self illumination) map			
			REFRACTION,			
			GLOSS, // Specular Power map, normally not used (alpha in specmap quicker)			
			NOTEXPORTED
		};

			/// Gets the raw data texture
		virtual void getData(hkUint8*& data, int& size, Format& format) = 0;
			/// Sets the raw data of the texture
		virtual void setData(hkUint8* data, int size, Format format) = 0;
			
			/// Gets the filename of the texture
		virtual const char* getFilename() const = 0;
			/// Sets the filename of the texture
		virtual void setFilename( const char* filename ) = 0;

			/// Gets if the texture has mip maps
		virtual bool getHasMipMaps() const = 0;
			/// Sets if the texture has mip maps
		virtual void setHasMipMaps(bool hasMipMaps) = 0;
		
			/// Gets the filtering mode of the texture
		virtual FilterMode getFilterMode() const = 0;
			/// Sets the filtering mode of the texture
		virtual void setFilterMode(FilterMode filterMode) = 0;

			/// Gets a usage hint flag for the texture.
		virtual TextureUsageType getUsageHint() const = 0;
			/// Sets a usage hint flag for the texture.
		virtual void setUsageHint( TextureUsageType hint ) = 0;

			/// Gets the index of the texture coordinate this texture should use in the vertex buffer.
		virtual hkInt32 getTextureCoordChannel() const = 0;
			/// Sets the index of the texture coordinate this texture.
		virtual void setTextureCoordChannel( hkInt32 channelIndex ) = 0;

};

#endif	//HK_MESH_TEXTURE_H

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
