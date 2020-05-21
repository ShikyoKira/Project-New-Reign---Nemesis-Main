/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
//	Creates a new hkQTransform using the rotation quaternion q and translation t.

hkQTransform::hkQTransform(const hkQuaternion& q, const hkVector4& t)
:	m_rotation(q)
,	m_translation(t)
{

}

//
//	Copy constructor

hkQTransform::hkQTransform(const hkQTransform& qt)
:	m_rotation(qt.m_rotation)
,	m_translation(qt.m_translation)
{

}

//
//	Constructor

hkQTransform::hkQTransform(class hkFinishLoadedObjectFlag f)
{

}

//
//	Sets the hkQTransform's values using the rotation quaternion q and translation t.

void hkQTransform::set(const hkQuaternion& q, const hkVector4& t)
{
	m_rotation		= q;
	m_translation	= t;
}

//
//	Sets the hkQTransform's values from a hkTransform.

void hkQTransform::set(const hkTransform& otherTransform)
{
	m_rotation.set(otherTransform.getRotation());
	m_translation = otherTransform.getTranslation();
}

//
//	Sets this hkQTransform to be the identity transform.

void hkQTransform::setIdentity()
{
	m_rotation.setIdentity();
	m_translation.setZero4();
}

//
//	Returns a global identity transform.

const hkQTransform& HK_CALL hkQTransform::getIdentity()
{
	extern hkQTransform hkQTransformIdentity;
	return hkQTransformIdentity;
}

//
//	Gets a writable translation component.

hkVector4& hkQTransform::getTranslation()
{
	return m_translation;
}

//
//	Gets the translation component.

const hkVector4& hkQTransform::getTranslation() const
{
	return m_translation;
}

//
//	Sets the translation component.

void hkQTransform::setTranslation(const hkVector4& t)
{
	m_translation = t;
}

//
//	Gets a writable rotation component.

hkQuaternion& hkQTransform::getRotation()
{
	return m_rotation;
}

//
//	Gets the rotation component.

const hkQuaternion& hkQTransform::getRotation() const
{
	return m_rotation;
}

//
//	Sets the rotation component.

void hkQTransform::setRotation(const hkQuaternion& q)
{
	m_rotation = q;
}

//
//	Sets this transform to be the product of the inverse of qt1 by qt2.  (this = qt1^-1 * qt2)

void hkQTransform::_setMulInverseMul(const hkQTransform& qt1, const hkQTransform& qt2)
{
	hkQuaternion invQ1;
	invQ1.setInverse(qt1.m_rotation);
	m_rotation.setMul(invQ1, qt2.m_rotation);

	hkVector4 tempV;
	tempV.setSub4(qt2.m_translation, qt1.m_translation);
	m_translation.setRotatedDir(invQ1, tempV);
}

//
//	Sets this transform to be the product of qt1 and the inverse of qt2. (this = qt1 * qt2^-1)

void hkQTransform::_setMulMulInverse(const hkQTransform &qt1, const hkQTransform &qt2)
{
	hkQuaternion invQ2;
	invQ2.setInverse(qt2.m_rotation);
	m_rotation.setMul(qt1.m_rotation, invQ2);

	hkVector4 tempV;
	tempV.setRotatedDir(m_rotation, qt2.m_translation);
	m_translation.setSub4(qt1.m_translation, tempV);
}

//
//	Sets this transform to be the product of qt1 and qt2.  (this = qt1 * qt2)

void hkQTransform::_setMul(const hkQTransform& qt1, const hkQTransform& qt2)
{
	hkVector4 tmp;
	tmp.setRotatedDir(qt1.m_rotation, qt2.m_translation);
	m_translation.setAdd4(tmp, qt1.m_translation);

	m_rotation.setMul(qt1.m_rotation, qt2.m_rotation);
	
}

//
//	END!
//

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
