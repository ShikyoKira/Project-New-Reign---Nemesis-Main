/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkSimdReal hkTreeBundleRayCastUtil::pointToLineSegment2(const hkVector4& a, const hkVector4& b, const hkVector4& p)
{
	hkVector4 ab;
	ab.setSub4( b, a );
	hkVector4 ap;
	ap.setSub4( p, a );
	hkVector4 bp;
	bp.setSub4( p, b );

	hkSimdReal dot = ab.dot3( ap );

	if( dot.getReal() < 0 )
		return ap.dot3( ap );
	hkSimdReal ab2 = ab.dot3( ab );
	if( dot.getReal() > ab2.getReal() )
		return bp.dot3( bp );

	return ap.dot3( ap ) - dot*dot/ab2;
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
