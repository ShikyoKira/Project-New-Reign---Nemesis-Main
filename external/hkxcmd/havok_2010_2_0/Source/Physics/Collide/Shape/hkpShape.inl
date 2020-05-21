/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkpShape::hkpShape( hkpShapeType type )
{
	m_userData = 0;
	m_type = type;
}

HK_CLASSALIGN16(class) hkAlignedShape: public hkpShape{
	hkAlignedShape();
};

hkpShapeType hkpShape::getType() const
{
	return static_cast<const hkAlignedShape*>(this)->m_type;
}

inline hkUlong hkpShape::getUserData() const
{
	return m_userData;
}

inline void hkpShape::setUserData( hkUlong data )
{
	m_userData = data;
}
		
void hkpShape::getAabb( const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) const
{
	getAabbImpl( localToWorld, tolerance, out );
}

hkBool hkpShape::castRay( const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& output ) const
{
	return castRayImpl( input, output );
}

void hkpShape::castRayWithCollector( const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const
{
	return castRayWithCollectorImpl( input, cdBody, collector );
}

hkVector4Comparison hkpShape::castRayBundle( const hkpShapeRayBundleCastInput& input, hkpShapeRayBundleCastOutput& output ) const
{
	//HK_WARN(0x3a6a6dc4, "castRayBundle called without a mask. This will probably cause more narrowphase checks than you intended.");
	//HK_ASSERT2(0x15315fcc, false, "castRayBundle called without a mask. This will probably cause more narrowphase checks than you intended.");
	hkVector4Comparison mask; mask.set(hkVector4Comparison::MASK_XYZW);
	return castRayBundleImpl( input, output, mask );
}

hkVector4Comparison hkpShape::castRayBundle( const hkpShapeRayBundleCastInput& input, hkpShapeRayBundleCastOutput& output, hkVector4ComparisonParameter mask ) const
{
	return castRayBundleImpl( input, output, mask );
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
