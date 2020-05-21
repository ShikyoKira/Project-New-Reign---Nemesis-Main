/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// construction

inline const hkVector4& hkQuaternion::getImag() const
{
	return m_vec;
}

inline void hkQuaternion::setImag(const hkVector4& i)
{
	m_vec.setXYZ(i);
}


inline hkReal hkQuaternion::getReal() const
{
	return m_vec(3);
}
inline void hkQuaternion::setReal(hkReal r)
{
	m_vec(3) = r;
}

inline hkQuaternion::hkQuaternion(const hkRotation& r)
{
	set(r);
}

inline hkQuaternion::hkQuaternion(hkReal x, hkReal y, hkReal z, hkReal w)
{
	m_vec.set(x,y,z,w);
	HK_ASSERT2(0x3c15eca2,  isOk(), "hkQuaternion components were not normalized." );
}

inline hkQuaternion::hkQuaternion(const hkVector4& axis, hkReal angle)
{
	setAxisAngle(axis,angle);
}

inline void hkQuaternion::operator= (const hkQuaternion& q)
{
	m_vec = q.m_vec;
}

inline void hkQuaternion::set(hkReal x, hkReal y, hkReal z, hkReal w)
{
	m_vec.set(x,y,z,w);
	HK_ASSERT2(0x1adaad0e,  isOk(), "hkQuaternion components were not normalized." );
}

inline void hkQuaternion::setIdentity()
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	extern const hkQuadReal hkQuadReal0001;
	m_vec.getQuad() = hkQuadReal0001;
#else
	m_vec.set(0,0,0,1);
#endif
}

inline void hkQuaternion::normalize()
{
	m_vec.normalize4();
}

inline void hkQuaternion::setMul(hkSimdRealParameter r, const hkQuaternion& q)
{
	m_vec.setMul4(r,q.m_vec);
}

inline void hkQuaternion::addMul(hkSimdRealParameter r, const hkQuaternion& q)
{
	m_vec.addMul4(r,q.m_vec);
}

inline void hkQuaternion::setMul(const hkQuaternion& q0, const hkQuaternion& q1)
{
	hkVector4 vec;
	vec.setCross(q0.getImag(), q1.getImag());
	//vec.zeroElement(3);
	vec.addMul4(q0.m_vec.getSimdAt(3), q1.getImag());
	vec.addMul4(q1.m_vec.getSimdAt(3), q0.getImag());
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 dot;
	dot.setAll(q0.m_vec.dot3( q1.m_vec ));
	hkVector4 w;
	w.setMul4(q0.m_vec, q1.m_vec);
	w.sub4(dot);
	m_vec.setXYZW(vec, w);
#else
	hkReal real = q0.getReal() * q1.getReal() - hkReal( q0.m_vec.dot3( q1.m_vec ) );
	m_vec = vec;
	m_vec(3) = real;
#endif
}


inline void hkQuaternion::mul(const hkQuaternion& q)
{
	hkVector4 vec;
	vec.setCross(getImag(), q.getImag());
	vec.addMul4(m_vec.getSimdAt(3), q.getImag());
	vec.addMul4(q.m_vec.getSimdAt(3), getImag());
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 dot; 	dot.setAll(getImag().dot3( q.getImag() ));
	hkVector4 w; 	w.setMul4(m_vec, q.m_vec);
	w.sub4(dot);
	m_vec.setXYZW(vec, w);
#else
	hkReal w = getReal() * q.getReal() - hkReal(getImag().dot3( q.getImag() ));
	m_vec = vec;
	m_vec(3) = w;
#endif
}

inline void hkQuaternion::setMulInverse(const hkQuaternion& q0, const hkQuaternion& q1)
{
	hkVector4 xyz; xyz.setCross(q1.getImag(), q0.getImag());
	xyz.subMul4(q0.m_vec.getSimdAt(3), q1.getImag());
	xyz.addMul4(q1.m_vec.getSimdAt(3), q0.getImag());
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 w; w.setAll( q0.m_vec.dot4(q1.m_vec) );
	m_vec.setXYZW(xyz, w);
#else
	const hkReal w = q0.m_vec.dot4(q1.m_vec);
	m_vec.setXYZ( xyz );
	m_vec(3) = w;
#endif
}

