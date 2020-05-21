/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VERTEX_BUFFER_UTIL_H
#define HK_VERTEX_BUFFER_UTIL_H

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>

class hkMeshSystem;

/// A utility to help in the processing of vertices and vertex buffers
class hkMeshVertexBufferUtil
{
    public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkMeshVertexBufferUtil);

        struct Thresholds
        {
            Thresholds():
                m_positionThreshold(1e-5f),
                m_normalThreshold(1e-3f),
                m_colorThreshold(1.0f / 255.0f),
                m_otherThreshold(1e-5f),
                m_texCoordThreshold(1e-6f)
            {
            }
            hkReal m_positionThreshold;
            hkReal m_normalThreshold;
            hkReal m_colorThreshold;
            hkReal m_otherThreshold;
            hkReal m_texCoordThreshold;
        };

        enum TransformFlag
        {
            TRANSFORM_NORMALIZE = 0x1,                      ///< Normalize 'normal' components (normal, tangent etc)
            TRANSFORM_PRE_NEGATE = 0x2,                     ///< Negate normal and binormal before transform
            TRANSFORM_POST_NEGATE = 0x4,                    ///< Negate normal and binormal after transform
        };

		typedef hkMeshVertexBuffer::LockedVertices LockedVertices;
		typedef LockedVertices::Buffer Buffer;
		typedef hkVertexFormat::Element Element;

            /// Extract the buffer as vector4s
        static hkResult HK_CALL getElementVectorArray(const Buffer& buffer, hkVector4* out, int numVertices);

            /// Extract the buffer as vector4s
        static hkResult HK_CALL getIndexedElementVectorArray(const Buffer& buffer, const int* indices, hkVector4* out, int numVertices);

            /// Get components and store in a hkVector4 array (helper function - calls getElementVectorArray with the appropriate buffer)
        static hkResult HK_CALL getElementVectorArray(const LockedVertices& lockedVertices, int bufferIndex, hkVector4* dst);

            /// Set components held in buffer from an array of hkVector4s
        static hkResult HK_CALL setElementVectorArray(const LockedVertices::Buffer& buffer, const hkVector4* src, int numVertices);

            /// Set components from a hkVector4 array (helper function - calls setElementVectorArray with the appropriate buffer)
        static hkResult HK_CALL setElementVectorArray(const LockedVertices& lockedVertices, int bufferIndex, const hkVector4* src);

			/// The elements being extracted must have been previously been locked with lock/partialLock.
		static hkResult HK_CALL getElementIntArray(const LockedVertices& lockedVertices, int elementIndex, int* dst);

			/// Sets the elements of an array doing conversions from ints as needed. Converts only integral types.
			/// The elements being set must have previously have been locked with lock/partialLock.
		static hkResult HK_CALL setElementIntArray(const LockedVertices& lockedVertices, int elementIndex, const int* src);

            /// Copy the elements of src to dst of elementSize with the appropriate striding
        static void HK_CALL stridedCopy(const void* srcIn, int srcStride, void* dstIn, int dstStride, int elementSize, int numVertices);

            /// Zero the strided destination data
        static void HK_CALL stridedZero(void* dstIn, int dstStride, int elementSize, int numVertices);

            /// Copy vertex data from srcVertices to dstVertices
        static void HK_CALL copy(const LockedVertices& srcVertices, const LockedVertices& dstVertices);

            /// Copy the data from one buffer to the other - they must be the same type (m_type + m_numValues the same)
        static void HK_CALL copy(const Buffer& srcBuffer, const Buffer& dstBuffer, int numVertices);

            /// Copy/convert all of the src buffers into the destination buffers.
        static void HK_CALL convert(const LockedVertices& srcVertices, const LockedVertices& dstVertices);

			/// Copys/converts all members of src into dst
		static void HK_CALL convert(hkMeshVertexBuffer* src, hkMeshVertexBuffer* dst);

            /// Copy and convert as necessary
        static void HK_CALL convert(const Buffer& srcBuffer, const Buffer& dstBuffer, int numVertices);

            /// Finds out if the locked vertex data is contiguous
        static hkBool HK_CALL isContiguous(const LockedVertices& srcVertices, void** start, int& dataSize);

            /// Partition the format into shared and non shared (instanced) elements
        static void HK_CALL partitionVertexFormat(const hkVertexFormat& format, hkVertexFormat& sharedFormat, hkVertexFormat& instanceFormat);

            /// Get array component
        static hkResult HK_CALL getElementVectorArray(hkMeshVertexBuffer* vertexBuffer, hkVertexFormat::ComponentUsage usage, int subUsage, hkArray<hkVector4>& vectorsOut);

            /// Transform the content of the buffer
        static void HK_CALL transform(const Buffer& srcBuffer, const hkMatrix4& transform, int transformFlags, int numVertices);

            /// Transform the content of the vertex buffer
        static hkResult HK_CALL transform(hkMeshVertexBuffer* buffer, const hkMatrix4& transform, int transformFlags);

            /// Produces a new vertex buffer which is a concatenation of the input buffers. If only 1 buffer is input the original buffer is returned with an extra ref count.
        static hkMeshVertexBuffer* HK_CALL concatVertexBuffers(hkMeshSystem* system, hkMeshVertexBuffer** buffers, int numBuffers);

            /// Compares two buffers - all values must be equal within the threshold to return true.
        static hkBool HK_CALL isBufferDataEqual(const Buffer& bufferA, const Buffer& bufferB, hkReal threshold);

			/// Compare - handle data as a normal (use dot products difference from 1 as measure of error)
		static hkBool HK_CALL isBufferNormalDataEqual(const Buffer& bufferA, const Buffer& bufferB, hkReal threshold);

            /// Compares an array of buffer data. Uses the thresholds structure to determine what is equal based on type
        static hkBool HK_CALL isBufferDataEqual(const Buffer* a, const Buffer* b, int numBuffers, const Thresholds& thresholds);

			/// Performs a linear interpolation of components. Will normalize 'normal' type components
			/// If interp is 0, a is output, 1 then b is output. I.e., its a * (1-interp) + b * interp
		static void HK_CALL interpolate(const hkVertexFormat::Element& element, const void* a, const void* b, hkReal interp, void* dst);

			/// Returns true if the buffer contains positions, normals, tangents and / or bitangents.
		static hkBool HK_CALL bufferIsSkinnable(hkMeshVertexBuffer* vertexBuffer);

			/// Returns true if the buffer contains blend weights and indices.
		static hkBool HK_CALL bufferHasWeights(hkMeshVertexBuffer* vertexBuffer);

};

#endif // HK_VERTEX_BUFFER_UTIL_H

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
