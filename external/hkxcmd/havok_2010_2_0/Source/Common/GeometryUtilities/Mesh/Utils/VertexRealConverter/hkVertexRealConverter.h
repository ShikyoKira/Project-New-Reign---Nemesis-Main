/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VERTEX_REAL_CONVERTER_H
#define HK_VERTEX_REAL_CONVERTER_H

#include <Common/GeometryUtilities/Mesh/Memory/hkMemoryMeshVertexBuffer.h>
#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

// This is designed to work with vertices which are in the hkMemoryVertexBuffer format
// It can convert a vertex into an array of hkReals, and back into the vertex format

class hkVertexRealConverter
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkVertexRealConverter );

			/// Initialize. Sets all the weights to 1
		void init(const hkVertexFormat& format, const hkAabb& aabb);
			/// Set the weights - the weights are set by each component
			/// Positions will be scaled into a 0-1 space, before a weight is applied
		void setWeight(hkVertexFormat::ComponentUsage usage, int subUsage, hkReal weight);
			/// Get a weight
		hkReal getWeight(hkVertexFormat::ComponentUsage usage, int subUsage) const;

			/// Get the number of reals that will be produced from the format
		int getNumReals() const { return m_numReals; }

			/// Convert the vertex into reals
		void convertVertexToReals(const void* vertex, hkReal* reals) const;
			/// Convert reals into a vertex
		void convertRealsToVertex(const hkReal* reals, void* vertex) const;

			/// Finds where an element is located in the reals. -1 if not found. The offset is in hkReals from the start.
		int findElementOffset(hkVertexFormat::ComponentUsage usage, int subUsage) const;

			/// Convert to internal position range
		void convertPositionToInternal(hkVector4Parameter in, hkVector4& out) const;
			/// Convert from internal range to external range
		void convertInternalToPosition(hkVector4Parameter in, hkVector4& out) const;

			/// Convert into internal range
		static void HK_CALL convertPositionsInternal(const hkAabb& aabb, hkReal weight, hkVector4* positions, int numPos);

			/// Ctor
		hkVertexRealConverter():m_numReals(0) {}

#ifdef HK_DEBUG
	static void HK_CALL selfTest();
#endif

	protected:
		enum SrcType
		{
			SRC_TYPE_UNKNOWN,
			SRC_TYPE_FLOAT32_POSITION,
			SRC_TYPE_FLOAT32_GENERAL,
			SRC_TYPE_ARGB32
		};
		enum DstType
		{
			DST_TYPE_UNKNOWN,
			DST_TYPE_FLOAT32_POSITION,		///< Position
			DST_TYPE_FLOAT32_NORMAL,		///< Normal
			DST_TYPE_FLOAT32_GENERAL,		///< General float handling
			DST_TYPE_FLOAT32_CLAMP1,		///< Clamps output into 0 - 1
			DST_TYPE_FLOAT32_CLAMP2,		///< Clamps output into 0 - 1
			DST_TYPE_FLOAT32_CLAMP3,		///< Clamps output into 0 - 1
			DST_TYPE_FLOAT32_CLAMP4,		///< Clamps output into 0 - 1
			DST_TYPE_ARGB32,				///< Has an implicit clamp 0-1 goes to 0-1
		};

		struct Entry
		{
			SrcType m_srcType;
			DstType m_dstType;
			int m_numValues;
			int m_offset;						// Vertex offset (bytes from start of vertex) 
			int m_realOffset;					// Offset in reals to the entry

			hkReal m_weight;
			hkReal m_recipWeight;
		};

		hkVertexFormat m_format;

		hkVector4 m_worldToLocalScale;
		hkVector4 m_localToWorldScale;
		hkVector4 m_offset;

		hkArray<Entry> m_entries;
		int m_numReals;

		int m_positionIndex;

		hkAabb m_positionAabb;
};

#endif // HK_VERTEX_REAL_CONVERTER_H

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
