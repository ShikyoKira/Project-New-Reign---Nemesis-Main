/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#	define FETCH_DMA( TYPE, base, index ) &base[index]

hkReal hkpCompressedMeshShape::getRadius() const
{
	return m_radius;
}


void hkpCompressedMeshShape::setRadius(hkReal r )
{
	m_radius = r;
}

void hkpCompressedMeshShape::getBigTriangle( int index, hkVector4& v0, hkVector4& v1, hkVector4& v2, hkUint16* welding ) const
{
	const BigTriangle* triangle = FETCH_DMA( BigTriangle, m_bigTriangles.begin(), index );
	if ( welding != HK_NULL )
	{
		*welding = triangle->m_weldingInfo;
	}

	v0 = *FETCH_DMA( hkVector4, m_bigVertices.begin(), triangle->m_a );
	v1 = *FETCH_DMA( hkVector4, m_bigVertices.begin(), triangle->m_b );
	v2 = *FETCH_DMA( hkVector4, m_bigVertices.begin(), triangle->m_c );

	if ( triangle->m_transformIndex != HK_CMS_NULL_REF )
	{
		hkQsTransform transform = *FETCH_DMA( hkQsTransform, m_transforms.begin(), triangle->m_transformIndex );
		v0.setTransformedPos( transform, v0 );
		v1.setTransformedPos( transform, v1 );
		v2.setTransformedPos( transform, v2 );
	}
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
