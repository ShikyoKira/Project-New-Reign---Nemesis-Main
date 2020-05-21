/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline void hkVector4::operator=(const hkQuadReal& r)
{
	getQuad() = r;
}

HK_FORCE_INLINE void hkVector4::setSelect4( hkVector4ComparisonParameter comp, hkVector4Parameter trueValue, hkVector4Parameter falseValue)
{
	select32( falseValue, trueValue, comp) ;
}


#ifndef HK_HALF8_packFirst
#	if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
#		error Invalid implementation for hkHalf8
#	endif

HK_FORCE_INLINE void hkHalf8::packFirst(hkVector4Parameter v)
{
	hkVector4 v0; v0.setMul4( v, hkVector4::getConstant( HK_QUADREAL_PACK_HALF ));
	m_quad[0] = v0(0);
	m_quad[1] = v0(1);
	m_quad[2] = v0(2);
	m_quad[3] = v0(3);
}
HK_FORCE_INLINE void hkHalf8::packSecond(hkVector4Parameter v)
{
	hkVector4 v0; v0.setMul4( v, hkVector4::getConstant( HK_QUADREAL_PACK_HALF ));
	m_quad[4] = v0(0);
	m_quad[5] = v0(1);
	m_quad[6] = v0(2);
	m_quad[7] = v0(3);
}
#endif

#ifndef HK_HALF8_pack
#	if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
#		error Invalid implementation for hkHalf8
#	endif

HK_FORCE_INLINE void hkHalf8::pack( hkVector4Parameter v0, hkVector4Parameter v1 )
{
	packFirst(v0);
	packSecond(v1);
}

HK_FORCE_INLINE void hkHalf8::unpack(hkVector4& v0, hkVector4& v1) const
{
	unpackFirst(v0);
	unpackSecond(v1);
}

HK_FORCE_INLINE void hkHalf8::unpackFirst(hkVector4& v) const
{
	v.set(m_quad[0],m_quad[1],m_quad[2],m_quad[3]);
}
HK_FORCE_INLINE void hkHalf8::unpackSecond(hkVector4& v) const
{
	v.set(m_quad[4],m_quad[5],m_quad[6],m_quad[7]);
}
#endif


#ifndef HK_HALF8_setZero
HK_FORCE_INLINE void hkHalf8::setZero()
{
	hkVector4 zero; zero.setZero4();
	*(hkVector4*)this = zero;
}
#endif


#ifndef HK_HALF8_reducePrecision
HK_FORCE_INLINE void hkHalf8::reducePrecision( hkVector4& v )
{
	v.setAnd( v, (const hkVector4&)s_precisionMask );
}
#endif


#ifndef HK_VECTOR4_setRotatedDir
HK_FORCE_INLINE void hkVector4::setRotatedDir(const hkQuaternion& quat, hkVector4Parameter direction)
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 minusHalf; minusHalf.getQuad() = hkQuadRealMinusHalf;
	hkVector4 qreal;	qreal.setBroadcast(quat.m_vec, 3);
	hkVector4 q2minus1;	q2minus1.setAddMul4( minusHalf, qreal, qreal );

	hkVector4 ret;	ret.setMul4( direction, q2minus1 );

	hkSimdReal imagDotDir = quat.getImag().dot3( direction );
#else
	hkReal qreal = quat.m_vec(3);
	hkReal q2minus1 = qreal*qreal - 0.5f;

	hkVector4 ret;
	ret.setMul4( q2minus1, direction );

	hkReal imagDotDir = quat.getImag().dot3( direction );
#endif
	ret.addMul4( imagDotDir, quat.getImag() ); 

	hkVector4 imagCrossDir;
	imagCrossDir.setCross( quat.getImag(), direction );
	ret.addMul4( qreal, imagCrossDir );

	this->setAdd4( ret, ret );
}
#endif

inline void hkVector4::setRotatedInverseDir(const hkRotation& t, hkVector4Parameter v)
{
	_setRotatedInverseDir(t,v);
}

#if !defined(HK_VECTOR4_setSimdReal4)
#	if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED 
inline void hkVector4::set( hkSimdRealParameter a, hkSimdRealParameter b, hkSimdRealParameter c, hkSimdRealParameter d )
{
		hkVector4 v0; v0.getQuad() = a.getQuad();
		hkVector4 v1; v1.getQuad() = b.getQuad();
		hkVector4 v2; v2.getQuad() = c.getQuad();
		hkVector4 v3; v3.getQuad() = d.getQuad();
	HK_TRANSPOSE4(v0,v1,v2,v3);
		*this = v0;
}

