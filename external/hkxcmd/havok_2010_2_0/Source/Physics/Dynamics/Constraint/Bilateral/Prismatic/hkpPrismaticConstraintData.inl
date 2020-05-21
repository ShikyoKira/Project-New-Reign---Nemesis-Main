/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline void hkpPrismaticConstraintData::allowRotationAroundAxis(bool allowRotation)
{
	if (allowRotation)
	{
		// We unfix rotation along the prismatic constraint's axis. Meaning, we change it into a cylindrical constraint.
		m_atoms.m_ang.m_firstConstrainedAxis = 1; // Originally was 0. 0 is the direction of the shaft axis.
		m_atoms.m_ang.m_numConstrainedAxes = 2; // Originally was 3. And was constraining all 3 axes of rotation.
	}
	else
	{
		m_atoms.m_ang.m_firstConstrainedAxis = 0; // Originally was 0. 0 is the direction of the shaft axis.
		m_atoms.m_ang.m_numConstrainedAxes = 3; // Originally was 3. And was constraining all 3 axes of rotation.
	}
}

inline void hkpPrismaticConstraintData::setMaxLinearLimit(hkReal mmax)
{
	m_atoms.m_linLimit.m_max = mmax;
}

inline void hkpPrismaticConstraintData::setMinLinearLimit(hkReal mmin)
{
	m_atoms.m_linLimit.m_min = mmin;
}

inline void hkpPrismaticConstraintData::setMaxFrictionForce(hkReal fmag)
{
	m_atoms.m_friction.m_maxFrictionForce = fmag;	
}

inline hkReal hkpPrismaticConstraintData::getMaxLinearLimit() const
{
	return m_atoms.m_linLimit.m_max;
}

inline hkReal hkpPrismaticConstraintData::getMinLinearLimit() const
{
	return m_atoms.m_linLimit.m_min;
}

inline hkReal hkpPrismaticConstraintData::getMaxFrictionForce() const
{
	return m_atoms.m_friction.m_maxFrictionForce;
}

inline hkpConstraintMotor* hkpPrismaticConstraintData::getMotor() const
{
	return m_atoms.m_motor.m_motor;
}

inline void hkpPrismaticConstraintData::setMotorTargetPosition( hkReal pos )
{
	m_atoms.m_motor.m_targetPosition = pos;
}

inline hkReal hkpPrismaticConstraintData::getMotorTargetPosition()
{
	return m_atoms.m_motor.m_targetPosition;
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
