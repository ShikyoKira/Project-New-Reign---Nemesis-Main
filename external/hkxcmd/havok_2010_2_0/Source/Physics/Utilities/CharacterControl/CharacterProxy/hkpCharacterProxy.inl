/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// Computes a metric to allow us to compare points for similarity.
//	Metric is based on:
//    - Angle between normals
//    - Contact Point Distances
//	  - Velocity
hkReal hkpCharacterProxy::surfaceDistance(const hkpRootCdPoint& p1
												 , const hkpRootCdPoint& p2
												 HK_SPU_ARG( hkpSpuCharacterProxyCollisionCache* collisionCache )
												 ) const
{
	// Small angle approx (changed from cross product HVK-2184)
	hkReal angleSquared = 1 - static_cast<hkReal>(p1.m_contact.getNormal().dot3(p2.m_contact.getNormal()));
	angleSquared *= m_contactAngleSensitivity * m_contactAngleSensitivity;

	hkReal planeDistanceSqrd = p1.m_contact.getDistance() - p2.m_contact.getDistance();
	planeDistanceSqrd *= planeDistanceSqrd;

	hkVector4 p1Vel, p2Vel;

	extractSurfaceVelocity(p1, p1Vel HK_SPU_ARG( collisionCache ) );
	extractSurfaceVelocity(p2, p2Vel HK_SPU_ARG( collisionCache ) );

	hkVector4 velDiff; velDiff.setSub4( p1Vel, p2Vel );
	hkReal velocityDiffSqrd = velDiff.lengthSquared3();

	return angleSquared * 10.0f + velocityDiffSqrd * 0.1f + planeDistanceSqrd;
}

// Extract velocity information from the contact point.
void hkpCharacterProxy::extractSurfaceVelocity(const hkpRootCdPoint& hit
											   , hkVector4& velocityOut
											   HK_SPU_ARG( hkpSpuCharacterProxyCollisionCache* collisionCache )
											   ) const
{
	velocityOut.setZero4();

	// Grab body information // Body A is always the shapePhantom

	hkpRigidBody* body = hkpGetRigidBody(hit.m_rootCollidableB);

	if (body)
	{
		// Extract velocity at point on surface
		body->getPointVelocity(hit.m_contact.getPosition(), velocityOut);
	}
}

void hkpCharacterProxy::convertFractionToDistance( const hkpRootCdPoint* cHit, int numHits, const hkVector4& displacement ) const
{
	hkpRootCdPoint* hit = const_cast<hkpRootCdPoint*>(cHit);
	for (int i = numHits-1; i>=0; i--)
	{
		hkReal fraction = hit->m_contact.getDistance();
		hit->m_contact.getPosition()(3) = fraction;
		hkReal projLength = displacement.dot3(hit->m_contact.getNormal());
		hit->m_contact.setDistance( fraction * -projLength );
		hit++;
	}
}

void hkpCharacterProxy::convertFractionToDistance( const HK_COLLECTOR_TYPE& collector, const hkVector4& displacement ) const
{
	const hkpRootCdPoint* cHit = collector.getHits().begin();
	int numHits = collector.getHits().getSize();
	convertFractionToDistance( cHit, numHits, displacement );
}

void hkpCharacterProxy::addEntityOrPhantom( const hkpCollidable* collidable
										   , hkArray<hkpRigidBody*>& bodies
										   , hkArray<hkpPhantom*>& phantoms )
{
	hkpRigidBody* body =  hkpGetRigidBody( collidable );
	hkpPhantom* phantom = hkpGetPhantom( collidable );

	if ( body && ( bodies.indexOf(body) == -1 ) )
	{
		bodies.pushBack( body );
		// Note: adding/removing listeners from bodies and phantoms is not thread safe.
		body->addEntityListener(this);	
	}

	if (phantom && (phantoms.indexOf(phantom) == -1) )
	{
		phantoms.pushBack( phantom );
		// Note: adding/removing listeners from bodies and phantoms is not thread safe.
		phantom->addPhantomListener( this );
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
