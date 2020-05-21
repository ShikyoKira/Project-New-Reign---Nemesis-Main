/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SCENE_DATA_TO_GEOMETRY_CONVERTER_H
#define HK_SCENE_DATA_TO_GEOMETRY_CONVERTER_H

struct hkGeometry;
class hkxScene;
class hkxNode;
class hkxMesh;
class hkxMaterial;
class hkpNamedMeshMaterial;

/// A conversion utility for creating hkGeometry from hkxMeshes contained in a hkxNode scene graph.
struct hkxSceneDataToGeometryConverter
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkxSceneDataToGeometryConverter);
		/// A struct to represent instanced geometry
		struct GeometryInstances
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkxSceneDataToGeometryConverter::GeometryInstances);
			struct Instance
			{
				hkMatrix4 m_worldFromLocal;
				int       m_geometryIdx;
			};

			/// Instances of unique hkGeometry
			hkArray< Instance >	m_instances;

			/// Unique geometries in local space
			hkArray< hkGeometry > m_geometries;
		};

			/// Convert all mesh nodes found below root to a single geometry, which is merged with geometryInOut.
			/// Materials in the output geometry index the materials in the materialsInOut array.
		static void	HK_CALL convertToSingleGeometry( const hkxScene* scene, const hkxNode* root, hkGeometry& geometryInOut, hkArray<hkxMaterial*>& materialsInOut );

			/// Convert all mesh nodes found below the nodes in the given array root to a single geometry, which is merged with geometryInOut.
			/// Materials in the output geometry index the materials in the materialsInOut array.
		static void	HK_CALL convertToSingleGeometry( const hkxScene* scene, const hkArray<hkRefPtr<hkxNode> >& roots, hkGeometry& geometryInOut, hkArray<hkxMaterial*>& materialsInOut );

			/// Convert all mesh nodes found below root to instanced geometries.
			/// A single hkGeometry is created for each unique hkxMesh in the scene.
			/// If hkxMeshes are shared then hkGeometries are correspondingly shared.
		static void	HK_CALL convertToGeometryInstances( const hkxScene* scene, const hkxNode* root, GeometryInstances& geometriesOut, hkArray<hkxMaterial*>& materialsInOut );

			/// Creates a single hkGeometry from the geometry instances. The final geometryInOut exists in world space.
		static void HK_CALL mergeInstances( const GeometryInstances& geometriesIn, hkGeometry& geometryInOut );

protected:

		typedef hkPointerMap<hkxMesh*, int> MeshToGeometryIndexMap;

		static void HK_CALL convertToSingleGeometryRecursive( const hkxNode* node, const hkMatrix4& transform, hkGeometry& geometryInOut, hkArray<hkxMaterial*>& materialsInOut );

		static void	HK_CALL convertToGeometryInstancesRecursive( const hkxNode* node, const hkMatrix4& transform, GeometryInstances& geometriesOut, hkArray<hkxMaterial*>& materialsInOut, MeshToGeometryIndexMap& meshToGeometryIndexMap );
};

#endif	// HK_SCENE_DATA_TO_GEOMETRY_CONVERTER_H

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
