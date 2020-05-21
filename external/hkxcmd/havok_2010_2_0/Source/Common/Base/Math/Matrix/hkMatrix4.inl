/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkVector4& hkMatrix4::getColumn(int x)
{
	return (&m_col0)[x];
}

inline const hkVector4& hkMatrix4::getColumn(int x) const
{
	return (&m_col0)[x];
}

inline hkReal& hkMatrix4::operator() (int r, int c)
{
	return getColumn(c)(r);
}

inline const hkReal& hkMatrix4::operator() (int r, int c) const
{
	return getColumn(c)(r);
}


inline void hkMatrix4::setRows( const hkVector4& r0,
								const hkVector4& r1,
								const hkVector4& r2,
								const hkVector4& r3)
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	m_col0 = r0;
	m_col1 = r1;
	m_col2 = r2;
	m_col3 = r3;
	HK_TRANSPOSE4(m_col0,m_col1,m_col2,m_col3);
#else
	getColumn(0).set( r0(0), r1(0), r2(0), r3(0) );
	getColumn(1).set( r0(1), r1(1), r2(1), r3(1) );
	getColumn(2).set( r0(2), r1(2), r2(2), r3(2) );
	getColumn(3).set( r0(3), r1(3), r2(3), r3(3) );
#endif
}


inline void hkMatrix4::getRows( hkVector4& r0,
								hkVector4& r1,
								hkVector4& r2,
								hkVector4& r3) const
{
	r0.set( getColumn(0)(0), getColumn(1)(0), getColumn(2)(0), getColumn(3)(0) );
	r1.set( getColumn(0)(1), getColumn(1)(1), getColumn(2)(1), getColumn(3)(1) );
	r2.set( getColumn(0)(2), getColumn(1)(2), getColumn(2)(2), getColumn(3)(2) );
	r3.set( getColumn(0)(3), getColumn(1)(3), getColumn(2)(3), getColumn(3)(3) );
}

inline void hkMatrix4::setCols( const hkVector4& r0,
								const hkVector4& r1,
								const hkVector4& r2,
								const hkVector4& r3)
{
	m_col0 = r0;
	m_col1 = r1;
	m_col2 = r2;
	m_col3 = r3;
}

inline void hkMatrix4::operator= ( const hkMatrix4& a )
{
	m_col0 = a.getColumn(0);
	m_col1 = a.getColumn(1);
	m_col2 = a.getColumn(2);
	m_col3 = a.getColumn(3);
}

inline void hkMatrix4::getCols( hkVector4& r0,
								hkVector4& r1,
								hkVector4& r2,
								hkVector4& r3) const
{
	r0 = getColumn(0);
	r1 = getColumn(1);
	r2 = getColumn(2);
	r3 = getColumn(3);
}

inline void hkMatrix4::getRow( int row, hkVector4& r) const
{
	r.set( getColumn(0)(row), getColumn(1)(row), getColumn(2)(row), getColumn(3)(row) );
}

inline void hkMatrix4::setZero()
{
	getColumn(0).setZero4();
	getColumn(1).setZero4();
	getColumn(2).setZero4();
	getColumn(3).setZero4();
}

inline void hkMatrix4::setDiagonal( hkReal m00, hkReal m11, hkReal m22, hkReal m33 )
{
	setZero();
	(*this)(0,0) = m00;
	(*this)(1,1) = m11;
	(*this)(2,2) = m22;
	(*this)(3,3) = m33;
}

inline void hkMatrix4::setIdentity()
{
	setZero();
	hkReal one = 1.0f;
	(*this)(0,0) = one;
	(*this)(1,1) = one;
	(*this)(2,2) = one;
	(*this)(3,3) = one;
}

inline void hkMatrix4::transformPosition (const hkVector4& positionIn, hkVector4& positionOut) const
{
	//HK_WARN_ON_DEBUG_IF(!isAffineTransformation(),0x872bbf1a, "Trying to transform a position by a 4x4 matrix not representing an affine transformation");	// removing warning (note: there is still hkTransform)

	hkVector4 xb; xb.setBroadcast(positionIn, 0);
	hkVector4 yb; yb.setBroadcast(positionIn, 1);
	hkVector4 zb; zb.setBroadcast(positionIn, 2);

	hkVector4 t0;	t0.setAddMul4( m_col3, xb, m_col0 );
	t0.addMul4( yb, m_col1 );
	t0.addMul4( zb, m_col2 );

	positionOut = t0;
}

