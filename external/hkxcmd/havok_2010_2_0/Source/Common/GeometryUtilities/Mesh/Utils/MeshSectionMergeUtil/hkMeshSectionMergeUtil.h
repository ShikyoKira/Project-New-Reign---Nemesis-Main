/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MESH_SECTION_MERGE_UTIL_H
#define HK_MESH_SECTION_MERGE_UTIL_H

class hkMeshShape;
class hkMeshSystem;

/// A utility to merge mesh sections.
///
/// Mesh sections can only be merged if they have the same vertex buffer format (or better vertex buffer)
/// and their primitives are compatible.
class hkMeshSectionMergeUtil
{
    public:
			/// Produce a new mesh shape made up of a merge of the sections with the same material
		static hkMeshShape* HK_CALL mergeShapeSectionsByMaterial(hkMeshSystem* system, hkMeshShape* meshShape);

			/// Returns true if a mesh shape has more than one mesh section with the same material
		static bool HK_CALL hasUniqueMaterials(hkMeshShape* meshShape);

			/// Merges the first shape into the second.
		static hkMeshShape* HK_CALL mergeShapes(hkMeshSystem* system, const hkMeshShape* one, const hkMeshShape* two);
};

#endif // HK_MESH_SECTION_MERGE_UTIL_H

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
