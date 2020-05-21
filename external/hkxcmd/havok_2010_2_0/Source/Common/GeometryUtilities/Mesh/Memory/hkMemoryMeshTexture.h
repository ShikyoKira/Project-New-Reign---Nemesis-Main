/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MEMORY_MESH_TEXTURE_H
#define HK_MEMORY_MESH_TEXTURE_H

#include <Common/GeometryUtilities/Mesh/hkMeshTexture.h>

extern const class hkClass hkMemoryMeshTextureClass;

	/// A memory based implementation of an hkMeshTexture
class hkMemoryMeshTexture: public hkMeshTexture
{
	public:
		//+version(2)		
		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// Default Ctor
		hkMemoryMeshTexture();
			/// Serialization Ctor
		hkMemoryMeshTexture( hkFinishLoadedObjectFlag flag ): hkMeshTexture(flag), m_filename(flag), m_data(flag) {}

			// hkMeshTexture implementation
		virtual void getData(hkUint8*& data, int& size, Format& format);
			// hkMeshTexture implementation
		virtual void setData(hkUint8* data, int size, Format format);
			
			// hkMeshTexture implementation
		virtual const char* getFilename() const;
			// hkMeshTexture implementation
		virtual void setFilename( const char* filename );

			// hkMeshTexture implementation
		virtual bool getHasMipMaps() const;
			// hkMeshTexture implementation
		virtual void setHasMipMaps(bool hasMipMaps);
		
			// hkMeshTexture implementation
		virtual FilterMode getFilterMode() const;
			// hkMeshTexture implementation
		virtual void setFilterMode(FilterMode filterMode);

			// hkMeshTexture implementation
		virtual TextureUsageType getUsageHint() const;
			// hkMeshTexture implementation
		virtual void setUsageHint( TextureUsageType hint );

			// hkMeshTexture implementation
		virtual hkInt32 getTextureCoordChannel() const;
			// hkMeshTexture implementation
		virtual void setTextureCoordChannel( hkInt32 channelIndex );

	protected:
			/// Filename of the texture
		hkStringPtr m_filename;
			/// Raw texture data
		hkArray<hkUint8> m_data;
			/// Format of texture data
		hkEnum<hkMeshTexture::Format, hkInt8> m_format;		
			/// True if the texture has mipmaps, false otherwise
		hkBool m_hasMipMaps;
			/// Filtering mode to use with the texture
		hkEnum<hkMeshTexture::FilterMode, hkInt8> m_filterMode;
			/// Texture usage hint
		hkEnum<hkMeshTexture::TextureUsageType, hkInt8> m_usageHint;
			/// Channel in the vertex buffer
		hkInt32 m_textureCoordChannel;
		
};

#endif	//HK_MEMORY_MESH_TEXTURE_H

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
