/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS_IMPULSE_ACCUMULATOR_H
#define HK_DYNAMICS_IMPULSE_ACCUMULATOR_H

class hkpRigidBody;

/// An impulse accumulator that allow for multiple applications of forces and/or impulses at different positions.
/// Actual change in velocity only happens when flushImpulses is called.
struct hkpImpulseAccumulator
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpImpulseAccumulator);
	/// ctor.
							hkpImpulseAccumulator(hkpRigidBody* body, hkReal timeStep);

	/// clear accumulated impulses.
	void					clearImpulses();

	/// apply and flush accumulated impulses.
	void					flushImpulses();

	/// add a force to the accumulator.
	HK_FORCE_INLINE	void	addForce(const hkVector4& position, const hkVector4& force);
	
	/// add an impulse to the accumulator.
	HK_FORCE_INLINE	void	addImpulse(const hkVector4& position, const hkVector4& impulse);
	

	hkpRigidBody*	m_body;					///< Target rigid body.
	hkSimdReal		m_dt;					///< Time step used to convert forces to impulses.
	hkVector4		m_com;					///< World center of mass of the rigid body.
	hkVector4		m_accLinearImpulses;	///< Accumulated linear impulse.
	hkVector4		m_accAngularImpulses;	///< Accumulated angular impulse.
};

#include <Physics/Utilities/Dynamics/ImpulseAccumulator/hkpImpulseAccumulator.inl>

#endif	// HK_DYNAMICS_IMPULSE_ACCUMULATOR_H

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
