/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE hkRefVariant::hkRefVariant()
{
}

HK_FORCE_INLINE hkRefVariant::hkRefVariant(const hkReferencedObject* o)
: hkRefPtr<hkReferencedObject>(const_cast<hkReferencedObject*>(o))
{
}

HK_FORCE_INLINE hkRefVariant::hkRefVariant(const hkRefVariant& other)
 : hkRefPtr<hkReferencedObject>(other)
{
}

HK_FORCE_INLINE hkRefVariant::hkRefVariant(hkFinishLoadedObjectFlag f)
: hkRefPtr<hkReferencedObject>(f)
{
}

HK_FORCE_INLINE void hkRefVariant::operator=(const hkRefVariant& other)
{
	if( this != &other )
	{
		hkRefPtr<hkReferencedObject>::operator=(static_cast<hkReferencedObject*>(other));
	}
}

HK_FORCE_INLINE void hkRefVariant::operator=(const hkReferencedObject* o)
{
	if( val() != o )
	{
		hkRefPtr<hkReferencedObject>::operator=(const_cast<hkReferencedObject*>(o));
	}
}

HK_FORCE_INLINE hkRefVariant::operator hkVariant() const
{
	hkVariant v;
	v.m_object = const_cast<hkReferencedObject*>(val());
	v.m_class = getClass();
	return v;
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
