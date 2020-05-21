/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkMatrix3::hkMatrix3( const hkMatrix3& other )
{
	m_col0 = other.m_col0;
	m_col1 = other.m_col1;
	m_col2 = other.m_col2;
}

#ifndef HK_MATRIX3_getColumn
#define HK_MATRIX3_getColumn

inline hkVector4& hkMatrix3::getColumn(int x)
{
	return (&m_col0)[x];
}

inline const hkVector4& hkMatrix3::getColumn(int x) const
{
	return (&m_col0)[x];
}

#endif

inline hkReal& hkMatrix3::operator() (int r, int c)
{
	return getColumn(c)(r);
}

inline const hkReal& hkMatrix3::operator() (int r, int c) const
{
	return getColumn(c)(r);
}


inline void hkMatrix3::setRows( const hkVector4& r0,
								const hkVector4& r1,
								const hkVector4& r2)
{
	getColumn(0).set( r0(0), r1(0), r2(0) );
	getColumn(1).set( r0(1), r1(1), r2(1) );
	getColumn(2).set( r0(2), r1(2), r2(2) );
}


inline void hkMatrix3::getRows( hkVector4& r0,
								hkVector4& r1,
								hkVector4& r2) const
{
	r0.set( getColumn(0)(0), getColumn(1)(0), getColumn(2)(0) );
	r1.set( getColumn(0)(1), getColumn(1)(1), getColumn(2)(1) );
	r2.set( getColumn(0)(2), getColumn(1)(2), getColumn(2)(2) );
}

inline void hkMatrix3::setCols( const hkVector4& r0,
								const hkVector4& r1,
								const hkVector4& r2)
{
	m_col0 = r0;
	m_col1 = r1;
	m_col2 = r2;
}

inline void hkMatrix3::operator= ( const hkMatrix3& a )
{
	m_col0 = a.getColumn(0);
	m_col1 = a.getColumn(1);
	m_col2 = a.getColumn(2);
}

inline void hkMatrix3::getCols( hkVector4& r0,
								hkVector4& r1,
								hkVector4& r2) const
{
	r0 = getColumn(0);
	r1 = getColumn(1);
	r2 = getColumn(2);
}

inline void hkMatrix3::getRow( int row, hkVector4& r) const
{
	r.set( getColumn(0)(row), getColumn(1)(row), getColumn(2)(row) );
}

	/// copy and transpose 
inline void hkMatrix3::_setTranspose( const hkMatrix3& s )
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 c0 = s.m_col0;
	hkVector4 c1 = s.m_col1;
	hkVector4 c2 = s.m_col2;
	HK_TRANSPOSE3(c0,c1,c2);
	m_col0 = c0;
	m_col1 = c1;
	m_col2 = c2;
#else
	hkMatrix3& d = *this;
	d(0,0) = s(0,0);
	d(1,1) = s(1,1);
	d(2,2) = s(2,2);
	d(3,0) = 0.0f;
	d(3,1) = 0.0f;
	d(3,2) = 0.0f;
	d(1,0) = s(0,1);
	d(0,1) = s(1,0);
	d(2,0) = s(0,2);
	d(0,2) = s(2,0);
	d(2,1) = s(1,2);
	d(1,2) = s(2,1);
#endif
}

//
//	Sets this matrix to be the product of a diagonal matrix (given as a vector) and matrix a (this = diag * a)

inline void hkMatrix3::setDiagonalMul(hkVector4Parameter vDiag, const hkMatrix3& a)
{
	m_col0.setMul4(vDiag, a.m_col0);
	m_col1.setMul4(vDiag, a.m_col1);
	m_col2.setMul4(vDiag, a.m_col2);
}

//
//	Set this matrix to be the product of qa and b.  (this = qa * b)

inline void hkMatrix3::setMul(const hkQuaternion& qa, const hkMatrix3& b)
{
	m_col0.setRotatedDir(qa, b.m_col0);
	m_col1.setRotatedDir(qa, b.m_col1);
	m_col2.setRotatedDir(qa, b.m_col2);
}