inline void hkVector4::setHorizX( hkVector4Parameter a, hkVector4Parameter b, hkVector4Parameter c, hkVector4Parameter d )
{
	hkVector4 v0 = a;
	hkVector4 v1 = b;
	hkVector4 v2 = c;
	hkVector4 v3 = d;
	HK_TRANSPOSE4(v0,v1,v2,v3);
		*this = v0;
	}
#else
	inline void hkVector4::set( hkSimdRealParameter a, hkSimdRealParameter b, hkSimdRealParameter c, hkSimdRealParameter d )
	{
		this->set( a.getReal(), b.getReal(), c.getReal(), d.getReal() );
}

	inline void hkVector4::setHorizX( hkVector4Parameter a, hkVector4Parameter b, hkVector4Parameter c, hkVector4Parameter d )
	{
		(*this)(0) = a(0);
		(*this)(1) = b(0);
		(*this)(2) = c(0);
		(*this)(3) = d(0);
	}
#	endif
#endif

#ifndef HK_VECTOR4_setRotatedInverseDir
HK_FORCE_INLINE void hkVector4::setRotatedInverseDir(const hkQuaternion& quat, hkVector4Parameter direction)
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 minusHalf; minusHalf.getQuad() = hkQuadRealMinusHalf;
	hkVector4 qreal;
	qreal.setBroadcast(quat.m_vec, 3);
	hkVector4 q2minus1;
	q2minus1.setAddMul4( minusHalf, qreal, qreal );

	hkVector4 ret;
	ret.setMul4( direction, q2minus1 );

	hkVector4 imagDotDir; 
	imagDotDir.getQuad() = quat.getImag().dot3( direction ).getQuad();
	imagDotDir.broadcast(0);
#else
	hkReal qreal = quat.m_vec(3);
	hkReal q2minus1 = qreal*qreal - 0.5f;

	hkVector4 ret;
	ret.setMul4( q2minus1, direction );

	hkReal imagDotDir = quat.getImag().dot3( direction );
#endif
	ret.addMul4( imagDotDir, quat.getImag() ); 

	hkVector4 imagCrossDir;
	imagCrossDir.setCross( direction, quat.getImag() );
	ret.addMul4( qreal, imagCrossDir );

	this->setAdd4( ret, ret );
}
#endif

inline hkVector4Comparison hkVector4::compareGreaterThan4(hkVector4Parameter a) const
{
	return a.compareLessThan4(*this);
}

inline hkVector4Comparison hkVector4::compareGreaterThanEqual4(hkVector4Parameter a) const
{
	return a.compareLessThanEqual4(*this);
}

#ifndef HK_VECTOR4_setReciprocal3
inline void hkVector4::setReciprocal3(hkVector4Parameter v)
{
	hkVector4 v1111; v1111.getQuad() = hkVector4::getConstant(HK_QUADREAL_1);
	hkVector4 tmp; tmp.setXYZW( v, v1111 );
	setReciprocal4(tmp);
}
#endif

#ifndef HK_VECTOR4_setPlaneConstant
inline void hkVector4::setPlaneConstant(hkVector4Parameter p)
{
	hkSimdReal dist = this->dot3(p);
	setXYZW(*this, -dist);
}
#endif


#ifndef HK_VECTOR4_getInt24W
inline int hkVector4::getInt24W( ) const 
{
	return (reinterpret_cast<const int*>(this))[3] & ~0x3f000000;
}
#endif

#ifndef HK_VECTOR4_setInt24W
inline void hkVector4::setInt24W( int value ) 
{
	(reinterpret_cast<hkUint32*>(this))[3] = value |= 0x3f000000;
}
#endif

#ifndef HK_VECTOR4_getInt16W
inline int hkVector4::getInt16W( ) const 
{
#if HK_ENDIAN_LITTLE == 1
	return (reinterpret_cast<const hkUint16*>(this))[6];
#else
	return (reinterpret_cast<const hkUint16*>(this))[7];
#endif
}
#endif