inline void hkQuaternion::setInverseMul(const hkQuaternion& q0, const hkQuaternion& q1)
{
	hkVector4 xyz; xyz.setCross(q1.getImag(), q0.getImag());
	xyz.addMul4(q0.m_vec.getSimdAt(3), q1.getImag());
	xyz.subMul4(q1.m_vec.getSimdAt(3), q0.getImag());
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 w; w.setAll( q0.m_vec.dot4(q1.m_vec) );
	m_vec.setXYZW(xyz,w);
#else
	const hkReal w = q0.m_vec.dot4(q1.m_vec);
	m_vec.setXYZ( xyz );
	m_vec(3) = w;
#endif
}

inline void hkQuaternion::estimateAngleTo(const hkQuaternion& to, hkVector4& angleOut) const
{
	const hkQuaternion& from = *this;
	angleOut.setCross(from.getImag(),         to.getImag());
	angleOut.subMul4(to.m_vec.getSimdAt(3),   from.getImag());
	angleOut.addMul4(from.m_vec.getSimdAt(3), to.getImag());
	angleOut.add4(angleOut);
	if ( hkMath::isNegative(  to.getImag().dot4( from.getImag() ) ) )
	{
		angleOut.setNeg4( angleOut );
	}
}


void hkQuaternion::setInverse( const hkQuaternion &q )
{
	m_vec.setNeg3( q.getImag() );
}

const hkReal& hkQuaternion::operator()(int i) const
{
	return m_vec(i);
}

inline hkReal hkQuaternion::getAngle() const
{
	hkReal angle = hkMath::fabs( m_vec(3) );
	angle = hkMath::acos(angle);
	angle *= 2.0f;
	return(angle);
}

inline hkBool hkQuaternion::hasValidAxis() const
{
	return m_vec.length3().getReal() > HK_REAL_EPSILON;
}

inline void hkQuaternion::getAxis(hkVector4 &axisOut) const
{
	hkVector4 axisTmp;

	HK_ASSERT2(0x266e2bd7, hasValidAxis(), "Cannot extract axis from a Quaternion representing (within numerical tolerance) the Identity rotation (or Quaternion may not be normalized).");
	axisTmp = m_vec;
	axisTmp.normalize3();
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 zero; zero.setZero4();
	if( m_vec.compareLessThan4( zero ).anyIsSet( hkVector4Comparison::MASK_W ) )
	{
		axisTmp.setNeg4(axisTmp);
	}
#else
	if(m_vec(3) < 0.0f)
	{
		axisTmp.setNeg4(axisTmp);
	}
#endif
	axisOut = axisTmp;
}

