/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_SCENE_HKXSCENE_HKCLASS_H
#define HKSCENEDATA_SCENE_HKXSCENE_HKCLASS_H

#include <Common/SceneData/Graph/hkxNode.h>
#include <Common/SceneData/Selection/hkxNodeSelectionSet.h>
#include <Common/SceneData/Camera/hkxCamera.h>
#include <Common/SceneData/Light/hkxLight.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Material/hkxTextureInplace.h>
#include <Common/SceneData/Material/hkxTextureFile.h>
#include <Common/SceneData/Skin/hkxSkinBinding.h>


/// hkxScene meta information
extern const class hkClass hkxSceneClass;

/// Contains a modeller-agnostic representation of a scene, as exported by the Havok exporters from Max, Maya, or XSI.
class hkxScene : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
		HK_DECLARE_REFLECTION();

		hkxScene();
		hkxScene(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_modeller(f), m_asset(f), m_rootNode(f), m_selectionSets(f), m_cameras(f), m_lights(f), m_meshes(f), m_materials(f), m_inplaceTextures(f), m_externalTextures(f), m_skinBindings(f) { }
		virtual ~hkxScene();

		//
		// Members
		//
	public:
		
			/// A text string describing the modeller used to create this scene
		hkStringPtr m_modeller;
		
			/// The full path and filename of the asset that was exported to make this scene.
		hkStringPtr m_asset;
		
			/// The total length of the scene, in seconds. Assumes that when you are given
			/// key frames, they will be evenly spaced across this period.
		hkReal m_sceneLength;
		
			/// Scene Graph root node
		hkRefPtr< class hkxNode > m_rootNode;
		
			/// Array of selection sets in the scene. Holds ref.
		hkArray< hkRefPtr<class hkxNodeSelectionSet> > m_selectionSets;		
				
			/// Array of cameras in the scene. Holds ref.
		hkArray< hkRefPtr<class hkxCamera> > m_cameras;
		
			/// Array of cameras in the scene. Holds ref.
		hkArray< hkRefPtr<class hkxLight> > m_lights;
		
			/// Meshes in the scene. Holds ref.
		hkArray< hkRefPtr<class hkxMesh> > m_meshes;
		
			/// Materials in the scene. Holds ref.
		hkArray< hkRefPtr<class hkxMaterial> > m_materials;
		
			/// In place (loaded) textures in the scene. Holds ref.
		hkArray< hkRefPtr<class hkxTextureInplace> > m_inplaceTextures;
		
			/// External texture filenames. Holds ref.
		hkArray< hkRefPtr<class hkxTextureFile> > m_externalTextures;
		
			/// Extra mesh info required to bind a skin to a hierarchy. Holds ref.
		hkArray< hkRefPtr<class hkxSkinBinding> > m_skinBindings;
		
			/// Transform applied to the scene
		hkMatrix3 m_appliedTransform; //+default(1 0 0 0  0 1 0 0  0 0 1 0)

			/// Recursively looks for a node by name (case insensitive), using depth-first search.
			/// Returns HK_NULL if not found, the node otherwise
		hkxNode* findNodeByName (const char* name) const;

			/// Constructs the full path (parent-first list of nodes from the root to the node itself, both included)
			/// Returns HK_FAILURE if the node doesn't belong to this scene
		hkResult getFullPathToNode (const hkxNode* theNode, hkArray<const hkxNode*>& pathOut) const;

			/// Gets the worldFromNode transform of the given node, by concatenating
			/// the transforms of its parents. By default, it uses the first stored key.
			/// Returns HK_FAILURE if the node doesn't belong to this scene
		hkResult getWorldFromNodeTransform (const hkxNode* theNode, class hkMatrix4& worldFromNodeOut, int key=0) const;
};

#endif // HKSCENEDATA_SCENE_HKXSCENE_HKCLASS_H

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