inline void hkMatrix4::multiplyVector (const hkVector4& v, hkVector4& resultOut) const
{
	hkVector4 xb; xb.setBroadcast(v, 0);
	hkVector4 yb; yb.setBroadcast(v, 1);
	hkVector4 zb; zb.setBroadcast(v, 2);
	hkVector4 wb; wb.setBroadcast(v, 3);

	hkVector4 t0;
	t0.setMul4( xb, m_col0 );
	t0.addMul4( yb, m_col1 );
	t0.addMul4( zb, m_col2 );
	t0.addMul4( wb, m_col3 );

	resultOut = t0;
}

inline void hkMatrix4::transformDirection (const hkVector4& directionIn, hkVector4& directionOut) const
{
	HK_WARN_ON_DEBUG_IF(!isAffineTransformation(),0x872bbf1c, "Trying to transform a direction by a 4x4 matrix not representing an affine transformation");

	hkVector4 xb; xb.setBroadcast(directionIn, 0);
	hkVector4 yb; yb.setBroadcast(directionIn, 1);
	hkVector4 zb; zb.setBroadcast(directionIn, 2);

	hkVector4 t0;
	t0.setMul4( xb, m_col0 );
	t0.addMul4( yb, m_col1 );
	t0.addMul4( zb, m_col2 );

	directionOut = t0;
}

HK_FORCE_INLINE void hkMatrix4::setTranspose( const hkMatrix4& s )
{
	setRows(s.getColumn(0), s.getColumn(1), s.getColumn(2), s.getColumn(3));
}

HK_FORCE_INLINE void hkMatrix4::transformInverseDirection (const hkVector4& directionIn, hkVector4& directionOut) const
{
	hkMatrix4	t; t.setTranspose(*this);
	t.transformDirection(directionIn,directionOut);
}

inline void hkMatrix4::setMulAffine ( const hkMatrix4& a, const hkMatrix4& b )
{
	HK_WARN_ON_DEBUG_IF(!a.isAffineTransformation(),0x872bbf1d, "Matrix A in setMulAffine() is not an affine transformation");
	HK_WARN_ON_DEBUG_IF(!b.isAffineTransformation(),0x872bbf1e, "Matrix B in setMulAffine() is not an affine transformation");

	// We need to make it alias save
	hkVector4 col0; a.transformDirection(b.m_col0, col0);
	hkVector4 col1; a.transformDirection(b.m_col1, col1);
	hkVector4 col2; a.transformDirection(b.m_col2, col2);
	hkVector4 col3; a.transformPosition(b.m_col3, col3);

	m_col0 = col0; m_col1 = col1; m_col2 = col2; m_col3 = col3;
}

inline void hkMatrix4::mulAffine ( const hkMatrix4& a )
{
	// SetMul is alias safe
	setMulAffine(*this, a);
}

inline const hkMatrix4& HK_CALL hkMatrix4::getIdentity()
{
	extern hkMatrix4 hkMatrix4Identity;
	return hkMatrix4Identity;	
}

HK_FORCE_INLINE hkBool32 hkMatrix4::isApproximatelyEqual( const hkMatrix4& m, hkReal epsilon) const
{
	hkVector4 epsilon_v; epsilon_v.setAll(epsilon);

	hkVector4 t0; t0.setSub4(this->m_col0, m.getColumn(0));	t0.setAbs4( t0 );
	hkVector4 t1; t1.setSub4(this->m_col1, m.getColumn(1));	t1.setAbs4( t1 );
	hkVector4 t2; t2.setSub4(this->m_col2, m.getColumn(2));	t2.setAbs4( t2 );
	hkVector4 t3; t3.setSub4(this->m_col3, m.getColumn(3));	t3.setAbs4( t3 );

	t0.setMax4( t0, t1 );
	t2.setMax4( t2, t3 );
	t0.setMax4(t0, t2 );
	return t0.compareLessThanEqual4( epsilon_v ).allAreSet( hkVector4Comparison::MASK_XYZW );
}

