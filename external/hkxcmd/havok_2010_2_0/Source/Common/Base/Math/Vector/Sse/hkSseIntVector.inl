/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


//
// Clearing, loading, and storing
//

inline void hkIntVector::setZero()
{
	m_quad = _mm_setzero_si128();
}

inline void hkIntVector::setAll(int i)
{
	m_quad = _mm_set1_epi32(i);
}

inline void hkIntVector::set(int i)
{
	m_quad = _mm_cvtsi32_si128(i);
}

inline int hkIntVector::get() const
{
	return _mm_cvtsi128_si32(m_quad);
}

inline void hkIntVector::loadAligned1x(const hkUint32* p)
{
	HK_ASSERT2(0x19df6be3, ((hkUlong)p % 4) == 0, "pointer for hkIntVector::loadAligned1x must be 4-byte aligned");
	m_quad = _mm_loadu_si128((hkQuadUint*) p);
}

inline void hkIntVector::loadUnaligned4(const hkUint32* p)
{
//	HK_ASSERT2(0x217c2b3b, ((hkUlong)p & 3) == 0, "pointer for hkIntVector::loadUnaligned4 must be 4-byte aligned");
	m_quad = _mm_loadu_si128((hkQuadUint*) p);
}

inline void hkIntVector::storeUnaligned4(void* p)
{
//	HK_ASSERT2(0x217c2b3b, ((hkUlong)p & 3) == 0, "pointer for hkIntVector::loadUnaligned4 must be 4-byte aligned");
	_mm_storeu_si128((hkQuadUint*) p, m_quad);
}

//
// Logical operations
//

inline void hkIntVector::setNot(hkIntVectorParameter a)
{
	hkQuadUint allBits = _mm_set1_epi32(-1);
	m_quad = _mm_andnot_si128( a.m_quad, allBits); 
}

HK_FORCE_INLINE hkBool32 hkIntVector::isNegativeAssumingAllValuesEqual(  ) const
{
	return _mm_movemask_ps( (const __m128&)m_quad );
}

HK_FORCE_INLINE hkVector4Comparison hkIntVector::compareLessThanS32( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkVector4Comparison m;
	hkQuadUint ic = _mm_cmplt_epi32(a.m_quad, b.m_quad);
	hkQuadReal* fc = (hkQuadReal*)&ic;
	m.m_mask = *fc;
	return m;
}

inline void hkIntVector::setOr(hkIntVectorParameter a, hkIntVectorParameter b)
{
	m_quad = _mm_or_si128(a.m_quad, b.m_quad);
}

inline void hkIntVector::setAnd(hkIntVectorParameter a, hkIntVectorParameter b)
{
	m_quad = _mm_and_si128(a.m_quad, b.m_quad);
}

inline void hkIntVector::setXor(hkIntVectorParameter a, hkIntVectorParameter b)
{
	m_quad = _mm_xor_si128(a.m_quad, b.m_quad);
}

inline void hkIntVector::setAndC(hkIntVectorParameter a, hkIntVectorParameter b)
{
	m_quad = _mm_andnot_si128(b.m_quad, a.m_quad);
}

//
// Arithmetic operations
//

inline void hkIntVector::setSubU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_sub_epi16( a.m_quad, b.m_quad );
}

inline void hkIntVector::setAddU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_add_epi16( a.m_quad, b.m_quad );
}

inline void hkIntVector::setSubU32( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_sub_epi32( a.m_quad, b.m_quad );
}

inline void hkIntVector::setAddU32( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_add_epi32( a.m_quad, b.m_quad );
}

inline void hkIntVector::setAddSaturateU32( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadUintUnion qu, qa, qb;
	qa.q = a; qb.q = b;
	
	const hkUint32 max = hkUint32(-1);
	qu.u[0] = (qa.u[0] < max - qb.u[0]) ? (qa.u[0] + qb.u[0]) : max; 
	qu.u[1] = (qa.u[1] < max - qb.u[1]) ? (qa.u[1] + qb.u[1]) : max; 
	qu.u[2] = (qa.u[2] < max - qb.u[2]) ? (qa.u[2] + qb.u[2]) : max; 
	qu.u[3] = (qa.u[3] < max - qb.u[3]) ? (qa.u[3] + qb.u[3]) : max; 

	m_quad = qu.q;
}

