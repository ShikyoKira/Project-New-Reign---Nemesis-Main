/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_SKIN_HKXSKINBINDING_HKCLASS_H
#define HKSCENEDATA_SKIN_HKXSKINBINDING_HKCLASS_H

#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Graph/hkxNode.h>

/// hkxSkinBinding meta information
extern const class hkClass hkxSkinBindingClass;

/// A relationship between a given mesh and a set of 'bones' (nodes in the scene
/// graph). Can be used to create more runtime specific structures after initial
/// export.
class hkxSkinBinding : public hkReferencedObject
{
	//+vtable(true)
	//+version(2)
	public:

	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
	HK_DECLARE_REFLECTION();

		hkxSkinBinding()  { }
		hkxSkinBinding(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_mesh(f), m_nodeNames(f), m_bindPose(f) { }

		//
		// Members
		//
	public:
		
			/// The mesh to be skinned.
		hkRefPtr< class hkxMesh > m_mesh;
		
			/// A mapping from mesh indices to hkxNode names.
		hkArray< hkStringPtr > m_nodeNames;
		
			/// The world transforms for the bind pose for each of the referenced bones above.
		hkArray<hkMatrix4> m_bindPose;
		
			/// The skin world transform when the skin was bound (world-from-skinMesh transform)
		hkMatrix4 m_initSkinTransform;
};

#endif // HKSCENEDATA_SKIN_HKXSKINBINDING_HKCLASS_H

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
