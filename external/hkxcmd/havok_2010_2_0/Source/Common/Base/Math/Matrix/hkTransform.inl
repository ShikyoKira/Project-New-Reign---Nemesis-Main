/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkVector4& hkTransform::getColumn(int x)
{
	return m_rotation.getColumn(x);
}

const hkVector4& hkTransform::getColumn(int x) const
{
	return m_rotation.getColumn(x);
}

hkVector4& hkTransform::getTranslation()
{
	return m_translation;
}

const hkVector4& hkTransform::getTranslation() const
{
	return m_translation;
}

void hkTransform::setTranslation(const hkVector4& t)
{
	m_translation = t;
}

hkRotation& hkTransform::getRotation()
{
	return m_rotation;
}

const hkRotation& hkTransform::getRotation() const
{
	return m_rotation;
}

void hkTransform::setRotation(const hkRotation& rotation)
{	
	m_rotation = rotation;
}

void hkTransform::setRotation(const hkQuaternion& quatRotation)
{
	m_rotation.set(quatRotation);
}

hkTransform::hkTransform(const hkTransform& t)
{
	const hkVector4 r0 = t.m_rotation.getColumn(0);
	const hkVector4 r1 = t.m_rotation.getColumn(1);
	const hkVector4 r2 = t.m_rotation.getColumn(2);
	const hkVector4 r3 = t.m_rotation.getColumn(3);
	m_rotation.getColumn(0) = r0;
	m_rotation.getColumn(1) = r1;
	m_rotation.getColumn(2) = r2;
	m_rotation.getColumn(3) = r3;
}

hkTransform::hkTransform(const hkQuaternion& q, const hkVector4& t)
	:	m_translation(t)
{
	m_rotation.set(q);
}


hkTransform::hkTransform(const hkRotation& r, const hkVector4& t)
	:	m_rotation(r),
		m_translation(t)
{
}

inline hkReal& hkTransform::operator() (int r, int c)
{
	return m_rotation(r,c);
}

inline const hkReal& hkTransform::operator() (int r, int c) const
{
	return m_rotation(r,c);
}

void hkTransform::set(const hkQuaternion& q, const hkVector4& t)
{
	m_rotation.set(q);
	m_translation = t;
}

const hkTransform& HK_CALL hkTransform::getIdentity()
{
	extern hkTransform hkTransformIdentity;
	return hkTransformIdentity;
}

void hkTransform::setIdentity()
{
	m_rotation.setIdentity();
	m_translation.setZero4();
}

void hkTransform::_setMulInverseMul( const hkTransform& bTa, const hkTransform &bTc )
{
	int i = 3;
	do
	{
		getColumn(i)._setRotatedInverseDir( bTa.getRotation(), bTc.getColumn(i) );
	} while( --i >= 0 );
	hkVector4 h; h.setSub4(bTc.m_translation, bTa.m_translation );
	m_translation._setRotatedInverseDir( bTa.m_rotation, h);
}


void hkTransform::setRows4( const hkVector4& r0, const hkVector4& r1,
								 const hkVector4& r2, const hkVector4& r3)
{
	getColumn(0).set( r0(0), r1(0), r2(0), r3(0) );
	getColumn(1).set( r0(1), r1(1), r2(1), r3(1) );
	getColumn(2).set( r0(2), r1(2), r2(2), r3(2) );
	getColumn(3).set( r0(3), r1(3), r2(3), r3(3) );
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
