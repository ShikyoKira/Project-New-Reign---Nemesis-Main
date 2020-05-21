/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MESH_MATERIAL_REGISTRY_H
#define HK_MESH_MATERIAL_REGISTRY_H

class hkMeshMaterial;

extern const hkClass hkMeshMaterialRegistryClass;

/// A registry for materials.
///
/// Note that materials do not need to be registered in the hkMaterialRegistry to work. The registry's only purpose is
/// to map names to materials and keep them in scope. A name must be unique for a material.
///
/// There may be an argument for having hkMaterials held in the registry that are not reference counted. When they go
/// out of scope they are removed automatically from the registry - this is an implementation specific feature though.
///
/// \sa hkMeshMaterial
class hkMeshMaterialRegistry: public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			
			/// If a material with this name is found it is returned. If not load will try and create the material
			/// from the name - this may entail reading the material description from disk - but is implementation specific.
			/// Method return HK_NULL if couldn't find or load a material with the specified name.
        virtual hkMeshMaterial* loadMaterial(const char* name) = 0;

			/// Find a material (that already exists). If not returns HK_NULL
        virtual hkMeshMaterial* findMaterial(const char* name) = 0;

			/// Register a material with a name. If there is already a material with the specified name it will be replaced.
        virtual void registerMaterial(const char* name, hkMeshMaterial* material) = 0;

			/// Remove the material
        virtual void unregisterMaterial(const char* name) = 0;

            /// Fills in an array with all of the materials
		inline void getMaterials(hkArray<hkMeshMaterial*>& materials)
		{ 
			getMaterials(materials, hkContainerHeapAllocator().get(&materials));;
		}

        virtual void getMaterials(hkArrayBase<hkMeshMaterial*>& materials, hkMemoryAllocator& a) = 0;

            /// Gets a materials name, or returns HK_NULL if it doesn't have a registered name
        virtual const char* getMaterialName(hkMeshMaterial* material) = 0;
};

#endif // HK_MESH_MATERIAL_REGISTRY_H

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
