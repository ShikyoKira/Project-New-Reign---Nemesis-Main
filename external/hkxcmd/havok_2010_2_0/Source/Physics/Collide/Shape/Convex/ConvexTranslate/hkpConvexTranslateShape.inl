/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline const hkpConvexShape* hkpConvexTranslateShape::getChildShape() const 
{
	return static_cast<const hkpConvexShape*>(m_childShape.getChild());
}

inline const hkVector4& hkpConvexTranslateShape::getTranslation() const 
{ 
	return m_translation; 
}

inline hkVector4& hkpConvexTranslateShape::getTranslation()
{ 
	return m_translation; 
}

inline void hkpConvexTranslateShape::initializeSpu( const hkpConvexShape* childShape, const hkVector4& translation, hkReal radius )
{
	m_type = HK_SHAPE_CONVEX_TRANSLATE;
	m_childShape = childShape; 
	m_radius = radius;
	m_translation = translation;
	m_translation(3) = 0;
	m_childShapeSize = 0;
}

HK_FORCE_INLINE hkpConvexTranslateShape::hkpConvexTranslateShape( const hkpConvexShape* childShape, const hkVector4& translation, hkpShapeContainer::ReferencePolicy ref )
: hkpConvexTransformShapeBase( HK_SHAPE_CONVEX_TRANSLATE, childShape->getRadius(), childShape, ref)
{
	HK_ASSERT2(0x6acf0520, childShape != HK_NULL, "Child shape cannot be NULL");  
#if defined(HK_DEBUG)
	if (childShape->getContainer() != HK_NULL)
	{
		HK_WARN_ONCE(0x6acf0521, "You have two translate shapes on top of each other. Slight performance decrease.");
	}
#endif
	hkVector4 t;	t.setXYZ0(translation);
	m_translation = t;
	m_childShapeSize = 0;
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
