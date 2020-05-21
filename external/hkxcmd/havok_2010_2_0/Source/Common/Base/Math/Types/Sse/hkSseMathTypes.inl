/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <xmmintrin.h>

#if HK_SSE_VERSION > 0x20
#	include <emmintrin.h>	
#endif

#define HK_TRANSPOSE4(A,B,C,D) _MM_TRANSPOSE4_PS(A,B,C,D)

#define HK_TRANSPOSE3(_a,_b,_c) { \
	hkQuadReal _tmp, _ctmp;	\
	_tmp = _mm_unpacklo_ps(_a,_b);	\
	HK_VECTOR4_SHUF(_ctmp,_mm_unpackhi_ps(_a,_b),_c,HK_VECTOR4_SHUFFLE(0,1,2,3));	\
	_a = _mm_movelh_ps(_tmp,_c);	\
	HK_VECTOR4_SHUF(_b,_mm_movehl_ps(_a,_tmp),_c,HK_VECTOR4_SHUFFLE(0,1,1,3));	\
	_c = _ctmp; }

#define HK_VECTOR4_SHUFFLE(_a,_b,_c,_d) _MM_SHUFFLE(_d,_c,_b,_a)
#define HK_VECTOR4_SHUF( tgt, src0, src1, shuf ) tgt = _mm_shuffle_ps(src0, src1, shuf)

#define HK_VECTOR4_PERM1ARG(_a,_b,_c,_d) _MM_SHUFFLE(_d,_c,_b,_a)
#define HK_VECTOR4_PERM1( tgt, src, shuf ) tgt = _mm_shuffle_ps(src, src, shuf)

typedef __m128 hkQuadReal;
extern const hkQuadReal g_vectorConstants[HK_QUADREAL_END];

class hkVector4;
typedef const hkVector4& hkVector4Parameter;
class hkVector4Comparison;
typedef const hkVector4Comparison& hkVector4ComparisonParameter;

union hkQuadRealUnion
{
	hkReal r[4];
	hkQuadReal q;
};

#define HK_QUADREAL_CONSTANT(a,b,c,d) {a,b,c,d}

class hkSimdReal;
typedef const hkSimdReal& hkSimdRealParameter;

class hkSimdReal
{
	public:
		HK_FORCE_INLINE hkSimdReal(const hkQuadReal& x)
		{
			m_real = _mm_shuffle_ps(x, x, 0);
		}

		HK_FORCE_INLINE hkSimdReal(){}

		HK_FORCE_INLINE hkReal getReal() const
		{
			hkReal s;
			_mm_store_ss(&s, m_real);
			return s;
		}

		static HK_FORCE_INLINE const hkSimdReal& HK_CALL getConstant(hkVectorConstant constant)
		{
			return *(const hkSimdReal*) (g_vectorConstants + constant);
		}
#if defined(HK_DISABLE_SIMD_REAL_FLOAT_CONVERSION)
		HK_FORCE_INLINE explicit hkSimdReal(const hkReal& x)
		{
			m_real = _mm_set_ps1(x);
		}

#else
		HK_FORCE_INLINE hkSimdReal(const hkReal& x)
		{
			m_real = _mm_set_ps1(x);
		}


		HK_FORCE_INLINE operator hkReal() const
		{
			hkReal s;
			_mm_store_ss(&s, m_real);
			return s;
		}
#endif
		/// convert a signed integer (note that conversion from unsigned integer is really slow on SSE
		HK_FORCE_INLINE explicit hkSimdReal(int&  x)
		{
			hkQuadReal q = _mm_cvtepi32_ps(_mm_cvtsi32_si128( x ));
			m_real = _mm_shuffle_ps(q, q, 0);
		} 

		HK_FORCE_INLINE explicit hkSimdReal(hkUint16& x)
		{
			hkQuadReal q = _mm_cvtepi32_ps(_mm_cvtsi32_si128( int(x) ));
			m_real = _mm_shuffle_ps(q, q, 0);
		}

		HK_FORCE_INLINE explicit hkSimdReal(hkUint8& x)
		{
			hkQuadReal q = _mm_cvtepi32_ps(_mm_cvtsi32_si128( int(x) ));
			m_real = _mm_shuffle_ps(q, q, 0);
		}

