/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


HK_FORCE_INLINE	void	hkpImpulseAccumulator::addForce(const hkVector4& position,const hkVector4& force)
{
	// Convert force to impulse	
	hkVector4	impulse;
	impulse.setMul4(m_dt,force);

	// Accumulate impulse		
	addImpulse(position,impulse);
}

HK_FORCE_INLINE	void	hkpImpulseAccumulator::addImpulse(const hkVector4& position,const hkVector4& impulse)
{
	// Accumulate linear		
	hkVector4 linearImpulse; linearImpulse.setAdd4( m_accLinearImpulses, impulse );

	// Accumulate angular		
	hkVector4	r;
	r.setSub4(position,m_com);
	hkVector4	a;
	a.setCross(r,impulse);
	m_accAngularImpulses.add4(a);
	
	m_accLinearImpulses = linearImpulse;
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
