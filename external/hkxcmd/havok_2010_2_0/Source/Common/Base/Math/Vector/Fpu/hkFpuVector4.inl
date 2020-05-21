/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/* quad, here for inlining */

inline hkQuadReal& hkVector4::getQuad()
{
	return reinterpret_cast<hkQuadReal&>(x);
}

inline const hkQuadReal& hkVector4::getQuad() const
{
	return reinterpret_cast<const hkQuadReal&>(x);
}

/* operator (),[] here for inlining */

inline hkReal& hkVector4::operator() (int i)
{
	HK_ASSERT(0x3d62369d, i>=0 && i<4);
	return (&x)[i];
}

inline const hkReal& hkVector4::operator() (int i) const
{
	HK_ASSERT(0x269e424a, i>=0 && i<4);
	return (&x)[i];
}

HK_FORCE_INLINE void hkVector4::setW(const hkVector4& v)
{
	w = v.w;
}

//
//	Copies some of the components from v

template<hkVector4Comparison::Mask mask>
inline void hkVector4::setComponent(hkVector4Parameter v)
{
	x = ( mask & hkVector4Comparison::MASK_X ) ? v.x : x;
	y = ( mask & hkVector4Comparison::MASK_Y ) ? v.y : y;
	z = ( mask & hkVector4Comparison::MASK_Z ) ? v.z : z;
	w = ( mask & hkVector4Comparison::MASK_W ) ? v.w : w;
}

template<hkVector4Comparison::Mask mask>
inline void hkVector4::setComponent(hkSimdRealParameter v)
{
	const hkReal val = v.getReal();
	x = ( mask & hkVector4Comparison::MASK_X ) ? val : x;
	y = ( mask & hkVector4Comparison::MASK_Y ) ? val : y;
	z = ( mask & hkVector4Comparison::MASK_Z ) ? val : z;
	w = ( mask & hkVector4Comparison::MASK_W ) ? val : w;
}

//
//	Specialization for X

template<>
inline void hkVector4::setComponent<hkVector4Comparison::MASK_X>(hkVector4Parameter v)
{
	x = v.x;
}

template<>
inline void hkVector4::setComponent<hkVector4Comparison::MASK_X>(hkSimdRealParameter v)
{
	x = v.getReal();
}

//
//	Specialization for Y

template<>
inline void hkVector4::setComponent<hkVector4Comparison::MASK_Y>(hkVector4Parameter v)
{
	y = v.y;
}

template<>
inline void hkVector4::setComponent<hkVector4Comparison::MASK_Y>(hkSimdRealParameter v)
{
	y = v.getReal();
}

//
//	Specialization for Z

template<>
inline void hkVector4::setComponent<hkVector4Comparison::MASK_Z>(hkVector4Parameter v)
{
	z = v.z;
}

template<>
inline void hkVector4::setComponent<hkVector4Comparison::MASK_Z>(hkSimdRealParameter v)
{
	z = v.getReal();
}

//
//	Specialization for W

template<>
inline void hkVector4::setComponent<hkVector4Comparison::MASK_W>(hkVector4Parameter v)
{
	w = v.w;
}

template<>
inline void hkVector4::setComponent<hkVector4Comparison::MASK_W>(hkSimdRealParameter v)
{
	w = v.getReal();
}

HK_FORCE_INLINE void hkVector4::setSwapXY(const hkVector4& v)
{
	hkReal tmp = v.x;
	x = v.y;
	y = tmp;
	z = v.z;
	w = v.w;
}