inline void hkIntVector::setAddSaturateS16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_adds_epi16(a.m_quad, b.m_quad);
}

inline void hkIntVector::setAddSaturateU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_adds_epu16(a.m_quad, b.m_quad);
}

//inline void hkIntVector::setAddS32( hkIntVectorParameter a, hkIntVectorParameter b )
//{
//	m_quad = _mm_add_epi32(a.m_quad, b.m_quad);
//}


inline void hkIntVector::setSubSaturateU32( hkIntVectorParameter a, hkIntVectorParameter b )
{
	// There are no unsigned 32-bit compare, min/max, or add/subtract instructions (with or without saturation) in SSE2.
	hkQuadUintUnion qu, qa, qb;
	qa.q = a; qb.q = b;

	qu.u[0] = (qa.u[0] > qb.u[0]) ? (qa.u[0] - qb.u[0]) : 0; 
	qu.u[1] = (qa.u[1] > qb.u[1]) ? (qa.u[1] - qb.u[1]) : 0; 
	qu.u[2] = (qa.u[2] > qb.u[2]) ? (qa.u[2] - qb.u[2]) : 0; 
	qu.u[3] = (qa.u[3] > qb.u[3]) ? (qa.u[3] - qb.u[3]) : 0; 

	m_quad = qu.q;
}

inline void hkIntVector::setSubSaturateS16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_subs_epi16(a.m_quad, b.m_quad);
}

inline void hkIntVector::setSubSaturateU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_subs_epu16(a.m_quad, b.m_quad);
}

inline void hkIntVector::setMinU8( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_min_epu8 (a.m_quad, b.m_quad);
}


// inline void hkIntVector::setMinU16( hkIntVectorParameter a, hkIntVectorParameter b )
// {
// #if HK_SSE_VERSION > 0x20
// 	m_quad = _mm_min_epu16( a.m_quad, b.m_quad );	// this is sse4
// #else
// 	hkUint16* ptr = (hkUint16*)&m_quad;
// 	for(int i=0; i<8; i++)
// 	{
// 		ptr[i] = hkMath::min2( a.getU16(i), b.getU16(i) );
// 	}
// #endif
// }
// 
// inline void hkIntVector::setMaxU16( hkIntVectorParameter a, hkIntVectorParameter b )
// {
// #if HK_SSE_VERSION > 0x20
// 	m_quad = _mm_max_epu16( a.m_quad, b.m_quad );
// #else
// 	hkUint16* ptr = (hkUint16*)&m_quad;
// 	for(int i=0; i<8; i++)
// 	{
// 		ptr[i] = hkMath::max2( a.getU16(i), b.getU16(i) );
// 	}
// #endif
// }

//inline void hkIntVector::setSubS32( hkIntVectorParameter a, hkIntVectorParameter b )
//{
//	m_quad = _mm_sub_epi32(a.m_quad, b.m_quad);
//}

//
// Shift operations
//

template<int shift> inline void hkIntVector::setShiftLeft16( hkIntVectorParameter a)
{
	m_quad =  _mm_slli_epi16( a.m_quad, shift);
}

template<int shift> inline void hkIntVector::setShiftRight16( hkIntVectorParameter a)
{
	m_quad =  _mm_srli_epi16( a.m_quad, shift);
}

template<int shift> inline void hkIntVector::setShiftLeft32( hkIntVectorParameter a)
{
	m_quad =  _mm_slli_epi32( a.m_quad, shift);
}

template<int shift> inline void hkIntVector::setShiftRight32( hkIntVectorParameter a)
{
	m_quad =  _mm_srli_epi32( a.m_quad, shift);	
}