/// aTc = aTb * bTc
inline void hkMatrix3::_setMul( const hkMatrix3& aTb, const hkMatrix3& bTc )
{
	HK_ASSERT(0x6d9d1d43,  this != &aTb );

	m_col0._setMul3(aTb, bTc.m_col0);
	m_col1._setMul3(aTb, bTc.m_col1);
	m_col2._setMul3(aTb, bTc.m_col2);
}

inline void hkMatrix3::_setMul( hkSimdRealParameter scale, const hkMatrix3& a)
{
	getColumn(0).setMul4(scale, a.getColumn(0));
	getColumn(1).setMul4(scale, a.getColumn(1));
	getColumn(2).setMul4(scale, a.getColumn(2));
}

inline void hkMatrix3::_add( const hkMatrix3& a )
{
	getColumn(0).add4( a.getColumn(0) );
	getColumn(1).add4( a.getColumn(1) );
	getColumn(2).add4( a.getColumn(2) );
}

inline void hkMatrix3::_sub( const hkMatrix3& a )
{
	getColumn(0).sub4( a.getColumn(0) );
	getColumn(1).sub4( a.getColumn(1) );
	getColumn(2).sub4( a.getColumn(2) );
}

inline void hkMatrix3::_invertSymmetric()
{
	hkVector4 r0; r0.setCross( getColumn(1), getColumn(2) );
    hkVector4 r1; r1.setCross( getColumn(2), getColumn(0) );
    hkVector4 r2; r2.setCross( getColumn(0), getColumn(1) );

    hkSimdReal determinant = getColumn(0).dot3(r0);
	HK_ASSERT2( 0xf0140200, determinant.getReal() > 0.0f, "You cannot invert this matrix");

	hkSimdReal dinv = hkSimdReal::getConstant(HK_QUADREAL_1) / determinant;
	getColumn(0).setMul4(dinv, r0);
	getColumn(1).setMul4(dinv, r1);
	getColumn(2).setMul4(dinv, r2);
}

//
//	Sets the current matrix to be the inverse of the given matrix. Matrix src is assumed to be symmetric non-singular. If the matrix
//	is singular (up to machine precision), the inverse will be set to zero.

inline void hkMatrix3::setInverseSymmetric(const hkMatrix3& src)
{
	// Get source columns
	const hkVector4& c0 = src.getColumn(0);
	const hkVector4& c1 = src.getColumn(1);
	const hkVector4& c2 = src.getColumn(2);

	hkVector4 r0; r0.setCross(c1, c2);
	hkVector4 r1; r1.setCross(c2, c0);
	hkVector4 r2; r2.setCross(c0, c1);

	// Compute 1 / determinant. Set it to zero in case of singular matrices!
	hkSimdReal dInv;
	{
		const hkSimdReal eps	= hkSimdReal::getConstant(HK_QUADREAL_EPS);
		const hkSimdReal zero	= hkSimdReal::getConstant(HK_QUADREAL_0);

		const hkSimdReal determinant = c0.dot3(r0);
		const hkVector4Comparison cmp = determinant.compareGreaterThan(eps);
		hkSimdReal rec;	rec.setReciprocal(determinant);
		dInv.setSelect(cmp, rec, zero);
	}

	getColumn(0).setMul4(dInv, r0);
	getColumn(1).setMul4(dInv, r1);
	getColumn(2).setMul4(dInv, r2);
}

//
//	Solves the linear system: (this) * v = b, for v. The matrix is assumed non-singular and symmetric

inline void hkMatrix3::solveSymmetric(const hkVector4& b, hkVector4& v)
{
	// Invert matrix
	hkMatrix3 inv;
	inv.setInverseSymmetric(*this);

	// v = inv * b
	inv.multiplyVector(b, v);
}

//
//	Sets this matrix as the cross product between the given vector and each of the given matrix' columns
//	Specifically, it sets this.getColumn(i) = cross(v, src.getColumn(i)

