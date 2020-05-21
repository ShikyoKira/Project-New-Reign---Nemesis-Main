/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkMotionState* hkpMotion::getMotionState() 
{
	return &m_motionState;
}

const hkMotionState* hkpMotion::getMotionState() const
{
	return &m_motionState;
}

// Get the mass inv of the rigid body.
hkSimdReal hkpMotion::getMassInv() const
{
	return m_inertiaAndMassInv.getSimdAt(3);
}


const hkVector4& hkpMotion::getCenterOfMassLocal() const
{
	return m_motionState.getSweptTransform().m_centerOfMassLocal;
}

const hkVector4& hkpMotion::getCenterOfMassInWorld() const
{
	return m_motionState.getSweptTransform().m_centerOfMass1;
}


// Return the position (Local Space origin) for this rigid body, in World space.
// Note: The center of mass is no longer necessarily the local space origin
const hkVector4& hkpMotion::getPosition() const
{
	return m_motionState.getTransform().getTranslation();
}

// Returns the rotation from Local to World space for this rigid body.
const hkQuaternion& hkpMotion::getRotation() const
{
	return m_motionState.getSweptTransform().m_rotation1;
}

// Returns the rigid body (local) to world transformation.
const hkTransform& hkpMotion::getTransform() const
{
	return m_motionState.getTransform();
}




/*
** VELOCITY ACCESS
*/

const hkVector4& hkpMotion::getLinearVelocity() const
{
	return m_linearVelocity;
}

const hkVector4& hkpMotion::getAngularVelocity() const
{
	return m_angularVelocity;
}


void hkpMotion::getPointVelocity(const hkVector4& p, hkVector4& pointVelOut) const
{
	// CODE IS getLinearVelocity() + getAngularVelocity().cross(p - centerOfMassWorld);
	hkVector4 relPos; relPos.setSub4( p, getCenterOfMassInWorld() );
	pointVelOut.setCross( m_angularVelocity, relPos);
	pointVelOut.add4( m_linearVelocity );
}



/*
** DAMPING
*/

hkReal hkpMotion::getLinearDamping()
{
	return m_motionState.m_linearDamping;
}

hkReal hkpMotion::getAngularDamping()
{
	return m_motionState.m_angularDamping;
}

hkReal hkpMotion::getTimeFactor()
{
	return m_motionState.m_timeFactor;
}

void hkpMotion::setLinearDamping( hkReal d )
{
	m_motionState.m_linearDamping = d;
}


void hkpMotion::setAngularDamping( hkReal d )
{
	m_motionState.m_angularDamping = d;
}

void hkpMotion::setTimeFactor( hkReal f )
{
	m_motionState.m_timeFactor = f;
}

/*
** GRAVITY FACTOR
*/

hkReal hkpMotion::getGravityFactor()
{
	return m_gravityFactor;
}

void hkpMotion::setGravityFactor( hkReal gravityFactor )
{
	m_gravityFactor = gravityFactor;
}


void hkpMotion::setWorldSelectFlagsNeg(int worldFlag0, int worldFlag1, int worldDeactivationIntegrateCounter)
{
	HK_ASSERT2(0xad7654df, (worldFlag0 & 0xfffffffc) == 0, "World's deactivation flag must be 0 or 1.");
	HK_ASSERT2(0xad7654de, (worldFlag1 & 0xfffffffc) == 0, "World's deactivation flag must be 0 or 1.");


	if ((((unsigned)worldDeactivationIntegrateCounter)%4) < (((unsigned)m_deactivationIntegrateCounter)%4))
	{
		worldFlag0 = (~worldFlag0) << 14;
	}
	else
	{
		worldFlag0 <<= 14;
	}

	if (worldDeactivationIntegrateCounter < m_deactivationIntegrateCounter)
	{
		worldFlag1 = (~worldFlag1) << 14;
	}
	else
	{
		worldFlag1 <<= 14;
	}

	m_deactivationNumInactiveFrames[0] = hkUint16((m_deactivationNumInactiveFrames[0] & 0x3fff) | worldFlag0); 
	m_deactivationNumInactiveFrames[1] = hkUint16((m_deactivationNumInactiveFrames[1] & 0x3fff) | worldFlag1);
}

/*
*	SOLVER LEVEL DEACTIVATION
*/

int hkpMotion::getDeactivationClass()
{
	return m_motionState.m_deactivationClass;
}