inline void hkMatrix4::setMul( const hkMatrix4& a, const hkMatrix4& b )
{
	HK_ASSERT(0x6d9d1d43,  this != &a );
	HK_ASSERT(0x64a8df81,  this != &b );

#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	hkVector4 col0, col1, col2, col3;

	col0.setMul4( b.m_col0.getSimdAt( 0 ), a.m_col0);
	col1.setMul4( b.m_col1.getSimdAt( 0 ), a.m_col0);
	col2.setMul4( b.m_col2.getSimdAt( 0 ), a.m_col0);
	col3.setMul4( b.m_col3.getSimdAt( 0 ), a.m_col0);

	col0.addMul4( b.m_col0.getSimdAt( 1 ), a.m_col1);
	col1.addMul4( b.m_col1.getSimdAt( 1 ), a.m_col1);
	col2.addMul4( b.m_col2.getSimdAt( 1 ), a.m_col1);
	col3.addMul4( b.m_col3.getSimdAt( 1 ), a.m_col1);

	col0.addMul4( b.m_col0.getSimdAt( 2 ), a.m_col2);
	col1.addMul4( b.m_col1.getSimdAt( 2 ), a.m_col2);
	col2.addMul4( b.m_col2.getSimdAt( 2 ), a.m_col2);
	col3.addMul4( b.m_col3.getSimdAt( 2 ), a.m_col2);

	col0.addMul4( b.m_col0.getSimdAt( 3 ), a.m_col3 );
	col1.addMul4( b.m_col1.getSimdAt( 3 ), a.m_col3 );
	col2.addMul4( b.m_col2.getSimdAt( 3 ), a.m_col3 );
	col3.addMul4( b.m_col3.getSimdAt( 3 ), a.m_col3 );
	
	m_col0 = col0;
	m_col1 = col1;
	m_col2 = col2;
	m_col3 = col3;

#else
	hkMatrix4 aTrans = a;
	aTrans.transpose();

	hkMatrix4& result = *this;

	for (int c=0; c < 4; c++)
	{
		for (int r=0; r < 4; r++)
		{
			result(r,c) = aTrans.getColumn(r).dot4( b.getColumn(c) ).getReal();
		}
	}
#endif
}


inline void hkMatrix4::mul( const hkMatrix4& a )
{
	
	hkMatrix4 temp;
	temp.setMul( *this, a );
	*this = temp;
}

inline void hkMatrix4::transpose()
{
	HK_TRANSPOSE4(m_col0,m_col1,m_col2,m_col3);
}

inline void hkMatrix4::setMul( hkSimdRealParameter scale, const hkMatrix4& a)
{
	HK_ASSERT(0x53e345d4,  this != &a);

	getColumn(0).setMul4(scale, a.getColumn(0));
	getColumn(1).setMul4(scale, a.getColumn(1));
	getColumn(2).setMul4(scale, a.getColumn(2));
	getColumn(3).setMul4(scale, a.getColumn(3));
}

inline void hkMatrix4::mul( hkSimdRealParameter scale)
{
	getColumn(0).mul4(scale);
	getColumn(1).mul4(scale);
	getColumn(2).mul4(scale);
	getColumn(3).mul4(scale);
}

inline void hkMatrix4::resetFourthRow ()
{
	m_col0.setXYZ0(m_col0);
	m_col1.setXYZ0(m_col1);
	m_col2.setXYZ0(m_col2);
	m_col3.setXYZW(m_col3, hkVector4::getConstant<HK_QUADREAL_1>());
}

inline void hkMatrix4::set(const hkTransform& t)
{
	const hkRotation& r = t.getRotation();
	m_col0 = r.getColumn(0);
	m_col1 = r.getColumn(1);
	m_col2 = r.getColumn(2);
	m_col3 = t.getTranslation();
	resetFourthRow();
}

inline void hkMatrix4::get(hkTransform& t) const
{
	t.getColumn(0) = m_col0;
	t.getColumn(1) = m_col1;
	t.getColumn(2) = m_col2;
	t.getColumn(3) = m_col3;
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
