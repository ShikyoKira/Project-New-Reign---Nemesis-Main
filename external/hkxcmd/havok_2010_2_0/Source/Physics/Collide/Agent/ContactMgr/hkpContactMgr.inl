/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkContactPointId hkpContactMgr::addContactPoint( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, const hkpGskCache* contactCache, hkContactPoint& cp )
{
	return addContactPointImpl( a, b, input, output, contactCache, cp );
}

hkResult hkpContactMgr::reserveContactPoints( int numPoints )
{
	return reserveContactPointsImpl( numPoints );
}

void hkpContactMgr::removeContactPoint( hkContactPointId cpId, hkCollisionConstraintOwner& constraintOwner )
{
	removeContactPointImpl( cpId, constraintOwner );
}

void hkpContactMgr::processContact( const hkpCollidable& a, const hkpCollidable& b, const hkpProcessCollisionInput& input, hkpProcessCollisionData& collisionData )
{
	processContactImpl( a, b, input, collisionData );
}

hkpContactMgr::ToiAccept hkpContactMgr::addToi( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, hkTime toi, hkContactPoint& cp, const hkpGskCache* gskCache, hkReal& projectedVelocity, hkpContactPointProperties& propertiesOut )
{
	return addToiImpl( a, b, input, output, toi, cp, gskCache, projectedVelocity, propertiesOut  );
}

void hkpContactMgr::removeToi( class hkCollisionConstraintOwner& constraintOwner, hkpContactPointProperties& properties )
{
	removeToiImpl( constraintOwner, properties );
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
