/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MEMORY_MESH_MATERIAL_H
#define HK_MEMORY_MESH_MATERIAL_H

#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>
#include <Common/GeometryUtilities/Mesh/hkMeshTexture.h>

class hkMemoryMeshTexture;

extern const hkClass hkMemoryMeshMaterialClass;


/// A memory based implementation of an hkMeshMaterial
class hkMemoryMeshMaterial: public hkMeshMaterial
{
    public:
        HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);
			/// Ctor
        hkMemoryMeshMaterial(const char* name = HK_NULL);
			/// Serialization Ctor
		hkMemoryMeshMaterial( hkFinishLoadedObjectFlag flag );

			// hkReferencedObject implementation
		virtual const hkClass* getClassType() const { return &hkMemoryMeshMaterialClass; }
			// hkReferencedObject implementation
		virtual void calcContentStatistics( hkStatisticsCollector* collector,const hkClass* cls ) const;


            // hkMeshMaterial implementation
        virtual hkResult createCompatibleVertexFormat(const hkVertexFormat& format, hkVertexFormat& compatibleFormat);
            // hkMeshMaterial implementation
        virtual hkMeshVertexBuffer* createCompatibleVertexBuffer(hkMeshVertexBuffer* buffer);
            // hkMeshMaterial implementation
        virtual bool isCompatible(const hkMeshVertexBuffer* buffer);

			// hkMeshMaterial implementation
		virtual void setName(const char* name);
			// hkMeshMaterial implementation
		virtual const char* getName() const;
			
			// hkMeshMaterial implementation
		virtual int getNumTextures() const;
			// hkMeshMaterial implementation
		virtual hkMeshTexture* getTexture(int index) const;
			// hkMeshMaterial implementation
		virtual void addTexture(hkMeshTexture* texture);		
		
	public:

        hkStringPtr m_materialName;
		hkArray< hkRefPtr<hkMeshTexture> > m_textures;
};


#endif // HK_MEMORY_MESH_MATERIAL_H

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
