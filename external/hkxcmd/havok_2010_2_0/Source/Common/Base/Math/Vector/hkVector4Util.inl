/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_VECTOR4UTIL_setNormalOfTriangle
HK_FORCE_INLINE void HK_CALL hkVector4Util::setNormalOfTriangle(hkVector4 &result, const hkVector4 &a, const hkVector4& b,const hkVector4& c)
{
	hkVector4 ba,ca;
	ba.setSub4( b, a );
	ca.setSub4( c, a );
	result.setCross( ba , ca);
}
#endif

#ifndef HK_VECTOR4UTIL_mulSigns4
inline void HK_CALL hkVector4Util::mulSigns4( hkVector4& inout, const hkVector4& signv)
{
	hkUint32* intp = reinterpret_cast<hkUint32*>(&inout);
	const hkUint32* sign = reinterpret_cast<const hkUint32*>(&signv);
	intp[0] ^= (sign[0] & 0x80000000);
	intp[1] ^= (sign[1] & 0x80000000);
	intp[2] ^= (sign[2] & 0x80000000);
	intp[3] ^= (sign[3] & 0x80000000);
}
#endif

#ifndef HK_VECTOR4UTIL_convertToUint16
inline void HK_CALL hkVector4Util::convertToUint16( const hkVector4& in, const hkVector4& offset, const hkVector4& scale, hkIntUnion64& out)
{
	static hkQuadReal magicQuad = HK_QUADREAL_CONSTANT(hkReal(0x30000), hkReal(0x30000), hkReal(0x30000), hkReal(0x30000) );

	hkVector4 x, magic;
	x.setAdd4(in,offset);
	x.mul4(scale);
	magic.getQuad() = magicQuad;
	x.add4( magic );

	union 
	{
		hkQuadReal q;
		hkUint32 i[4];
	} u;

	u.q = x.getQuad();
	// Note: throw away top 16 bits
	out.i16[0] = static_cast<hkInt16>(u.i[0] >> 6);
	out.i16[1] = static_cast<hkInt16>(u.i[1] >> 6);
	out.i16[2] = static_cast<hkInt16>(u.i[2] >> 6);
	out.i16[3] = static_cast<hkInt16>(u.i[3] >> 6);
}
#endif

#ifndef HK_VECTOR4UTIL_convertToUint16WithClip
// can't be inside convert to clip as gcc will not inline funcs with statics in them
static hkQuadReal convertToUint16WithClipMagicQuad = HK_QUADREAL_CONSTANT( 65536.0f, 65536.0f, 65536.0f, 65536.0f );
inline void HK_CALL hkVector4Util::convertToUint16WithClip( const hkVector4& in, const hkVector4& offset, const hkVector4& scale, const hkVector4& min, const hkVector4& max, hkIntUnion64& out)
{
	
	
	hkVector4 x;
	x.setAdd4(in,offset);
	x.mul4(scale);
	x.setMin4(x,max);
	x.setMax4(x,min);
	hkVector4 convertToUint16WithClipMagic; 
	convertToUint16WithClipMagic.getQuad() = convertToUint16WithClipMagicQuad;
	x.add4( convertToUint16WithClipMagic );
	
	union 
	{
		hkQuadReal q;
		hkUint32 i[4];
	} u;

	u.q = x.getQuad();
	// note implicitly throw away top 16 bits
	out.i16[0] = hkInt16(u.i[0] >> 7);
	out.i16[1] = hkInt16(u.i[1] >> 7);
	out.i16[2] = hkInt16(u.i[2] >> 7);
	out.i16[3] = hkInt16(u.i[3] >> 7);
}
#endif

#ifndef HK_VECTOR4UTIL_convertToUint32
inline void HK_CALL hkVector4Util::convertToUint32( const hkVector4& in, const hkVector4& offset, const hkVector4& scale, hkUint32* out)
{
	hkVector4 x;
	x.setAdd4(in,offset);
	x.mul4(scale);

	out[0] = static_cast<hkUint32>(hkMath::hkToIntFast(x(0)));
	out[1] = static_cast<hkUint32>(hkMath::hkToIntFast(x(1)));
	out[2] = static_cast<hkUint32>(hkMath::hkToIntFast(x(2)));
	out[3] = static_cast<hkUint32>(hkMath::hkToIntFast(x(3)));
}
#endif

