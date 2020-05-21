/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKGP_VERTEX_TRIANGLE_TOPOLOGY_H
#define HKGP_VERTEX_TRIANGLE_TOPOLOGY_H


#include <Common/Base/Memory/Allocator/FreeList/hkFreeList.h>
#include <Common/Base/Container/PointerMultiMap/hkPointerMultiMap.h>

class hkgpVertexTriangleTopologyBase
{
    public:
    HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkgpVertexTriangleTopologyBase);
            /// EdgeId will be a combination of the triangle address, and bottom 2 bits will be the index of the
            /// edge in that triangle
        typedef hk_size_t EdgeId;
        typedef hkUint32 VertexIndex;

        enum
        {
            INVALID_EDGE_ID = EdgeId(0),
        };

        enum CheckFlag
        {
            CHECK_FLAG_NULL_EDGE = 1,            ///< If there is an edge with the same start and end index
			CHECK_FLAG_ALL = 0x1,
        };

            /// Get the next triangle index
        static HK_FORCE_INLINE int NextTriIndex(int i)  { return (int(1) << i) & 3; }
            /// Get the previous triangle index - equiv to two nexts
        static HK_FORCE_INLINE int PrevTriIndex(int i)  { return ((int(0x12) >> (i + i)) & 3); }

        struct Edge;

        struct Triangle
        {
            friend class hkgpVertexTriangleTopologyBase;

                /// Returns true if indexes the vertex
            HK_FORCE_INLINE hkBool32 hasVertexIndex(VertexIndex i) const;
				/// Returns the index
			int findVertexIndex(VertexIndex i) const;
				/// Returns true if contains an edge that goes to the same vertex
			HK_FORCE_INLINE hkBool32 hasNullEdge() const { return m_vertexIndices[0] == m_vertexIndices[1] || m_vertexIndices[0] == m_vertexIndices[2] || m_vertexIndices[1] == m_vertexIndices[2]; }
                /// Disconnect
            void disconnect();
                /// Check the connections see if they look ok
            hkBool checkConnections() const;
				/// Returns true if contains an edge that goes to the same vertex index
			hkBool hadNullEdge() const { return m_vertexIndices[0] == m_vertexIndices[1] || m_vertexIndices[1] == m_vertexIndices[2] || m_vertexIndices[0] == m_vertexIndices[2]; }

			// NOTE! These shouldn't be changed directly - as information about the topolology will get out of step
			// Use the setTriangleVertexIndex for example.
            VertexIndex m_vertexIndices[3];             ///< The vertex indices
			private:
            int m_triangleIndex;                        ///< The index of the triangle in the mesh
        };

        struct Edge
        {
        	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkgpVertexTriangleTopologyBase::Edge);
            friend class hkgpVertexTriangleTopologyBase;
                /// Returns if the edge is valid, and is associated with a triangle
            HK_FORCE_INLINE hkBool32 isValid() const { return m_triangle != HK_NULL; }

                /// Get the next
            HK_FORCE_INLINE Edge getNextEdge() const { return Edge(m_triangle, NextTriIndex(m_index)); }
                /// Get the previous
            HK_FORCE_INLINE Edge getPrevEdge() const { return Edge(m_triangle, PrevTriIndex(m_index)); }

                /// Inplace next
            HK_FORCE_INLINE void next() { HK_ASSERT(0x3214a423, m_triangle); m_index = NextTriIndex(m_index); }
                /// Inplace previous
            HK_FORCE_INLINE void prev() { HK_ASSERT(0x3214a42a, m_triangle); m_index = PrevTriIndex(m_index); }

                /// ==
            HK_FORCE_INLINE hkBool32 operator==(const Edge& rhs) const { return m_triangle == rhs.m_triangle && m_index == rhs.m_index; }
                /// !=
            HK_FORCE_INLINE hkBool32 operator!=(const Edge& rhs) const { return m_triangle != rhs.m_triangle || m_index != rhs.m_index; }

                /// Get an edge id
            HK_FORCE_INLINE operator EdgeId () const { return hk_size_t(m_triangle) | m_index; }
                /// Get the edge id
            HK_FORCE_INLINE EdgeId getEdgeId() const { return hk_size_t(m_triangle) | m_index; }

				/// Get the triangle
			HK_FORCE_INLINE Triangle* getTriangle() const { return m_triangle; }

				/// Set the start vertex index
			HK_FORCE_INLINE void setStart(VertexIndex v) { m_triangle->m_vertexIndices[m_index] = v; }
				/// Set the end vertex index
			HK_FORCE_INLINE void setEnd(VertexIndex v) { m_triangle->m_vertexIndices[NextTriIndex(m_index)] = v; }

                /// Get the start vertex index
            HK_FORCE_INLINE VertexIndex getStart() const { HK_ASSERT(0x42434baa, m_triangle); return m_triangle->m_vertexIndices[m_index]; }
                /// Get the end vertex index
            HK_FORCE_INLINE VertexIndex getEnd() const { HK_ASSERT(0xab0a0231, m_triangle); return m_triangle->m_vertexIndices[NextTriIndex(m_index)]; }
                /// Get the apex vertex index (the apex is the vertex opposite the edge - the apex of the triangle)
            HK_FORCE_INLINE VertexIndex getApex() const { HK_ASSERT(0x423423aa, m_triangle); return m_triangle->m_vertexIndices[PrevTriIndex(m_index)]; }

				/// Get the index
			HK_FORCE_INLINE int getIndex() const { return m_index; }
				/// Get the next index
			HK_FORCE_INLINE int getNextIndex() const { return (1 << m_index) & 3; }
				/// Get the previous index
			HK_FORCE_INLINE int getPreviousIndex() const { return (int(0x12) >> (m_index + m_index)) & 3; }

                /// Ctor
            HK_FORCE_INLINE Edge(Triangle* tri, int index):m_triangle(tri), m_index(index) {}
                /// Default ctor
            HK_FORCE_INLINE Edge():m_triangle(HK_NULL) {}
                /// Ctor from the id
            HK_FORCE_INLINE Edge(EdgeId id):m_triangle((Triangle*)(id & ~hk_size_t(3))), m_index(int(id) & 3) {}
                /// Copy ctor
            HK_FORCE_INLINE Edge(const Edge& rhs):m_triangle(rhs.m_triangle), m_index(rhs.m_index) {}
                /// Assignment
            HK_FORCE_INLINE void operator=(const Edge& rhs) { m_triangle = rhs.m_triangle; m_index = rhs.m_index; }

				/// Set from an edge id
			HK_FORCE_INLINE void setEdgeId(EdgeId edgeId) { m_triangle = (Triangle*)(edgeId & ~hk_size_t(3)); m_index = int(edgeId) & 3; }

				/// Get the edge id
			HK_FORCE_INLINE static EdgeId HK_CALL getEdgeId(Triangle* tri, int index) { HK_ASSERT(0x2342a423, index >= 0 && index < 3); return hk_size_t(tri) | index; }
				/// Get the triangle from an edge Id
			HK_FORCE_INLINE static Triangle* HK_CALL getTriangle(EdgeId edgeId) { return (Triangle*)(edgeId & ~hk_size_t(3)); }

            protected:
            Triangle* m_triangle;               ///< The triangle this edge belongs
            int m_index;                        ///< The index of this edge in the triangle
        };

			/// Finds all of the edges leaving a vertex
		void findVertexEdges(int vertexIndex, hkArray<EdgeId>& edges) const;

		int calcNumVertexEdges(int vertexIndex) const;

            /// Get all of the triangles
        const hkArray<Triangle*>& getTriangles() const { return m_triangles; }

            /// Delete a triangle (in doing may cause the last triangle to be reindexed)
        void deleteTriangle(Triangle* tri);
            /// Create a triangle
        Triangle* createTriangle(const VertexIndex* indices);
			/// Create with int indices
		Triangle* createTriangle(const int* indices);

			/// Returns all of the triangles, that index the vertex index
		void findVertexTriangles(int vertexIndex, hkArray<Triangle*>& tris) const;

			/// Delete all of the triangles that index this vertex index
		void disconnectVertex(int vertexIndex);

			/// Return the triangle at index.
		Triangle* getTriangle(int index) { return m_triangles[index]; }
			/// Get the triangle at the index
		const Triangle* getTriangle(int index) const { return m_triangles[index]; }

			/// Set the vertex index 'index' (0 to 2) to the value to
		void setTriangleVertexIndex(Triangle* tri, int index, int to);

			/// Change all entrys which are from to 'to'. Returns the number of vertices reindexed.
		int reindexTriangle(Triangle* tri, int from, int to);

			/// Find all the edges that run between two vertices
		void findAllEdges(int start, int end, hkArray<EdgeId>& edgeIds) const;

			/// Get the total number of triangles
		int getNumTriangles() const { return m_triangles.getSize(); }

			/// Returns true if every edge has a pair - its only 'closed' if checkTopology is ok, and a call to this is ok.
		hkBool32 isClosed() const;

			/// Returns bit combination of CheckFlag values. A bit set indicates an issue. Returning 0 means that all checks didn't find a topological issue.
			/// The input flags indicate which flags to ignore
		int checkTopology(int ignoreFlags = 0) const;

			/// Make all the triangles indexing from, index to
		void reindexVertexIndex(int from, int to);

			/// Clear the contents
		void clear() { m_triangleFreeList.freeAll(); m_triangles.clear(); }

			/// Returns true if the structure appears ok
		hkBool32 isOk() const;

			/// For each vertex specifies the new vertex index (i.e., size must be as big as the amount of vertices)
		void remapVertexIndices(const hkArray<int>& remap);

			/// Ctor
		hkgpVertexTriangleTopologyBase(hk_size_t triangleSize = sizeof(Triangle));

			/// From a set of triangles modifies tris, so it only contains the unique triangles
		static void HK_CALL uniqueTriangles(hkArray<Triangle*>& tris);

			/// Given a list of edges returns the unique tris indexed
		static void HK_CALL uniqueTriangles(hkArray<EdgeId>& edges, hkArray<Triangle*>& tris);

