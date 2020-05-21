/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MEMORY_MESH_BODY_H
#define HK_MEMORY_MESH_BODY_H

#include <Common/GeometryUtilities/Mesh/hkMeshBody.h>
#include <Common/GeometryUtilities/Mesh/IndexedTransformSet/hkIndexedTransformSet.h>

extern const class hkClass hkMemoryMeshBodyClass;

class hkMeshSystem;

/// An memory only version of a hkMeshBody
///
/// The memory implementation is useful for just processing hkMesh data.
///
/// \sa hkMeshBody
class hkMemoryMeshBody: public hkMeshBody
{
    public:
		HK_DECLARE_REFLECTION();
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

            /// Ctor
        hkMemoryMeshBody(hkMeshSystem* meshSystem, const hkMeshShape* shape, const hkMatrix4& transform, hkIndexedTransformSetCinfo* transformSet);
            
			/// Dtor
		virtual ~hkMemoryMeshBody();

            // hkMeshBody implementation
        virtual const hkMeshShape* getMeshShape() const { return m_shape; }

            // hkMeshBody implementation
        virtual void getTransform( hkMatrix4& transform ) const { transform = m_transform; }

            // hkMeshBody implementation
        virtual void setTransform(const hkMatrix4& matrix) { m_transform = matrix; }

            // hkMeshBody implementation
        virtual hkResult setPickingData(int id, void* data) { return HK_FAILURE; }

            // hkMeshBody implementation
        virtual hkMeshVertexBuffer* getVertexBuffer(int sectionIndex) { return m_vertexBuffers[sectionIndex]; }

            // hkMeshBody implementation
        virtual int getNumIndexedTransforms() { return m_transformSet ? m_transformSet->getNumMatrices() : 0; }
            // hkMeshBody implementation
        virtual void setIndexedTransforms(int startIndex, const hkMatrix4* matrices, int numMatrices) { HK_ASSERT(0x34234, m_transformSet); m_transformSet->setMatrices(startIndex, matrices, numMatrices); }
            // hkMeshBody implementation
        virtual void getIndexedTransforms(int startIndex, hkMatrix4* matrices, int numMatrices) { HK_ASSERT(0x423432, m_transformSet); m_transformSet->getMatrices(startIndex, matrices, numMatrices); }
            // hkMeshBody implementation
        virtual void getIndexedInverseTransforms(int startIndex, hkMatrix4* matrices, int numMatrices) { HK_ASSERT(0x34243207, m_transformSet); m_transformSet->getInverseMatrices(startIndex, matrices, numMatrices); }

			// hkMeshBody implementation
		virtual const hkInt16* getIndexTransformsOrder() const { HK_ASSERT(0x34243207, m_transformSet); return m_transformSet->m_matricesOrder.begin(); }
			// hkMeshBody implementation
		virtual const hkStringPtr* getIndexTransformNames() const { HK_ASSERT(0x34243207, m_transformSet); return m_transformSet->m_matricesNames.begin(); }
			// hkMeshBody implementation
		virtual const hkMeshBoneIndexMapping* getIndexMappings() const { HK_ASSERT(0x34243207, m_transformSet); return m_transformSet->getIndexMappings().begin(); }
			// hkMeshBody implementation
		virtual hkInt32 getNumIndexMappings()  const { return m_transformSet == HK_NULL ? 0 : m_transformSet->getIndexMappings().getSize(); }

            // hkMeshBody implementation
        virtual void completeUpdate() {}
		virtual void completeUpdate(const hkMatrix4& transform) {}

		virtual const char* getName() const { return m_name.cString(); }
		virtual void setName(const char* n) { m_name = n; }

			/// Serialization Ctor
		hkMemoryMeshBody( hkFinishLoadedObjectFlag flag );

protected:

		hkMatrix4 m_transform;								///< The transform
        hkRefPtr<hkIndexedTransformSet> m_transformSet;     ///< The transform set (can be HK_NULL)
        hkRefPtr<const hkMeshShape> m_shape;				///< The originating shape
        hkArray<hkMeshVertexBuffer*> m_vertexBuffers;		///< Vertex buffer for each mesh section (hold the per body/instance data)
		hkStringPtr m_name;
};


#endif // HK_MEMORY_MESH_BODY_H

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