#ifndef HK_VECTOR4UTIL_convertToUint32WithClip
inline void HK_CALL hkVector4Util::convertToUint32WithClip( const hkVector4& in, const hkVector4& offset, const hkVector4& scale, const hkVector4& min, const hkVector4& max, hkUint32* out)
{
	hkVector4 x;
	x.setAdd4(in,offset);
	x.mul4(scale);
	x.setMin4(x,max);
	x.setMax4(x,min);
	
	out[0] = static_cast<hkUint32>(hkMath::hkToIntFast(x(0)));
	out[1] = static_cast<hkUint32>(hkMath::hkToIntFast(x(1)));
	out[2] = static_cast<hkUint32>(hkMath::hkToIntFast(x(2)));
	out[3] = static_cast<hkUint32>(hkMath::hkToIntFast(x(3)));
}
#endif

#if !defined(HK_VECTOR4UTIL_convertAabbToUint32)
inline void HK_CALL hkVector4Util::convertAabbToUint32( const hkAabb& aabb, const hkVector4& offsetLow, const hkVector4& offsetHigh, const hkVector4& scale, hkAabbUint32& aabbOut )
{
	HK_ASSERT2(0x4868f301,  aabb.isValid(), "AABB at " << &aabb << " was invalid. (Contains a NaN or min > max)");

	hkVector4 maxVal; maxVal.getQuad() = hkVector4Util::hkAabbUint32MaxVal;
	hkVector4Util::convertToUint32WithClip( aabb.m_min, offsetLow,  scale, hkVector4::getZero(), maxVal, aabbOut.m_min );
	hkVector4Util::convertToUint32WithClip( aabb.m_max, offsetHigh, scale, hkVector4::getZero(), maxVal, aabbOut.m_max );
}
#endif

#ifndef HK_VECTOR4UTIL_convertAabbFromUint32
//Based on void hkp3AxisSweep::getAabbFromNode(const hkpBpNode& node, hkAabb & aabb) const
inline void HK_CALL hkVector4Util::convertAabbFromUint32( const hkAabbUint32& aabbIn, const hkVector4& offsetLow, const hkVector4& scale, hkAabb& aabbOut)
{
	aabbOut.m_min.set( hkReal(aabbIn.m_min[0]), hkReal(aabbIn.m_min[1]), hkReal(aabbIn.m_min[2]) );
	aabbOut.m_max.set( hkReal(aabbIn.m_max[0]), hkReal(aabbIn.m_max[1]), hkReal(aabbIn.m_max[2]) );

	hkVector4 invScale; invScale.setReciprocal3(scale);

	aabbOut.m_min.mul4(invScale);
	aabbOut.m_max.mul4(invScale);

	aabbOut.m_min.sub4(offsetLow);
	aabbOut.m_max.sub4(offsetLow);
}
#endif


#ifndef HK_VECTOR4UTIL_calculatePerpendicularVector
inline void HK_CALL hkVector4Util::calculatePerpendicularVector(const hkVector4& vectorIn, hkVector4& biVectorOut)
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 d0; d0.setCross( vectorIn, hkVector4::getConstant( HK_QUADREAL_1) );
	hkVector4 d1; d1.setCross( vectorIn, hkVector4::getConstant( HK_QUADREAL_1000) );
	hkSimdReal l0 = d0.lengthSquared3();
	hkSimdReal l1 = d1.lengthSquared3();
	hkVector4 s0; s0.setAll( l0 );
	hkVector4 s1; s1.setAll( l1 );
	hkVector4Comparison c = s0.compareLessThan4( s1 );
	biVectorOut.select32(d0,d1,c);
#else
	// find the indices of (one of) the smallest component(s) and the indices of
	// the remaining two components in the vector
	int min = 0;
	int ok1 = 1;
	int ok2 = 2;

	hkReal a0 = hkMath::fabs(vectorIn(0));
	hkReal a1 = hkMath::fabs(vectorIn(1));
	hkReal a2 = hkMath::fabs(vectorIn(2));

	// sort the indices to make min index point to the smallest
	if( a1 < a0 )
	{
		ok1 = 0;
		min = 1;
		a0 = a1;
	}

	if(a2 < a0)
	{
		ok2 = min;
		min = 2;
	}

	biVectorOut.setZero4();
	biVectorOut(ok1) =  vectorIn(ok2);
	biVectorOut(ok2) = -vectorIn(ok1);
