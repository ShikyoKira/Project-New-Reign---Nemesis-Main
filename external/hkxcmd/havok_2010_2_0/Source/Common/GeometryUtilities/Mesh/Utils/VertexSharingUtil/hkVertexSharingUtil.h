/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VERTEX_SHARING_UTIL_H
#define HK_VERTEX_SHARING_UTIL_H

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>

/// Small utility which helps to share vertices which have the same attributes (like uv/normals/tangents ...).
///
/// The utility is designed to be able to share vertices of any format. It works out if a vertex is that same
/// by comparing the elements. It does not use a memory compare - but goes though each element type using a
/// suitable comparison for the type and taking into account a threshold for equality. The threshold value
/// is a public member that can be altered.
///
/// Not using a memory comparison allows the sharing to compare vertices that are very similar in value and equate them
/// to being the same value. The work required to the comparison this way is greater - but gives more flexibility. In particular
/// when converting vertices from hkdGeometry - using barycentric coordinates, vertices can be produced with slightly different
/// values. This utility is used to share the vertices in this situation.
///
/// Internally the vertices are held in a contiguous chunk of memory - in the same layout as vertices held in the
/// hkMemoryVertexBuffer.
///
/// To use the tool, a hkVertexSharing object should be instantiated. 'Begin' should be called to start sharing
/// vertices - the parameter determines the format of the vertices to be added.
///
/// The data for the vertex you want to add should be stored in the 'work vertex' which is a chunk of memory large enough
/// to hold a complete vertex. The size of a vertex in bytes - can always be determined by the getVertexStride method.
/// To actually add the vertex held in the 'work vertex' the addVertex method is used. It takes as a parameter a
/// hash that can be rapidly be used to determine vertices which could be shared. For example - there could be multiple
/// vertices produced for a single vertex index (say with different normals) - the index could be used as hash in this
/// case. Or if the position can uniquely be used to identify a vertex position to share - a hash of the position could be used.
///
/// Once all of the vertices have been added calling the 'end' method will produce a LockedVertices structure holding
/// all of the unique vertices.
///
/// \sa hkMeshVertexBuffer
class hkVertexSharingUtil
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkVertexSharingUtil );

            /// Ctor
        hkVertexSharingUtil();

            /// Get the vertex format
        HK_FORCE_INLINE const hkVertexFormat& getVertexFormat() const { return m_format; }

            /// Must be called before any other operation
        void begin(const hkVertexFormat& format, int numVerticesEstimate = 1024);

            /// Get the work vertex
        HK_FORCE_INLINE hkUint8* getWorkVertex() { return m_workVertex.begin(); }

            /// Get the work vertex layout
        HK_FORCE_INLINE const hkMeshVertexBuffer::LockedVertices& getLockedWorkVertex() const { return m_lockedWorkVertex; }

			/// Get the vertex stride
		HK_FORCE_INLINE int getVertexStride() const { return m_vertexStride; }

            /// Read only access to vertex data
        HK_FORCE_INLINE const hkUint8* getVertexData() const { return m_vertices.begin(); }

            /// Read access to vertex data
        HK_FORCE_INLINE hkUint8* getVertexData() { return m_vertices.begin(); }

            /// Add a vertex. The vertex data must be stored in the workVertex. Returns the output vertex buffer index. The hash
            /// must not be zero. If the vertices are different the hash has to be different.
        int addVertex(hkUint32 hash);

            /// Finds the vertex index. Returns -1 if not found
        int findVertexIndex(hkUint32 hash, const void* vertex) const;

            /// Add a vertex. The data for the vertex is held in data pointed to by vertex. The data must be
            /// dword aligned, and be in exactly the same layout (memorywise) as the work vertex.
            /// Any padding in the vertex must be zeroed for correct comparison with threshold of 0
        int addVertex(hkUint32 hash, const void* vertex);

            /// The total amount of vertices
        int getNumVertices() const { return m_numVertices; }

            /// Returns total number of unique vertices, and layout pointing to them
        int end(hkMeshVertexBuffer::LockedVertices& lockedVertices);


	public:
		//
        // public members
		//

        hkReal m_threshold;             ///< Controls the threshold for testing for equality

    protected:

        bool isVertexEqual(const hkUint8* a, const hkUint8* b) const;
        bool isVertexExactlyEqual(const hkUint8* a, const hkUint8* b) const;

        // Maps a hash to the entry index
        hkPointerMap<hkUint32, int> m_hashMap;
        struct Entry
        {
            int m_vertexIndex;
            int m_nextEntryIndex;
        };
        hkArray<Entry> m_entries;

        hkVertexFormat m_format;		///< The vertex format
        int m_vertexStride;             ///< The vertex stride

        hkArray<hkUint8> m_vertices;    ///< All of the vertices
        int m_numVertices;              ///< Number of vertices currently set

        hkArray<hkUint8> m_workVertex;  ///< The work vertex
        hkMeshVertexBuffer::LockedVertices m_lockedWorkVertex;

        // The layout of the vertices
        int m_elementOffsets[hkVertexFormat::MAX_ELEMENTS];    ///< Offsets from start for each element type
};

#endif // HK_VERTEX_SHARING_UTIL_H

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
