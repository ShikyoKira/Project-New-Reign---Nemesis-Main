/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MESH_TO_HK_GEOMETRY_CONVERTER_H
#define HK_MESH_TO_HK_GEOMETRY_CONVERTER_H

class hkMeshShape;
class hkMeshSystem;
class hkMeshMaterial;
struct hkGeometry;

/// Simple converter to convert from a mesh to an hkGeometry
///
/// hkGeometry is a simplified geometry structure which just consists of vertices and triangles.
class hkMeshTohkGeometryConverter
{
    public:
			/// Get simple lists of all vertices and indices of a mesh section.
			/// If the arrays are not empty, the vertices and indices of the section are appended and the indices are corrected to account for the vertex offset.
		static void HK_CALL getVerticesIndicesFromMeshSection(const hkMeshShape* shape, int sectionIndex, hkArray<hkVector4>& vertices, hkArray<int>& indices);

			/// Simple converter to convert from a mesh to an hkGeometry
        static hkGeometry* HK_CALL convert(const hkMeshShape* shape);

			/// Simple converter to convert from a hkGeometry to a mesh. Will copy all hkGeom data.
		static hkMeshShape* HK_CALL convert(const hkGeometry* geom, hkMeshSystem* meshSystem, hkMeshMaterial* overrideMaterial );
};

#endif // HK_MESH_TO_HK_GEOMETRY_CONVERTER_H

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
