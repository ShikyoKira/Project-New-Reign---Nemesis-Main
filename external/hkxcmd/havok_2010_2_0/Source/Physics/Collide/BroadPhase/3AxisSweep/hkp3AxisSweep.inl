/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


void hkp3AxisSweep::_convertAabbToInt( const hkAabb& aabb, const hkVector4& offsetLow, const hkVector4& offsetHigh, const hkVector4& scale, hkAabbUint32& aabbOut )
{
	HK_ASSERT2(0x4868f302,  aabb.isValid(), "AABB at " << &aabb << " was invalid. (Contains a NaN or min > max)");

	HK_COMPILE_TIME_ASSERT( HK_OFFSET_EQUALS( hkAabbUint32, m_min, 0 ) );
	HK_COMPILE_TIME_ASSERT( HK_OFFSET_EQUALS( hkAabbUint32, m_max, 0x10 ) );

#if !defined(HK_BROADPHASE_32BIT)
	hkVector4 clipMin; clipMin.setZero4();
	hkVector4 clipMax; clipMax.getQuad() = hkp3AxisSweep::MaxVal;

	hkIntUnion64 mi;
	hkIntUnion64 ma;

	hkVector4Util::convertToUint16WithClip( aabb.m_min, offsetLow, scale,	clipMin, clipMax, mi );
	hkVector4Util::convertToUint16WithClip( aabb.m_max, offsetHigh, scale,	clipMin, clipMax, ma );

	aabbOut.m_min[0] = mi.u16[0] & 0xfffe;
	aabbOut.m_min[1] = mi.u16[1] & 0xfffe;
	aabbOut.m_min[2] = mi.u16[2] & 0xfffe;
	aabbOut.m_max[0] = ma.u16[0] | 1;
	aabbOut.m_max[1] = ma.u16[1] | 1;
	aabbOut.m_max[2] = ma.u16[2] | 1;
#else
	hkVector4Util::convertAabbToUint32(aabb, offsetLow, offsetHigh, scale, aabbOut);

	aabbOut.m_min[0] &= 0xfffffffe;
	aabbOut.m_min[1] &= 0xfffffffe;
	aabbOut.m_min[2] &= 0xfffffffe;
	aabbOut.m_max[0] |= 1;
	aabbOut.m_max[1] |= 1;
	aabbOut.m_max[2] |= 1;
#endif
}

// adapated from http://www.hackersdelight.org/corres.txt (search for usaddv8qi)
// adds 1 to x, makes sure that the result is <= 0xFFFFF, and sets the low bit.
// So calling this on 0xFFFD, 0xFFFE, and 0xFFFF will all return 0xFFFF
static HK_FORCE_INLINE hkUint32 add1SaturateAndSetLowBit(hkUint32 x)
{
	//hkUint32 y = 1;
	//hkUint32 t0, t1;
	//const hkUint32 signMask = 0x8000;

	//t0 = (y ^ x) & signMask;
	//t1 = (y & x) & signMask;

	//x &= ~signMask;
	//y &= ~signMask;
	//x += y;

	//t1 |= t0 & x;
	// // The original code shifted right by 7, but they were dealing with bytes
	//t1 = (t1 << 1) - (t1 >> 15);
	// // We also need to OR with 1 here
	//return ( (x ^ t0) | t1 ) | 1;

	HK_ASSERT2(0xDE115E10, x <= 0xFFFF, "Bad AABB value.  Check that the hkpWorldObject's transform doesn't contain INFINITY or NAN");

	x = (x == 0xFFFF) ? x : x+1;
	return x | 1;
}

HK_FORCE_INLINE void hkp3AxisSweep::convertAabbToBroadPhaseResolution(const hkAabbUint32& aabbIn, hkAabbUint32& aabbOut)
{
#if !defined(HK_BROADPHASE_32BIT)
	aabbOut.m_min[0] = aabbIn.m_min[0] >> 15;
	aabbOut.m_min[1] = aabbIn.m_min[1] >> 15;
	aabbOut.m_min[2] = aabbIn.m_min[2] >> 15;
	aabbOut.m_max[0] = add1SaturateAndSetLowBit(aabbIn.m_max[0] >> 15);
	aabbOut.m_max[1] = add1SaturateAndSetLowBit(aabbIn.m_max[1] >> 15);
	aabbOut.m_max[2] = add1SaturateAndSetLowBit(aabbIn.m_max[2] >> 15);

	aabbOut.m_min[0] &= 0xfffe;
	aabbOut.m_min[1] &= 0xfffe;
	aabbOut.m_min[2] &= 0xfffe;

#else
	aabbOut.m_min[0] = aabbIn.m_min[0] & 0xfffffffe;
	aabbOut.m_min[1] = aabbIn.m_min[1] & 0xfffffffe;
	aabbOut.m_min[2] = aabbIn.m_min[2] & 0xfffffffe;
	aabbOut.m_max[0] = aabbIn.m_max[0] | 1;
	aabbOut.m_max[1] = aabbIn.m_max[1] | 1;
	aabbOut.m_max[2] = aabbIn.m_max[2] | 1;
#endif
}


HK_FORCE_INLINE void hkp3AxisSweep::getOffsetLowHigh32bit(hkVector4& offsetLow32bit, hkVector4& offsetHigh32bit) const
{
	offsetLow32bit  = m_offsetLow32bit;
	offsetHigh32bit = m_offsetHigh32bit;
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
