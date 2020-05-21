/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_NAMED_MESH_MATERIAL_H
#define HK_NAMED_MESH_MATERIAL_H

#include <Physics/Collide/Shape/Compound/Collection/Mesh/hkpMeshMaterial.h>

	/// The name given to materials for faces which are not in any selection.
#define DEFAULT_MESH_MATERIAL_NAME "Default"

extern const hkClass hkpNamedMeshMaterialClass;

/// In the content tools, a string can be attached to sets of triangles using mesh section materials or triangle
/// selections from the Havok Mesh Channel Toolbox. A material table containing hkpNamedMeshMaterials will be built
/// by the create rigid bodies filter. This table is intended only as an intermediate structure which can be replaced
/// by a user-designed subclass of hkpMeshMaterial.
class hkpNamedMeshMaterial : public hkpMeshMaterial
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpNamedMeshMaterial );
		HK_DECLARE_REFLECTION();

		hkpNamedMeshMaterial() : m_name( DEFAULT_MESH_MATERIAL_NAME ) {	m_filterInfo = 0; }

		hkpNamedMeshMaterial( const hkStringPtr& name ) : m_name( name ) { m_filterInfo = 0; }

		inline hkBool32 operator==( const hkpNamedMeshMaterial& other ) const { return ( other.m_filterInfo == m_filterInfo ) && ( other.m_name == m_name ); }

		hkpNamedMeshMaterial(hkFinishLoadedObjectFlag f) : m_name(f) {}
	
	public:
		hkStringPtr m_name;
};

#endif // HK_NAMED_MESH_MATERIAL_H

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