#ifndef HK_VECTOR4_setAnd
inline void hkVector4::setAnd( hkVector4Parameter v0, hkVector4Parameter v1 )
{
	const hkUint32* i0 = reinterpret_cast<const hkUint32*>( &v0 );
	const hkUint32* i1 = reinterpret_cast<const hkUint32*>( &v1 );
	hkUint32* d = reinterpret_cast<hkUint32*>( this );
	d[0] = i0[0] & i1[0];
	d[1] = i0[1] & i1[1];
	d[2] = i0[2] & i1[2];
	d[3] = i0[3] & i1[3];
}
#endif

#ifndef HK_VECTOR4_storeX
inline void hkVector4::storeX( float* dest) const
{
	const hkSimdReal& s = *(const hkSimdReal*)this;
	s.store( dest );
}
#endif

inline hkBool32 hkVector4::equals3(const hkVector4 &v, hkReal epsilon) const
{
	//non_euclidean, manhattan based
	hkVector4 t;
	t.setSub4(*this, v);
	t.setAbs4( t );
	hkVector4 epsilon_v;
	epsilon_v.setAll3( epsilon );
	return t.compareLessThanEqual4( epsilon_v ).allAreSet( hkVector4Comparison::MASK_XYZ );
}

inline hkBool32 hkVector4::equals4(const hkVector4 &v, hkReal epsilon) const
{
	//non_euclidean, manhattan based
	hkVector4 t;
	t.setSub4(*this, v);
	t.setAbs4( t );
	hkVector4 epsilon_v;
	epsilon_v.setAll(epsilon);
	return t.compareLessThanEqual4( epsilon_v ).allAreSet( hkVector4Comparison::MASK_XYZW );
}

inline int hkVector4::getMajorAxis3() const
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 abs; abs.setAbs4( *this );
	abs.zeroElement( 3 );
	hkVector4 ma; ma.setHorizontalMax4( abs );
	hkVector4Comparison isMax = ma.compareLessThanEqual4( abs );
	int mask = isMax.getMask(); 
	if ( hkVector4Comparison::MASK_X == 8 )	// constant compare
	{
		return ((0x000055a0)>>(mask+mask))&3;
	}
	else
	{
		return ((0x0000aa50)>>(mask+mask))&3;
	}
#else
	hkVector4 tmp;
	tmp.setAbs4(*this);
	return tmp(0) < tmp(1) ? (tmp(1) < tmp(2) ? 2 : 1) : (tmp(0) < tmp(2) ? 2 : 0);
#endif
}

inline int hkVector4::getMaxElementIndex4() const
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 ma; ma.setHorizontalMax4( *this );
	hkVector4Comparison isMax = ma.compareLessThanEqual4( *this );
	int mask = isMax.getMask(); 
	if ( hkVector4Comparison::MASK_X == 8 )
	{		return ((0x000055af)>>(mask+mask))&3;	}
	else{	return ((0xffffaa50)>>(mask+mask))&3;	}
#else
	hkVector4 tmp = *this;

	int		xyIndex = 0;
	hkReal	xyValue = tmp(0);
	if ( tmp(0) < tmp(1) )
	{
		xyIndex = 1;
		xyValue = tmp(1);
	}

	int		zwIndex = 2;
	hkReal	zwValue = tmp(2);
	if ( tmp(2) < tmp(3) )
	{
		zwIndex = 3;
		zwValue = tmp(3);
	}
	if ( xyValue < zwValue )
	{
		return zwIndex;
	}
	return xyIndex;
#endif
}

inline const hkVector4& HK_CALL hkVector4::getConstant(hkVectorConstant constant)
{
	return reinterpret_cast<const hkVector4*>(g_vectorConstants)[constant];
}

inline const hkVector4& HK_CALL hkVector4::getZero()
{
	return getConstant( HK_QUADREAL_0 );
}

template<int vectorConstant>
inline const hkVector4& HK_CALL hkVector4::getConstant()
{
	return reinterpret_cast<const hkVector4*>(g_vectorConstants)[vectorConstant];
}


#ifndef HK_VECTOR4_distanceTo3
inline hkSimdReal hkVector4::distanceTo3(hkVector4Parameter p) const
{
	hkVector4 d; d.setSub4(*this, p);
	return d.length3();
}
#endif

