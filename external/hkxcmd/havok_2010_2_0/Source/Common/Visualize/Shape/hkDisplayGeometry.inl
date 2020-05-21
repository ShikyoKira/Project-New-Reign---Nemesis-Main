/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

		
inline hkDisplayGeometry::hkDisplayGeometry(hkDisplayGeometryType type)
:	m_geometry(HK_NULL),
	m_type(type)
{
	m_transform.setIdentity();
}

const hkGeometry* hkDisplayGeometry::getGeometry() const
{ 
	return m_geometry; 
}

hkGeometry* hkDisplayGeometry::getGeometry() 
{ 
 	return m_geometry; 
}

inline const hkTransform& hkDisplayGeometry::getTransform() const
{
	return m_transform;
}

inline void hkDisplayGeometry::setTransform( const hkTransform& transform )
{
	HK_ASSERT2( 0xb37ae889, ( m_type != HK_DISPLAY_AABB ) && ( m_type != HK_DISPLAY_CONE ) && ( m_type != HK_DISPLAY_SEMICIRCLE ) && ( m_type != HK_DISPLAY_PLANE ), "This display shape does not permit a non-identity transform." );
	m_transform = transform;
}

inline void hkDisplayGeometry::setTranslation( const hkVector4& translation )
{
	HK_ASSERT2( 0xb37ae889, ( m_type != HK_DISPLAY_AABB ) && ( m_type != HK_DISPLAY_CONE ) && ( m_type != HK_DISPLAY_SEMICIRCLE ) && ( m_type != HK_DISPLAY_PLANE ), "This display shape does not permit a non-identity transform." );
	m_transform.setTranslation( translation );
}

inline hkDisplayGeometryType hkDisplayGeometry::getType() const
{
	return m_type;
}

inline void hkDisplayGeometry::releaseGeometry()
{
	m_geometry = HK_NULL;
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
