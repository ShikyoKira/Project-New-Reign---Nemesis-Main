/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//typedef hkReal hkSimdReal;
//typedef hkReal hkSimdRealParameter;

struct hkQuadReal
{
	HK_ALIGN16( hkReal x );
	hkReal y;
	hkReal z;
	hkReal w;
};

#define HK_VECTOR4_PERM1ARG(_a, _b, _c, _d)	( ((_d) << 6) | ((_c) << 4) | ((_b) << 2) | ((_a)) )
#define HK_VECTOR4_PERM1(tgt, src, shuf)\
{\
	const hkQuadReal& __qrSrc = (src).getQuad();\
	const hkReal* __p = (const hkReal*)(&__qrSrc);\
	const hkReal __p0 = __p[(shuf) & 3];\
	const hkReal __p1 = __p[((shuf) >> 2) & 3];\
	const hkReal __p2 = __p[((shuf) >> 4) & 3];\
	const hkReal __p3 = __p[((shuf) >> 6) & 3];\
\
	tgt(0) = __p0;\
	tgt(1) = __p1;\
	tgt(2) = __p2;\
	tgt(3) = __p3;\
}

class hkSimdReal;
typedef const hkSimdReal hkSimdRealParameter;

class hkVector4;
typedef const hkVector4& hkVector4Parameter;

class hkVector4Comparison;
typedef hkVector4Comparison hkVector4ComparisonParameter;

extern const hkQuadReal g_vectorConstants[HK_QUADREAL_END];

#define HK_SIMD_REAL(x) hkSimdReal(x)

class hkSimdReal		
{
	public:
		hkSimdReal(const hkQuadReal& x) : m_real(x.x){}
		hkSimdReal(){}
		hkReal getReal() const { return m_real; }

// #if defined(HK_DISABLE_SIMD_REAL_FLOAT_CONVERSION)
// 		explicit hkSimdReal(hkReal x){m_real = x;}
// #else
		hkSimdReal(hkReal x){m_real = x;}

		static inline const hkSimdReal& HK_CALL getConstant(hkVectorConstant constant)
		{
			return *reinterpret_cast<const hkSimdReal*>( &(g_vectorConstants[constant]) );
		}


		/// convert a signed integer (note that conversion from unsigned integer is really slow on SSE
		explicit hkSimdReal(const int&     x){m_real = float(x);} 

		explicit hkSimdReal(const hkUint16& x){m_real = float(x);}

		explicit hkSimdReal(const hkUint8& x){m_real = float(x);}

		HK_FORCE_INLINE void convertToSaturateInt32( int& result )	
		{
			const int maxValueInt = 0x7FFFFFFF;
			const hkReal maxValueReal = hkReal(0x7FFFFFFF);
			result = (m_real < maxValueReal) ? int(m_real) : maxValueInt;
		}

		HK_FORCE_INLINE void convertToSaturateUInt16( hkUint16& result )
		{
			const int maxValueInt = 0x7FFFFFFF;
			const hkReal maxValueReal = hkReal(0x7FFFFFFF);
			int i = (m_real < maxValueReal) ? int(m_real) : maxValueInt;
			if ( i < 0 ) i = 0;
			if ( i > 0xffff) i = 0xffff;
			result = hkUint16(i);
		}

		operator hkReal() const { return m_real; }
		operator hkReal() { return m_real; }
// #endif
		//	broadcast, getQuad
		HK_FORCE_INLINE void setMax(  hkSimdRealParameter a, hkSimdRealParameter b ) 
		{
			*this = (a.m_real > b.m_real) ? a:b;
		}

		HK_FORCE_INLINE void setMin(  hkSimdRealParameter a, hkSimdRealParameter b ) 
		{
			*this = (a.m_real <b.m_real ) ? a:b;
		}

// 		HK_FORCE_INLINE void setOr(  hkSimdRealParameter a, hkSimdRealParameter b ) 
// 		{
// 			*(int*)this = ((*(int*)&a.m_real) | (*(int*)&b.m_real));
// 		}
// 
// 		HK_FORCE_INLINE void setXor(  hkSimdRealParameter a, hkSimdRealParameter b ) 
// 		{
// 			*(int*)this = ((*(int*)&a.m_real) ^ (*(int*)&b.m_real));
// 		}