#ifndef HK_VECTOR4_distanceToSquared3
inline hkSimdReal hkVector4::distanceToSquared3(hkVector4Parameter p) const
{
	hkVector4 d; d.setSub4(*this, p);
	return d.lengthSquared3();
}
#endif

inline hkResult hkVector4::normalize3IfNotZero ()
{
	const hkSimdReal lengthSqrd3 = this->lengthSquared3();
	if ( hkMath::isNegative( -lengthSqrd3 ) )
	{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
		this->mul4( hkSimdReal( hkMath::quadReciprocalSquareRoot( lengthSqrd3.getQuad() ) ) );
#else
		this->mul4( hkSimdReal( hkMath::sqrtInverse( lengthSqrd3 ) ) );
#endif
		return HK_SUCCESS;
	}
	return HK_FAILURE;
}

inline void hkVector4::load3(const hkReal* p)
{
	hkVector4& v = *this;
	v(0) = p[0];
	v(1) = p[1];
	v(2) = p[2];
}

#ifndef HK_VECTOR4_load4
inline void hkVector4::load4(const hkReal* p)
{
	hkVector4& v = *this;
	v(0) = p[0];
	v(1) = p[1];
	v(2) = p[2];
	v(3) = p[3];
}
#endif

#ifndef HK_VECTOR4_setSqrtInverse4_7BitAccuracy
	inline void hkVector4::setSqrtInverse4_7BitAccuracy(hkVector4Parameter vec)
	{
		this->setSqrtInverse4( vec );
	}
#endif

inline void hkVector4::store3(hkReal* p) const
{
	const hkVector4& v = *this;
	p[0] = v(0);
	p[1] = v(1);
	p[2] = v(2);
}

inline void hkVector4::store4(hkReal* p) const
{
	const hkVector4& v = *this;
	p[0] = v(0);
	p[1] = v(1);
	p[2] = v(2);
	p[3] = v(3);
}

#ifndef HK_VECTOR4_load4a
inline void hkVector4::load4a(const hkReal* p)
{
	hkVector4& v = *this;
	v(0) = p[0];
	v(1) = p[1];
	v(2) = p[2];
	v(3) = p[3];
}
#endif
#ifndef HK_VECTOR4_store4a
inline void hkVector4::store4a(hkReal* p) const
{
	const hkVector4& v = *this;
	p[0] = v(0);
	p[1] = v(1);
	p[2] = v(2);
	p[3] = v(3);
}
#endif

//
//	Sets this vector to the vector v transformed by qt. (this = qt * v)

inline void hkVector4::_setTransformedPos(const hkQTransform& qt, const hkVector4& v)
{
	hkVector4 temp;
	temp.setRotatedDir(qt.getRotation(), v);
	setAdd4(qt.getTranslation(), temp);
}

//
//	Sets this vector to the vector v transformed by the inverse of the transform qt. 
//	(this = qt^-1 * v)

inline void hkVector4::_setTransformedInversePos(const hkQTransform& qt, const hkVector4& v)
{
	hkVector4 temp;
	temp.setSub4(v, qt.getTranslation());

	setRotatedInverseDir(qt.getRotation(), temp);
}

//
//	Sets this vector equal to the source vector with its magnitude clamped to maxLen if necessary

#ifndef HK_VECTOR4_setClamped
inline void hkVector4::setClamped(hkVector4Parameter vSrc, const hkSimdReal& constMaxLen)
{
	// Get constants
	const hkSimdReal eps = hkSimdReal::getConstant(HK_QUADREAL_EPS);
	const hkSimdReal one = hkSimdReal::getConstant(HK_QUADREAL_1);

	// Make sure maxLen is > 0
	hkSimdReal maxLen;
	maxLen.setMax(constMaxLen, eps);

	// maxLen can be big, i.e. HK_REAL_MAX, so we can't square it
	hkSimdReal len = vSrc.length3();
	const hkVector4Comparison cmp = len.compareGreaterThan(maxLen);
	
	// len = (len > maxLen) ? len : maxLen
	len.setSelect(cmp, len, maxLen);
	hkSimdReal invLen;
	invLen.setReciprocal(len);

	// f must be >= 1, otherwise we don't clamp
	hkSimdReal f = maxLen * invLen;
	f.setMin(f, one);
	setMul4(f, vSrc);
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