#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	static const hkQuadReal nearlyOneQuad =			HK_QUADREAL_CONSTANT(1.0f - 1e-5f, 1.0f - 1e-5f, 1.0f - 1e-5f, 1.0f - 1e-5f);
	static const hkQuadReal somewhatNearlyNegOneQuad =	HK_QUADREAL_CONSTANT(-(1.0f - 1e-3f), -(1.0f - 1e-3f), -(1.0f - 1e-3f), -(1.0f - 1e-3f));

	inline void hkQuaternion::setShortestRotation(const hkVector4& from, const hkVector4& to)
	{
		HK_ASSERT2(0xad87ba22, from.isNormalized3() && to.isNormalized3(), "The input vectors are not normalized.");

		hkVector4 dotProd; dotProd.setAll( from.dot3( to ) ); // cos(theta)

		hkVector4 nearlyOne, somewhatNearlyNegOne;
		nearlyOne.getQuad() = nearlyOneQuad;
		somewhatNearlyNegOne.getQuad() = somewhatNearlyNegOneQuad;

		hkVector4Comparison isNearlyOne = dotProd.compareGreaterThan4(nearlyOne);

		nearlyOne.setNeg4(nearlyOne);
		hkVector4Comparison isNearlyNegativeOne = dotProd.compareLessThan4(nearlyOne);

		if( isNearlyOne.anyIsSet() )
		{
			setIdentity();
		}
		else if( isNearlyNegativeOne.anyIsSet() )
		{
			setFlippedRotation( from );
		}
		else
		{
			// Using cos(theta) = 2*cos^2(theta/2) - 1
			hkVector4 oneHalf; oneHalf.getQuad() = hkQuadRealHalf;
			hkVector4 c; c.setAddMul4(oneHalf, oneHalf, dotProd); // .5 * ( 1 + dotProd) 
			hkVector4 sqrtInverseC; sqrtInverseC.setSqrtInverse4(c);

			//const hkReal cosT2 = hkMath::sqrt( c );

			hkVector4 cross;
			cross.setCross( from, to ); // ~sin(theta)* axis -- this is not exact sin(theta) .. error around 1-e2 for angles close to 180deg

			//hkReal rescaleSin  = 0.5f / cosT2;
			hkVector4 rescaleSin; rescaleSin.setMul4(oneHalf, sqrtInverseC);

			hkVector4Comparison isSomewhatNearlyNegativeOne = dotProd.compareLessThan4(somewhatNearlyNegOne);

			//if (dotProd < -somewhatNearlyOne)
			if(isSomewhatNearlyNegativeOne.anyIsSet())
			{
				// Extra correction needed for angles around 180deg
				//
				//const hkReal sinT2 = hkMath::sqrt( cosT2 * cosT2 - dotProd );
				hkVector4 sinT4; sinT4.setSub4( c, dotProd );
				const hkSimdReal invApproxSinT = cross.lengthInverse3(); // this has errors around 1-e2 for angles around 180 deg.
				//const hkReal sinT = 2 * sinT2 * cosT2;
				//rescaleSin *= (sinT / approxSinT);
				hkVector4 invSinT2; invSinT2.setSqrtInverse4(sinT4);

				//rescaleSin = sinT2 / approxSinT;
				rescaleSin.setMul4(invSinT2, sinT4);
				rescaleSin.mul4(invApproxSinT);
			}

			// Using sin(theta) = 2*cos(theta/2)*sin(theta/2)
			cross.mul4( rescaleSin );

			hkVector4 cosT2; cosT2.setMul4(c, sqrtInverseC); // = sqrt(c)
			cross.setW( cosT2 );
			m_vec = cross;
		}

		HK_ASSERT2(0xad78999d, isOk(), "Resulting quaternion is not normalized.");
	}
#else
	inline void hkQuaternion::setShortestRotation(const hkVector4& from, const hkVector4& to)
	{
		HK_ASSERT2(0xad87ba22, from.isNormalized3() && to.isNormalized3(), "The input vectors are not normalized.");

		const hkReal dotProd = from.dot3( to ); // cos(theta)
		const hkReal nearlyOne = 1.0f - 1e-5f;
		const hkReal somewhatNearlyOne = 1.0f - 1e-3f;
		if( dotProd > nearlyOne )
		{
			setIdentity();
		}
		else if( dotProd < -nearlyOne )
		{
			setFlippedRotation( from );
		}
		else
		{
			// Using cos(theta) = 2*cos^2(theta/2) - 1
			const hkReal c = (dotProd + 1.0f) * 0.5f;
			const hkReal cosT2 = hkMath::sqrt( c );

			hkVector4 cross;
			cross.setCross( from, to ); // ~sin(theta)* axis -- this is not exact sin(theta) .. error around 1-e2 for angles close to 180deg

			hkReal rescaleSin  = 0.5f / cosT2;

			if (dotProd < -somewhatNearlyOne)
			{
				// Extra correction needed for angles around 180deg
				//
				const hkReal sinT2 = hkMath::sqrt( cosT2 * cosT2 - dotProd );
				const hkReal approxSinT = cross.length3(); // this has errors around 1-e2 for angles around 180 deg.
				const hkReal sinT = 2 * sinT2 * cosT2;
				rescaleSin *= (sinT / approxSinT);
			}

			// Using sin(theta) = 2*cos(theta/2)*sin(theta/2)
			cross.mul4( rescaleSin );
			cross(3) = cosT2;
			m_vec = cross;
		}

		HK_ASSERT2(0xad78999d, isOk(), "Resulting quaternion is not normalized.");
	}