inline void hkMatrix3::setCross(hkVector4Parameter v, const hkMatrix3& src)
{
	// Get source columns
	const hkVector4& sc0 = src.m_col0;
	const hkVector4& sc1 = src.m_col1;
	const hkVector4& sc2 = src.m_col2;

	// Target columns
	hkVector4 tc0, tc1, tc2;

	// First part
	{
		hkVector4 shufV;	// (vy, vz, vx, *)
		HK_VECTOR4_PERM1(shufV, v, HK_VECTOR4_PERM1ARG(1, 2, 0, 3));

		hkVector4 shufC0;	// (c0z, c0x, c0y)
		HK_VECTOR4_PERM1(shufC0, sc0, HK_VECTOR4_PERM1ARG(2, 0, 1, 3));

		hkVector4 shufC1;	// (c1z, c1x, c1y)
		HK_VECTOR4_PERM1(shufC1, sc1, HK_VECTOR4_PERM1ARG(2, 0, 1, 3));

		hkVector4 shufC2;	// (c2z, c2x, c2y)
		HK_VECTOR4_PERM1(shufC2, sc2, HK_VECTOR4_PERM1ARG(2, 0, 1, 3));

		tc0.setMul4(shufV, shufC0);	// (vy * c0z, vz * c0x, vx * c0y)
		tc1.setMul4(shufV, shufC1);	// (vy * c1z, vz * c1x, vx * c1y)
		tc2.setMul4(shufV, shufC2);	// (vy * c2z, vz * c2x, vx * c2y)
	}

	// Second part
	{
		hkVector4 shufV;	// (vz, vx, vy, *)
		HK_VECTOR4_PERM1(shufV, v, HK_VECTOR4_PERM1ARG(2, 0, 1, 3));

		hkVector4 shufC0;	// (c0y, c0z, c0x)
		HK_VECTOR4_PERM1(shufC0, sc0, HK_VECTOR4_PERM1ARG(1, 2, 0, 3));

		hkVector4 shufC1;	// (c1y, c1z, c1x)
		HK_VECTOR4_PERM1(shufC1, sc1, HK_VECTOR4_PERM1ARG(1, 2, 0, 3));

		hkVector4 shufC2;	// (c2y, c2z, c2x)
		HK_VECTOR4_PERM1(shufC2, sc2, HK_VECTOR4_PERM1ARG(1, 2, 0, 3));

		tc0.subMul4(shufV, shufC0);	// (vy * c0z - vz * c0y, vz * c0x - vx * c0z, vx * c0y - vy * c0x)
		tc1.subMul4(shufV, shufC1);	// (vy * c1z - vz * c1y, vz * c1x - vx * c1z, vx * c1y - vy * c1x)
		tc2.subMul4(shufV, shufC2);	// (vy * c2z - vz * c2y, vz * c2x - vx * c2z, vx * c2y - vy * c2x)
	}

	// Store
	m_col0 = tc0;
	m_col1 = tc1;
	m_col2 = tc2;
}

//
//	Sets this matrix to be the reflection of matrix D through matrix P, i.e. this = Transpose(P) * A * P
//	Matrix D is assumed diagonal.

inline void hkMatrix3::setReflectedDiagonal(const hkMatrix3& P, hkVector4Parameter D)
{
	// Resulting matrix is symmetric and element(i, j) = dot3(P(*, i) * P(*, j), D)
	// Get columns of p
	const hkVector4& p0 = P.getColumn(0);
	const hkVector4& p1 = P.getColumn(1);
	const hkVector4& p2 = P.getColumn(2);

	// Compute dots
	{
		hkVector4 p00;	p00.setMul4(p0, p0);
		hkVector4 p01;	p01.setMul4(p0, p1);
		hkVector4 p02;	p02.setMul4(p0, p2);
		hkVector4 p11;	p11.setMul4(p1, p1);
		hkVector4 p12;	p12.setMul4(p1, p2);
		hkVector4 p22;	p22.setMul4(p2, p2);
		
		hkVector4 dotsA;	dotsA.setDot3_1vs4(D, p00, p01, p02, p11);	// (m00, m01, m02, m11)
		hkVector4 dotsB;	dotsB.setDot3_1vs4(D, p02, p12, p22, p11);	// (m02, m12, m22, m11)
		
		m_col0 = dotsA;
		m_col2 = dotsB;

		// dotsA = (m01, m11, m02, m00)
		HK_VECTOR4_PERM1(dotsA, dotsA, HK_VECTOR4_PERM1ARG(1, 3, 2, 0));

		// dotsB = (m12, m12, m12, m12)
		HK_VECTOR4_PERM1(dotsB, dotsB, HK_VECTOR4_PERM1ARG(1, 1, 1, 1));

		hkVector4Comparison cmp;
		cmp.set(hkVector4Comparison::MASK_XY);
		m_col1.setSelect4(cmp, dotsA, dotsB);
	}
}

