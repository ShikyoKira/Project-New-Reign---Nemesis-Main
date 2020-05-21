/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
HK_FORCE_INLINE hkReal hkVector2::signedDistanceToLineTimesLength( const hkVector2& p0, const hkVector2& p1 ) const
{
	return ((p1.x - p0.x)*(y - p0.y) - (x - p0.x)*(p1.y - p0.y));
}

inline hkBool32 hkVector2::rightOfLine( const hkVector2& p0, const hkVector2& p1 ) const
{
	return signedDistanceToLineTimesLength(p0,p1) < 0;
}

inline hkBool32 hkVector2::leftOfLine( const hkVector2& p0, const hkVector2& p1 ) const
{
	return signedDistanceToLineTimesLength(p0,p1) > 0;
}

inline hkBool32 hkVector2::inCircumcircle( const hkVector2& p0, const hkVector2& p1, const hkVector2& p2, hkReal tolerance ) const
{
	hkVector4 a; a.set( p0.x - x, p1.x - x, p2.x - x );
	hkVector4 b; b.set( p0.y - y, p1.y - y, p2.y - y );
	hkVector4 temp1; temp1.setMul4( a, a );
	hkVector4 temp2; temp2.setMul4( b, b );
	hkVector4 c; c.setAdd4( temp1, temp2 );
	hkVector4 cross; cross.setCross( b, c );
	return a.dot3( cross ).getReal() > -tolerance; // XXX
}

inline hkBool32 hkVector2::equals( const hkVector2& p0 ) const
{
	return x == p0.x && y == p0.y;
}

inline hkReal hkVector2::dot( const hkVector2& p ) const
{
	return x*p.x + y*p.y;
}

inline void hkVector2::setMax( const hkVector2& a, const hkVector2& b )
{
	x = hkMath::max2(a.x,b.x);
	y = hkMath::max2(a.y,b.y);
}

inline void hkVector2::setMin( const hkVector2& a, const hkVector2& b )
{
	x = hkMath::min2(a.x,b.x);
	y = hkMath::min2(a.y,b.y);
}

inline void hkVector2::setAll( hkReal a )
{
	x = a;
	y = a;
}

inline void hkVector2::set( hkReal a, hkReal b )
{
	x = a;
	y = b;
}

inline void hkVector2::load( const hkReal* p )
{
	x = p[0];
	y = p[1];
}

inline void hkVector2::setPerp( const hkVector2& a )
{
	hkReal t = a.x; // alias safe
	x = -a.y;
	y =  t;
}

inline void hkVector2::setAdd( const hkVector2& a, const hkVector2& b )
{
	x = a.x + b.x;
	y = a.y + b.y;
}

inline void hkVector2::setMul( const hkVector2& v, hkReal r )
{
	x = v.x * r;
	y = v.y * r;
}

inline void hkVector2::setMul( const hkVector2& v, const hkVector2& w )
{
	x = v.x * w.x;
	y = v.y * w.y;
}

inline void hkVector2::mul( hkReal r )
{
	x *= r;
	y *= r;
}

inline void hkVector2::setAddMul( const hkVector2& a, const hkVector2& b, hkReal r )
{
	x = a.x + b.x * r;
	y = a.y + b.y * r;
}

inline void hkVector2::setSub( const hkVector2& a, const hkVector2& b )
{
	x = a.x - b.x;
	y = a.y - b.y;
}

inline void hkVector2::setInterpolate( const hkVector2& a, const hkVector2& b, hkReal t)
{
	x = a.x + t * (b.x-a.x);
	y = a.y + t * (b.y-a.y);
}

inline hkReal hkVector2::distanceTo( const hkVector2& p ) const
{
	hkReal dx = x - p.x;
	hkReal dy = y - p.y;
	return hkMath::sqrt( dx*dx + dy*dy );
}

inline void hkVector2::setProject( hkVector4Parameter p, hkVector4Parameter ax, hkVector4Parameter ay )
{
	x = ax.dot3(p).getReal();
	y = ay.dot3(p).getReal();
}

inline void hkVector2::normalize()
{
	hkReal n = hkMath::sqrtInverse(x*x + y*y);
	x *= n;
	y *= n;
}

inline hkReal hkVector2::normalizeWithLength()
{
	hkReal len2 = x*x + y*y;
	hkReal len = hkMath::sqrt(len2);
	const hkReal invLen = (len ? (1.0f)/len : 0.0f);

	x *= invLen;
	y *= invLen;

	return len;
}

inline hkReal hkVector2::length() const
{
	return hkMath::sqrt(x*x + y*y);
}

inline void hkVector2::setZero()
{
	x = y = 0;
}

inline hkBool32 hkVector2::lexLess( const hkVector2& v )
{
	return (x<v.x) || (x==v.x && y<v.y);
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