#endif

inline void hkQuaternion::setShortestRotationDamped(hkReal gain, const hkVector4& from, const hkVector4& to)
{
	const hkReal dotProd = from.dot3( to ).getReal(); // cos(theta)
	const hkReal dampedDot = 1.0f - gain + gain * dotProd;
	const hkReal nearlyOne = 1.0f - 1e-6f;

	const hkReal c = (dampedDot + 1.0f) * 0.5f;
	if( (c <= 0.0f) || (dotProd < -nearlyOne) )
	{
		setFlippedRotation( from );
	}
	else
	{
		if( dotProd > nearlyOne )
		{
			setIdentity();
		}
		else
		{
			const hkReal cosT2 = hkMath::sqrt( c );

			hkVector4 cross;
			cross.setCross( from, to ); // sin(theta)* axis

			// Using sin(theta) = 2*cos(theta/2)*sin(theta/2)
			const hkReal rescaleSin  = gain * 0.5f / cosT2;
			cross.mul4( hkSimdReal( rescaleSin ) );
			cross(3) = cosT2;

			// renormalize for gain.
			cross.normalize4();
			m_vec = cross;
		}
	}

	HK_ASSERT2(0xad78999e, isOk(), "Resulting quaternion is not normalized.");
}

//
//	Sets the quaternion to the given quaternion, eventually transforming it so they are in the same hemisphere

inline void hkQuaternion::setClosest(const hkQuaternion& q, const hkQuaternion& qReference)
{
	m_vec.setMulSigns4(q.m_vec, qReference.m_vec.dot4(q.m_vec));
}

//
//	Computes the logarithm of a quaternion

inline void hkQuaternion::getLog(hkVector4& vOut) const
{
	// Get axis and cos(angle)
	const hkReal cosA = getReal();
	vOut = getImag();

	// If the angle is not zero, return angle * axis,
	// otherwise return just axis, because axis = sin(angle) * direction
	// and for very small angles, the angle / sin(angle) -> 1.
	if ( hkMath::fabs(cosA) < 1.0f )
	{
		const hkReal angle = hkMath::acos(cosA);
		const hkReal sinA = hkMath::sin(angle);

		if ( hkMath::fabs(sinA) >= HK_REAL_EPSILON )
		{
			vOut.mul4( hkSimdReal(angle / sinA) );
		}
	}
}

//
//	Set the quaternion as the exponential of the given (axis-angle) vector

inline void hkQuaternion::setExp(const hkVector4& vIn)
{
	hkReal angle = vIn.length3().getReal();
	hkReal sinA = hkMath::sin(angle);

	setReal(hkMath::cos(angle));
	hkVector4 quatDir = vIn;
	if (hkMath::fabs(sinA) >= HK_REAL_EPSILON)
	{
		quatDir.mul4( hkSimdReal(sinA / angle) );
	}
	setImag(quatDir);
}

//
//	Inlined. Sets/initializes this quaternion from a given rotation matrix.
//	The rotation r must be orthonormal.

inline void hkQuaternion::_set(const hkRotation& r)
{
#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)

	setFromRotationSimd(r);

#else

	setFromRotationFpu(r);

#endif
}

//
//	Inlined. Sets/initializes this quaternion from a given rotation matrix.
//	The rotation r must be orthonormal.

