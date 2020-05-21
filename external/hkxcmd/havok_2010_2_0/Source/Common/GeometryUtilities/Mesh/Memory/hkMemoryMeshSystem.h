/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MEMORY_MESH_SYSTEM_H
#define HK_MEMORY_MESH_SYSTEM_H

#include <Common/GeometryUtilities/Mesh/hkMeshSystem.h>

extern const hkClass hkMemoryMeshSystemClass;

	/// A memory based implementation of an hkMeshSystem
class hkMemoryMeshSystem: public hkMeshSystem
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// Ctor
		hkMemoryMeshSystem();

            /// Dtor
        virtual ~hkMemoryMeshSystem();

            // hkMeshSystem implementation
        virtual hkMeshMaterialRegistry* getMaterialRegistry() const;

            // hkMeshSystem implementation
        virtual hkMeshShape* createShape(const hkMeshSectionCinfo* sections, int numSections);

            // hkMeshSystem implementation
        virtual hkMeshShape* createCompoundShape(const hkMeshShape*const* shapes, const hkMatrix4* transforms, int numShapes);

            // hkMeshSystem implementation
        virtual hkMeshBody* createBody(const hkMeshShape* shape, const hkMatrix4& mat, hkIndexedTransformSetCinfo* transformSet);

            // hkMeshSystem implementation
        virtual hkMeshVertexBuffer* createVertexBuffer(const hkVertexFormat& vertexFormat, int numVertices);

            // hkMeshSystem implementation
        virtual void findSuitableVertexFormat(const hkVertexFormat& format, hkVertexFormat& formatOut);

            // hkMeshSystem implementation
        virtual void addBody(hkMeshBody* body);

            // hkMeshSystem implementation
        virtual void removeBody(hkMeshBody* body);

            // hkMeshSystem implementation
        virtual hkMeshMaterial* createMaterial();
	
			// hkMeshSystem implementation
		virtual hkMeshTexture* createTexture();

            // hkMeshSystem implementation
        virtual void update() {}

		//
		//	Internal section
		//

            // hkReferencedObject Implementation
        virtual const hkClass* getClassType() const { return &hkMemoryMeshSystemClass; }

            // hkReferencedObject Implementation
        virtual void calcContentStatistics( hkStatisticsCollector* collector,const hkClass* cls ) const;

		bool isSkinnedFormat(const hkVertexFormat& vertexFormat);

		virtual hkMeshVertexBuffer* createSkinnedVertexBuffer(const hkVertexFormat& vertexFormat, int numVertices);

		void setMaterialRegistry(hkMeshMaterialRegistry* materialRegistry);

    protected:

        hkPointerMap<hkMeshBody*, int> m_bodies;				///< the bodies added
        hkRefPtr<hkMeshMaterialRegistry> m_materialRegistry;	///< the material registry
};


#endif // HK_MEMORY_MESH_SYSTEM_H

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