// x, y = high, z, w = low

template<int shift> inline void hkIntVector::setShiftLeft128( hkIntVectorParameter a)
{
	m_quad =  _mm_srli_si128( a.m_quad, shift);
}

template<int shift> inline void hkIntVector::setShiftRight128( hkIntVectorParameter a)
{
	m_quad =  _mm_slli_si128( a.m_quad, shift);
}

// inline void hkIntVector::setShiftLeft128( hkIntVectorParameter a, int shift)
// {
// 	m_quad =  _mm_srli_si128( a.m_quad, shift);
// }
// 
// inline void hkIntVector::setShiftRight128( hkIntVectorParameter a, int shift)
// {
// 	m_quad =  _mm_slli_si128( a.m_quad, shift);
// }

inline void hkIntVector::setShiftLeft32( hkIntVectorParameter a, hkIntVectorParameter shift )
{
	
	// Not sure if it's faster to do this in scalar, or do a bunch of shuffles and 4 shifts

	hkQuadUintUnion qu, qa, qshift;
	qa.q = a;
	qshift.q = shift;

	qu.u[0] = qa.u[0] << qshift.u[0];
	qu.u[1] = qa.u[1] << qshift.u[1];
	qu.u[2] = qa.u[2] << qshift.u[2];
	qu.u[3] = qa.u[3] << qshift.u[3];

	m_quad = qu.q;
	
	/*m_quad = _mm_sll_epi32(a.m_quad, shift.m_quad);*/
}

inline void hkIntVector::setShiftRight32( hkIntVectorParameter a, hkIntVectorParameter shift )
{
	
	hkQuadUintUnion qu, qa, qshift;
	qa.q = a;
	qshift.q = shift;

	qu.u[0] = qa.u[0] >> qshift.u[0];
	qu.u[1] = qa.u[1] >> qshift.u[1];
	qu.u[2] = qa.u[2] >> qshift.u[2];
	qu.u[3] = qa.u[3] >> qshift.u[3];

	m_quad = qu.q;

	//m_quad = _mm_srl_epi32(a.m_quad, shift.m_quad);
}

inline void hkIntVector::setShiftLeft16( hkIntVectorParameter a, int shift )
{
	m_quad = _mm_slli_epi16( a.m_quad, shift );
}

inline void hkIntVector::setShiftRight16( hkIntVectorParameter a, int shift )
{
	m_quad = _mm_srai_epi16( a.m_quad, shift );
}

//
// Merge operations
// Stick to the convention that "high" means x,y, "low" means z,w
//
inline void hkIntVector::setMergeHead32(hkIntVectorParameter a, hkIntVectorParameter b)
{
	m_quad = _mm_unpacklo_epi32(a.m_quad, b.m_quad);
}

inline void hkIntVector::setMergeTail32(hkIntVectorParameter a, hkIntVectorParameter b)
{
	m_quad = _mm_unpackhi_epi32(a.m_quad, b.m_quad);
}

inline void hkIntVector::setMergeHead16(hkIntVectorParameter a, hkIntVectorParameter b)
{

	m_quad = _mm_unpacklo_epi16(a.m_quad, b.m_quad);
}

inline void hkIntVector::setMergeTail16(hkIntVectorParameter a, hkIntVectorParameter b)
{

	m_quad = _mm_unpackhi_epi16(a.m_quad, b.m_quad);
}

inline void hkIntVector::setMergeHead8(hkIntVectorParameter a, hkIntVectorParameter b)
{
	m_quad = _mm_unpacklo_epi8(a.m_quad, b.m_quad);
}

inline void hkIntVector::setMergeTail8(hkIntVectorParameter a, hkIntVectorParameter b)
{
	m_quad = _mm_unpackhi_epi8(a.m_quad, b.m_quad);
}