#endif
}
#endif

#ifndef HK_VECTOR4UTIL_transformPoints
inline void HK_CALL hkVector4Util::transformPoints( const hkTransform& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut )
{
	HK_ASSERT2( 0xf0237abd, numVectors > 0, "At least one element required");
	checkRegisters( 0xf0000000 );
	hkTransform unaliased = t;
	int i = numVectors - 1;
	do
	{
		vectorsOut[i]._setTransformedPos( unaliased, vectorsIn[i] );
	} while( --i >= 0 );
}
#endif

#ifndef HK_VECTOR4UTIL_qtransformPoints
inline void HK_CALL hkVector4Util::transformPoints( const hkQTransform& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut )
{
	HK_ASSERT2( 0xf0237abd, numVectors > 0, "At least one element required");
	checkRegisters( 0xf0000000 );
	hkQTransform unaliased = t;
	int i = numVectors - 1;
	do
	{
		vectorsOut[i]._setTransformedPos( unaliased, vectorsIn[i] );
	} while( --i >= 0 );
}
#endif


#ifndef HK_VECTOR4UTIL_transformPlaneEquations
HK_FORCE_INLINE void HK_CALL hkVector4Util::transformPlaneEquations( const hkTransform& t, const hkVector4* HK_RESTRICT planes, int numPlanes, hkVector4* HK_RESTRICT planesOut )
{
	const hkRotation rotation = t.getRotation();	// we are making a copy here to allow for better compiler support

	hkVector4 tr;	tr.setNeg4(t.getTranslation()); 
	hkVector4 pivotShift; pivotShift._setRotatedInverseDir( rotation, tr );

	int p = numPlanes - 1;
	do
	{
		hkSimdReal newDist = planes->dot4xyz1(pivotShift);
		planesOut->setRotatedDir(rotation, planes[0] );

		planes++;

		hkVector4 w; w.setAll( newDist );
		planesOut->setW( w );
		planesOut++;
	}
	while ( --p >=0 );
}
#endif

#ifndef HK_VECTOR4UTIL_mul4xyz1Points
inline void HK_CALL hkVector4Util::mul4xyz1Points( const hkTransform& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut )
{
	HK_ASSERT2( 0xf0237abe, numVectors > 0, "At least one element required");
	checkRegisters( 0xf0000000 );
	hkTransform unaliased = t;
	int i = numVectors - 1;
	do
	{
		vectorsOut[i]._setMul4xyz1( unaliased, vectorsIn[i] );
	} while( --i >= 0 );
}
#endif

#ifndef HK_VECTOR4UTIL_transformSpheres
inline void HK_CALL hkVector4Util::transformSpheres( const hkTransform& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut )
{
	HK_ASSERT2( 0xf0237abf, numVectors > 0, "At least one element required");
	checkRegisters( 0xf0000000 );
	hkTransform unaliased = t;
	int i = numVectors - 1;
	do
	{
		hkVector4 v;
		v._setTransformedPos( unaliased, vectorsIn[i] ); // position
		v.setW(vectorsIn[i]);			// radius
		vectorsOut[i] = v;
	} while( --i >= 0 );
}
#endif

#ifndef HK_VECTOR4UTIL_rotatePoints
inline void HK_CALL hkVector4Util::rotatePoints( const hkMatrix3& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut )
{
	HK_ASSERT2( 0xf0237ac0, numVectors > 0, "At least one element required");
	checkRegisters( 0x70000000 );
	hkMatrix3 unaliased = t;
	int i = numVectors - 1;
	do
	{
		vectorsOut[i]._setMul3( unaliased, vectorsIn[i] );
	} while( --i >= 0 );
}
#endif

#ifndef HK_VECTOR4UTIL_rotateInversePoints
inline void HK_CALL hkVector4Util::rotateInversePoints( const hkRotation& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut )
{
	HK_ASSERT2( 0xf0237ac1, numVectors > 0, "At least one element required");
	hkRotation unaliased = t;
	int i = numVectors - 1;
	do
	{
		vectorsOut[i]._setRotatedInverseDir( unaliased, vectorsIn[i] );
	} while( --i >= 0 );
}
#endif