		HK_FORCE_INLINE void convertToSaturateInt32( int& result )
		{
			static HK_ALIGN16( const float maxValue[4] ) = { float(0x7fffff00), float(0x7fffff00), float(0x7fffff00), float(0x7fffff00) };
			__m128i qi = _mm_cvtps_epi32( _mm_min_ps( m_real, (const hkQuadReal&) maxValue ) );
			int i = _mm_cvtsi128_si32 (qi);
			result = i;
		}

		HK_FORCE_INLINE void convertToSaturateUInt16( hkUint16& result )
		{
			static HK_ALIGN16( const hkUint32 offset[4] ) = { 0x8000, 0x8000, 0x8000, 0x8000 };
			//static HK_ALIGN16( const hkUint16 offset2[8] ) = { 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff };

			__m128i qi = _mm_cvtps_epi32( m_real );
			qi = _mm_sub_epi32(qi, (const __m128i&)offset);		// shift 0 to -max
			__m128i q16 = _mm_packs_epi32( qi, qi );
			q16 = _mm_add_epi16( q16, (const __m128i&)offset );	// revert shift
			int i = _mm_cvtsi128_si32 (q16);
			result = hkUint16(i);
		}


		HK_FORCE_INLINE hkQuadReal broadcast() const
		{
			return _mm_shuffle_ps(m_real, m_real, 0);
		}

		HK_FORCE_INLINE hkQuadReal& getQuad()
		{
			return m_real;
		}

		HK_FORCE_INLINE const hkQuadReal& getQuad() const
		{
			return m_real;
		}


		HK_FORCE_INLINE void setMax(  hkSimdRealParameter a, hkSimdRealParameter b ) 
		{
			*this = hkSimdReal(_mm_max_ps( a.getQuad(), b.getQuad() ));
		}

		HK_FORCE_INLINE void setMin(  hkSimdRealParameter a, hkSimdRealParameter b ) 
		{
			*this = hkSimdReal(_mm_min_ps( a.getQuad(), b.getQuad() ));
		}

		HK_FORCE_INLINE void setSelect( hkVector4ComparisonParameter comp, hkSimdRealParameter trueValue, hkSimdRealParameter falseValue );

			/// Returns the comparison mask from this >= a
		HK_FORCE_INLINE hkVector4Comparison compareGreaterThanEqual(hkSimdRealParameter a) const;

			/// Returns the comparison mask from this > a
		HK_FORCE_INLINE hkVector4Comparison compareGreaterThan(hkSimdRealParameter a) const;

		/// Returns the comparison mask from this <= a
		HK_FORCE_INLINE hkVector4Comparison compareLessThanEqual(hkSimdRealParameter a) const;

			/// Returns the comparison mask from this < a
		HK_FORCE_INLINE hkVector4Comparison compareLessThan(hkSimdRealParameter a) const;

			/// Returns the comparison mask from this == a
		HK_FORCE_INLINE hkVector4Comparison compareEqual(hkSimdRealParameter a) const;

			/// Sets this = approx(1 / a)
		HK_FORCE_INLINE void setReciprocal(hkSimdRealParameter a);

			/// Sets this *= sign(a) 
		HK_FORCE_INLINE void mulSigns(hkSimdRealParameter a);

// 		HK_FORCE_INLINE void setOr(  hkSimdRealParameter a, hkSimdRealParameter b ) 
// 		{
// 			*this = hkSimdReal(_mm_or_ps( a.getQuad(), b.getQuad() ));
// 		}
// 
// 		HK_FORCE_INLINE void setXor(  hkSimdRealParameter a, hkSimdRealParameter b ) 
// 		{
// 			*this = hkSimdReal(_mm_xor_ps( a.getQuad(), b.getQuad() ));
// 		}

		HK_FORCE_INLINE void store(  float *f ) const
		{
			_mm_store_ss( f, m_real );
		}


	private:

		hkQuadReal m_real;
};



inline hkSimdReal HK_CALL operator* (hkSimdRealParameter r, hkSimdRealParameter s)
{
	return _mm_mul_ps(r.getQuad(),s.getQuad());
}

inline hkSimdReal HK_CALL operator- (hkSimdRealParameter r, hkSimdRealParameter s)
{
	return _mm_sub_ps(r.getQuad(),s.getQuad());
}

inline hkSimdReal HK_CALL operator+ (hkSimdRealParameter r, hkSimdRealParameter s)
{
	return _mm_add_ps(r.getQuad(),s.getQuad());
}

