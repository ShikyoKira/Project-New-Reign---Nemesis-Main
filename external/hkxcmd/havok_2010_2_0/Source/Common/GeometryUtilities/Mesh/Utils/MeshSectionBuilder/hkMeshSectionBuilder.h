/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MESH_SECTION_BUILDER_H
#define HK_MESH_SECTION_BUILDER_H

#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>

/// This utility is designed to help the construction of mesh sections
///
/// Specifically it hold all of the indices needed to construct a shape, as well as the hkMeshSectionCinfos
///
/// Usage is of the form of calling 'startMeshSection' at the start of a new mesh section. You can optionally pass
/// in the vertexBuffer and material associated with the section. Next multiple addTriangleIndices or addUnindexed
/// calls can be made - and these will be accumulated in internally buffers. Once all of the indices have been
/// added for the section a call to endMeshSection can be called.
///
/// Before calling endMeshSection - if the vertexBuffer or the material haven't been set - they can be set with
/// setVertexBuffer or setMaterial. It is invalid to have not set this data either in startMeshSection or with these methods
/// before the endMeshSection call.
///
/// Once all of the mesh sections have been added the hkMeshSectionCinfo array needed for construction can be retrieved
/// by calling 'getSections' and the number of sections from getNumSections
///
/// A hkMeshSectionBuilder can be emptied of its contents by calling 'clear' - allowing any internal allocation used for
/// building to be potentially reused.
///
/// \sa hkMeshMaterial hkMeshShape
class hkMeshSectionBuilder
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkMeshSectionBuilder );

			/// Dtor
		~hkMeshSectionBuilder();

            /// Remove everything
        void clear();

            /// Start a new primitive
        void startMeshSection(hkMeshVertexBuffer* vertexBuffer = HK_NULL, hkMeshMaterial* material = HK_NULL);

			/// Add indices to the current primitive
		hkResult concatPrimitives(hkMeshSection::PrimitiveType primType, const hkUint16* indices, int numIndices, int indexBase = 0);

			/// Add indices to the current primitive
		hkResult concatPrimitives(hkMeshSection::PrimitiveType primType, const hkUint32* indices, int numIndices, int indexBase = 0);

			/// Add non indexed - this will add a new mesh section
		hkResult concatUnindexed(hkMeshSection::PrimitiveType primType, int vertexStartIndex, int numIndices);

            /// End the current primitive
        void endMeshSection();

			/// Can only be performed once in start/end - and must not have been set up at start
		void setVertexBuffer(hkMeshVertexBuffer* vertexBuffer);

			/// Can only be performed once in start/end - and must not have been set up at start
		void setMaterial(hkMeshMaterial* material);

            /// Get an array of primitives
        HK_FORCE_INLINE const hkMeshSectionCinfo* getSections() const { return m_sections.begin(); }

            /// Get the number of primitives
        HK_FORCE_INLINE int getNumSections() const { return m_sections.getSize(); }

		static bool canConcatPrimitives(hkMeshSection::PrimitiveType b, hkMeshSection::PrimitiveType a);

    protected:

		hkUint32* _addIndices32(int numIndices);
		hkUint16* _addIndices16(int numIndices);
		bool _canConcatPrimitive(hkMeshSection::PrimitiveType primType);
		void _makeIndices32();
		void _concatIndices(int vertexStartIndex, int numIndices);
		void _concatIndices(const hkUint16* srcIndices, int numIndices, int indexBase = 0);
		void _concatIndices(const hkUint32* srcIndices, int numIndices, int indexBase = 0);
		hkResult _makeConcatable(hkMeshSection::PrimitiveType primType);
		hkBool _isIndexed() const;
		hkResult _concatPrimitives(hkMeshSection::PrimitiveType primType, const hkUint16* indices, int numIndices, int indexBase);
		hkResult _concatPrimitives(hkMeshSection::PrimitiveType primType, const hkUint32* indices, int numIndices, int indexBase);

		static bool HK_CALL _needsIndices32(const hkUint16* srcIndices, int numIndices, int indexBase);
		
		int m_indexBase16;									
		int m_indexBase32;									

		hkArray<hkMeshSectionCinfo> m_sections;             ///< The sections produced
        hkArray<hkUint16> m_indices16;                        ///< Triangle indices
		hkArray<hkUint32> m_indices32;						///< 
};

#endif // HK_MESH_SECTION_BUILDER_H

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
