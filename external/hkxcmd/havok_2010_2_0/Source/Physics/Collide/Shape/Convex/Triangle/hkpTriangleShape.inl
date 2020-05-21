/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


hkUint16 hkpTriangleShape::getWeldingInfo() const
{
	return m_weldingInfo;

}

void hkpTriangleShape::setWeldingInfo( hkUint16 info )
{
	m_weldingInfo = info;
}

HK_FORCE_INLINE hkpWeldingUtility::WeldingType hkpTriangleShape::getWeldingType() const
{
	return m_weldingType;
}

HK_FORCE_INLINE void hkpTriangleShape::setWeldingType( hkpWeldingUtility::WeldingType type )
{
	m_weldingType = type;
}

HK_FORCE_INLINE hkpTriangleShape::hkpTriangleShape(hkReal radius, hkUint16 weldingInfo, hkpWeldingUtility::WeldingType type)
: hkpConvexShape( HK_SHAPE_TRIANGLE, radius)
{
	setWeldingInfo(weldingInfo);
	setWeldingType( type );
	m_extrusion.setZero4();
	m_isExtruded = 0;
}

HK_FORCE_INLINE hkpTriangleShape::hkpTriangleShape(const hkVector4& v0, const hkVector4& v1, const hkVector4& v2, hkReal radius)
: hkpConvexShape(HK_SHAPE_TRIANGLE, radius)
{
	m_vertexA = v0;
	m_vertexB = v1;
	m_vertexC = v2;

	setWeldingInfo(0);
	setWeldingType(hkpWeldingUtility::WELDING_TYPE_NONE);
	m_extrusion.setZero4();
	m_isExtruded = 0;
}

const hkVector4* hkpTriangleShape::getVertices() const
{
	return &m_vertexA;
}

hkVector4& hkpTriangleShape::getVertex(int i)
{
	HK_ASSERT2(0x312d54aa,  i>=0 && i < 3, "A triangle has only 3 vertices");
	return (&m_vertexA)[i];
}

const hkVector4& hkpTriangleShape::getVertex(int i) const
{
	HK_ASSERT2(0x7d790924,  i>=0 && i < 3, "A triangle has only 3 vertices");
	return (&m_vertexA)[i];
}

void hkpTriangleShape::setVertex(int i, const hkVector4& vertex)
{
	HK_ASSERT2(0x18d4155c,  i>=0 && i < 3, "A triangle has only 3 vertices");
	(&m_vertexA)[i] = vertex;
}

bool hkpTriangleShape::isExtruded() const
{
	return (m_isExtruded != 0);
}

const hkVector4& hkpTriangleShape::getExtrusion() const
{
	return m_extrusion;
}

void hkpTriangleShape::setExtrusion( const hkVector4& extrusion )
{
	m_isExtruded = extrusion.lengthSquared3().getReal() > 0.0f;
	m_extrusion = extrusion;
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