HK_FORCE_INLINE void hkVector4::setXYZ(const hkVector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

HK_FORCE_INLINE void hkVector4::setXYZ0(const hkVector4& xyz)
{
	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
	w = 0;
}

//
//	Sets this = (dot3(v, a0), dot3(v, a1), dot3(v, a2), dot3(v, a3))

HK_FORCE_INLINE void hkVector4::setDot3_1vs4(hkVector4Parameter v, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4Parameter a3)
{
	x = v.dot3(a0);
	y = v.dot3(a1);
	z = v.dot3(a2);
	w = v.dot3(a3);
}

HK_FORCE_INLINE void hkVector4::setXYZW(const hkVector4& xyz, const hkVector4& ww)
{
	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
	w = ww.w;
}

inline void hkVector4::setXYZW(const hkVector4& xyz, hkSimdRealParameter ww)
{
	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
	w = ww.getReal();
}


HK_FORCE_INLINE void hkVector4::setReciprocal4(const hkVector4& v)
{
	x = 1.0f / v.x;
	y = 1.0f / v.y;
	z = 1.0f / v.z;
	w = 1.0f / v.w;
}

#define HK_VECTOR4_setReciprocal3
inline void hkVector4::setReciprocal3(const hkVector4& v)
{
	x = 1.0f / v.x;
	y = 1.0f / v.y;
	z = 1.0f / v.z;
	w = 1.0f;
}

HK_FORCE_INLINE void hkVector4::setSqrtInverse4(const hkVector4& v)
{
	x = hkMath::sqrtInverse(v.x);
	y = hkMath::sqrtInverse(v.y);
	z = hkMath::sqrtInverse(v.z);
	w = hkMath::sqrtInverse(v.w);
}

inline hkSimdReal hkVector4::getSimdAt(int i) const
{
	HK_ASSERT(0x5e605c8e, i>=0 && i<4);
	return (&x)[i];
}

// inline void hkVector4::setSimdAt(int i, hkSimdRealParameter val)
// {
// 	HK_ASSERT(0x5e605c8f, i>=0 && i<4);
// 	(&x)[i] = val;
// }

inline void hkVector4::operator= (const hkVector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

#ifndef HK_DISABLE_VECTOR4_CONSTRUCTORS

/* construct, assign, zero */
inline hkVector4::hkVector4(hkReal a, hkReal b, hkReal c, hkReal d)
	: x(a), y(b), z(c), w(d)
{
}

inline hkVector4::hkVector4(const hkQuadReal& q)
	: x(q.x), y(q.y), z(q.z), w(q.w)
{
}

inline hkVector4::hkVector4( const hkVector4& v)
{
	*this = v;
}
#endif

inline void hkVector4::set(hkReal a, hkReal b, hkReal c, hkReal d)
{
	x = a;
	y = b;
	z = c;
	w = d;
}

inline void hkVector4::setAll(hkReal a)
{
	x = a;
	y = a;
	z = a;
	w = a;
}

inline void hkVector4::setAll(hkSimdRealParameter a)
{
	setAll( a.getReal() );
}


inline void hkVector4::setAll3(hkReal a)
{
	x = a;
	y = a;
	z = a;
	w = a;
}

inline void hkVector4::setZero4()
{
	x = y = z = w = 0;
}

inline void hkVector4::zeroElement( int i )
{
	(*this)(i) = 0.0f;
}


inline void hkVector4::storeUncached( void* dest) const
{
	*static_cast<hkVector4*>(dest) = *this;
}

/* vector methods */

inline void hkVector4::add4(const hkVector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

inline void hkVector4::add3clobberW(const hkVector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

inline void hkVector4::setAdd4(const hkVector4& v0, const hkVector4& v1)
{
	x = v0.x + v1.x;
	y = v0.y + v1.y;
	z = v0.z + v1.z;
	w = v0.w + v1.w;
}

inline void hkVector4::sub4(const hkVector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

inline void hkVector4::sub3clobberW(const hkVector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

inline void hkVector4::setSub4(const hkVector4& v0, const hkVector4& v1)
{
	x = v0.x - v1.x;
	y = v0.y - v1.y;
	z = v0.z - v1.z;
	w = v0.w - v1.w;
}

inline void hkVector4::mul4(const hkVector4& a)
{
	x *= a.x;
	y *= a.y;
	z *= a.z;
	w *= a.w;
}
inline void hkVector4::setMul4(const hkVector4& a, const hkVector4& b)
{
	x = a.x * b.x;
	y = a.y * b.y;
	z = a.z * b.z;
	w = a.w * b.w;
}

inline void hkVector4::mul4(hkSimdRealParameter rs)
{  
	hkReal s = hkReal(rs);
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}
inline void hkVector4::setMul4(hkSimdRealParameter rs, const hkVector4& a)
{
	hkReal r = hkReal(rs);
	x = r * a.x;
	y = r * a.y;
	z = r * a.z;
	w = r * a.w;
}

#define HK_VECTOR4_setMulSigns4
inline void hkVector4::setMulSigns4(hkVector4Parameter a, hkVector4Parameter signs)
{
	// Switched from the impl x = (signs.x <= 0.0f) ? -1.0f : 1.0f * a.x
	// to handle +0 and -0 properly
	
	const int* iSigns = reinterpret_cast<const int*>(&signs.x);
	const int* iA = reinterpret_cast<const int*>(&a.x);
	int* res = reinterpret_cast<int*>(&x);

	res[0] = iA[0] ^ (iSigns[0] & 0x80000000);
	res[1] = iA[1] ^ (iSigns[1] & 0x80000000);
	res[2] = iA[2] ^ (iSigns[2] & 0x80000000);
	res[3] = iA[3] ^ (iSigns[3] & 0x80000000);			
}

//
//	Sets this = a * sign(b) per component

inline void hkVector4::setMulSigns4(hkVector4Parameter a, hkSimdRealParameter sharedSign)
{
	const hkReal sign = (sharedSign.getReal() >= 0.0f) ? 1.0f : -1.0f;
	x = sign * a.x;
	y = sign * a.y;
	z = sign * a.z;
	w = sign * a.w;
}

inline void hkVector4::setDiv4(hkVector4Parameter a, hkVector4Parameter b)
{
	x = a.x / b.x;
	y = a.y / b.y;
	z = a.z / b.z;
	w = a.w / b.w;
}

inline void hkVector4::div4(hkVector4Parameter a)
{
	x = x / a.x;
	y = y / a.y;
	z = z / a.z;
	w = w / a.w;
}

inline void hkVector4::setDiv4fast(hkVector4Parameter a, hkVector4Parameter b)
{
	setDiv4(a, b);
}

inline void hkVector4::div4fast(hkVector4Parameter a)
{
	div4(a);
}

inline void hkVector4::addMul4(hkSimdRealParameter rs, const hkVector4& a)
{
	hkReal r = hkReal(rs);
	x += r * a.x;
	y += r * a.y;
	z += r * a.z;
	w += r * a.w;
}

inline void hkVector4::addMul4(const hkVector4& a, const hkVector4& b)
{
	x += a.x * b.x;
	y += a.y * b.y;
	z += a.z * b.z;
	w += a.w * b.w;
}

inline void hkVector4::subMul4(hkSimdRealParameter rs, const hkVector4& a)
{
	hkReal r = hkReal(rs);
	x -= r * a.x;
	y -= r * a.y;
	z -= r * a.z;
	w -= r * a.w;
}

inline void hkVector4::subMul4(const hkVector4& a, const hkVector4& b)
{
	x -= a.x * b.x;
	y -= a.y * b.y;
	z -= a.z * b.z;
	w -= a.w * b.w;
}

inline void hkVector4::setAddMul4(const hkVector4& a, const hkVector4& b, hkSimdRealParameter r)
{
	hkReal rr = r;
	x = a.x + rr * b.x;
	y = a.y + rr * b.y;
	z = a.z + rr * b.z;
	w = a.w + rr * b.w;
}

inline void hkVector4::setAddMul4(const hkVector4& a, const hkVector4& m0, const hkVector4& m1)
{
	x = a.x + m0.x * m1.x;
	y = a.y + m0.y * m1.y;
	z = a.z + m0.z * m1.z;
	w = a.w + m0.w * m1.w;
}

inline void hkVector4::setSubMul4(const hkVector4& a, const hkVector4& b, hkSimdRealParameter r)
{
	hkReal rr = r;
	x = a.x - rr * b.x;
	y = a.y - rr * b.y;
	z = a.z - rr * b.z;
	w = a.w - rr * b.w;
}

inline void hkVector4::setSubMul4(const hkVector4& a, const hkVector4& m0, const hkVector4& m1)
{
	x = a.x - m0.x * m1.x;
	y = a.y - m0.y * m1.y;
	z = a.z - m0.z * m1.z;
	w = a.w - m0.w * m1.w;
}


#if !defined( HK_VECTOR4_setCross)
	inline void hkVector4::setCross(const hkVector4& v1, const hkVector4& v2)
	{
		const hkReal nx = v1(1)*v2(2) - v1(2)*v2(1);
		const hkReal ny = v1(2)*v2(0) - v1(0)*v2(2);
		const hkReal nz = v1(0)*v2(1) - v1(1)*v2(0);
		this->set( nx, ny, nz , 0.0f );
	}
#endif // HK_VECTOR4_SET_CROSS


inline void hkVector4::setInterpolate4(const hkVector4& v0, const hkVector4& v1, hkSimdRealParameter rt)
{
	hkReal t = hkReal(rt);
	hkReal s = 1.0f - t;
	hkVector4& d = *this;
	d(0) = s * v0(0) + t * v1(0);
	d(1) = s * v0(1) + t * v1(1);
	d(2) = s * v0(2) + t * v1(2);
	d(3) = s * v0(3) + t * v1(3);
}

inline hkVector4Comparison hkVector4::compareLessThan4(const hkVector4& a) const
{
	hkVector4Comparison ret;
	ret.m_mask =
		((x<a.x) ? hkVector4Comparison::MASK_X : 0) |
		((y<a.y) ? hkVector4Comparison::MASK_Y : 0) |
		((z<a.z) ? hkVector4Comparison::MASK_Z : 0) |
		((w<a.w) ? hkVector4Comparison::MASK_W : 0);
	return ret;
}

inline hkVector4Comparison hkVector4::compareEqual4(const hkVector4& a) const
{
	hkVector4Comparison ret;
	ret.m_mask =
		((x==a.x) ? hkVector4Comparison::MASK_X : 0) |
		((y==a.y) ? hkVector4Comparison::MASK_Y : 0) |
		((z==a.z) ? hkVector4Comparison::MASK_Z : 0) |
		((w==a.w) ? hkVector4Comparison::MASK_W : 0);
	return ret;
}

inline hkVector4Comparison hkVector4::compareLessThanEqual4(const hkVector4& a) const
{
	hkVector4Comparison ret;
	ret.m_mask =
		((x<=a.x) ? hkVector4Comparison::MASK_X : 0) |
		((y<=a.y) ? hkVector4Comparison::MASK_Y : 0) |
		((z<=a.z) ? hkVector4Comparison::MASK_Z : 0) |
		((w<=a.w) ? hkVector4Comparison::MASK_W : 0);
	return ret;
}

inline hkVector4Comparison hkVector4::compareLessThanZero4() const
{
	hkVector4Comparison ret;
	ret.m_mask =
		((x<0) ? hkVector4Comparison::MASK_X : 0) |
		((y<0) ? hkVector4Comparison::MASK_Y : 0) |
		((z<0) ? hkVector4Comparison::MASK_Z : 0) |
		((w<0) ? hkVector4Comparison::MASK_W : 0);
	return ret;
}

inline hkBool32 hkVector4::allLessThan3(hkVector4Parameter a) const
{
	return x<a.x && y<a.y && z<a.z;
}

inline hkBool32 hkVector4::allLessThan4(hkVector4Parameter a) const
{
	return x<a.x && y<a.y && z<a.z && w<a.w;
}

inline void hkVector4::select32( hkVector4Parameter falseValue, hkVector4Parameter trueValue, hkVector4ComparisonParameter comp)
{
	int m = comp.getMask();
	x = (m & hkVector4Comparison::MASK_X) ? trueValue.x : falseValue.x;
	y = (m & hkVector4Comparison::MASK_Y) ? trueValue.y : falseValue.y;
	z = (m & hkVector4Comparison::MASK_Z) ? trueValue.z : falseValue.z;
	w = (m & hkVector4Comparison::MASK_W) ? trueValue.w : falseValue.w;
}


inline void hkVector4::setNeg4(const hkVector4& v)
{
	x = -v.x;
	y = -v.y;
	z = -v.z;
	w = -v.w;
}

inline void hkVector4::setNeg3(const hkVector4& v)
{
	x = -v.x;
	y = -v.y;
	z = -v.z;
	w =  v.w;
}

inline void hkVector4::setAbs4(const hkVector4& v)
{
	x = hkMath::fabs(v.x);
	y = hkMath::fabs(v.y);
	z = hkMath::fabs(v.z);
	w = hkMath::fabs(v.w);
}

inline void hkVector4::setNegMask4(const hkVector4& v, int mask)
{
    hkUint32* dst = (hkUint32*)this;
    const hkUint32* src = (const hkUint32*)&v;
    dst[0] = src[0] ^ (hkUint32(mask & 1) << 31);
    dst[1] = src[1] ^ (hkUint32(mask & 2) << 30);
    dst[2] = src[2] ^ (hkUint32(mask & 4) << 29);
    dst[3] = src[3] ^ (hkUint32(mask & 8) << 28);
}

#if !defined(HK_VECTOR4_setMin4)
	inline void hkVector4::setMin4(const hkVector4& a, const hkVector4& b)
	{
		x = a.x < b.x ? a.x : b.x;
		y = a.y < b.y ? a.y : b.y;
		z = a.z < b.z ? a.z : b.z;
		w = a.w < b.w ? a.w : b.w;
	}
#endif

#if !defined( HK_VECTOR4_setMax4)
	inline void hkVector4::setMax4(const hkVector4& a, const hkVector4& b)
	{
		x = a.x > b.x ? a.x : b.x;
		y = a.y > b.y ? a.y : b.y;
		z = a.z > b.z ? a.z : b.z;
		w = a.w > b.w ? a.w : b.w;
	}
#endif

//
//	Returns the maximum component, i.e. max(v(0), v(1), v(2))

#if !defined(HK_VECTOR4_getHorizontalMax3)

	inline hkSimdReal hkVector4::getHorizontalMax3() const
	{
		return hkMath::max2(x, hkMath::max2(y, z));
	}

#endif

//
//	Returns the minimum component, i.e. min(this(0), this(1), this(2))

#if !defined(HK_VECTOR4_getHorizontalMin3)

	inline hkSimdReal hkVector4::getHorizontalMin3() const
	{
		return hkMath::min2(x, hkMath::min2(y, z));
	}

#endif

#if !defined( HK_VECTOR4__setMul3)
	inline void hkVector4::_setMul3( const hkMatrix3& r, const hkVector4& v )
	{
		//HK_ASSERT(0x28487ddd, hkMath::isAliased(r,this) == false );
		hkReal v0 = v(0);
		hkReal v1 = v(1);
		hkReal v2 = v(2);
		hkVector4& t = *this;
		t(0) = r(0,0)*v0 + r(0,1)*v1 + r(0,2)*v2;
		t(1) = r(1,0)*v0 + r(1,1)*v1 + r(1,2)*v2;
		t(2) = r(2,0)*v0 + r(2,1)*v1 + r(2,2)*v2;
		t(3) = 0;
	}
#endif

#if !defined( HK_VECTOR4__setMul4)
	inline void hkVector4::_setMul4( const hkMatrix3& t, const hkVector4& v )
	{
		//HK_ASSERT(0x28487ddd, hkMath::isAliased(t,this) == false );
		hkReal v0 = v(0);
		hkReal v1 = v(1);
		hkReal v2 = v(2);
		hkVector4& d = *this;
		d(0) = t(0,0)*v0 + t(0,1)*v1 + t(0,2)*v2;
		d(1) = t(1,0)*v0 + t(1,1)*v1 + t(1,2)*v2;
		d(2) = t(2,0)*v0 + t(2,1)*v1 + t(2,2)*v2;
		d(3) = t(3,0)*v0 + t(3,1)*v1 + t(3,2)*v2;
	}
#endif

#if !defined( HK_VECTOR4__setMul4xyz1)
	inline void hkVector4::_setMul4xyz1( const hkTransform& t, const hkVector4& v )
	{
		//HK_ASSERT(0x28487ddd, hkMath::isAliased(t,this) == false );
		hkReal v0 = v(0);
		hkReal v1 = v(1);
		hkReal v2 = v(2);
		hkVector4& d = *this;
		d(0) = t(0,0)*v0 + t(0,1)*v1 + t(0,2)*v2 + t(0,3);
		d(1) = t(1,0)*v0 + t(1,1)*v1 + t(1,2)*v2 + t(1,3);
		d(2) = t(2,0)*v0 + t(2,1)*v1 + t(2,2)*v2 + t(2,3);
		d(3) = t(3,0)*v0 + t(3,1)*v1 + t(3,2)*v2 + t(3,3);
	}
#endif

#if !defined( HK_VECTOR4__setRotatedDir)
	inline void hkVector4::_setRotatedDir(const hkRotation& r, const hkVector4& v)
	{
		_setMul3(r,v);	
	}
#endif

#if !defined( HK_VECTOR4__setRotatedInverseDir)
	inline void hkVector4::_setRotatedInverseDir(const hkRotation& r, const hkVector4& v)
	{
		//HK_ASSERT(0x28487ddd, hkMath::isAliased(r,this) == false );
		hkReal v0 = v(0);
		hkReal v1 = v(1);
		hkReal v2 = v(2);
		hkVector4& t = *this;
		t(0) = r(0,0)*v0 + r(1,0)*v1 + r(2,0)*v2;
		t(1) = r(0,1)*v0 + r(1,1)*v1 + r(2,1)*v2;
		t(2) = r(0,2)*v0 + r(1,2)*v1 + r(2,2)*v2;
		t(3) = 0;
	}
#endif

#if !defined( HK_VECTOR4__setTransformedPos)
	inline void hkVector4::_setTransformedPos(const hkTransform& t, const hkVector4& v)
	{
		//HK_ASSERT(0x28487ddd, hkMath::isAliased(t,this) == false );
		hkReal v0 = v(0);
		hkReal v1 = v(1);
		hkReal v2 = v(2);
		hkVector4& d = *this;
		d(0) = t(0,0)*v0 + t(0,1)*v1 + t(0,2)*v2 + t(0,3);
		d(1) = t(1,0)*v0 + t(1,1)*v1 + t(1,2)*v2 + t(1,3);
		d(2) = t(2,0)*v0 + t(2,1)*v1 + t(2,2)*v2 + t(2,3);
		d(3) = 0;
	}
#endif

#if !defined( HK_VECTOR4__setTransformedInversePos)
	inline void hkVector4::_setTransformedInversePos(const hkTransform& t, const hkVector4& v)
	{
		//HK_ASSERT(0x28487ddd, hkMath::isAliased(t,this) == false );
		hkReal v0 = v(0) - t(0,3);
		hkReal v1 = v(1) - t(1,3);
		hkReal v2 = v(2) - t(2,3);
	
		hkVector4& d = *this;
		d(0) = t(0,0)*v0 + t(1,0)*v1 + t(2,0)*v2;
		d(1) = t(0,1)*v0 + t(1,1)*v1 + t(2,1)*v2;
		d(2) = t(0,2)*v0 + t(1,2)*v1 + t(2,2)*v2;
		d(3) = 0.0f;
	}
#endif
	
/* length and distance */

inline hkSimdReal hkVector4::dot3(const hkVector4& a) const
{
	return (x * a.x) + ( y * a.y ) + ( z * a.z );
}

inline void hkVector4::setDot3(hkVector4Parameter a, hkVector4Parameter b)
{
	x = ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z );
}

inline hkReal hkVector4::dot3fpu(const hkVector4& a) const
{
	return (x * a.x) + ( y * a.y ) + ( z * a.z );
}

inline hkSimdReal hkVector4::dot4(const hkVector4& a) const
{
	return (x * a.x) + ( y * a.y ) + ( z * a.z ) + ( w * a.w );
}

inline void hkVector4::setDot4(hkVector4Parameter a, hkVector4Parameter b)
{
	x = y = z = w = ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z) + ( a.w * b.w );
}

inline hkSimdReal hkVector4::dot4xyz1(const hkVector4& a) const
{
	return (x * a.x) + ( y * a.y ) + ( z * a.z ) + ( w  );
}

inline hkSimdReal hkVector4::horizontalAdd3() const
{
	return x+y+z;
}

inline hkSimdReal hkVector4::horizontalAdd4() const
{
	return x+y+z+w;
}

inline void hkVector4::setHorizontalMax4( hkVector4Parameter p)
{
	hkReal xy = hkMath::max2( p(0), p(1) );
	hkReal zw = hkMath::max2( p(2), p(3) );
	hkReal xyzw = hkMath::max2( xy, zw );
	setAll( xyzw );
}

#if !defined( HK_VECTOR4_length3)
	inline hkSimdReal hkVector4::length3() const
	{
		return hkMath::sqrt((x * x) + ( y * y ) + ( z * z ));
	}
#endif
	
	
inline hkSimdReal hkVector4::lengthSquared3() const
{
	return this->dot3(*this);
}

#if !defined( HK_VECTOR4_lengthInverse3)
	inline hkSimdReal hkVector4::lengthInverse3() const
	{
		hkReal l2 = (x*x) + (y*y) + (z*z);
		return l2 ? hkMath::sqrtInverse(l2) : 0;
	}
#endif

#if !defined( HK_VECTOR4_length4)	
	inline hkSimdReal hkVector4::length4() const
	{
		return hkMath::sqrt( (x * x) + (y * y) + (z * z) + (w * w) );
	}
#endif

inline hkSimdReal hkVector4::lengthSquared4() const
{
	return this->dot4(*this);
}

#if !defined( HK_VECTOR4_lengthInverse4)
	inline hkSimdReal hkVector4::lengthInverse4() const
	{
		hkReal l2 = (x*x) + (y*y) + (z*z) + (w*w);
		return l2 ? hkMath::sqrtInverse(l2) : 0;
	}
#endif

#if !defined( HK_VECTOR4_normalize3 )
	inline void hkVector4::normalize3()
	{
		this->mul4( this->lengthInverse3() );
	}
#endif
	
#if !defined( HK_VECTOR4_normalizeWithLength3)	
	inline hkSimdReal hkVector4::normalizeWithLength3()
	{
		const hkReal len = this->length3();
		const hkReal invLen = (len ? (1.0f)/len : 0.0f);
		this->mul4( invLen );
		return len;
	}
#endif

#if !defined( HK_VECTOR4_normalize4)
	inline void hkVector4::normalize4()
	{
		this->mul4( this->lengthInverse4() );
	}
#endif

#if !defined( HK_VECTOR4_normalizeWithLength4)
	inline hkSimdReal hkVector4::normalizeWithLength4()
	{
		const hkReal len = this->length4();
		const hkReal invLen = (len ? (1.0f)/len : 0.0f);
		this->mul4( invLen );
		return len;
	}
#endif
	
inline void hkVector4::fastNormalize3()
{
	normalize3();
}

inline void hkVector4::setFastNormalize3NonZero(hkVector4Parameter a)
{
	*this = a;
	this->normalize3();
}

inline void hkVector4::fastNormalize3NonZero()
{
	hkReal l2 = (x*x) + (y*y) + (z*z);
	l2 = hkMath::sqrtInverse(l2);
	this->mul4( l2 );
}

inline hkSimdReal hkVector4::fastNormalizeWithLength3()
{
	return normalizeWithLength3();
}


inline void hkVector4::broadcast(int i)
{
	HK_ASSERT2(0x4a4d61ec, i>=0 && i<4, "index error in broadcast");
	hkReal r = (&x)[i];
	x = r;
	y = r;
	z = r;
	w = r;
}

inline void hkVector4::setBroadcast(const hkVector4& v, int i)
{
	HK_ASSERT2(0x294fb42b, i>=0 && i<4, "index error in broadcast");
	hkReal r = (&v.x)[i];
	x = r;
	y = r;
	z = r;
	w = r;
}

inline void hkVector4::setBroadcast3clobberW(const hkVector4& v, int i)
{
	HK_ASSERT2(0x294fb42b, i>=0 && i<4, "index error in broadcast");
	hkReal r = (&v.x)[i];
	x = r;
	y = r;
	z = r;
	w = r;
}

template <int S>
inline void hkVector4::setShuffle(hkVector4Parameter v)
{
    const hkReal* src = (const hkReal*)&v;
    // Store in regs before writing to the destination - to handle case when v and this are the same
    const hkReal t0 = src[(S & 0x3000) >> 12];
    const hkReal t1 = src[(S & 0x0300) >>  8];
    const hkReal t2 = src[(S & 0x0030) >>  4];
    const hkReal t3 = src[(S & 0x0003) >>  0];

    x = t0;
    y = t1;
    z = t2;
    w = t3;
}

inline void hkVector4::_setTransformedPos(const hkQsTransform& a, const hkVector4& b)
{
	hkVector4 scaled; scaled.setMul4(b, a.m_scale);
	hkVector4 rotated; rotated.setRotatedDir( a.m_rotation, scaled );
	setAdd4( rotated, a.m_translation );
}

inline void hkVector4::_setTransformedInversePos(const hkQsTransform& a, const hkVector4& b)
{
	//HK_ASSERT(0x28487ddd, hkMath::isAliased(a,this) == false );
	hkVector4 temp(b);
	temp.sub4(a.m_translation);
	setRotatedInverseDir(a.m_rotation, temp);

	hkVector4 invScale; invScale.set(1.0f/a.m_scale(0), 1.0f/a.m_scale(1), 1.0f/a.m_scale(2));
	mul4(invScale);
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