//
//	Adds the same value to all elements of the diagonal

inline void hkMatrix3::addDiagonal(hkSimdRealParameter diag)
{
	// Make a vector v = (diag, diag, diag, 0)
	hkVector4 v;
	v.setAll(diag);
	v.setXYZW(v, hkSimdReal::getConstant(HK_QUADREAL_0));

	// Shuffle it
	// v0 = (diag, 0, 0, 0)
	// v1 = (0, diag, 0, 0)
	// v2 = (0, 0, diag, 0)
	hkVector4 v0, v1, v2;
	HK_VECTOR4_PERM1(v0, v, HK_VECTOR4_PERM1ARG(0, 3, 3, 3));
	HK_VECTOR4_PERM1(v1, v, HK_VECTOR4_PERM1ARG(3, 0, 3, 3));
	HK_VECTOR4_PERM1(v2, v, HK_VECTOR4_PERM1ARG(3, 3, 0, 3));

	// Add to columns
	m_col0.add4(v0);
	m_col1.add4(v1);
	m_col2.add4(v2);
}

inline void hkMatrix3::multiplyVector (const hkVector4& v, hkVector4& resultOut) const
{
	hkVector4 xb; xb.setBroadcast(v, 0);
	hkVector4 yb; yb.setBroadcast(v, 1);
	hkVector4 zb; zb.setBroadcast(v, 2);

	hkVector4 t0;
	t0.setMul4( xb, m_col0 );
	t0.addMul4( yb, m_col1 );
	t0.addMul4( zb, m_col2 );

	resultOut = t0;
}

//
//	Add the product of a and scale (this += a * scale)

inline void hkMatrix3::_addMul( hkSimdRealParameter scale, const hkMatrix3& a)
{
	HK_ASSERT(0x53e345d4,  this != &a);
	m_col0.addMul4(scale, a.m_col0);
	m_col1.addMul4(scale, a.m_col1);
	m_col2.addMul4(scale, a.m_col2);
}

// has the default implementation be overridden?
#if !defined( HK_MATRIX3_setZero )
inline void hkMatrix3::setZero()
{
	hkVector4 zero; zero.setZero4();
	getColumn(0) = zero;
	getColumn(1) = zero;
	getColumn(2) = zero;
}
#endif

#if !defined( HK_MATRIX3_setDiagonal )
inline void hkMatrix3::setDiagonal( hkReal m00, hkReal m11, hkReal m22 )
{
	setZero();
	(*this)(0,0) = m00;
	(*this)(1,1) = m11;
	(*this)(2,2) = m22;
}

inline void hkMatrix3::setDiagonal( hkVector4Parameter vDiagonal )
{
	m_col0.setMul4(vDiagonal.getSimdAt(0), hkVector4::getConstant<HK_QUADREAL_1000>());
	m_col1.setMul4(vDiagonal.getSimdAt(1), hkVector4::getConstant<HK_QUADREAL_0100>());
	m_col2.setMul4(vDiagonal.getSimdAt(2), hkVector4::getConstant<HK_QUADREAL_0010>());
}

inline void hkMatrix3::setDiagonal(hkSimdRealParameter diag)
{
	m_col0.setMul4(diag, hkVector4::getConstant<HK_QUADREAL_1000>());
	m_col1.setMul4(diag, hkVector4::getConstant<HK_QUADREAL_0100>());
	m_col2.setMul4(diag, hkVector4::getConstant<HK_QUADREAL_0010>());
}

#endif

#if !defined( HK_MATRIX3_setIdentity )
inline void hkMatrix3::setIdentity()
{
	setZero();
	hkReal one = 1.0f;
	(*this)(0,0) = one;
	(*this)(1,1) = one;
	(*this)(2,2) = one;
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
