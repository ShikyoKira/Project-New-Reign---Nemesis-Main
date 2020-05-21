/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_STORAGEMESHSHAPE_H
#define HK_STORAGEMESHSHAPE_H

#include <Physics/Collide/Shape/Deprecated/Mesh/hkpMeshShape.h>

extern const hkClass hkpStorageMeshShapeClass;
extern const hkClass hkpStorageMeshShapeSubpartStorageClass;

/// DEPRECATED. This shape will be removed in future releases of Havok Physics.
/// 
/// A mesh shape which stores its data. See also hkpSimpleMeshShape.
/// NOTE: it is very error prone to modify the subparts of an
/// hkpStorageMeshShape directly because the subpart pointers need to
/// be updated if the storage is resized.
/// This class is primarily used internally, for taking snapshots of a world containing hkMeshShapes.
/// All hkMeshShapes are converted into hkStorageMeshShapes for serialization.
class hkpStorageMeshShape : public hkpMeshShape
{
	//+hk.ReflectedFile("hkpMeshShape")
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Default constructor.
			/// The data for this shape is public, so simply fill in the
			/// member data after construction.
		hkpStorageMeshShape( hkReal radius = hkConvexShapeDefaultRadius, int numBitsForSubpartIndex = 12 );

			/// Copy the mesh into this mesh.
		hkpStorageMeshShape( const hkpMeshShape* mesh );

			///
		~hkpStorageMeshShape();

			/// Add the part and copy its data internally.
			/// NOTE: it is not recommended to modify a subpart after it
			/// has been added.
			/// NOTE: materials are not copied.
		virtual void addSubpart( const Subpart& part );

	public:

		hkpStorageMeshShape( hkFinishLoadedObjectFlag flag );

		struct SubpartStorage : public hkReferencedObject
		{
			public:

				HK_DECLARE_REFLECTION();
				HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SHAPE);

				SubpartStorage() {}
				virtual ~SubpartStorage() {}

				hkArray<hkReal> m_vertices;
				hkArray<hkUint16> m_indices16;
				hkArray<hkUint32> m_indices32;
				hkArray<hkUint8> m_materialIndices; //materialIndices8
				hkArray<hkUint32> m_materials;
				hkArray<hkUint16> m_materialIndices16;

			public:

				SubpartStorage( hkFinishLoadedObjectFlag flag );
		};

	protected:

		hkArray<struct SubpartStorage*> m_storage;
};

#endif //HK_STORAGEMESHSHAPE_H

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