#ifdef HK_DEBUG
		static void HK_CALL selfTest();
#endif

    protected:
		typedef hkPointerMultiMap<int, EdgeId> VertexEdgeMapType;


        hkFreeList m_triangleFreeList;				///< The freelist holding all of the triangles
        hkArray<Triangle*> m_triangles;				///< The pointers to all of the triangles
		VertexEdgeMapType m_vertexEdgeMap;			///< Holds the list of edges that leave a vertex
};

#include <Common/Internal/GeometryProcessing/Topology/hkgpVertexTriangleTopology.inl>


template <typename TRI>
class hkgpVertexTriangleTopology: public hkgpVertexTriangleTopologyBase
{
	public:
		typedef hkgpVertexTriangleTopologyBase Parent;

		struct Edge: public Parent::Edge
		{
			HK_FORCE_INLINE Edge(TRI* tri, int index):Parent::Edge(tri, index) {}
                /// Default ctor
			HK_FORCE_INLINE Edge():Parent::Edge() {}
                /// Ctor from the id
			HK_FORCE_INLINE Edge(EdgeId id):Parent::Edge(id) {}
                /// Copy ctor
			HK_FORCE_INLINE Edge(const Edge& rhs):Parent::Edge(rhs) {}
                /// Assignment
            HK_FORCE_INLINE void operator=(const Edge& rhs) { m_triangle = rhs.m_triangle; m_index = rhs.m_index; }

				/// Get the triangle
			HK_FORCE_INLINE TRI* getTriangle() const { return static_cast<TRI*>(m_triangle); }
		};

			/// Create
		HK_FORCE_INLINE TRI* createTriangle(const VertexIndex* indices) { return static_cast<TRI*>(Parent::createTriangle(indices)); }
			/// Create with int indices
		HK_FORCE_INLINE TRI* createTriangle(const int* indices) { return static_cast<TRI*>(Parent::createTriangle(indices)); }

			/// Return the triangle at index.
		TRI* getTriangle(int index) { return static_cast<TRI*>(m_triangles[index]); }
			/// Get the triangle at the index
		const TRI* getTriangle(int index) const { return static_cast<const TRI*>(m_triangles[index]); }

			/// Ctor
		hkgpVertexTriangleTopology():Parent(sizeof(TRI)) {}
};

#endif // HKGP_TRIANGLE_TOPOLOGY_H

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