//
// Pack operations
//
inline void hkIntVector::setConvertU32ToU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadUint a0 = _mm_slli_epi32(a.m_quad, 16);	// moduloU16 by sign extending bit 15
	hkQuadUint b0 = _mm_slli_epi32(b.m_quad, 16);

	a0 = _mm_srai_epi32( a0, 16 );
	b0 = _mm_srai_epi32( b0, 16 );
	m_quad = _mm_packs_epi32(a0, b0);
}

inline void hkIntVector::setConvertSaturateS32ToU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	static HK_ALIGN16( const hkUint32 offset[4] ) = { 0x8000, 0x8000, 0x8000, 0x8000 };
	static HK_ALIGN16( const hkUint16 offset2[8] ) = { 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000 };

	hkQuadUint a0 = _mm_sub_epi32(a.m_quad, (const hkQuadUint&)offset);		// shift 0 to -max
	hkQuadUint b0 = _mm_sub_epi32(b.m_quad, (const hkQuadUint&)offset);
	hkQuadUint r  = _mm_packs_epi32(a0, b0);	// signed saturate
	m_quad = _mm_add_epi16( r, (const hkQuadUint&)offset2 );	// revert shift
}

inline void hkIntVector::setConvertSaturateS16ToU8( hkIntVectorParameter a, hkIntVectorParameter b )
{
	m_quad = _mm_packus_epi16(a.m_quad, b.m_quad);
}

//
// Shuffle
//
template<int A, int B, int C, int D> HK_FORCE_INLINE void hkIntVector::setShuffle( hkIntVectorParameter a )
{
	// Shuffle order is backwards of what one might expect!
	m_quad = _mm_shuffle_epi32( a.m_quad, _MM_SHUFFLE(D,C,B,A) );
}

inline void hkIntVector::setPermuteU8(hkIntVectorParameter a, hkIntVectorParameter mask)
{
	union Quad { hkQuadUint q; hkUint8 u8[16]; };
	Quad qv; qv.q = a.m_quad; Quad qm; qm.q = mask.m_quad;
	m_quad = _mm_set_epi8(	qv.u8[qm.u8[15]],
				qv.u8[qm.u8[14]],
				qv.u8[qm.u8[13]],
				qv.u8[qm.u8[12]],
				qv.u8[qm.u8[11]],
				qv.u8[qm.u8[10]],
				qv.u8[qm.u8[9]],
				qv.u8[qm.u8[8]],
				qv.u8[qm.u8[7]],
				qv.u8[qm.u8[6]],
				qv.u8[qm.u8[5]],
				qv.u8[qm.u8[4]],
				qv.u8[qm.u8[3]],
				qv.u8[qm.u8[2]],
				qv.u8[qm.u8[1]],
				qv.u8[qm.u8[0]]);	
}

inline void hkIntVector::setPermuteU8(hkIntVectorParameter a, hkIntVectorParameter b, hkIntVectorParameter mask)
{
	const char* v[2] = {reinterpret_cast<const char*>(&a), reinterpret_cast<const char*>(&b)};

	union Quad { hkQuadUint q; hkUint8 u8[16]; };
	Quad qidx; qidx.q = mask.m_quad;

	m_quad = _mm_set_epi8(	v[qidx.u8[15] >> 4][qidx.u8[15] & 0xF],
							v[qidx.u8[14] >> 4][qidx.u8[14] & 0xF],
							v[qidx.u8[13] >> 4][qidx.u8[13] & 0xF],
							v[qidx.u8[12] >> 4][qidx.u8[12] & 0xF],
							v[qidx.u8[11] >> 4][qidx.u8[11] & 0xF],
							v[qidx.u8[10] >> 4][qidx.u8[10] & 0xF],
							v[qidx.u8[9] >> 4][qidx.u8[9] & 0xF],
							v[qidx.u8[8] >> 4][qidx.u8[8] & 0xF],
							v[qidx.u8[7] >> 4][qidx.u8[7] & 0xF],
							v[qidx.u8[6] >> 4][qidx.u8[6] & 0xF],
							v[qidx.u8[5] >> 4][qidx.u8[5] & 0xF],
							v[qidx.u8[4] >> 4][qidx.u8[4] & 0xF],
							v[qidx.u8[3] >> 4][qidx.u8[3] & 0xF],
							v[qidx.u8[2] >> 4][qidx.u8[2] & 0xF],
							v[qidx.u8[1] >> 4][qidx.u8[1] & 0xF],
							v[qidx.u8[0] >> 4][qidx.u8[0] & 0xF]);
}

