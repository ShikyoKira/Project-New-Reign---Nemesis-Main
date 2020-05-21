/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MEMORY_MESH_VERTEX_BUFFER_H
#define HK_MEMORY_MESH_VERTEX_BUFFER_H

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>

extern const hkClass hkMemoryMeshVertexBufferClass;

	/// A memory implementation of a hkMeshVertexBuffer
class hkMemoryMeshVertexBuffer: public hkMeshVertexBuffer
{
    public:
        //+version(1)
		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

            /// Default Ctor
        hkMemoryMeshVertexBuffer();
            /// Ctor
        hkMemoryMeshVertexBuffer(const hkVertexFormat& format, int numVertices);

            /// Dtor
        virtual ~hkMemoryMeshVertexBuffer();

            // hkMeshVertexBuffer implementation
        virtual void getVertexFormat( hkVertexFormat& formatOut ) { formatOut = m_format; }

            // hkMeshVertexBuffer implementation
        virtual int getNumVertices() { return m_numVertices; }

            // hkMeshVertexBuffer implementation
		virtual LockResult lock( const LockInput& input, LockedVertices& lockedVerticesOut );

            // hkMeshVertexBuffer implementation
		virtual LockResult partialLock( const LockInput& input, const PartialLockInput& partialInput, LockedVertices& lockedOut);

            // hkMeshVertexBuffer implementation
		virtual void getElementVectorArray(const LockedVertices& lockedVertices, int elementIndex, hkVector4* compData);

            // hkMeshVertexBuffer implementation
		virtual void setElementVectorArray(const LockedVertices& lockedVertices, int elementIndex, const hkVector4* compData);

            // hkMeshVertexBuffer implementation
		virtual void getElementIntArray(const LockedVertices& lockedVertices, int elementIndex, int* compData);

            // hkMeshVertexBuffer implementation
		virtual void setElementIntArray(const LockedVertices& lockedVertices, int elementIndex, const int* compData);

            // hkMeshVertexBuffer implementation
        virtual void unlock( const LockedVertices& lockedVertices );		    

            /// hkMeshVertexBuffer implementation
		virtual hkMeshVertexBuffer* clone();

            /// hkMeshVertexBuffer implementation
        virtual bool isSharable() { return m_isSharable; }

            /// Get the buffer (avoids having to do a lock)
        void getLockedVerticesBuffer(int elementIndex, LockedVertices::Buffer& buffer);

            /// Get a locked vertex buffer structure for a range of vertices. (avoids doing a lock)
        void getLockedVertices( int startVertex, int numVertices, LockedVertices& lockedVerticesOut );

            /// Set a different vertex format - will set up as having zero vertices
        void setVertexFormat(const hkVertexFormat& format);
            /// Set up the amount of vertices - will zero the memory
        void setNumVerticesAndZero(int numVertices);

            /// Use an external buffer for data storage. bufferSize must be equal to the vertex stride * numVertices
            /// The buffer must stay in scope as long as the hkMemoryVertexBuffer exists
        void useExternalBuffer(void* data, int numVertices, int bufferSize);

            /// Get the vertex data
        hkUint8* getVertexData() { return m_memory.begin(); }
            /// Get the vertex data
        const hkUint8* getVertexData() const { return m_memory.begin(); }

            /// Get the vertex stride
        int getVertexStride() const { return m_vertexStride; }

			/// Add a single vertex
		hkUint8* expandOne() { m_numVertices++; return m_memory.expandBy(m_vertexStride); }
			/// Add multiple vertices
		hkUint8* expandBy(int numVertices) { m_numVertices += numVertices; return m_memory.expandBy(numVertices * m_vertexStride); }

            /// Calculates the offset to each component, as well as the total size of a vertex
        static int HK_CALL calculateElementOffsets(const hkVertexFormat& format, int offsets[hkVertexFormat::MAX_ELEMENTS]);
			/// Calculate offset of specific component
		static int HK_CALL calculateElementOffset(const hkVertexFormat& format, hkVertexFormat::ComponentUsage usage, int subUsage);

			/// Serialization Ctor
		hkMemoryMeshVertexBuffer( hkFinishLoadedObjectFlag flag );

    protected:

			/// Bytes swaps the memory buffer if the vertex buffer was serialized on a machine with a different endian
		void handleEndian();

	protected:


        struct hkVertexFormat m_format;                             ///< The vertex format
        int m_elementOffsets[hkVertexFormat::MAX_ELEMENTS];         ///< Offsets from start for each element type
        hkArray<hkUint8> m_memory;                                  ///< The storage for the vertices
        int m_vertexStride;                                         ///< The amount of bytes between one vertex to the next
        hkBool m_locked;                                            ///< True if this vertex buffer is locked
        int m_numVertices;                                          ///< Total number of vertices
		
		hkBool m_isBigEndian;											///< If true, was saved on a big endian machine, false other wise. Used to byte swap after serialization.

        hkBool m_isSharable;
};



#endif // HK_MEMORY_MESH_VERTEX_BUFFER_H

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
