/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/Collide/hkpSimpleConstraintContactMgr.h>
#include <Physics/Dynamics/Constraint/Contact/hkpSimpleContactConstraintUtil.h>

hkpContactPointEvent::hkpContactPointEvent( CallbackSource source, hkpRigidBody* a, hkpRigidBody* b, hkpSimpleConstraintContactMgr* mgr,
												Type type,
												hkContactPoint* cp, hkpContactPointProperties* cpp,
												hkReal* sepVel, hkReal* rotNorm, 
												hkBool fullManifold, hkBool firstCallback, hkBool lastCallback,
												hkpShapeKey* keyStorage,
												hkpVelocityAccumulator* accA, hkpVelocityAccumulator* accB )
											 :	hkpCollisionEvent( source, a, b, mgr ),
												m_type( type ),
												m_contactPoint(cp),
												m_contactPointProperties( cpp ),
												m_firingCallbacksForFullManifold( fullManifold ),
												m_firstCallbackForFullManifold( firstCallback ),
												m_lastCallbackForFullManifold( lastCallback ),
												m_separatingVelocity( sepVel ),
												m_rotateNormal( rotNorm ),
												m_shapeKeyStorage( keyStorage )
{
	m_accumulators[0] = accA;
	m_accumulators[1] = accB;
}

hkBool hkpContactPointEvent::isToi() const
{
	return m_type <= TYPE_TOI; 
}

inline hkpShapeKey* hkpContactPointEvent::getShapeKeys( int bodyIdx ) const
{
	HK_ASSERT2( 0x4e66ea80, bodyIdx == 0 || bodyIdx == 1, "The bodyIdx must be 0 or 1." );
	hkpShapeKey* shapeKeys = HK_NULL;
	// Was shape key storage made available for this body?
	if ( getBody(bodyIdx)->m_numShapeKeysInContactPointProperties > 0 )
	{
		shapeKeys = m_shapeKeyStorage + ( bodyIdx * m_bodies[0]->m_numShapeKeysInContactPointProperties );
	}
	return shapeKeys;
}

inline hkReal hkpContactPointEvent::getSeparatingVelocity() const
{
	if ( m_separatingVelocity )
	{
		return *m_separatingVelocity;
	}
	else
	{
		return hkpSimpleContactConstraintUtil::calculateSeparatingVelocity( m_bodies[0], m_bodies[1], m_bodies[0]->getCenterOfMassInWorld(), m_bodies[1]->getCenterOfMassInWorld(), m_contactPoint );
	}
}

inline void hkpContactPointEvent::setSeparatingVelocity( hkReal sepVel ) const
{
	HK_ASSERT2( 0x4ef35a8, m_separatingVelocity, "The separating velocity cannot be set for this contactPoint because the event type is wrong, or the contactPoint is not new." );
	*m_separatingVelocity = sepVel;
}

inline hkReal hkpContactPointEvent::getRotateNormal() const
{
	HK_ASSERT2( 0x4ef35a8, m_rotateNormal, "You cannot get the rotateNormal in a contactPointCallback of this type" );
	return *m_rotateNormal;
}

inline void hkpContactPointEvent::setRotateNormal( hkReal rotNorm ) const
{
	HK_ASSERT2( 0x4ef35a8, m_rotateNormal, "You cannot set the rotateNormal in a contactPointCallback of this type" );
	*m_rotateNormal = rotNorm;
}

inline void hkpContactPointEvent::accessVelocities( int bodyIdx ) const
{
	HK_ASSERT2( 0x4e66ea80, bodyIdx == 0 || bodyIdx == 1, "The bodyIdx must be 0 or 1." );
	hkpMotion* motion = getBody( bodyIdx )->getMotion();
	if ( ( m_type == TYPE_MANIFOLD ) || ( m_type == TYPE_EXPAND_MANIFOLD ) )
	{
		motion->m_linearVelocity = m_accumulators[bodyIdx]->m_linearVel;
		if ( ( motion->getType() == hkpMotion::MOTION_BOX_INERTIA ) || ( motion->getType() == hkpMotion::MOTION_THIN_BOX_INERTIA ) )
		{
			motion->m_angularVelocity._setRotatedDir( motion->getTransform().getRotation(), m_accumulators[bodyIdx]->m_angularVel );
		}
		else
		{
			motion->m_angularVelocity = m_accumulators[bodyIdx]->m_angularVel;
		}
	}
}

inline void hkpContactPointEvent::updateVelocities( int bodyIdx ) const
{
	HK_ASSERT2( 0x4e66ea80, bodyIdx == 0 || bodyIdx == 1, "The bodyIdx must be 0 or 1." );
	hkpMotion* motion = getBody( bodyIdx )->getMotion();
	if ( ( m_type == TYPE_MANIFOLD ) || ( m_type == TYPE_EXPAND_MANIFOLD ) )
	{
		m_accumulators[bodyIdx]->m_linearVel = motion->m_linearVelocity;
		if ( ( motion->getType() == hkpMotion::MOTION_BOX_INERTIA ) || ( motion->getType() == hkpMotion::MOTION_THIN_BOX_INERTIA ) )
		{
			m_accumulators[bodyIdx]->m_angularVel._setRotatedInverseDir( motion->getTransform().getRotation(), motion->m_angularVelocity );
		}
		else
		{
			m_accumulators[bodyIdx]->m_angularVel = motion->m_angularVelocity;
		}
	}
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
