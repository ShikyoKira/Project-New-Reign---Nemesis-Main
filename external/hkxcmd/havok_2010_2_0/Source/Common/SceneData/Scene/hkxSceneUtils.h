/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKX_SCENE_UTILS_H
#define HKX_SCENE_UTILS_H

#include <Common/Base/Math/Util/hkMathUtil.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>

/// Scene utilities
class hkxSceneUtils
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkxSceneUtils);
			/// Options taken by "hkxSceneUtils::transformScene"
		struct SceneTransformOptions
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkxSceneUtils::SceneTransformOptions );

				/// Should we apply this transformation to scene graph nodes
			hkBool m_applyToNodes;

				/// Should we apply this transformation to vertex buffers
			hkBool m_applyToBuffers;

				/// Should we apply this transformation to float channels
			hkBool m_applyToFloatChannels;

				/// Should we apply this transformation to lights
			hkBool m_applyToLights;

				/// Should we apply this transformation to cameras
			hkBool m_applyToCameras;

				/// Should we flip index buffer winding
			hkBool m_flipWinding;

				/// The transform to apply
			hkMatrix3 m_transformMatrix;
		};

			/// Given a scene and the options specified in the SceneTransformOption struct, it goes
			/// through nodes, attributes, meshes, etc.. Applying the specified transform to the scene
			/// Useful for scaling scenes as well as for transforming coordinate systems
		static void HK_CALL transformScene( class hkxScene& scene, const SceneTransformOptions& opts );

			/// Extracts environment data from an hkxScene - used mostly for backwards compatibility as previously
			/// environment information was stored in the hkxScene object. The variables extracted are:
			/// "asset" (ex: "car"),
			/// "assetPath" (ex: "c:/temp/car.max"),
			/// "assetFolder" (ex: "c:/temp/"),
			/// "modeller" (ex: "3ds max 8.0.0"),
			/// "selected" (ex: "chassis")
		static void HK_CALL fillEnvironmentFromScene (const class hkxScene& scene, class hkxEnvironment& environment);

			/// Reorders the nodes in an hkxScene by their names. It does so alphabetically, regardless of case.
		static void HK_CALL reorderNodesAlphabetically ( class hkxScene& scene );

			/// Checks the contents of the node, and if it's a mesh it returns it, or if it's a skin binding, it returns the mesh associated
			/// with it.
		static class hkxMesh* HK_CALL getMeshFromNode (const class hkxNode* aNode);

			// Reverse lookup. Handy to get a name for a mesh
		static const hkxNode* HK_CALL findFirstNodeUsingMesh(const hkxNode* rootNode, const class hkxMesh* aMesh);

			/// finds the first mesh in the scene
		static hkxNode* HK_CALL findFirstMeshNode(hkxScene* scene);

		struct GraphicsNode
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkxSceneUtils::GraphicsNode);
			hkxNode* m_node;
			const char* m_name;
		};

			/// finds all nodes under node, which have a mesh attached, which is probably visible in the Havok demos.
			/// Also if the mesh name can be found in extraNodesToFind, the node will be returned.
		static void HK_CALL findAllGraphicsNodes(bool collectShapes, bool ignorePShapes, const hkStringMap<int>& extraNodesToFind, hkxNode* node, hkArray<GraphicsNode>& nodesOut);

			/// finds all nodes under node
		static void HK_CALL findAllNodes(hkxNode* node, hkArray< hkRefPtr<hkxNode> >& nodes );

			/// Finds all nodes under node, which have a mesh attached and optionally compute its corresponding worldFromLocal transform.
			/// This function gets called recursively and hence for the root we need to pass in the identity for the rootTransform.
		static void HK_CALL findAllMeshNodes(	hkxScene* scene, 
												hkxNode* node, 
												hkArray< hkRefPtr<hkxNode> >& nodes, 
												hkMatrix4* rootTransform = HK_NULL,
												hkArray<hkMatrix4>* worldFromLocalTransforms = HK_NULL);

			// Helper function used by the createskin filter which allows for *translations* as well as rotations
		static void transformVertexBuffer( const hkTransform& tr, class hkxVertexBuffer& vbuffer);

	public:

			// Contains useful information about the transform
		struct TransformInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkxSceneUtils::TransformInfo );

				// The transform as a Matrix4
			hkMatrix3 m_transform;

				// The inverse of the transform
			hkMatrix3 m_inverse;

				// The inverse of the transform, transposed
			hkMatrix3 m_inverseTranspose;

				// The transform decomposed
			hkMathUtil::Decomposition m_decomposition;
		};

	private:

			// Transforms a node and its children. It also transform node attributes
		static void transformNode( const TransformInfo& transformInfo, class hkxNode& node);

		static void transformSkinBinding( const TransformInfo& transformInfo, class hkxSkinBinding& skinBinding);

		static void transformVertexBuffer( const TransformInfo& transformInfo, class hkxVertexBuffer& vbuffer);

		static void transformFloatChannel( const TransformInfo& transformInfo, class hkxVertexFloatDataChannel& floatChannel);

		static void transformLight( const TransformInfo& transformInfo, class hkxLight& light);

		static void transformCamera( const TransformInfo& transformInfo, class hkxCamera& camera);

		static void flipWinding( class hkxIndexBuffer &ibuffer );

			// Called by transformNode
		static void transformAttributeGroup ( const TransformInfo& transformInfo, struct hkxAttributeGroup& attributeGroup);

			// Called by transformAttributeGroup
		static void transformAnimatedFloat (const TransformInfo& transformInfo, class hkxAnimatedFloat& animatedFloat);
		static void transformAnimatedQuaternion (const TransformInfo& transformInfo, class hkxAnimatedQuaternion& animatedQuaternion);
		static void transformAnimatedMatrix (const TransformInfo& transformInfo, class hkxAnimatedMatrix& animatedMatrix);
		static void transformAnimatedVector (const TransformInfo& transformInfo, class hkxAnimatedVector& animatedVector);

			// Transforms a fullMatrix4, reused in different places
		static void transformMatrix4 (const TransformInfo& transformInfo, hkMatrix4& matrix4);

};

#endif // HK_SCENE_UTILS_H

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