void hkpMotion::enableDeactivation( bool value, int randomNumber, int worldFlag0, int worldFlag1, int worldDeactivationIntegrateCounter)
{
	if ( value )
	{
		m_deactivationIntegrateCounter = hkUint8(0xf & randomNumber);
		setWorldSelectFlagsNeg(worldFlag0, worldFlag1, worldDeactivationIntegrateCounter); 	 
	}
	else
	{
		m_deactivationIntegrateCounter = 0xff;
		m_deactivationNumInactiveFrames[0] = 0;
		m_deactivationNumInactiveFrames[1] = 0;
	}
}

bool hkpMotion::isDeactivationEnabled() const
{
	return m_deactivationIntegrateCounter != 0xff;
}

int hkpMotion::getNumInactiveFrames(int select)
{
	return m_deactivationNumInactiveFrames[select] & 0x7f;
}

int hkpMotion::getNumInactiveFramesMt(int select, int worldDeactivationNumInactiveFramesSelectFlag)
{
	int dc = m_deactivationNumInactiveFrames[select]; // must be atomic
	int motionDeactivationFlag = dc >> 14;
	HK_ASSERT2(0xad7654da, (worldDeactivationNumInactiveFramesSelectFlag & 0xfffffffc) == 0, "World's deactivation flag must be 0 or 1.");
	//int numInactiveFrames = dc >> (7*(1 - (worldDeactivationNumInactiveFramesSelectFlag ^ motionDeactivationFlag)));
	int numInactiveFrames;
	if (worldDeactivationNumInactiveFramesSelectFlag != motionDeactivationFlag)
	{
		numInactiveFrames = dc & 0x7f;
	}
	else
	{
		numInactiveFrames = (dc >> 7) & 0x7f;
	}
	return numInactiveFrames & 0x7f;
}

void hkpMotion::incrementNumInactiveFramesMt(int select, int worldDeactivationNumInactiveFramesSelectFlag)
{
	HK_ASSERT2(0xad7654db, (worldDeactivationNumInactiveFramesSelectFlag & 0xfffffffc) == 0, "World's deactivation flag must be 0 or 1.");
	HK_ON_DEBUG(int numInactiveFrames = getNumInactiveFramesMt(select, worldDeactivationNumInactiveFramesSelectFlag) );
	int dc = m_deactivationNumInactiveFrames[select];
	HK_ON_DEBUG( int motionDeactivationFlag = dc >> 14);
	HK_ASSERT2(0xad8155dd, motionDeactivationFlag != worldDeactivationNumInactiveFramesSelectFlag, "Flags match -- deactivation run for the 2nd time?");
	dc = dc & 0x7f;
	int newdc = dc + 1 - (dc>>6);
	dc = newdc | (dc<<7) | (worldDeactivationNumInactiveFramesSelectFlag<<14);
	m_deactivationNumInactiveFrames[select] = hkUint16(dc); // must be atomic
	HK_ASSERT2(0xad8555dd, numInactiveFrames == getNumInactiveFramesMt(select, worldDeactivationNumInactiveFramesSelectFlag), "Num inactive frames changed.");
}

void hkpMotion::zeroNumInactiveFramesMt(int select, int worldDeactivationNumInactiveFramesSelectFlag)
{
	//HK_ASSERT2(0xad7654dc, unsigned(worldDeactivationNumInactiveFramesSelectFlag) <= 1, "World's deactivation flag must be 0 or 1.");
	HK_ON_DEBUG(int numInactiveFrames = getNumInactiveFramesMt(select, worldDeactivationNumInactiveFramesSelectFlag) );
	int dc = m_deactivationNumInactiveFrames[select];
	HK_ON_DEBUG( int motionDeactivationFlag = dc >> 14);
	HK_ASSERT2(0xad8455dd, motionDeactivationFlag != worldDeactivationNumInactiveFramesSelectFlag, "Flags match -- deactivation run for the 2nd time?");
	dc = dc & 0x7f;
	dc = (dc<<7) | (worldDeactivationNumInactiveFramesSelectFlag<<14);
	m_deactivationNumInactiveFrames[select] = hkUint16(dc); // must be atomic
	HK_ASSERT2(0xad8756dd, numInactiveFrames == getNumInactiveFramesMt(select, worldDeactivationNumInactiveFramesSelectFlag), "Num inactive frames changed.");
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
