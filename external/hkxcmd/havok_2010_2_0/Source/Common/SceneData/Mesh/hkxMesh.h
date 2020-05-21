/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_MESH_HKXMESH_HKCLASS_H
#define HKSCENEDATA_MESH_HKXMESH_HKCLASS_H

#include <Common/SceneData/Attributes/hkxAttributeHolder.h>
#include <Common/SceneData/Mesh/hkxMeshSection.h>

extern const class hkClass hkxMeshUserChannelInfoClass;

/// hkxMesh meta information
extern const class hkClass hkxMeshClass;

/// The information needed to construct a mesh.
class hkxMesh : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
		HK_DECLARE_REFLECTION();

		hkxMesh() { }
		hkxMesh(hkFinishLoadedObjectFlag f );
		virtual ~hkxMesh();

		class UserChannelInfo : public hkxAttributeHolder
		{
		public:
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			HK_DECLARE_REFLECTION();

			UserChannelInfo() { }
			UserChannelInfo(hkFinishLoadedObjectFlag f) : hkxAttributeHolder(f), m_name(f), m_className(f) {}
			UserChannelInfo( const UserChannelInfo& o ) : hkxAttributeHolder(o)
			{ 
				m_name = o.m_name;
				m_className = o.m_className;
			}

			hkStringPtr m_name;
			hkStringPtr m_className;
		};

		//
		// Members
		//
	public:
		
		/// The sections (split by material)
		/// Meshes own the sections
		hkArray< hkRefPtr<hkxMeshSection> > m_sections;
		
		hkArray< hkRefPtr<hkxMesh::UserChannelInfo> > m_userChannelInfos;
		
		//
		// Utility methods
		//

			/// Traverses all the mesh sections and collects all vertex positions to the given array.
		void collectVertexPositions (hkArray<hkVector4>& verticesInOut) const;

			/// Traverses all the mesh sections and constructs a single geometry out of them which is merged with geometryInOut.
			/// If materialsInOut is null, the output geometry will not contain any material information. Otherwise, if the mesh
			/// section uses materials then are contained in materialsInOut, they will be re-used; materials that aren't already
			/// in materialsInOut will be added.
		void appendGeometry (struct hkGeometry& geometryInOut, hkArray<class hkxMaterial*>* materialsInOut = HK_NULL) const;

			/// Removes the given user channel
		void removeUserChannel(int userChannelIndex);
};


#endif // HKSCENEDATA_MESH_HKXMESH_HKCLASS_H

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
