/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULT_MESH_MATERIAL_REGISTRY_H
#define HK_DEFAULT_MESH_MATERIAL_REGISTRY_H

#include <Common/GeometryUtilities/Mesh/hkMeshMaterialRegistry.h>

extern const hkClass hkDefaultMeshMaterialRegistryClass;

/// Simple implementation of a hkMeshMaterialRegistry
///
/// The loadMaterial method just calls 'findMaterial'. For a registry that will load up an hkMeshSystem specific material
/// deriving from this class and implementing 'loadMaterial' is much simpler than having to implement the whole
/// of the hkMeshMaterialRegistry
///
/// \sa hkMeshMaterialRegistry
class hkDefaultMeshMaterialRegistry: public hkMeshMaterialRegistry
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);


            // hkMeshMaterialRegistry implementation
        virtual hkMeshMaterial* loadMaterial(const char* name);

            // hkMeshMaterialRegistry implementation
        virtual hkMeshMaterial* findMaterial(const char* name);

            // hkMeshMaterialRegistry implementation
        virtual void registerMaterial(const char* name, hkMeshMaterial* material);

            // hkMeshMaterialRegistry implementation
        virtual void unregisterMaterial(const char* name);

            // hkMeshMaterialRegistry implementation
		virtual void getMaterials(hkArrayBase<hkMeshMaterial*>& materials, hkMemoryAllocator& a);

            // hkMeshMaterialRegistry implementation
        virtual const char* getMaterialName(hkMeshMaterial* material);

            // hkReferencedObject Implementation
        virtual const hkClass* getClassType() const { return &hkDefaultMeshMaterialRegistryClass; }

            // hkReferencedObject Implementation
        virtual void calcContentStatistics( hkStatisticsCollector* collector,const hkClass* cls ) const;

			/// A material entry
        struct Entry
        {
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkDefaultMeshMaterialRegistry::Entry );
			
            hkStringPtr m_name;						///< the registered name of the material
            hkRefPtr<hkMeshMaterial> m_material;	///< the material
        };

    protected:

		int _findEntryIndex(const char* name) const;


		//
		// Members
		//

	protected:
	
        hkArray<Entry> m_entries; ///< The registered materials
};

#endif // HK_DEFAULT_MESH_MATERIAL_REGISTRY_H

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