inline void hkQuaternion::setFromRotationSimd(const hkRotation& r)
{
	// Get matrix columns
	const hkVector4& col0 = r.getColumn(0);
	const hkVector4& col1 = r.getColumn(1);
	const hkVector4& col2 = r.getColumn(2);

	// Get constants
	const hkSimdReal zero = hkSimdReal::getConstant(HK_QUADREAL_0);
	const hkSimdReal half = hkSimdReal::getConstant(HK_QUADREAL_INV_2);
	const hkSimdReal one = hkSimdReal::getConstant(HK_QUADREAL_1);

	// Get diagonal elements
	const hkSimdReal m00 = col0.getSimdAt(0);
	const hkSimdReal m11 = col1.getSimdAt(1);
	const hkSimdReal m22 = col2.getSimdAt(2);

	// Compute s = 1 + [m00 - m11 - m22, -m00 + m11 - m22, -m00 -m11 + m22, m00 + m11 + m22] = 
	//			 = 1 + [m00, -m00, -m00, m00] + [-m11, m11, -m11, m11] + [-m22, -m22, m22, m22]
	hkVector4 s = hkVector4::getConstant<HK_QUADREAL_1>();
	{
		// sgn2 = [-1, 1, -1, 1]
		const hkVector4 sgn2 = hkVector4::getConstant<HK_QUADREAL_m11m11>();

		// Shuffle signs
		hkVector4 sgn1;	// sgn1 = [1, -1, -1, 1]				
		HK_VECTOR4_PERM1(sgn1, sgn2, HK_VECTOR4_PERM1ARG(1, 0, 2, 3));
		hkVector4 sgn3;	// sgn3 = [-1, -1, 1, 1]
		HK_VECTOR4_PERM1(sgn3, sgn2, HK_VECTOR4_PERM1ARG(0, 2, 1, 3));

		s.addMul4(m00, sgn1);
		s.addMul4(m11, sgn2);
		s.addMul4(m22, sgn3);
	}

	// trace = 1 + m00 + m11 + m22
	const hkSimdReal trace = s.getSimdAt(3);

	// We need to compute the 4 possible quaternions
	//	qr0 = [m21 - m12, m02 - m20, m10 - m01, *]
	//	qr1 = [*, m10 + m01, m02 + m20, m21 - m12]
	//	qr2 = [m10 + m01, *, m21 + m12, m02 - m20]
	//	qr3 = [m02 + m20, m21 + m12, *, m10 - m01]

	// We need to evaluate the sum [m10, m02, m21] +/- [m01, m20, m12]
	hkVector4 qr0, qr1, qr2, qr3;
	{
		hkVector4 tmp1;	// tmp1 = [m21, m02, m10, 0]
		tmp1.set(col1.getSimdAt(2), col2.getSimdAt(0), col0.getSimdAt(1), zero);
		hkVector4 tmp2;	// tmp2 = [m12, m20, m01, 0]
		tmp2.set(col2.getSimdAt(1), col0.getSimdAt(2), col1.getSimdAt(0), zero);

		// qr3 = [m21 - m12, m02 - m20, m10 - m01, 0]
		qr3.setSub4(tmp1, tmp2);

		hkVector4 sum;	// sum = [m21 + m12, m02 + m20, m10 + m01, 0]
		sum.setAdd4(tmp1, tmp2);

		// qr0 = [0, m10 + m01, m02 + m20, m21 + m12]
		HK_VECTOR4_PERM1(qr0, sum, HK_VECTOR4_PERM1ARG(3, 2, 1, 0));

		// qr1 = [m10 + m01, 0, m21 + m12, m02 + m20]
		HK_VECTOR4_PERM1(qr1, sum, HK_VECTOR4_PERM1ARG(2, 3, 0, 1));

		// qr2 = [m02 + m20, m21 + m12, 0, m10 + m01]
		HK_VECTOR4_PERM1(qr2, sum, HK_VECTOR4_PERM1ARG(1, 0, 3, 2));

		qr0.setComponent<hkVector4Comparison::MASK_W>(qr3.getSimdAt(0));	// qr0 = [0, m10 + m01, m02 + m20, m21 - m12]
		qr1.setComponent<hkVector4Comparison::MASK_W>(qr3.getSimdAt(1));	// qr1 = [m10 + m01, 0, m21 + m12, m02 - m20]
		qr2.setComponent<hkVector4Comparison::MASK_W>(qr3.getSimdAt(2));	// qr2 = [m02 + m20, m21 + m12, 0, m10 - m01]
	}

	// Compute square root of s, and finish computing the quaternions
	{
		hkVector4 t;	// t = 0.5 / sqrt(s)
		t.setSqrtInverse4(s);
		t.mul4(half);

		qr0.setComponent<hkVector4Comparison::MASK_X>(s);
		qr1.setComponent<hkVector4Comparison::MASK_Y>(s);
		qr2.setComponent<hkVector4Comparison::MASK_Z>(s);
		qr3.setComponent<hkVector4Comparison::MASK_W>(s);

		qr0.mul4(t.getSimdAt(0));
		qr1.mul4(t.getSimdAt(1));
		qr2.mul4(t.getSimdAt(2));
		qr3.mul4(t.getSimdAt(3));
	}

	// Select proper quaternion
	{
		const hkVector4Comparison m00_gt_m11 = m00.compareGreaterThan(m11);
		const hkVector4Comparison m11_gt_m22 = m11.compareGreaterThan(m22);
		const hkVector4Comparison m22_gt_m00 = m22.compareGreaterThan(m00);
		const hkVector4Comparison trace_le_0 = trace.compareLessThanEqual(one);

		// cmpq1 = (m00 > m11) && (trace <= 0) && !(m22 > m00)
		// cmpq2 = (m11 > m22) && (trace <= 0) && !(m00 > m11)
		// cmpq3 = (m22 > m00) && (trace <= 0) && !(m11 > m22)
		hkVector4Comparison cmpq1, cmpq2, cmpq3;
		cmpq1.setAnd(m00_gt_m11, trace_le_0);
		cmpq2.setAnd(m11_gt_m22, trace_le_0);
		cmpq3.setAnd(m22_gt_m00, trace_le_0);

		cmpq1.setAndNot(cmpq1, m22_gt_m00);
		cmpq2.setAndNot(cmpq2, m00_gt_m11);
		cmpq3.setAndNot(cmpq3, m11_gt_m22);

		hkVector4 q;
		q.setSelect4(cmpq1, qr0, qr3);
		q.setSelect4(cmpq2, qr1, q);
		m_vec.setSelect4(cmpq3, qr2, q);
	}

	// Note: we don't renormalize here as we assume the rotation is orthonormal
	HK_ASSERT2(0x70dc41cb, isOk(), "hkRotation used for hkQuaternion construction is invalid. hkQuaternion is not normalized/invalid!");
}