		HK_FORCE_INLINE void store(  float *f ) const
		{
			*f = this->getReal();
		}

		HK_FORCE_INLINE void setSelect(hkVector4ComparisonParameter comp, hkSimdRealParameter trueValue, hkSimdRealParameter falseValue);

			/// Returns the comparison mask from this >= a
		HK_FORCE_INLINE hkVector4Comparison compareGreaterThanEqual(hkSimdRealParameter a) const;

			/// Returns the comparison mask from this > a
		HK_FORCE_INLINE hkVector4Comparison compareGreaterThan(hkSimdRealParameter a) const;

			/// Returns the comparison mask from this <= a
		HK_FORCE_INLINE hkVector4Comparison compareLessThanEqual(hkSimdRealParameter a) const;

			/// Returns the comparison mask from this < a
		HK_FORCE_INLINE hkVector4Comparison compareLessThan(hkSimdRealParameter a) const;

			/// Sets this = approx(1 / a)
		HK_FORCE_INLINE void setReciprocal(hkSimdRealParameter a);

	private:
		hkReal m_real;
};

HK_FORCE_INLINE hkSimdReal HK_CALL operator*(hkSimdRealParameter r, hkSimdRealParameter s)
{
	return (r.getReal()*s.getReal());
}

HK_FORCE_INLINE hkSimdReal HK_CALL operator-(hkSimdRealParameter r, hkSimdRealParameter s)
{
	return (r.getReal()-s.getReal());
}

HK_FORCE_INLINE hkSimdReal HK_CALL operator+(hkSimdRealParameter r, hkSimdRealParameter s)
{
	return (r.getReal()+s.getReal());
}

HK_FORCE_INLINE hkSimdReal HK_CALL operator/(hkSimdRealParameter r, hkSimdRealParameter s)
{
	return (r.getReal()/s.getReal());
}

HK_FORCE_INLINE hkSimdReal HK_CALL operator-(hkSimdRealParameter r)
{
	return (-r.getReal());
}


#define HK_TRANSPOSE4_SWAP(a,b) t = a; a = b; b = t
#define HK_TRANSPOSE4(v0,v1,v2,v3) { hkReal t; \
	HK_TRANSPOSE4_SWAP( v0(1), v1(0) ); \
	HK_TRANSPOSE4_SWAP( v0(2), v2(0) ); \
	HK_TRANSPOSE4_SWAP( v0(3), v3(0) ); \
	HK_TRANSPOSE4_SWAP( v1(2), v2(1) ); \
	HK_TRANSPOSE4_SWAP( v1(3), v3(1) ); \
	HK_TRANSPOSE4_SWAP( v2(3), v3(2) ); }

#define HK_TRANSPOSE3_SWAP(a,b) t = a; a = b; b = t
#define HK_TRANSPOSE3(v0,v1,v2) { hkReal t; \
	HK_TRANSPOSE3_SWAP( v0(1), v1(0) ); \
	HK_TRANSPOSE3_SWAP( v0(2), v2(0) ); \
	HK_TRANSPOSE3_SWAP( v1(2), v2(1) ); }

#define HK_QUADREAL_CONSTANT(a,b,c,d) {a,b,c,d}

union hkQuadRealUnion
{
	hkReal r[4];
	hkQuadReal q;
};

#define HK_SIMD_COMPARE_MASK_X 8

	/// Helper class which greatly helps avoiding LHS on XBOX360, try to use it if you can delay using a mask
class hkMaskStorage
{
	public:
		int getMask(){ return m_mask; }

	public:
		int m_mask;

};

	/// Result of a hkVector4 comparison.
class hkVector4Comparison
{
	public:

