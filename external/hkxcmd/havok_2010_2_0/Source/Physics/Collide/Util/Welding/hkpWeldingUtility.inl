/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



hkpWeldingUtility::SectorType hkpWeldingUtility::getSector(const hkVector4& triangleNormal, const hkVector4& collisionNormal, int edgeBitcode )
{

	// Calculate the cosine for the angle between the triangle's normal and the collision normal
	// Note: this is the projection of the collision normal onto the triangle's normal and will be compared against
	//       the current edge's cosine values.

	hkReal cosAngle = triangleNormal.dot3(collisionNormal).getReal();

	if      ( cosAngle >= m_sinCosTable[edgeBitcode].m_cosAccept0 )
	{
		return hkpWeldingUtility::ACCEPT_0;
	}
	else if ( cosAngle >= m_sinCosTable[edgeBitcode].m_cosSnap0 )
	{
		return hkpWeldingUtility::SNAP_0;
	}
	else if ( cosAngle >= m_sinCosTable[edgeBitcode].m_cosSnap1 )
	{
		return hkpWeldingUtility::REJECT;
	}
	else if ( cosAngle >= m_sinCosTable[edgeBitcode].m_cosAccept1 )
	{
		return hkpWeldingUtility::SNAP_1;
	}

	return hkpWeldingUtility::ACCEPT_1;
}

hkBool hkpWeldingUtility::shouldSnapOneSided(hkpWeldingUtility::WeldingType weldingType, const hkVector4& triangleNormal, const hkVector4& collisionNormal, int edgeBitcode )
{
	if ( edgeBitcode == hkpWeldingUtility::NUM_ANGLES )
	{
		return false;
	}

	hkReal cosAngle = triangleNormal.dot3(collisionNormal).getReal();
	hkReal* sinCosTableEntry = &m_sinCosTable[edgeBitcode].m_cosAccept0;
	return (weldingType == hkpWeldingUtility::WELDING_TYPE_ANTICLOCKWISE) ? cosAngle < sinCosTableEntry[weldingType] : cosAngle > sinCosTableEntry[weldingType];
}

void hkpWeldingUtility::calcSnapVector( const hkVector4& triangleNormal, const hkVector4& edge, int edgeBitcode, SectorType sector, hkVector4& snapVectorOut )
{
	hkVector4 orthNormalEdge; orthNormalEdge.setCross(edge, triangleNormal);

	// rebuild the 'to-be-snapped-to' accept vector by accessing the float values directly via the sector id (either SNAP_0 or SNAP_1)
	hkReal* sinCosTableEntry = &m_sinCosTable[edgeBitcode].m_cosAccept0;
	hkVector4 hVec0; hVec0.setMul4( hkSimdReal( sinCosTableEntry[sector+0] ), triangleNormal );
	hkVector4 hVec1; hVec1.setMul4( hkSimdReal( sinCosTableEntry[sector+1] ), orthNormalEdge );

	snapVectorOut.setAdd4(hVec0, hVec1);
	snapVectorOut.normalize3();
}

void hkpWeldingUtility::calcSnapVectorOneSided( const hkVector4& triangleNormal, const hkVector4& edge, int edgeBitcode, hkpWeldingUtility::WeldingType weldingType, hkVector4& snapVectorOut )
{
	calcSnapVector(triangleNormal, edge, edgeBitcode, (SectorType) weldingType, snapVectorOut );
}

void hkpWeldingUtility::snapCollisionNormal(const hkVector4& triangleNormal, const hkVector4& edge, int edgeBitcode, SectorType sector, hkVector4& collisionNormalInOut )
{

	HK_ASSERT( 0xf02345ef, sector == SNAP_0 || sector == SNAP_1 );

	//
	// rebuild the original snap vector from the triangle normal and the sector information
	//
	hkVector4 snapVector; calcSnapVector( triangleNormal, edge, edgeBitcode, sector, snapVector );

	//
	// get the projection values of the collision normal onto the snap vector and the edge vector
	//
	hkSimdReal dot0 = snapVector.dot3(collisionNormalInOut);
	hkSimdReal dot1 = edge.dot3(collisionNormalInOut);

	//
	// build a new 'snapped' collision normal from the above reconstructed snap vector and the edge vector by using the above
	// calculated projection values
	//
	{
		hkVector4 hVec0; hVec0.setMul4(dot0, snapVector);
		hkVector4 hVec1; hVec1.setMul4(dot1, edge);
		collisionNormalInOut.setAdd4(hVec0, hVec1);
		collisionNormalInOut.normalize3();	
	}

	HK_ASSERT2( 0xf0f3eaad, hkMath::equal( collisionNormalInOut.lengthSquared3().getReal(), 1.0f, 0.01f), "CollisionNormal is not normalized" );

	return;
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
