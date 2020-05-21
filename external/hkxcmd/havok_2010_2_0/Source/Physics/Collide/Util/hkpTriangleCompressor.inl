/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//#define ONE_BYTE_PER_VERT

#ifdef ONE_BYTE_PER_VERT
typedef hkUint8 hkByteVert;
#else
typedef hkUint16 hkByteVert;
#endif

struct CompressedMesh
{
	float m_x;
	float m_y;
	float m_z;	

	hkUint8 m_numVerts;
	hkUint8 m_numTriangles;
	hkUint8 m_numMaterialIndices;
	hkUint8 pad1;

	float m_xInc;
	float m_yInc;
	float m_zInc;

	hkUint32 pad2;
};


struct ByteVec
{
	hkByteVert m_verts[3];
};

struct TriIndices
{
	hkUint8 m_vert[3];
};

typedef hkUint16 WeldingInfo;

namespace hkTriCompressor
{
	// This endian swaps the data
	// Big endian data (e.g PS3/ Xbox360) is unchanged. Little endian data is swapped back and forth
	template<typename T>
	HK_FORCE_INLINE T HK_CALL ConvertEndian( T tIn )
	{
	#if (HK_ENDIAN_BIG==1)
		return tIn;
	#else

		switch ( sizeof(T) )
		{
		case 1:
			{
				return tIn;
			}
		case 2:
			{
				union {
					hkUint8 bytes[2];
					T data;
				} t;
				t.data = tIn;
				hkAlgorithm::swap( t.bytes[0], t.bytes[1] );
				return t.data;
			}
		case 4:
			{
				union {
					hkUint8 bytes[4];
					T data;
				} t;
				t.data = tIn;
				hkAlgorithm::swap( t.bytes[1], t.bytes[2] );
				hkAlgorithm::swap( t.bytes[0], t.bytes[3] );
				return t.data;
			}
		default:
			{
				HK_BREAKPOINT(0x0);
				return tIn; // silence compiler warning
			}
		}
	#endif
	}
}

hkUint8 hkpTriangleCompressor::getMaterialIndex(int triangleIndex, const void* data )
{
	HK_ASSERT2( 0x765e4534, (hkUlong(data) & 0x3) == 0, "Data must be at least 4 byte aligned");
	const CompressedMesh* compressedMesh = reinterpret_cast<const CompressedMesh*>( data );
	int baseOffset =  HK_HINT_SIZE16(sizeof(CompressedMesh)) + 
					  HK_HINT_SIZE16(sizeof(ByteVec)) * compressedMesh->m_numVerts + 
					 (HK_HINT_SIZE16(sizeof(TriIndices)) + HK_HINT_SIZE16(sizeof(WeldingInfo))) * compressedMesh->m_numTriangles;

	HK_ASSERT2( 0x65e43e45, triangleIndex < compressedMesh->m_numMaterialIndices, "Material index out of range" );

	return *((hkUint8*)data + baseOffset + triangleIndex);
}

void hkpTriangleCompressor::setWeldingInfo(int triangleIndex, void* data, hkUint16 weldingInfo)
{
	HK_ASSERT2( 0x765e4534, (hkUlong(data) & 0x3) == 0, "Data must be at least 4 byte aligned");
	const CompressedMesh* compressedMesh = reinterpret_cast<const CompressedMesh*>( data );
	HK_ASSERT2( 0x65e43e45, triangleIndex < compressedMesh->m_numTriangles, "Triangle index out of range" );

	int baseOffset =  HK_HINT_SIZE16(sizeof(CompressedMesh)) + 
					  HK_HINT_SIZE16(sizeof(ByteVec)) * compressedMesh->m_numVerts + 
					  HK_HINT_SIZE16(sizeof(TriIndices)) * compressedMesh->m_numTriangles;
	
	hkUint16* weldingBase = reinterpret_cast<hkUint16*>( (hkUint8*)data + baseOffset );
	weldingBase[ triangleIndex ] = hkTriCompressor::ConvertEndian<hkUint16>(weldingInfo);
}


void hkpTriangleCompressor::getTriangleShape( hkpTriangleShape& triangleShape, int index, const void* data )
{
	HK_ASSERT2( 0x765e4534, (hkUlong(data) & 0x3) == 0, "Data must be at least 4 byte aligned");

	CompressedMesh& compressedMesh = *(CompressedMesh*)data;

	hkVector4 offset; offset.set( hkTriCompressor::ConvertEndian<hkReal>(compressedMesh.m_x),
								  hkTriCompressor::ConvertEndian<hkReal>(compressedMesh.m_y),
								  hkTriCompressor::ConvertEndian<hkReal>(compressedMesh.m_z));

	hkVector4 incr; incr.set( hkTriCompressor::ConvertEndian<hkReal>(compressedMesh.m_xInc),
							  hkTriCompressor::ConvertEndian<hkReal>(compressedMesh.m_yInc),
							  hkTriCompressor::ConvertEndian<hkReal>(compressedMesh.m_zInc));

	ByteVec* verts = (ByteVec*)((hkUlong)data + sizeof( CompressedMesh ));
	TriIndices* triIndicesArray = (TriIndices*)( verts + compressedMesh.m_numVerts);
	TriIndices& triIndices = triIndicesArray[ index ];

	for (int i = 0; i < 3; ++i)
	{
		ByteVec& byteV = verts[triIndices.m_vert[i]];
		hkVector4 quantizedVert; 
		quantizedVert.set( (hkReal) hkTriCompressor::ConvertEndian<hkByteVert>(byteV.m_verts[0]), 
						   (hkReal) hkTriCompressor::ConvertEndian<hkByteVert>(byteV.m_verts[1]), 
						   (hkReal) hkTriCompressor::ConvertEndian<hkByteVert>(byteV.m_verts[2])); 

		hkVector4 vert;
		vert.setAddMul4( offset, incr, quantizedVert );
		triangleShape.setVertex(i, vert);
	}

	WeldingInfo* weldingInfoBase = (WeldingInfo*)( triIndicesArray + compressedMesh.m_numTriangles );
	triangleShape.setWeldingInfo( hkTriCompressor::ConvertEndian<WeldingInfo>(weldingInfoBase[index]) );
}

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