//
//	Inlined. Sets/initializes this quaternion from a given rotation matrix.
//	The rotation r must be orthonormal.

inline void hkQuaternion::setFromRotationFpu(const hkRotation& r)
{
	hkReal trace = r(0,0) + r(1,1) + r(2,2);
	hkQuadRealUnion u;

	// This is an exceptional case:
	// if trace==-1.0 since this means real=sqrt(trace+1) =0.0
	// hence we can't use real to compute the imaginary terms
	// if trace is close to -1.0, then the normal algorithm is
	// subject to numerical error.
	// Either way, we should use an alternate algorithm.
	// Please see doc "Numerical Problem In Quaternion-Matrix Conversion.doc"
	if( trace > 0 )
	{
		// else we calculate simply:
		hkReal s = hkMath::sqrt( trace + 1.0f );
		hkReal t = 0.5f / s;
		u.r[0] = (r(2,1)-r(1,2)) * t;
		u.r[1] = (r(0,2)-r(2,0)) * t;
		u.r[2] = (r(1,0)-r(0,1)) * t;
		u.r[3] = 0.5f*s;
	}
	else
	{
		const int next[] = {1,2,0};
		int i=0;

		if(r(1,1) > r(0,0)) i=1;
		if(r(2,2) > r(i,i)) i=2;

		int j = next[i];
		int k = next[j];

		hkReal s = hkMath::sqrt(r(i,i) - (r(j,j)+r(k,k)) + 1.0f);
		hkReal t = 0.5f / s;

		u.r[i] = 0.5f * s;
		u.r[3] = (r(k,j)-r(j,k)) * t;
		u.r[j] = (r(j,i)+r(i,j)) * t;
		u.r[k] = (r(k,i)+r(i,k)) * t;
	}
	m_vec.getQuad() = u.q;
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