//
// Splat
//
template<int val> inline void hkIntVector::splatImmediate32()
{
	// We don't need these for SSE, but they're to make sure that PPC code is compatible
	HK_COMPILE_TIME_ASSERT( val <=  15 );
	HK_COMPILE_TIME_ASSERT( val >= -16 );

	m_quad = _mm_set1_epi32(val);
}

template<int val> inline void hkIntVector::splatImmediate16()
{
	HK_COMPILE_TIME_ASSERT( val <=  15 );
	HK_COMPILE_TIME_ASSERT( val >= -16 );

	m_quad = _mm_set1_epi16(val);
}

template<int val> inline void hkIntVector::splatImmediate8()
{
	HK_ASSERT2(0x3be5dcf3, val >= -16 && val <= 15, "Invalid value for hkIntVector::splatImmediate8.");

	m_quad = _mm_set1_epi8(val);
}

template<int idx> inline void hkIntVector::setBroadcast(hkIntVectorParameter a)
{
	m_quad = _mm_shuffle_epi32( a.m_quad, _MM_SHUFFLE(idx, idx, idx, idx) );
}


//
// int <-> float conversion
//

// from Apple's Introduction to AltiVec/SSE Migration Guide
// http://developer.apple.com/documentation/Performance/Conceptual/Accelerate_sse_migration/
inline void hkIntVector::convertU32ToF32(hkVector4& vOut) const
{
	const hkQuadReal two16 = HK_QUADREAL_CONSTANT(1.0e16f, 1.0e16f, 1.0e16f, 1.0e16f);

	//Convert vUInt32 to vFloat according to the current rounding mode
	hkQuadUint v = m_quad;

	{
		// Avoid double rounding by doing two exact conversions
		// of high and low 16-bit segments

		hkQuadUint hi = _mm_srli_epi32( v, 16 );
		hkQuadUint lo = _mm_srli_epi32( _mm_slli_epi32(  v, 16 ), 16 );
		hkQuadReal fHi = _mm_mul_ps( _mm_cvtepi32_ps( hi ), two16);
		hkQuadReal fLo = _mm_cvtepi32_ps( lo );

		// do single rounding according to current rounding mode
		// note that AltiVec always uses round to nearest. We use current
		// rounding mode here, which is round to nearest by default.
		vOut.getQuad() =  _mm_add_ps( fHi, fLo );
	}
}

inline void hkIntVector::convertS32ToF32(hkVector4& vOut) const
{
	vOut.getQuad() = _mm_cvtepi32_ps(m_quad);
}

inline void hkIntVector::setConvertF32toU32(hkVector4Parameter vIn)
{
	m_quad = _mm_cvtps_epi32( vIn.getQuad() ); 
}

inline void hkIntVector::setConvertF32toS32(hkVector4Parameter vIn)
{
	m_quad = _mm_cvttps_epi32( vIn.getQuad() ); 
}

// The following functions are valid for all platforms (there wont' be many of these)
inline void hkIntVector::loadAligned(const hkUint32* p)
{
	HK_ASSERT2(0x70aae483, ((hkUlong)p & 0xf) == 0, "pointer for hkIntVector::loadAligned must be 16-byte aligned");
	m_quad = *reinterpret_cast<const hkQuadUint*>(p);
}

inline hkQuadUint& hkIntVector::getQuad()
{
	return m_quad;
}

inline const hkQuadUint& hkIntVector::getQuad() const
{
	return m_quad;
}


//

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
