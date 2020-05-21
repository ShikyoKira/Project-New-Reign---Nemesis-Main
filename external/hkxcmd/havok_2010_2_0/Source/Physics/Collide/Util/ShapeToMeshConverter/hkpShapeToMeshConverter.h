/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_SHAPE_TO_MESH_CONVERTER_H
#define HKP_SHAPE_TO_MESH_CONVERTER_H

#include <Common/GeometryUtilities/Mesh/hkMeshSystem.h>
#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>

class hkpShape;

	/// Havok demo helper utility
class hkpShapeToMeshConverter
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpShapeToMeshConverter );

			/// Create a mesh shape from vertices and triangle indices - map with box mapping.
		static hkMeshShape* HK_CALL createMeshShape( hkMeshSystem* system, const hkArray<hkVector4>& vertices, const hkArray<hkVector4>& normals, const hkArray<hkUint16>& triangleIndices, const hkMatrix4& texCoordTransform, hkMeshMaterial* material);

			/// Compute the box mapping texture coordinates.
		static void HK_CALL computeBoxTextureCoords( const hkVector4& pos, const hkVector4& norm, hkVector4& uvOut, hkReal textureMapScale);

			/// Create a mesh shape physics shape
			/// cosSmoothingAngle - is the cos of the maximum angle between triangle normals to allow smoothing
		static hkMeshShape* HK_CALL convertShapeToMesh( hkMeshSystem* system, const hkpShape* physicsShape, hkReal foldFactor, const hkMatrix4& texCoordsTransform, hkMeshMaterial* material, hkReal cosSmoothingAngle = 1.0f);

};

#endif // HKP_SHAPE_TO_MESH_CONVERTER_H

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
