/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkpMotorAction::hkpMotorAction()
: hkpUnaryAction(HK_NULL), 
  m_spinRate(0.0f),
  m_gain(2.0f),
  m_active(true)
{
	m_axis.setZero4();
}

inline hkReal hkpMotorAction::getSpinRate() const
{
	return m_spinRate;
}

inline void hkpMotorAction::setSpinRate(hkReal new_rate)
{
	m_spinRate = new_rate;
}

inline hkReal hkpMotorAction::getGain() const
{
	return m_gain;
}

inline void hkpMotorAction::setGain(hkReal new_gain)
{
	m_gain = new_gain;
}

inline const hkVector4& hkpMotorAction::getAxis() const
{
	return m_axis;
}

inline void hkpMotorAction::setAxis(const hkVector4& axis)
{
	m_axis = axis;
}

inline hkBool hkpMotorAction::isActive() const
{
	return m_active;
}

inline void hkpMotorAction::setActivation(hkBool b)
{
	m_active = b;
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