#ifndef HK_VECTOR4UTIL_setMul
inline void HK_CALL hkVector4Util::setMul( const hkMatrix3& aTb, const hkMatrix3& bTc, hkMatrix3& aTcOut )
{
	HK_ASSERT(0x6d9d1d43,  &aTcOut != &aTb );
	HK_ASSERT(0x64a8df81,  &aTcOut != &bTc );
	hkVector4Util::rotatePoints( aTb, &bTc.getColumn(0), 3, &aTcOut.getColumn(0) );
}
#endif

#ifndef HK_VECTOR4UTIL_setInverseMul
inline void HK_CALL hkVector4Util::setInverseMul( const hkRotation& bTa, const hkMatrix3& bTc, hkMatrix3& aTcOut )
{
	hkVector4Util::rotateInversePoints( bTa, &bTc.getColumn(0), 3, &aTcOut.getColumn(0) );
}
#endif

#ifndef HK_VECTOR4UTIL_transpose
inline void HK_CALL hkVector4Util::transpose( hkMatrix3& m )
{
	hkReal h1 = m(0,1); m(0,1) = m(1,0), m(1,0) = h1;
	hkReal h2 = m(0,2); m(0,2) = m(2,0), m(2,0) = h2;
	hkReal h3 = m(1,2); m(1,2) = m(2,1), m(2,1) = h3;
}
#endif

#ifndef HK_VECTOR4UTIL_add
inline void HK_CALL hkVector4Util::add( hkMatrix3& m, const hkMatrix3& a )
{
	m.getColumn(0).add4( a.getColumn(0) );
	m.getColumn(1).add4( a.getColumn(1) );
	m.getColumn(2).add4( a.getColumn(2) );
}
#endif

#ifndef HK_VECTOR4UTIL_buildOrthonormal
inline void HK_CALL hkVector4Util::buildOrthonormal( const hkVector4& dir, hkMatrix3& out )
{
	out.getColumn(0) = dir;
	calculatePerpendicularVector( dir, out.getColumn(1));
	out.getColumn(1).normalize3();
	out.getColumn(2).setCross( dir, out.getColumn(1));
}
#endif

#ifndef HK_VECTOR4UTIL_buildOrthonormal
void HK_CALL hkVector4Util::buildOrthonormal( const hkVector4& dir, const hkVector4& dir2, hkMatrix3& out )
{
	out.getColumn(0) = dir;
	out.getColumn(2).setCross( dir, dir2 );
	if ( out.getColumn(2).length3().getReal() < HK_REAL_EPSILON )
	{
		buildOrthonormal( dir, out );
		return;
	}
	out.getColumn(2).normalize3();
	out.getColumn(1).setCross( out.getColumn(2), dir );
}
#endif

#ifndef HK_VECTOR4UTIL_dot3_3vs3
inline void HK_CALL hkVector4Util::dot3_3vs3(const hkVector4& a0, const hkVector4& b0, const hkVector4& a1, const hkVector4& b1, const hkVector4& a2, const hkVector4& b2, hkVector4& dotsOut)
{
	dotsOut(0) = a0.dot3( b0 );
	dotsOut(1) = a1.dot3( b1 );
	dotsOut(2) = a2.dot3( b2 );
	dotsOut(3) = 0;
}
#endif

#ifndef HK_VECTOR4UTIL_dot3_4vs4
inline void HK_CALL hkVector4Util::dot3_4vs4( const hkVector4& a0, const hkVector4& b0, const hkVector4& a1, const hkVector4& b1, const hkVector4& a2, const hkVector4& b2, const hkVector4& a3, const hkVector4& b3, hkVector4& dotsOut)
{
	dotsOut(0) = a0.dot3( b0 );
	dotsOut(1) = a1.dot3( b1 );
	dotsOut(2) = a2.dot3( b2 );
	dotsOut(3) = a3.dot3( b3 );
}
#endif

