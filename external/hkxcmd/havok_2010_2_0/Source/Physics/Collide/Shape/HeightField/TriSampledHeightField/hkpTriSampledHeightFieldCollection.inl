/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



inline hkReal hkpTriSampledHeightFieldCollection::getRadius() const 
{ 
	return m_radius; 
}

inline void hkpTriSampledHeightFieldCollection::setRadius(hkReal newRadius)
{
	m_radius = newRadius;
}

inline void hkpTriSampledHeightFieldCollection::setTriangleExtrusion(hkVector4Parameter e)
{
	m_triangleExtrusion = e;
}

inline const hkVector4& hkpTriSampledHeightFieldCollection::getTriangleExtrusion() const
{
	return m_triangleExtrusion;
}

inline int hkpTriSampledHeightFieldCollection::getXFromShapeKey(hkpShapeKey key) const
{
	return (key & 0x0000ffff) >> 1;
}

inline int hkpTriSampledHeightFieldCollection::getZFromShapeKey(hkpShapeKey key) const
{
	return key >> 16;
}

inline int hkpTriSampledHeightFieldCollection::getIndexFromShapeKey(hkpShapeKey key) const
{
	const int x = getXFromShapeKey(key);
	const int z = getZFromShapeKey(key);
	const int index = (z * (getHeightFieldShape()->m_xRes-1) + x) << 1 | (key&1);
	return index;
}

inline hkUint16 hkpTriSampledHeightFieldCollection::getWeldingInfo(hkpShapeKey key) const
{
	const int index = getIndexFromShapeKey(key);

	return m_weldingInfo[index];
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