inline hkSimdReal HK_CALL operator/ (hkSimdRealParameter r, hkSimdRealParameter s)
{
	return _mm_div_ps(r.getQuad(),s.getQuad());
}

inline hkSimdReal HK_CALL operator- (hkSimdRealParameter r)
{
    extern const hkQuadReal hkQuadReal0000;
	return _mm_sub_ps(hkQuadReal0000,r.getQuad());
}

	/// Result of a hkVector4 comparison.
class hkVector4Comparison;
typedef const hkVector4Comparison& hkVector4ComparisonParameter;

class hkMaskStorage
{
public:
	int getMask(){ return m_mask; }
public:
	int m_mask;

};

class hkVector4Comparison
{
	public:

		enum Mask
		{
			MASK_NONE	= 0,
			MASK_X		= 1,
			MASK_Y		= 2,
			MASK_XY		= 3,

			MASK_Z		= 4,
			MASK_XZ		= 5,
			MASK_YZ		= 6,
			MASK_XYZ	= 7,

			MASK_W		= 8,
			MASK_XW		= 9,
			MASK_YW		= 10,
			MASK_XYW	= 11,

			MASK_ZW		= 12,
			MASK_XZW	= 13,
			MASK_YZW	= 14,
			MASK_XYZW	= 15
		};

		static const Mask s_components[4];

		HK_FORCE_INLINE void setAnd( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b )	{ m_mask = _mm_and_ps( a.m_mask,b.m_mask ); }
		HK_FORCE_INLINE void setAndNot(hkVector4ComparisonParameter a, hkVector4ComparisonParameter b)	{ m_mask = _mm_andnot_ps(b.m_mask, a.m_mask); }
		HK_FORCE_INLINE void setOr( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b ) { m_mask = _mm_or_ps( a.m_mask,b.m_mask ); }
		HK_FORCE_INLINE void setNot( hkVector4ComparisonParameter a )
		{ 
			static HK_ALIGN16( const hkUint32 allBits[4] ) = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };		
			m_mask = _mm_andnot_ps( a.m_mask, *(const hkQuadReal*)&allBits); 
		}

		HK_FORCE_INLINE void setSelect( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b, hkVector4ComparisonParameter comp) 
		{
			m_mask = _mm_or_ps( _mm_and_ps(comp.m_mask, b.m_mask), _mm_andnot_ps(comp.m_mask, a.m_mask) );
		}

		static const hkQuadReal s_maskFromBits[MASK_XYZW+1];
		static const hkQuadReal s_invMaskFromBits[MASK_XYZW+1];

		HK_FORCE_INLINE void set( Mask m) { m_mask = s_maskFromBits[m]; }
		HK_FORCE_INLINE void operator=(hkVector4ComparisonParameter rhs) { m_mask = rhs.m_mask; }

		HK_FORCE_INLINE hkBool32 allAreSet( Mask m ) const { return (_mm_movemask_ps(m_mask) & m) == m; }
		HK_FORCE_INLINE hkBool32 anyIsSet( Mask m ) const { return _mm_movemask_ps(m_mask) & m; }

		HK_FORCE_INLINE hkBool32 allAreSet() const { return _mm_movemask_ps(m_mask) == MASK_XYZW; }
		HK_FORCE_INLINE hkBool32 anyIsSet() const { return _mm_movemask_ps(m_mask); }

		HK_FORCE_INLINE int getMask() const { return _mm_movemask_ps(m_mask); }
		HK_FORCE_INLINE int getMask(Mask m) const { return _mm_movemask_ps(m_mask) & m; }
		HK_FORCE_INLINE void getMask(hkMaskStorage&  m) const { m.m_mask = getMask(); }
		static HK_FORCE_INLINE int HK_CALL getCombinedMask(hkVector4Comparison& ca, hkVector4Comparison& cb, hkVector4Comparison& cc )
		{
			return _mm_movemask_ps(ca.m_mask) | (_mm_movemask_ps(cb.m_mask)<<4 ) | (_mm_movemask_ps(cc.m_mask)<<8);
		}

	private:
		friend class hkVector4;
		friend class hkMxMask4;
		friend class hkSimdReal;
		friend class hkIntVector;

		hkQuadReal m_mask;
};

#define HK_SIMD_COMPARE_MASK_X 1

extern const hkQuadReal hkQuadRealHalf;
extern const hkQuadReal hkQuadReal3333;

