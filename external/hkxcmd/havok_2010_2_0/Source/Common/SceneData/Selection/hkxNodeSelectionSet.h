/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_SELECTION_HKXNODESELECTIONSET_HKCLASS_H
#define HKSCENEDATA_SELECTION_HKXNODESELECTIONSET_HKCLASS_H

#include <Common/SceneData/Attributes/hkxAttributeHolder.h>
#include <Common/SceneData/Graph/hkxNode.h>

/// hkxNodeSelectionSet meta information
extern const class hkClass hkxNodeSelectionSetClass;

/// A selected set of nodes in the scene graph
class hkxNodeSelectionSet : public hkxAttributeHolder
{
	//+vtable(true)
	//+version(0)
public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
		HK_DECLARE_REFLECTION();

		hkxNodeSelectionSet()  { }
		hkxNodeSelectionSet(hkFinishLoadedObjectFlag f) : hkxAttributeHolder(f), m_selectedNodes(f), m_name(f) { }

		//  Holds ref to nodes so that you can load a selection live from file etc
		hkArray< hkRefPtr< class hkxNode> > m_selectedNodes;
		hkStringPtr m_name;
};

#endif // HKSCENEDATA_SELECTION_HKXNODESELECTIONSET_HKCLASS_H

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