		enum Mask
		{
			MASK_NONE	= 0,
			MASK_W		= 1,
			MASK_Z		= 2,
			MASK_ZW		= 3,

			MASK_Y		= 4,
			MASK_YW		= 5,
			MASK_YZ		= 6,
			MASK_YZW	= 7,

			MASK_X		= 8,
			MASK_XW		= 9,
			MASK_XZ		= 10,
			MASK_XZW	= 11,

			MASK_XY		= 12,
			MASK_XYW	= 13,
			MASK_XYZ	= 14,
			MASK_XYZW	= 15
		};

		static const Mask s_components[4];

		HK_FORCE_INLINE void setAnd( hkVector4Comparison a, hkVector4Comparison b ) { m_mask = a.m_mask & b.m_mask; }
		HK_FORCE_INLINE void setAndNot(hkVector4ComparisonParameter a, hkVector4ComparisonParameter b)	{ m_mask = a.m_mask & (~b.m_mask); }
		HK_FORCE_INLINE void set( Mask m ) { m_mask = m; }
		HK_FORCE_INLINE void operator=(hkVector4ComparisonParameter rhs) { m_mask = rhs.m_mask; }
		HK_FORCE_INLINE void setOr( hkVector4Comparison a, hkVector4Comparison b ) { m_mask = a.m_mask | b.m_mask; }
		HK_FORCE_INLINE void setNot( hkVector4Comparison a ) { m_mask = (~a.m_mask) & MASK_XYZW ; }

		HK_FORCE_INLINE void setSelect( hkVector4Comparison a, hkVector4Comparison b, hkVector4Comparison comp) 
		{
			m_mask = (comp.m_mask & b.m_mask) | (~comp.m_mask & a.m_mask);
		}

		HK_FORCE_INLINE hkBool32 allAreSet( Mask m ) const { return (m_mask & m) == m; }
		HK_FORCE_INLINE hkBool32 anyIsSet( Mask m ) const { return m_mask & m; }

		HK_FORCE_INLINE hkBool32 allAreSet() const { return m_mask == MASK_XYZW; }
		HK_FORCE_INLINE hkBool32 anyIsSet() const { return m_mask; }

		HK_FORCE_INLINE int getMask() const { return m_mask; }
		HK_FORCE_INLINE int getMask(Mask m) const { return m_mask & m; }
		HK_FORCE_INLINE void getMask(hkMaskStorage&  m) const { m.m_mask = m_mask; }

		static HK_FORCE_INLINE int HK_CALL getCombinedMask(hkVector4Comparison& ca, hkVector4Comparison& cb, hkVector4Comparison& cc )
		{
			return ca.m_mask | (cb.m_mask<<4 ) | (cc.m_mask<<8);
		}


	private:

		int m_mask;
		friend class hkVector4;
};

HK_FORCE_INLINE void hkSimdReal::setSelect(hkVector4ComparisonParameter comp, hkSimdRealParameter trueValue, hkSimdRealParameter falseValue)
{
	m_real = comp.allAreSet() ? trueValue.getReal() : falseValue.getReal();
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
	const hkVector4Comparison::Mask m = (this->m_real <= a.getReal()) ? hkVector4Comparison::MASK_XYZW : hkVector4Comparison::MASK_NONE;

	hkVector4Comparison cmp;
	cmp.set(m);
	return cmp;
}

//
//	Returns the comparison mask from this < a

inline hkVector4Comparison hkSimdReal::compareLessThan(hkSimdRealParameter a) const
{
	const hkVector4Comparison::Mask m = (this->m_real < a.getReal()) ? hkVector4Comparison::MASK_XYZW : hkVector4Comparison::MASK_NONE;

	hkVector4Comparison cmp;
	cmp.set(m);
	return cmp;
}

//
//	Sets this = approx(1 / a)

inline void hkSimdReal::setReciprocal(hkSimdRealParameter a)
{
	m_real = 1.0f / a.m_real;
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
