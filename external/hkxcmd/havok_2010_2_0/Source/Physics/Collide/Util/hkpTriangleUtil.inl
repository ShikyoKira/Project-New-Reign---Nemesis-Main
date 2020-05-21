/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline void HK_CALL hkpTriangleUtil::calcNormal( hkVector4& normal, const hkVector4& a, const hkVector4& b, const hkVector4& c ) 
{
	hkVector4 cb;
	hkVector4 ab;
	cb.setSub4(c,b);
	ab.setSub4(a,b);

	normal.setCross( cb , ab);
}

inline void HK_CALL hkpTriangleUtil::calcCentroid( hkVector4& centroid, const hkVector4& a, const hkVector4& b, const hkVector4& c ) 
{
	centroid.setAdd4(a,b);
	centroid.add4(c);
	centroid.mul4(hkSimdReal(1.0f/3.0f));
}

inline hkBool HK_CALL hkpTriangleUtil::inFront( const hkVector4& point, const hkVector4& a, const hkVector4& b, const hkVector4& c ) 
{
	hkVector4 triangleNormal;
	calcNormal ( triangleNormal, a, b, c );

	hkVector4 d; d.setSub4(point, a);
	return 	d.dot3(triangleNormal).getReal() > 0;
}


//
// Checks that a point is inside a triangle
// Assumes the point already lies in the plane of the triangle
//
inline hkBool HK_CALL hkpTriangleUtil::containsPoint( const hkVector4 &pt, const hkVector4& a, const hkVector4& b, const hkVector4& c ) 
{
	// Get the normal of the triangle
	hkVector4 triangleNormal;
	calcNormal ( triangleNormal, a, b, c );

	// Check that the point is on the 'correct'
	// side of each triangle edge
	hkVector4 pta;
	pta.setSub4(pt,a);
	hkVector4 ba;
	ba.setSub4(b,a);
	pta.setCross(pta,ba);

	hkVector4 ptb;
	ptb.setSub4(pt,b);
	hkVector4 cb;
	cb.setSub4(c,b);
	ptb.setCross(ptb,cb);

	hkVector4 ptc;
	ptc.setSub4(pt,c);
	hkVector4 ac;
	ac.setSub4(a,c);
	ptc.setCross(ptc,ac);

	hkRotation simultaneousDots;

	//set column and then inverse mul
	//is actually faster than set row
	//and then mul
	simultaneousDots.setCols(pta,ptb,ptc);
	//pta now holds the dot products
	pta._setRotatedInverseDir(simultaneousDots,triangleNormal);

	// all dots less than zero means we are inside
	hkVector4 zero; zero.setZero4();
	return pta.allLessThan3(zero) != 0;
}

inline hkReal HK_CALL hkpTriangleUtil::dot3fpu(const hkVector4& a, const hkVector4& b)
{
	volatile hkReal p0 = a(0) * b(0);
	volatile hkReal p1 = a(1) * b(1);
	volatile hkReal p2 = a(2) * b(2);
	return p0 + p1 + p2;
}

// The Triangle lies in the plane defined by x.N+d, where x is any point in the 
// plane,  N is the normal to the plane and d is the distance from the plane to the 
// origin (the planar distance)
//inline hkReal HK_CALL hkpTriangleUtil::calcPlaneConstant( const hkVector4& a, const hkVector4& b, const hkVector4& c ) 
//{
//	hkVector4 n;
//	calcNormal( n, a, b, c );
//	n.normalize3();
//
//	return n.dot3(a);
//}

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