#ifndef HK_VECTOR4UTIL_dot4_4vs4
inline void HK_CALL hkVector4Util::dot4_4vs4( const hkVector4& a0, const hkVector4& b0, const hkVector4& a1, const hkVector4& b1, const hkVector4& a2, const hkVector4& b2, const hkVector4& a3, const hkVector4& b3, hkVector4& dotsOut)
{
	dotsOut.set( a0.dot4( b0 ),
				 a1.dot4( b1 ),
				 a2.dot4( b2 ),
				 a3.dot4( b3 ) );
}
#endif

#ifndef HK_VECTOR4UTIL_dot3_1vs4
inline void HK_CALL hkVector4Util::dot3_1vs4( const hkVector4& vectorIn, const hkVector4& a0, const hkVector4& a1, const hkVector4& a2, const hkVector4& a3, hkVector4& dotsOut)
{
	dotsOut(0) = a0.dot3( vectorIn );
	dotsOut(1) = a1.dot3( vectorIn );
	dotsOut(2) = a2.dot3( vectorIn );
	dotsOut(3) = a3.dot3( vectorIn );
}
#endif

#ifndef HK_VECTOR4UTIL_dot3_2vs2
inline void HK_CALL hkVector4Util::dot3_2vs2( const hkVector4& a0, const hkVector4& a2, const hkVector4& b0, const hkVector4& b1, hkVector4& dotsOut)
{
	dotsOut(0) = a0.dot3( b0 );
	dotsOut(1) = a0.dot3( b1 );
	dotsOut(2) = a2.dot3( b0 );
	dotsOut(3) = a2.dot3( b1 );
}
#endif



#ifndef HK_VECTOR4UTIL_invertSymmetric
inline void HK_CALL hkVector4Util::invertSymmetric( const hkMatrix3& in, hkReal eps, hkMatrix3& out )
{
	hkVector4 r0; r0.setCross( in.getColumn(1), in.getColumn(2) );
    hkVector4 r1; r1.setCross( in.getColumn(2), in.getColumn(0) );
    hkVector4 r2; r2.setCross( in.getColumn(0), in.getColumn(1) );

	hkSimdReal eps3 = hkSimdReal(eps*eps*eps);
    hkSimdReal determinant = in.getColumn(0).dot3(r0);
	determinant.setMax( determinant, eps3 );

	hkSimdReal dinv = hkSimdReal::getConstant(HK_QUADREAL_1) / determinant;
	out.getColumn(0).setMul4(dinv, r0);
	out.getColumn(1).setMul4(dinv, r1);
	out.getColumn(2).setMul4(dinv, r2);
}
#endif

#ifndef HK_VECTOR4UTIL_exitMmx
inline void HK_CALL hkVector4Util::exitMmx()
{
}
#endif

#ifndef HK_VECTOR4UTIL_distToLineSquared
inline hkSimdReal HK_CALL hkVector4Util::distToLineSquared( const hkVector4& a, const hkVector4& b, const hkVector4& p )
{
	hkVector4 ab, ap;
	ab.setSub4( b, a );
	ap.setSub4( p, a );

	const hkSimdReal ab_ = ab.lengthSquared3();
	const hkSimdReal projectionAsPercentOfSegment = ap.dot3(ab)/ab_;
	ap.subMul4( projectionAsPercentOfSegment, ab );
	return ap.lengthSquared3();
}
#endif

HK_FORCE_INLINE void HK_CALL hkVector4Util::setMax44(const hkVector4& a, const hkVector4& b, const hkVector4& c, const hkVector4& d, hkVector4& out)
{
	hkVector4	t0; t0.setMax4(a,b);
	hkVector4	t1; t1.setMax4(c,d);
	out.setMax4(t0,t1);
}

HK_FORCE_INLINE void HK_CALL hkVector4Util::setMin44(const hkVector4& a, const hkVector4& b, const hkVector4& c, const hkVector4& d, hkVector4& out)
{
	hkVector4	t0; t0.setMin4(a,b);
	hkVector4	t1; t1.setMin4(c,d);
	out.setMin4(t0,t1);
}


#ifndef HK_VECTOR4UTIL_unPackInt32IntoQuaternion
inline void HK_CALL hkVector4Util::unPackInt32IntoQuaternion( hkUint32 ivalue, hkVector4& qout )
{
	int a = ivalue;
	int b = ivalue >> 8;
	int c = ivalue >> 16;
	int d = ivalue >> 24;
	a &= 0xff;
	b &= 0xff;
	c &= 0xff;
	a -= 0x80;
	b -= 0x80;
	c -= 0x80;
	d -= 0x80;
	const hkReal f = 1.1f/128.0f;
	qout(0) = f * a;
	qout(1) = f * b;
	qout(2) = f * c;
	qout(3) = f * d;
}
#endif

