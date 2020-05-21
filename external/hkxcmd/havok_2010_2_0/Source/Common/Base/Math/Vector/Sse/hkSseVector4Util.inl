/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#define HK_VECTOR4UTIL_mulSigns4
inline void HK_CALL hkVector4Util::mulSigns4( hkVector4& inout, const hkVector4& signs)
{
	static HK_ALIGN16( const hkUint32 signmask[4] ) =
		{ 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
	inout.getQuad() = _mm_xor_ps( inout.getQuad(),
							_mm_and_ps(signs.getQuad(), *(const hkQuadReal*)&signmask));
}

#define HK_VECTOR4UTIL_rotateInversePoints
inline void	HK_CALL hkVector4Util::rotateInversePoints( const hkRotation& r, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut )
{
	checkRegisters( 0x70000000 );		

	hkQuadReal c0 = r.getColumn(0).getQuad();
	hkQuadReal c1 = r.getColumn(1).getQuad();
	hkQuadReal c2 = r.getColumn(2).getQuad();
	HK_TRANSPOSE3(c0,c1,c2);

	HK_ASSERT2( 0xf0200104, numVectors > 0, "At least one element required");
	for( int i = 0; i < numVectors; ++i )
	{
		hkQuadReal b = vectorsIn[i].getQuad();
		const hkQuadReal b0 = _mm_shuffle_ps( b, b, _MM_SHUFFLE(0,0,0,0));
		const hkQuadReal r0 = _mm_mul_ps( c0, b0 );
		const hkQuadReal b1 = _mm_shuffle_ps( b, b, _MM_SHUFFLE(1,1,1,1));
		const hkQuadReal r1 = _mm_mul_ps( c1, b1 );
		const hkQuadReal b2 = _mm_shuffle_ps( b, b, _MM_SHUFFLE(2,2,2,2));
		const hkQuadReal r2 = _mm_mul_ps( c2, b2 );
		vectorsOut[i].getQuad() = _mm_add_ps( _mm_add_ps(r0, r1), r2 );
	}
}

#define HK_VECTOR4UTIL_exitMmx
inline void HK_CALL hkVector4Util::exitMmx()
{
#	ifndef HK_ARCH_X64
	_mm_empty();
#	endif
}

#define HK_VECTOR4UTIL_dot3_3vs3
inline void HK_CALL hkVector4Util::dot3_3vs3(const hkVector4& a0, const hkVector4& b0, const hkVector4& a1, const hkVector4& b1, const hkVector4& a2, const hkVector4& b2, hkVector4& dotsOut)
{
	register hkQuadReal c0 = _mm_mul_ps( a0.getQuad(), b0.getQuad());
	register hkQuadReal c1 = _mm_mul_ps( a1.getQuad(), b1.getQuad());
	register hkQuadReal c2 = _mm_mul_ps( a2.getQuad(), b2.getQuad());

	HK_TRANSPOSE3(c0,c1,c2);

	dotsOut.getQuad() = _mm_add_ps( _mm_add_ps(c0, c1), c2 );
}

#define HK_VECTOR4UTIL_dot3_4vs4
inline void HK_CALL hkVector4Util::dot3_4vs4(const hkVector4& a0, const hkVector4& b0, const hkVector4& a1, const hkVector4& b1, const hkVector4& a2, const hkVector4& b2, const hkVector4& a3, const hkVector4& b3, hkVector4& dotsOut)
{
	register hkQuadReal c0 = _mm_mul_ps( a0.getQuad(), b0.getQuad());
	register hkQuadReal c1 = _mm_mul_ps( a1.getQuad(), b1.getQuad());
	register hkQuadReal c2 = _mm_mul_ps( a2.getQuad(), b2.getQuad());
	register hkQuadReal c3 = _mm_mul_ps( a3.getQuad(), b3.getQuad());

	HK_TRANSPOSE4(c0,c1,c2,c3);
	dotsOut.getQuad() = _mm_add_ps( _mm_add_ps(c0, c1), c2 );
}

#define HK_VECTOR4UTIL_dot3_1vs4
inline void HK_CALL hkVector4Util::dot3_1vs4( const hkVector4& vectorIn, const hkVector4& a0, const hkVector4& a1, const hkVector4& a2, const hkVector4& a3, hkVector4& dotsOut)
{
	dotsOut.setDot3_1vs4(vectorIn, a0, a1, a2, a3);
}

#define HK_VECTOR4UTIL_dot3_2vs2
inline void HK_CALL hkVector4Util::dot3_2vs2( const hkVector4& a0, const hkVector4& a2, const hkVector4& b0, const hkVector4& b1, hkVector4& dotsOut)
{
	register hkQuadReal c0 = _mm_mul_ps( a0.getQuad(), b0.getQuad());
	register hkQuadReal c1 = _mm_mul_ps( a0.getQuad(), b1.getQuad());
	register hkQuadReal c2 = _mm_mul_ps( a2.getQuad(), b0.getQuad());
	register hkQuadReal c3 = _mm_mul_ps( a2.getQuad(), b1.getQuad());

	_MM_TRANSPOSE4_PS(c0,c1,c2,c3);

	dotsOut.getQuad() = _mm_add_ps( _mm_add_ps(c0, c1), c2 );
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