namespace hkMath
{

#define HK_MATH_isNegative
	inline int HK_CALL isNegative(const hkSimdReal& r0)
	{
		return _mm_movemask_ps(r0.getQuad()) & hkVector4Comparison::MASK_X;
	}

	inline hkSimdReal HK_CALL sqrt(hkSimdRealParameter r)
	{
		return _mm_sqrt_ps(r.getQuad());
	}

#	if defined(HK_ARCH_IA32) && !defined(HK_COMPILER_GCC)
#	define HK_MATH_hkToIntFast
		// Fast rounding, however last bit might be wrong
	inline int HK_CALL hkToIntFast( hkReal r ){
		int i;
		_asm {
			fld r
			fistp i
		}
		return i;
	}
#endif

#	define HK_MATH_prefetch128
	inline void prefetch128( const void* p)
	{
		_mm_prefetch( (const char*)p, _MM_HINT_NTA );
	}

#	define HK_MATH_forcePrefetch
	template<int SIZE>
	inline void forcePrefetch( const void* p )
	{
		const char* q = (const char*)p;
		_mm_prefetch( q, _MM_HINT_NTA );
		if ( SIZE > 64){  _mm_prefetch( q + 64, _MM_HINT_NTA ); }
		if ( SIZE > 128){ _mm_prefetch( q + 128, _MM_HINT_NTA ); }
		if ( SIZE > 192){ _mm_prefetch( q + 192, _MM_HINT_NTA ); }
	}

    inline hkQuadReal quadReciprocal( hkQuadReal r )
    {
		hkQuadReal e = _mm_rcp_ps( r );
		//One round of Newton-Raphson refinement
        return _mm_sub_ps(_mm_add_ps(e,e), _mm_mul_ps(_mm_mul_ps(e, r), e));
    }

    inline hkQuadReal quadReciprocalSquareRoot( hkQuadReal r )
    {
		hkQuadReal e = _mm_rsqrt_ps(r);
		hkQuadReal he = _mm_mul_ps(hkQuadRealHalf,e);
		hkQuadReal ree = _mm_mul_ps(_mm_mul_ps(r,e),e);
		return _mm_mul_ps(he, _mm_sub_ps(hkQuadReal3333, ree) );
    }
}

HK_FORCE_INLINE void hkSimdReal::setSelect( hkVector4ComparisonParameter comp, hkSimdRealParameter trueValue, hkSimdRealParameter falseValue )
{
	m_real = _mm_or_ps( _mm_and_ps(comp.m_mask, trueValue.m_real), _mm_andnot_ps(comp.m_mask, falseValue.m_real) );
}

//
//	Returns the comparison mask from this >= a

inline hkVector4Comparison hkSimdReal::compareGreaterThanEqual(hkSimdRealParameter a) const
{
	return a.compareLessThanEqual(*this);
}

//
//	Returns the comparison mask from this > a

inline hkVector4Comparison hkSimdReal::compareGreaterThan(hkSimdRealParameter a) const
{
	return a.compareLessThan(*this);
}

//
//	Returns the comparison mask from this <= a

inline hkVector4Comparison hkSimdReal::compareLessThanEqual(hkSimdRealParameter a) const
{
	hkVector4Comparison m; m.m_mask =_mm_cmple_ps(m_real, a.m_real);
	return m;
}

//
//	Returns the comparison mask from this < a

inline hkVector4Comparison hkSimdReal::compareLessThan(hkSimdRealParameter a) const
{
	hkVector4Comparison m; m.m_mask =_mm_cmplt_ps(m_real, a.m_real);
	return m;
}

//
//	Returns the comparison mask from this == a

inline hkVector4Comparison hkSimdReal::compareEqual(hkSimdRealParameter a) const
{
	hkVector4Comparison m; m.m_mask =_mm_cmpeq_ps(m_real, a.m_real);
	return m;
}

//
//	Sets this = approx(1 / a)

inline void hkSimdReal::setReciprocal(hkSimdRealParameter a)
{
	m_real = hkMath::quadReciprocal(a.m_real);
}

//
//	Sets this *= sign(a) 

inline void hkSimdReal::mulSigns(hkSimdRealParameter a)
{
	static HK_ALIGN16( const hkUint32 signmask[4] ) = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
	m_real = _mm_xor_ps(m_real, _mm_and_ps(a.m_real, *(const hkQuadReal*)&signmask));
}

//
//	END!
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
