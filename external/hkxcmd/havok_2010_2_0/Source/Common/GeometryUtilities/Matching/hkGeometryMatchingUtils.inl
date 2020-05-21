/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE hkGeometryMatchingUtils::Geometry::Geometry()
{
	m_vertexStride = sizeof(hkVector4);
	m_triangleIndexStride = sizeof(int) * 3;
}

HK_FORCE_INLINE const hkGeometryMatchingUtils::TriangleMap::Hit& hkGeometryMatchingUtils::TriangleMap::getBestMatchingTriangle( int geometryIndex, int triangleIndex) const
{
	const hkUint32 startIndex = m_startIndexPerGeometry[geometryIndex];
	return m_foundReferenceTriangle[ startIndex + triangleIndex ];
}

HK_FORCE_INLINE hkGeometryMatchingUtils::Geometry::Geometry( const hkGeometry* geometry )
{
	m_vertices			= geometry->m_vertices.begin();
	m_numVertices		= geometry->m_vertices.getSize();
	m_triangleIndices	= &geometry->m_triangles.begin()->m_a;
	m_numTriangles		= geometry->m_triangles.getSize();
	m_triangleIndexStride = sizeof(hkGeometry::Triangle);
	m_vertexStride = sizeof(hkVector4);

}

HK_FORCE_INLINE void hkGeometryMatchingUtils::FullMap::getBestMatchingVertex (int searchGeometryIndex, int searchVertexIndex, VertexHit& hitOut) const
{
	const VertexTriangleEntry& vTri = _getSearchTriangleForSearchVertex(searchGeometryIndex, searchVertexIndex);
	hitOut.m_gotHit = false;

	if (vTri.m_triangleIndex > -1) 
	{
		const TriangleMap::Hit& tHit = m_triangleMap.getBestMatchingTriangle(searchGeometryIndex, vTri.m_triangleIndex);

		hitOut.m_gotHit = true;
		hitOut.m_geometryIndex = tHit.m_geometryIndex;
		hitOut.m_triangleIndex = tHit.m_triangleIndex;

		if (!tHit.m_flippedWinding)
		{
			hitOut.m_trianglePos = (vTri.m_trianglePos + tHit.m_indexOffset) % 3;
		}
		else
		{
			hitOut.m_trianglePos = ((3-vTri.m_trianglePos) + tHit.m_indexOffset) % 3;
		}
	}	
}


HK_FORCE_INLINE const hkGeometryMatchingUtils::FullMap::VertexTriangleEntry& hkGeometryMatchingUtils::FullMap::_getSearchTriangleForSearchVertex (int searchGeometryIndex, int searchVertexIndex) const
{
	const hkUint32 startIndex = m_startEntryPerGeometry[searchGeometryIndex];
	return m_searchTrianglePerSearchVertex [startIndex + searchVertexIndex];
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
