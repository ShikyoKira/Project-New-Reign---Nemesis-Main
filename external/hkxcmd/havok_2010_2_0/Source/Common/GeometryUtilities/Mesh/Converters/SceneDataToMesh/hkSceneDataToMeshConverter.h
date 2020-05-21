/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SCENE_DATA_TO_MESH_CONVERTER_H
#define HK_SCENE_DATA_TO_MESH_CONVERTER_H

#include <Common/Base/Math/Util/hkMathUtil.h>

#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>
#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>

#include <Common/SceneData/Mesh/hkxVertexDescription.h>

class hkxScene;
class hkxNode;
class hkxMeshSection;
class hkxVertexBuffer;
class hkxMaterial;
class hkxMesh;
class hkMeshSystem;
class hkMeshBody;
class hkMeshMaterial;
class hkMeshSectionBuilder;
class hkQsTransform;
class hkTransform;

/// A converter from the Havok HKX scene data formats into more generic hkMeshShape interfaces
class hkSceneDataToMeshConverter
{
    public:
			/// defines which transform to apply
		enum Space
		{
			SPACE_ONLY_USING_SCALE_SKEW,		///< only extract skew and scale from the node matrix and apply it
			SPACE_TO_WORLD_SPACE,				///< use the full matrix of node matrix
			SPACE_LOCAL,						///< do not apply any matrix multiplies
		};
			
            /// Converts a hkxNode into a hkxMeshShape
            /// The node has to be an hkxMeshNode, and the scene has to be the scene the node came from. The hkMeshShape will be constructed
            /// using the mesh system object passed in.
            /// The overrideMaterial parameter, if passed as HK_NULL the materials will be looked up on the material repository on the
            /// mesh system. If it is non-null the whole shape will use in the passed in material - overriding the materials specified in
            /// the hkxMeshNode
        static hkMeshShape* HK_CALL convert(hkMeshSystem* meshSystem, hkMeshMaterial* overrideMaterial, const class hkxScene* scene, const hkxNode* node, Space space);

			/// Converts a hkxMesh into a hkMeshShape - see previous convert method for details.
		static hkMeshShape* HK_CALL convert(hkMeshSystem* meshSystem, hkMeshMaterial* overrideMaterial, const hkMatrix4& worldTransform, hkxMesh* mesh);
			
            /// Creates a vertex buffer from a hkxVertexBuffer. The decomposition parameter is used for baking in transform information -
            /// for example if the modeler had skew or scaling in its transform matrices.
        static hkMeshVertexBuffer* HK_CALL convertVertexBuffer(hkMeshSystem* meshSystem, hkMathUtil::Decomposition& decomposition, hkxVertexBuffer* srcVertex);

            /// Convert usage value to hkVertexFormat usage type
        static hkVertexFormat::ComponentUsage HK_CALL convertUsage(hkxVertexDescription::DataUsage usage);

            /// Convert a element desc to hkVertexFormat::Element
        static void HK_CALL convertToElement(const hkxVertexDescription::ElementDecl* decl, hkVertexFormat::Element& ele);

            /// Convert the indices specified in the section, and add them to the current mesh section in the
            /// mesh section builder. \a vertexBufferOffset is added to all indices.
        static hkResult HK_CALL convertIndices(const hkxMeshSection& section, hkMeshSectionBuilder& builder, hkMeshVertexBuffer* vertexBuffer, hkMeshMaterial* dstMaterial);

			/// Convert an hkxMaterial into a hkMeshMaterial
		static hkMeshMaterial* convert(hkMeshSystem* meshSystem, hkxMaterial* material);

			/// Retrieves the vertex positions from a hkMeshShape
		static void HK_CALL collectVertexPositions(const hkMeshShape* mesh, hkArray<hkVector4>& verticesInOut);
};

#endif // HK_SCENE_DATA_TO_MESH_CONVERTER_H

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