#ifndef HK_VECTOR4UTIL_compressExpandedAabbUint32
inline void HK_CALL hkVector4Util::compressExpandedAabbUint32(const hkAabbUint32& expandedAabbInt, hkAabbUint32& unexpandedAabbInt_InOut)
{
	hkInt32 mix = unexpandedAabbInt_InOut.m_min[0] - expandedAabbInt.m_min[0];
	hkInt32 miy = unexpandedAabbInt_InOut.m_min[1] - expandedAabbInt.m_min[1];
	hkInt32 miz = unexpandedAabbInt_InOut.m_min[2] - expandedAabbInt.m_min[2];
	hkInt32 maxx= expandedAabbInt.m_max[0] - unexpandedAabbInt_InOut.m_max[0];
	hkInt32 may = expandedAabbInt.m_max[1] - unexpandedAabbInt_InOut.m_max[1];
	hkInt32 maz = expandedAabbInt.m_max[2] - unexpandedAabbInt_InOut.m_max[2];

	hkInt32 m = (mix | miy) | (miz | maxx) | (may | maz);
	HK_ASSERT2( 0xf0ed3454, m >= 0, "Your expanded AABB is smaller than the unexpanded AABB" );
	int shift = 0;
	while ( m >=  0x800){ shift += 4; m = unsigned(m) >> 4; };
	while ( m >=  0x100){ shift += 1; m = unsigned(m) >> 1; };

	unexpandedAabbInt_InOut.m_expansionShift = hkUchar(shift);
	unexpandedAabbInt_InOut.m_expansionMin[0] = hkUchar(mix >> shift);
	unexpandedAabbInt_InOut.m_expansionMin[1] = hkUchar(miy >> shift);
	unexpandedAabbInt_InOut.m_expansionMin[2] = hkUchar(miz >> shift);
	unexpandedAabbInt_InOut.m_expansionMax[0] = hkUchar(maxx >> shift);
	unexpandedAabbInt_InOut.m_expansionMax[1] = hkUchar(may >> shift);
	unexpandedAabbInt_InOut.m_expansionMax[2] = hkUchar(maz >> shift);

}
#endif


#ifndef HK_VECTOR4UTIL_uncompressExpandedAabbUint32
inline void HK_CALL hkVector4Util::uncompressExpandedAabbUint32(const hkAabbUint32& unexpandedAabbInt, hkAabbUint32& expandedAabbOut)
{
	int shift = unexpandedAabbInt.m_expansionShift;
	hkUint32 mix = unexpandedAabbInt.m_min[0] - (unexpandedAabbInt.m_expansionMin[0] << shift);
	hkUint32 miy = unexpandedAabbInt.m_min[1] - (unexpandedAabbInt.m_expansionMin[1] << shift);
	hkUint32 miz = unexpandedAabbInt.m_min[2] - (unexpandedAabbInt.m_expansionMin[2] << shift);
	expandedAabbOut.m_min[0]= mix;
	expandedAabbOut.m_min[1]= miy;
	expandedAabbOut.m_min[2]= miz;
	hkUint32 maxx = unexpandedAabbInt.m_max[0] + (unexpandedAabbInt.m_expansionMax[0] << shift);
	hkUint32 may  = unexpandedAabbInt.m_max[1] + (unexpandedAabbInt.m_expansionMax[1] << shift);
	hkUint32 maz  = unexpandedAabbInt.m_max[2] + (unexpandedAabbInt.m_expansionMax[2] << shift);
	expandedAabbOut.m_max[0]= maxx;
	expandedAabbOut.m_max[1]= may;
	expandedAabbOut.m_max[2]= maz;
}
#endif

#ifndef HK_VECTOR4UTIL_memclear16
void hkVector4Util::memClear16( void *dest, int numQuads )
{
	hkVector4 val; val.setZero4();
	hkVector4 *d = (hkVector4 *)dest;
	while ( --numQuads >=0 )
	{
		*(d++) = val;
	}
}
#endif

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
