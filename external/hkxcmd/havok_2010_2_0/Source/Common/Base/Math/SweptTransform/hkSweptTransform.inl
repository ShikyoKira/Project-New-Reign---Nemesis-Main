/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


hkTime hkSweptTransform::getBaseTime() const
{
	return hkTime(m_centerOfMass0(3));
}

hkReal hkSweptTransform::getInvDeltaTime() const 
{
	return m_centerOfMass1(3);
}

#define HK_SWEPT_TRANSFORM_ASSERT_STRING "Disable this assert if it gets fired from your user code. It is only used to check internal consistency."

hkReal hkSweptTransform::getInterpolationValue( hkTime t ) const
{
	HK_ASSERT2(0xf0ff0080, getInvDeltaTime() == 0 || getInvDeltaTime() > 0.01f, "hkSweptTransforms time period (which corresponds to simulation-step deltaTime) is neither set to infinity (as for fixed/not-integrated motion) nor is less than 100 seconds.");
	hkReal dt = (t - getBaseTime()) * getInvDeltaTime();
	//HK_ASSERT2(0xf0ff0080, hkMath::isFinite(dt), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_ASSERT2(0xf0ff0080, dt >= 0.0f,  HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_ASSERT2(0xf0ff0080, dt <= 1.01f, HK_SWEPT_TRANSFORM_ASSERT_STRING);
	return dt;
}

hkReal hkSweptTransform::getInterpolationValueHiAccuracy( hkTime t, hkReal deltaTimeAddon ) const
{
	HK_ASSERT2(0xad67fa3a, getInvDeltaTime() == 0 || getInvDeltaTime() > 0.01f, "hkSweptTransforms time period (which corresponds to simulation-step deltaTime) is neither set to infinity (as for fixed/not-integrated motion) nor is less than 100 seconds.");
	hkReal dt = t - getBaseTime();
	dt += deltaTimeAddon;
	hkReal ddt = dt * getInvDeltaTime();
	//HK_ASSERT2(0xad67fa3a, hkMath::isFinite(ddt), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_ASSERT2(0xad67fa3a, ddt >= 0.0f,  HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_ASSERT2(0xad67fa3a, ddt <= 1.01f, HK_SWEPT_TRANSFORM_ASSERT_STRING);
	return ddt;
}


void hkSweptTransform::_approxTransformAt( hkTime t, hkTransform& transformOut ) const 
{
	hkSimdReal dt = hkSimdReal( (t - getBaseTime()) * getInvDeltaTime());
		
	hkQuaternion q;
	q.m_vec.setInterpolate4( m_rotation0.m_vec, m_rotation1.m_vec, dt );
	q.m_vec.normalize4();

	transformOut.setRotation( q );
	transformOut.getTranslation().setInterpolate4( m_centerOfMass0, m_centerOfMass1, dt);

	hkVector4 centerShift;
	centerShift._setRotatedDir( transformOut.getRotation(), m_centerOfMassLocal);
	transformOut.getTranslation().sub4( centerShift );
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
