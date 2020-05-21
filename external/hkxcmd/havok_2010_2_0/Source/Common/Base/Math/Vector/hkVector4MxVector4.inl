/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


namespace hkCompileError
{
	template <bool b> struct SUBVECTOR_INDEX_OUT_OF_RANGE;
	template <> struct SUBVECTOR_INDEX_OUT_OF_RANGE<true>{ };

	template <bool b> struct UNKNOWN_PERMUTATION;
	template <> struct UNKNOWN_PERMUTATION<true>{ };
}
#define INDEX_CHECK HK_COMPILE_TIME_ASSERT2((I>=0)&&(I<=3), SUBVECTOR_INDEX_OUT_OF_RANGE)

HK_FORCE_INLINE hkMxVector4::hkMxVector4(hkVector4Parameter v)
{
	m_vec.m_vec[0] = v;
	m_vec.m_vec[1] = v;
	m_vec.m_vec[2] = v;
	m_vec.m_vec[3] = v; 
}

HK_FORCE_INLINE hkMxVector4::hkMxVector4(hkMxRealParameter v)
{
	m_vec.m_vec[0] = v.m_real.m_vec[0];
	m_vec.m_vec[1] = v.m_real.m_vec[1];
	m_vec.m_vec[2] = v.m_real.m_vec[2];
	m_vec.m_vec[3] = v.m_real.m_vec[3];
}

HK_FORCE_INLINE hkMxVector4::hkMxVector4(hkReal v)
{
	m_vec.m_vec[0].setAll(v);
	m_vec.m_vec[1].setAll(v);
	m_vec.m_vec[2].setAll(v);
	m_vec.m_vec[3].setAll(v);
}

HK_FORCE_INLINE void hkMxVector4::load(const hkVector4* v)
{
	m_vec.m_vec[0] = v[0];
	m_vec.m_vec[1] = v[1];
	m_vec.m_vec[2] = v[2];
	m_vec.m_vec[3] = v[3];
}

template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE void hkMxVector4::gather(const hkVector4* HK_RESTRICT base)
{
	m_vec.m_vec[0] = *hkAddByteOffsetConst( base, 0*byteAddressIncrement );
	m_vec.m_vec[1] = *hkAddByteOffsetConst( base, 1*byteAddressIncrement );
	m_vec.m_vec[2] = *hkAddByteOffsetConst( base, 2*byteAddressIncrement );
	m_vec.m_vec[3] = *hkAddByteOffsetConst( base, 3*byteAddressIncrement );
}

HK_FORCE_INLINE void hkMxVector4::gatherSLOW(const hkVector4* base, hkUint32 byteAddressIncrement)
{
	m_vec.m_vec[0] = *hkAddByteOffsetConst( base, 0*byteAddressIncrement );
	m_vec.m_vec[1] = *hkAddByteOffsetConst( base, 1*byteAddressIncrement );
	m_vec.m_vec[2] = *hkAddByteOffsetConst( base, 2*byteAddressIncrement );
	m_vec.m_vec[3] = *hkAddByteOffsetConst( base, 3*byteAddressIncrement );
}

HK_FORCE_INLINE void hkMxVector4::gather(const hkVector4* base, hkUint32 byteAddressIncrement, const hkUint16* indices)
{
	m_vec.m_vec[0] = *hkAddByteOffsetConst( base, indices[0] * byteAddressIncrement );
	m_vec.m_vec[1] = *hkAddByteOffsetConst( base, indices[1] * byteAddressIncrement );
	m_vec.m_vec[2] = *hkAddByteOffsetConst( base, indices[2] * byteAddressIncrement );
	m_vec.m_vec[3] = *hkAddByteOffsetConst( base, indices[3] * byteAddressIncrement );
}

HK_FORCE_INLINE void hkMxVector4::gather(const hkVector4* base, hkUint32 byteAddressIncrement, const hkInt32* indices)
{
	m_vec.m_vec[0] = *hkAddByteOffsetConst( base, indices[0] * byteAddressIncrement );
	m_vec.m_vec[1] = *hkAddByteOffsetConst( base, indices[1] * byteAddressIncrement );
	m_vec.m_vec[2] = *hkAddByteOffsetConst( base, indices[2] * byteAddressIncrement );
	m_vec.m_vec[3] = *hkAddByteOffsetConst( base, indices[3] * byteAddressIncrement );
}

HK_FORCE_INLINE void hkMxVector4::setXYZ(hkMxVector4Parameter v)
{
	m_vec.m_vec[0].setXYZ(v.m_vec.m_vec[0]);
	m_vec.m_vec[1].setXYZ(v.m_vec.m_vec[1]);
	m_vec.m_vec[2].setXYZ(v.m_vec.m_vec[2]);
	m_vec.m_vec[3].setXYZ(v.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::set(hkMxVector4Parameter v)
{
	m_vec.m_vec[0] = v.m_vec.m_vec[0];
	m_vec.m_vec[1] = v.m_vec.m_vec[1];
	m_vec.m_vec[2] = v.m_vec.m_vec[2];
	m_vec.m_vec[3] = v.m_vec.m_vec[3];
}

HK_FORCE_INLINE void hkMxVector4::setW(hkMxVector4Parameter v)
{
	m_vec.m_vec[0].setW(v.m_vec.m_vec[0]);
	m_vec.m_vec[1].setW(v.m_vec.m_vec[1]);
	m_vec.m_vec[2].setW(v.m_vec.m_vec[2]);
	m_vec.m_vec[3].setW(v.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setW(hkMxSingleParameter s)
{
	m_vec.m_vec[0].setW(s.m_single);
	m_vec.m_vec[1].setW(s.m_single);
	m_vec.m_vec[2].setW(s.m_single);
	m_vec.m_vec[3].setW(s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::setXYZW(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	setXYZ(v0);
	setW(v1);
}

HK_FORCE_INLINE void hkMxVector4::setZero()
{
	hkVector4 zero; zero.setZero4();
	m_vec.m_vec[0] = zero;
	m_vec.m_vec[1] = zero;
	m_vec.m_vec[2] = zero;
	m_vec.m_vec[3] = zero;
}

HK_FORCE_INLINE void hkMxVector4::setCross(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	m_vec.m_vec[0].setCross(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0]);
	m_vec.m_vec[1].setCross(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1]);
	m_vec.m_vec[2].setCross(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2]);
	m_vec.m_vec[3].setCross(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setCross(hkMxSingleParameter s, hkMxVector4Parameter v)
{
	m_vec.m_vec[0].setCross(s.m_single, v.m_vec.m_vec[0]);
	m_vec.m_vec[1].setCross(s.m_single, v.m_vec.m_vec[1]);
	m_vec.m_vec[2].setCross(s.m_single, v.m_vec.m_vec[2]);
	m_vec.m_vec[3].setCross(s.m_single, v.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setCross(hkMxVector4Parameter v, hkMxSingleParameter s)
{
	m_vec.m_vec[0].setCross(v.m_vec.m_vec[0], s.m_single);
	m_vec.m_vec[1].setCross(v.m_vec.m_vec[1], s.m_single);
	m_vec.m_vec[2].setCross(v.m_vec.m_vec[2], s.m_single);
	m_vec.m_vec[3].setCross(v.m_vec.m_vec[3], s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::getVectorSLOW(int i, hkVector4& vOut) const
{
	HK_ASSERT2(0x354ff00, (i>=0)&&(i<=3), "subvector index out of range");
	vOut = m_vec.m_vec[i];
}

template <int I>
HK_FORCE_INLINE void hkMxVector4::getVector(hkVector4& vOut) const
{
	INDEX_CHECK;
	vOut = m_vec.m_vec[I];
}

template <int I>
HK_FORCE_INLINE hkVector4 hkMxVector4::getVector() const
{
	INDEX_CHECK;
	return m_vec.m_vec[I];
}

template <int I>
HK_FORCE_INLINE void hkMxVector4::setVector(hkVector4Parameter v)
{
	INDEX_CHECK;
	m_vec.m_vec[I] = v;
}

HK_FORCE_INLINE void hkMxVector4::setVectorSLOW(int i, hkVector4Parameter v)
{
	HK_ASSERT2(0x354ff00, (i>=0)&&(i<=3), "subvector index out of range");
	m_vec.m_vec[i] = v;
}

template <int I>
HK_FORCE_INLINE void hkMxVector4::getSingle(hkMxSingle& vOut) const
{
	INDEX_CHECK;
	vOut.m_single = m_vec.m_vec[I];
}

HK_FORCE_INLINE void hkMxVector4::store(hkVector4* v) const
{
	v[0] = m_vec.m_vec[0];
	v[1] = m_vec.m_vec[1];
	v[2] = m_vec.m_vec[2];
	v[3] = m_vec.m_vec[3];
}

HK_FORCE_INLINE void hkMxVector4::storeTransposed4(hkVector4* matrix4) const
{
	hkVector4 q0 = m_vec.m_vec[0];
	hkVector4 q1 = m_vec.m_vec[1];
	hkVector4 q2 = m_vec.m_vec[2];
	hkVector4 q3 = m_vec.m_vec[3];
	HK_TRANSPOSE4( q0,q1,q2,q3);
	matrix4[0] = q0;
	matrix4[1] = q1;
	matrix4[2] = q2;
	matrix4[3] = q3;
}

template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE void hkMxVector4::scatter(hkVector4* base) const
{
	*hkAddByteOffset(base, 0*byteAddressIncrement) = m_vec.m_vec[0];
	*hkAddByteOffset(base, 1*byteAddressIncrement) = m_vec.m_vec[1];
	*hkAddByteOffset(base, 2*byteAddressIncrement) = m_vec.m_vec[2];
	*hkAddByteOffset(base, 3*byteAddressIncrement) = m_vec.m_vec[3];
}

HK_FORCE_INLINE void hkMxVector4::scatterSLOW(hkVector4* base, hkUint32 byteAddressIncrement) const
{
	*hkAddByteOffset(base, 0*byteAddressIncrement) = m_vec.m_vec[0];
	*hkAddByteOffset(base, 1*byteAddressIncrement) = m_vec.m_vec[1];
	*hkAddByteOffset(base, 2*byteAddressIncrement) = m_vec.m_vec[2];
	*hkAddByteOffset(base, 3*byteAddressIncrement) = m_vec.m_vec[3];
}

HK_FORCE_INLINE void hkMxVector4::scatter(hkVector4* HK_RESTRICT base, hkUint32 byteAddressIncrement, const hkUint16* HK_RESTRICT indices) const
{
	hkVector4* HK_RESTRICT ptr0 = hkAddByteOffset(base, indices[0] * byteAddressIncrement);
	hkVector4* HK_RESTRICT ptr1 = hkAddByteOffset(base, indices[1] * byteAddressIncrement);
	hkVector4* HK_RESTRICT ptr2 = hkAddByteOffset(base, indices[2] * byteAddressIncrement);
	hkVector4* HK_RESTRICT ptr3 = hkAddByteOffset(base, indices[3] * byteAddressIncrement);
	*ptr0 = m_vec.m_vec[0];
	*ptr1 = m_vec.m_vec[1];
	*ptr2 = m_vec.m_vec[2];
	*ptr3 = m_vec.m_vec[3];
}

HK_FORCE_INLINE void hkMxVector4::scatter(hkVector4* HK_RESTRICT base, hkUint32 byteAddressIncrement, const hkInt32* HK_RESTRICT indices) const
{
	//const hkInt32* indices
	hkVector4* HK_RESTRICT ptr0 = hkAddByteOffset(base, indices[0] * byteAddressIncrement);
	hkVector4* HK_RESTRICT ptr1 = hkAddByteOffset(base, indices[1] * byteAddressIncrement);
	hkVector4* HK_RESTRICT ptr2 = hkAddByteOffset(base, indices[2] * byteAddressIncrement);
	hkVector4* HK_RESTRICT ptr3 = hkAddByteOffset(base, indices[3] * byteAddressIncrement);
	*ptr0 = m_vec.m_vec[0];
	*ptr1 = m_vec.m_vec[1];
	*ptr2 = m_vec.m_vec[2];
	*ptr3 = m_vec.m_vec[3];
}

HK_FORCE_INLINE void hkMxVector4::compareGreaterThan(hkMxVector4Parameter v, hkMxMask4& mask) const
{
	v.compareLessThan(*this, mask);
}

HK_FORCE_INLINE void hkMxVector4::compareGreaterThan(hkMxSingleParameter s, hkMxMask4& mask) const
{
	mask.set( s.m_single.compareLessThan4(m_vec.m_vec[0]), 
			  s.m_single.compareLessThan4(m_vec.m_vec[1]),
			  s.m_single.compareLessThan4(m_vec.m_vec[2]),
			  s.m_single.compareLessThan4(m_vec.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxVector4::compareLessThan(hkMxVector4Parameter v, hkMxMask4& mask) const
{
	mask.set( m_vec.m_vec[0].compareLessThan4(v.m_vec.m_vec[0]), 
			  m_vec.m_vec[1].compareLessThan4(v.m_vec.m_vec[1]),
			  m_vec.m_vec[2].compareLessThan4(v.m_vec.m_vec[2]),
			  m_vec.m_vec[3].compareLessThan4(v.m_vec.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxVector4::compareLessThan(hkMxSingleParameter s, hkMxMask4& mask) const
{
	mask.set( m_vec.m_vec[0].compareLessThan4(s.m_single), 
			  m_vec.m_vec[1].compareLessThan4(s.m_single),
			  m_vec.m_vec[2].compareLessThan4(s.m_single),
			  m_vec.m_vec[3].compareLessThan4(s.m_single) );
}

HK_FORCE_INLINE void hkMxVector4::compareLessThanEqual(hkMxVector4Parameter v, hkMxMask4& mask) const
{
	mask.set( m_vec.m_vec[0].compareLessThanEqual4(v.m_vec.m_vec[0]),
			  m_vec.m_vec[1].compareLessThanEqual4(v.m_vec.m_vec[1]),
			  m_vec.m_vec[2].compareLessThanEqual4(v.m_vec.m_vec[2]),
			  m_vec.m_vec[3].compareLessThanEqual4(v.m_vec.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxVector4::compareEqual(hkMxVector4Parameter v, hkMxMask4& mask) const
{
	mask.set( m_vec.m_vec[0].compareEqual4(v.m_vec.m_vec[0]),
			  m_vec.m_vec[1].compareEqual4(v.m_vec.m_vec[1]),
			  m_vec.m_vec[2].compareEqual4(v.m_vec.m_vec[2]),
			  m_vec.m_vec[3].compareEqual4(v.m_vec.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxVector4::compareEqual(hkMxSingleParameter s, hkMxMask4& mask) const
{
	mask.set( m_vec.m_vec[0].compareEqual4(s.m_single),
			  m_vec.m_vec[1].compareEqual4(s.m_single),
			  m_vec.m_vec[2].compareEqual4(s.m_single),
			  m_vec.m_vec[3].compareEqual4(s.m_single) );
}

HK_FORCE_INLINE void hkMxVector4::setSub(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	m_vec.m_vec[0].setSub4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0]);
	m_vec.m_vec[1].setSub4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1]);
	m_vec.m_vec[2].setSub4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2]);
	m_vec.m_vec[3].setSub4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setSub(hkMxVector4Parameter v, hkMxSingleParameter s)
{
	m_vec.m_vec[0].setSub4(v.m_vec.m_vec[0], s.m_single);
	m_vec.m_vec[1].setSub4(v.m_vec.m_vec[1], s.m_single);
	m_vec.m_vec[2].setSub4(v.m_vec.m_vec[2], s.m_single);
	m_vec.m_vec[3].setSub4(v.m_vec.m_vec[3], s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::sub(hkMxVector4Parameter m)
{
	m_vec.m_vec[0].sub4(m.m_vec.m_vec[0]);
	m_vec.m_vec[1].sub4(m.m_vec.m_vec[1]);
	m_vec.m_vec[2].sub4(m.m_vec.m_vec[2]);
	m_vec.m_vec[3].sub4(m.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::sub(hkMxSingleParameter s)
{
	m_vec.m_vec[0].sub4(s.m_single);
	m_vec.m_vec[1].sub4(s.m_single);
	m_vec.m_vec[2].sub4(s.m_single);
	m_vec.m_vec[3].sub4(s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::setSubMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, hkMxVector4Parameter v2)
{
	m_vec.m_vec[0].setSubMul4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0], v2.m_vec.m_vec[0]);
	m_vec.m_vec[1].setSubMul4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1], v2.m_vec.m_vec[1]);
	m_vec.m_vec[2].setSubMul4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2], v2.m_vec.m_vec[2]);
	m_vec.m_vec[3].setSubMul4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3], v2.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setSubMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, hkMxSingleParameter s)
{
	m_vec.m_vec[0].setSubMul4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0], s.m_single);
	m_vec.m_vec[1].setSubMul4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1], s.m_single);
	m_vec.m_vec[2].setSubMul4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2], s.m_single);
	m_vec.m_vec[3].setSubMul4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3], s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::setAdd(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	m_vec.m_vec[0].setAdd4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0]);
	m_vec.m_vec[1].setAdd4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1]);
	m_vec.m_vec[2].setAdd4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2]);
	m_vec.m_vec[3].setAdd4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setAdd(hkMxVector4Parameter v0, hkMxSingleParameter s)
{
	m_vec.m_vec[0].setAdd4(v0.m_vec.m_vec[0], s.m_single);
	m_vec.m_vec[1].setAdd4(v0.m_vec.m_vec[1], s.m_single);
	m_vec.m_vec[2].setAdd4(v0.m_vec.m_vec[2], s.m_single);
	m_vec.m_vec[3].setAdd4(v0.m_vec.m_vec[3], s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::add(hkMxVector4Parameter m)
{
	m_vec.m_vec[0].add4(m.m_vec.m_vec[0]);
	m_vec.m_vec[1].add4(m.m_vec.m_vec[1]);
	m_vec.m_vec[2].add4(m.m_vec.m_vec[2]);
	m_vec.m_vec[3].add4(m.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::add(hkMxSingleParameter s)
{
	m_vec.m_vec[0].add4(s.m_single);
	m_vec.m_vec[1].add4(s.m_single);
	m_vec.m_vec[2].add4(s.m_single);
	m_vec.m_vec[3].add4(s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::addMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	m_vec.m_vec[0].addMul4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0]);
	m_vec.m_vec[1].addMul4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1]);
	m_vec.m_vec[2].addMul4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2]);
	m_vec.m_vec[3].addMul4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::addMul(hkMxVector4Parameter v, hkMxSingleParameter s)
{
	m_vec.m_vec[0].addMul4(v.m_vec.m_vec[0], s.m_single);
	m_vec.m_vec[1].addMul4(v.m_vec.m_vec[1], s.m_single);
	m_vec.m_vec[2].addMul4(v.m_vec.m_vec[2], s.m_single);
	m_vec.m_vec[3].addMul4(v.m_vec.m_vec[3], s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::addMul(hkMxRealParameter r, hkMxVector4Parameter v)
{
	m_vec.m_vec[0].addMul4(r.m_real.m_vec[0], v.m_vec.m_vec[0]);
	m_vec.m_vec[1].addMul4(r.m_real.m_vec[1], v.m_vec.m_vec[1]);
	m_vec.m_vec[2].addMul4(r.m_real.m_vec[2], v.m_vec.m_vec[2]);
	m_vec.m_vec[3].addMul4(r.m_real.m_vec[3], v.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setAddMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, hkMxVector4Parameter v2)
{
	m_vec.m_vec[0].setAddMul4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0], v2.m_vec.m_vec[0]);
	m_vec.m_vec[1].setAddMul4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1], v2.m_vec.m_vec[1]);
	m_vec.m_vec[2].setAddMul4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2], v2.m_vec.m_vec[2]);
	m_vec.m_vec[3].setAddMul4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3], v2.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setAddMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, hkMxSingleParameter s)
{
	m_vec.m_vec[0].setAddMul4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0], s.m_single);
	m_vec.m_vec[1].setAddMul4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1], s.m_single);
	m_vec.m_vec[2].setAddMul4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2], s.m_single);
	m_vec.m_vec[3].setAddMul4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3], s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::setAddMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1, const hkMxPackedReal& v2)
{
	m_vec.m_vec[0].setAddMul4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0], v2.getSimdAt(0));
	m_vec.m_vec[1].setAddMul4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1], v2.getSimdAt(1));
	m_vec.m_vec[2].setAddMul4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2], v2.getSimdAt(2));
	m_vec.m_vec[3].setAddMul4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3], v2.getSimdAt(3));
}

HK_FORCE_INLINE void hkMxVector4::select(hkMxMask4Parameter mask, hkMxVector4Parameter trueValue, hkMxVector4Parameter falseValue )
{
	m_vec.m_vec[0].setSelect4(mask.get<0>(), trueValue.m_vec.m_vec[0], falseValue.m_vec.m_vec[0] );
	m_vec.m_vec[1].setSelect4(mask.get<1>(), trueValue.m_vec.m_vec[1], falseValue.m_vec.m_vec[1] );
	m_vec.m_vec[2].setSelect4(mask.get<2>(), trueValue.m_vec.m_vec[2], falseValue.m_vec.m_vec[2] );
	m_vec.m_vec[3].setSelect4(mask.get<3>(), trueValue.m_vec.m_vec[3], falseValue.m_vec.m_vec[3] );
}

HK_FORCE_INLINE void hkMxVector4::select(hkMxMask4Parameter mask, hkMxSingleParameter trueSingle, hkMxVector4Parameter falseVector )
{
	m_vec.m_vec[0].setSelect4(mask.get<0>(), trueSingle.m_single, falseVector.m_vec.m_vec[0]);
	m_vec.m_vec[1].setSelect4(mask.get<1>(), trueSingle.m_single, falseVector.m_vec.m_vec[1]);
	m_vec.m_vec[2].setSelect4(mask.get<2>(), trueSingle.m_single, falseVector.m_vec.m_vec[2]);
	m_vec.m_vec[3].setSelect4(mask.get<3>(), trueSingle.m_single, falseVector.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::select(hkMxMask4Parameter mask, hkMxVector4Parameter trueVector, hkMxSingleParameter falseSingle )
{
	m_vec.m_vec[0].setSelect4(mask.get<0>(), trueVector.m_vec.m_vec[0], falseSingle.m_single);
	m_vec.m_vec[1].setSelect4(mask.get<1>(), trueVector.m_vec.m_vec[1], falseSingle.m_single);
	m_vec.m_vec[2].setSelect4(mask.get<2>(), trueVector.m_vec.m_vec[2], falseSingle.m_single);
	m_vec.m_vec[3].setSelect4(mask.get<3>(), trueVector.m_vec.m_vec[3], falseSingle.m_single);
}

HK_FORCE_INLINE void hkMxVector4::setDiv(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	m_vec.m_vec[0].setDiv4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0]);
	m_vec.m_vec[1].setDiv4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1]);
	m_vec.m_vec[2].setDiv4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2]);
	m_vec.m_vec[3].setDiv4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setDivFast(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	m_vec.m_vec[0].setDiv4fast(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0]);
	m_vec.m_vec[1].setDiv4fast(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1]);
	m_vec.m_vec[2].setDiv4fast(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2]);
	m_vec.m_vec[3].setDiv4fast(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::setMul(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	m_vec.m_vec[0].setMul4(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0]);
	m_vec.m_vec[1].setMul4(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1]);
	m_vec.m_vec[2].setMul4(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2]);
	m_vec.m_vec[3].setMul4(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3]);
}
HK_FORCE_INLINE void hkMxVector4::setMul(hkMxRealParameter r, hkMxVector4Parameter v)
{
	m_vec.m_vec[0].setMul4(r.m_real.m_vec[0], v.m_vec.m_vec[0]);
	m_vec.m_vec[1].setMul4(r.m_real.m_vec[1], v.m_vec.m_vec[1]);
	m_vec.m_vec[2].setMul4(r.m_real.m_vec[2], v.m_vec.m_vec[2]);
	m_vec.m_vec[3].setMul4(r.m_real.m_vec[3], v.m_vec.m_vec[3]);
}


HK_FORCE_INLINE void hkMxVector4::setMul(hkMxVector4Parameter v, hkMxSingleParameter s)
{
	m_vec.m_vec[0].setMul4(v.m_vec.m_vec[0], s.m_single);
	m_vec.m_vec[1].setMul4(v.m_vec.m_vec[1], s.m_single);
	m_vec.m_vec[2].setMul4(v.m_vec.m_vec[2], s.m_single);
	m_vec.m_vec[3].setMul4(v.m_vec.m_vec[3], s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::mul(hkMxVector4Parameter m)
{
	m_vec.m_vec[0].mul4(m.m_vec.m_vec[0]);
	m_vec.m_vec[1].mul4(m.m_vec.m_vec[1]);
	m_vec.m_vec[2].mul4(m.m_vec.m_vec[2]);
	m_vec.m_vec[3].mul4(m.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::mul(hkMxRealParameter r)
{
	m_vec.m_vec[0].mul4(r.m_real.m_vec[0]);
	m_vec.m_vec[1].mul4(r.m_real.m_vec[1]);
	m_vec.m_vec[2].mul4(r.m_real.m_vec[2]);
	m_vec.m_vec[3].mul4(r.m_real.m_vec[3]);
}

HK_FORCE_INLINE void hkMxVector4::mul(hkMxSingleParameter s)
{
	m_vec.m_vec[0].mul4(s.m_single);
	m_vec.m_vec[1].mul4(s.m_single);
	m_vec.m_vec[2].mul4(s.m_single);
	m_vec.m_vec[3].mul4(s.m_single);
}

HK_FORCE_INLINE void hkMxVector4::horizontalMin4Packed( hkVector4& minsOut ) const
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 v0 = m_vec.m_vec[0];
	hkVector4 v1 = m_vec.m_vec[1];
	hkVector4 v2 = m_vec.m_vec[2];
	hkVector4 v3 = m_vec.m_vec[3];
	HK_TRANSPOSE4( v0, v1, v2, v3 );
#else
	hkVector4 v0,v1,v2,v3;
	v0.set( m_vec.m_vec[0](0), m_vec.m_vec[1](0), m_vec.m_vec[2](0), m_vec.m_vec[3](0) );
	v1.set( m_vec.m_vec[0](1), m_vec.m_vec[1](1), m_vec.m_vec[2](1), m_vec.m_vec[3](1) );
	v2.set( m_vec.m_vec[0](2), m_vec.m_vec[1](2), m_vec.m_vec[2](2), m_vec.m_vec[3](2) );
	v3.set( m_vec.m_vec[0](3), m_vec.m_vec[1](3), m_vec.m_vec[2](3), m_vec.m_vec[3](3) );
#endif
	hkVector4 t0; t0.setMin4( v0, v1 );
	hkVector4 t1; t1.setMin4( v2, v3 );
	minsOut.setMin4( t0, t1 );
}

HK_FORCE_INLINE void hkMxVector4::horizontalMax4Packed( hkVector4& maxsOut ) const
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 v0 = m_vec.m_vec[0];
	hkVector4 v1 = m_vec.m_vec[1];
	hkVector4 v2 = m_vec.m_vec[2];
	hkVector4 v3 = m_vec.m_vec[3];
	HK_TRANSPOSE4( v0, v1, v2, v3 );
#else
	hkVector4 v0,v1,v2,v3;
	v0.set( m_vec.m_vec[0](0), m_vec.m_vec[1](0), m_vec.m_vec[2](0), m_vec.m_vec[3](0) );
	v1.set( m_vec.m_vec[0](1), m_vec.m_vec[1](1), m_vec.m_vec[2](1), m_vec.m_vec[3](1) );
	v2.set( m_vec.m_vec[0](2), m_vec.m_vec[1](2), m_vec.m_vec[2](2), m_vec.m_vec[3](2) );
	v3.set( m_vec.m_vec[0](3), m_vec.m_vec[1](3), m_vec.m_vec[2](3), m_vec.m_vec[3](3) );
#endif
	hkVector4 t0; t0.setMax4( v0, v1 );
	hkVector4 t1; t1.setMax4( v2, v3 );
	maxsOut.setMax4( t0, t1 );
}


HK_FORCE_INLINE void hkMxVector4::horizontalAdd4( hkVector4& addsOut ) const
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 v0 = m_vec.m_vec[0];
	hkVector4 v1 = m_vec.m_vec[1];
	hkVector4 v2 = m_vec.m_vec[2];
	hkVector4 v3 = m_vec.m_vec[3];
	HK_TRANSPOSE4( v0, v1, v2, v3 );
#else
	hkVector4 v0,v1,v2,v3;
	v0.set( m_vec.m_vec[0](0), m_vec.m_vec[1](0), m_vec.m_vec[2](0), m_vec.m_vec[3](0) );
	v1.set( m_vec.m_vec[0](1), m_vec.m_vec[1](1), m_vec.m_vec[2](1), m_vec.m_vec[3](1) );
	v2.set( m_vec.m_vec[0](2), m_vec.m_vec[1](2), m_vec.m_vec[2](2), m_vec.m_vec[3](2) );
	v3.set( m_vec.m_vec[0](3), m_vec.m_vec[1](3), m_vec.m_vec[2](3), m_vec.m_vec[3](3) );
#endif
	hkVector4 t0; t0.setAdd4( (const hkVector4&)v0, (const hkVector4&)v1 );
	hkVector4 t1; t1.setAdd4( (const hkVector4&)v2, (const hkVector4&)v3 );
	addsOut.setAdd4( t0, t1 );
}

HK_FORCE_INLINE void hkMxVector4::horizontalAdd3( hkVector4& addsOut ) const
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	hkVector4 v0 = m_vec.m_vec[0];
	hkVector4 v1 = m_vec.m_vec[1];
	hkVector4 v2 = m_vec.m_vec[2];
	hkVector4 v3 = m_vec.m_vec[3];
	HK_TRANSPOSE4( v0, v1, v2, v3 );
#else
	hkVector4 v0,v1,v2;
	v0.set( m_vec.m_vec[0](0), m_vec.m_vec[1](0), m_vec.m_vec[2](0), m_vec.m_vec[3](0) );
	v1.set( m_vec.m_vec[0](1), m_vec.m_vec[1](1), m_vec.m_vec[2](1), m_vec.m_vec[3](1) );
	v2.set( m_vec.m_vec[0](2), m_vec.m_vec[1](2), m_vec.m_vec[2](2), m_vec.m_vec[3](2) );
#endif
	hkVector4 t0; t0.setAdd4( v0, v1 );
	addsOut.setAdd4( t0, v2 );
}

HK_FORCE_INLINE void hkMxVector4::reduceAdd( hkVector4& addOut ) const
{
	hkVector4 a; a.setAdd4(m_vec.m_vec[0], m_vec.m_vec[1]);
	hkVector4 b; b.setAdd4(m_vec.m_vec[2], m_vec.m_vec[3]);
	addOut.setAdd4(a, b);
}

HK_FORCE_INLINE void hkMxVector4::dot4(hkMxVector4Parameter v, hkMxReal& lensOut ) const
{
	lensOut.m_real.m_vec[0].setAll( m_vec.m_vec[0].dot4(v.m_vec.m_vec[0]));
	lensOut.m_real.m_vec[1].setAll( m_vec.m_vec[1].dot4(v.m_vec.m_vec[1]));
	lensOut.m_real.m_vec[2].setAll( m_vec.m_vec[2].dot4(v.m_vec.m_vec[2]));
	lensOut.m_real.m_vec[3].setAll( m_vec.m_vec[3].dot4(v.m_vec.m_vec[3]));
}

HK_FORCE_INLINE void hkMxVector4::dot3(hkMxVector4Parameter v, hkMxReal& lensOut ) const
{
	lensOut.m_real.m_vec[0].setAll( m_vec.m_vec[0].dot3(v.m_vec.m_vec[0]));
	lensOut.m_real.m_vec[1].setAll( m_vec.m_vec[1].dot3(v.m_vec.m_vec[1]));
	lensOut.m_real.m_vec[2].setAll( m_vec.m_vec[2].dot3(v.m_vec.m_vec[2]));
	lensOut.m_real.m_vec[3].setAll( m_vec.m_vec[3].dot3(v.m_vec.m_vec[3]));
}

HK_FORCE_INLINE void hkMxVector4::dot3(hkMxVector4Parameter v, hkMxSingle& lensOut ) const
{
	hkSimdReal s0 = m_vec.m_vec[0].dot3(v.m_vec.m_vec[0]);
	hkSimdReal s1 = m_vec.m_vec[1].dot3(v.m_vec.m_vec[1]);
	hkSimdReal s2 = m_vec.m_vec[2].dot3(v.m_vec.m_vec[2]);
	hkSimdReal s3 = m_vec.m_vec[3].dot3(v.m_vec.m_vec[3]);

	lensOut.m_single.set( s0, s1, s2, s3 );
}

HK_FORCE_INLINE void hkMxVector4::lengthInverse4( hkMxReal& lensOut ) const
{
	lensOut.m_real.m_vec[0].setAll( m_vec.m_vec[0].lengthInverse4() );
	lensOut.m_real.m_vec[1].setAll( m_vec.m_vec[1].lengthInverse4() );
	lensOut.m_real.m_vec[2].setAll( m_vec.m_vec[2].lengthInverse4() );
	lensOut.m_real.m_vec[3].setAll( m_vec.m_vec[3].lengthInverse4() );
}

HK_FORCE_INLINE void hkMxVector4::normalize4()
{
	m_vec.m_vec[0].normalize4();
	m_vec.m_vec[1].normalize4();
	m_vec.m_vec[2].normalize4();
	m_vec.m_vec[3].normalize4();
}

HK_FORCE_INLINE void hkMxVector4::length4( hkMxReal& lensOut ) const
{
	lensOut.m_real.m_vec[0].setAll( m_vec.m_vec[0].length4() );
	lensOut.m_real.m_vec[1].setAll( m_vec.m_vec[1].length4() );
	lensOut.m_real.m_vec[2].setAll( m_vec.m_vec[2].length4() );
	lensOut.m_real.m_vec[3].setAll( m_vec.m_vec[3].length4() );
}

HK_FORCE_INLINE void hkMxVector4::setFastNormalize3NonZero(hkMxVector4Parameter m)
{
	m_vec.m_vec[0].setFastNormalize3NonZero(m.m_vec.m_vec[0]);
	m_vec.m_vec[1].setFastNormalize3NonZero(m.m_vec.m_vec[1]);
	m_vec.m_vec[2].setFastNormalize3NonZero(m.m_vec.m_vec[2]);
	m_vec.m_vec[3].setFastNormalize3NonZero(m.m_vec.m_vec[3]);
}


HK_FORCE_INLINE void hkMxVector4::setSqrtInverse( hkMxVector4Parameter m )
{
	m_vec.m_vec[0].setSqrtInverse4(m.m_vec.m_vec[0]);
	m_vec.m_vec[1].setSqrtInverse4(m.m_vec.m_vec[1]);
	m_vec.m_vec[2].setSqrtInverse4(m.m_vec.m_vec[2]);
	m_vec.m_vec[3].setSqrtInverse4(m.m_vec.m_vec[3]);
}


template <int I>
HK_FORCE_INLINE void hkMxVector4::setBroadcast(hkMxVector4Parameter v)
{
	INDEX_CHECK;
	m_vec.m_vec[0].setBroadcast(v.m_vec.m_vec[0], I);
	m_vec.m_vec[1].setBroadcast(v.m_vec.m_vec[1], I);
	m_vec.m_vec[2].setBroadcast(v.m_vec.m_vec[2], I);
	m_vec.m_vec[3].setBroadcast(v.m_vec.m_vec[3], I);
}




template <hkMxVector4::Permutation P>
HK_FORCE_INLINE void hkMxVector4::setPermutation(hkMxVector4Parameter m)
{
	HK_COMPILE_TIME_ASSERT2((P==hkMxVector4::SHIFT_LEFT_CYCLIC)||(P==hkMxVector4::SHIFT_RIGHT_CYCLIC)||(P==hkMxVector4::REVERSE),UNKNOWN_PERMUTATION);
	HK_ASSERT( 0xf03454f5, this != &m );

	switch (P)
	{
		case SHIFT_LEFT_CYCLIC:
			m_vec.m_vec[3] = m.m_vec.m_vec[0];
			m_vec.m_vec[2] = m.m_vec.m_vec[3];
			m_vec.m_vec[1] = m.m_vec.m_vec[2];
			m_vec.m_vec[0] = m.m_vec.m_vec[1];
			break;

		case SHIFT_RIGHT_CYCLIC:
			m_vec.m_vec[3] = m.m_vec.m_vec[2];
			m_vec.m_vec[2] = m.m_vec.m_vec[1];
			m_vec.m_vec[1] = m.m_vec.m_vec[0];
			m_vec.m_vec[0] = m.m_vec.m_vec[3];
			break;

		case REVERSE:
			m_vec.m_vec[3] = m.m_vec.m_vec[0];
			m_vec.m_vec[2] = m.m_vec.m_vec[1];
			m_vec.m_vec[1] = m.m_vec.m_vec[2];
			m_vec.m_vec[0] = m.m_vec.m_vec[3];
			break;

		default:
			break;
	}
}


HK_FORCE_INLINE hkMxReal::hkMxReal(hkVector4Parameter v)
{
	m_real.m_vec[0].setAll(v.getSimdAt(0));
	m_real.m_vec[1].setAll(v.getSimdAt(1));
	m_real.m_vec[2].setAll(v.getSimdAt(2));
	m_real.m_vec[3].setAll(v.getSimdAt(3));
}


HK_FORCE_INLINE void hkMxReal::load(const hkReal* r)
{
	hkVector4 v; v.load4( r );
	m_real.m_vec[0].setBroadcast(v,0);
	m_real.m_vec[1].setBroadcast(v,1);
	m_real.m_vec[2].setBroadcast(v,2);
	m_real.m_vec[3].setBroadcast(v,3);
}

HK_FORCE_INLINE void hkMxReal::gather(const hkReal* base, hkUint32 byteAddressIncrement)
{
	m_real.m_vec[0].setAll(*hkAddByteOffsetConst(base, 0*byteAddressIncrement));
	m_real.m_vec[1].setAll(*hkAddByteOffsetConst(base, 1*byteAddressIncrement));
	m_real.m_vec[2].setAll(*hkAddByteOffsetConst(base, 2*byteAddressIncrement));
	m_real.m_vec[3].setAll(*hkAddByteOffsetConst(base, 3*byteAddressIncrement));
}

HK_FORCE_INLINE void hkMxReal::gather(const hkReal* base, hkUint32 byteAddressIncrement, const hkInt32* indices)
{
	const hkReal* ptr0 = hkAddByteOffsetConst(base, indices[0] * byteAddressIncrement);
	const hkReal* ptr1 = hkAddByteOffsetConst(base, indices[1] * byteAddressIncrement);
	const hkReal* ptr2 = hkAddByteOffsetConst(base, indices[2] * byteAddressIncrement);
	const hkReal* ptr3 = hkAddByteOffsetConst(base, indices[3] * byteAddressIncrement);
	m_real.m_vec[0].setAll(*ptr0);
	m_real.m_vec[1].setAll(*ptr1);
	m_real.m_vec[2].setAll(*ptr2);
	m_real.m_vec[3].setAll(*ptr3);
}

HK_FORCE_INLINE void hkMxReal::gather(const hkReal* base, hkUint32 byteAddressIncrement, const hkUint16* indices)
{
	const hkReal* ptr0 = hkAddByteOffsetConst(base, indices[0] * byteAddressIncrement);
	const hkReal* ptr1 = hkAddByteOffsetConst(base, indices[1] * byteAddressIncrement);
	const hkReal* ptr2 = hkAddByteOffsetConst(base, indices[2] * byteAddressIncrement);
	const hkReal* ptr3 = hkAddByteOffsetConst(base, indices[3] * byteAddressIncrement);
	m_real.m_vec[0].setAll(*ptr0);
	m_real.m_vec[1].setAll(*ptr1);
	m_real.m_vec[2].setAll(*ptr2);
	m_real.m_vec[3].setAll(*ptr3);
}


HK_FORCE_INLINE void hkMxReal::store(hkReal* r) const
{	
	m_real.m_vec[0].storeX( r+0 );
	m_real.m_vec[1].storeX( r+1 );
	m_real.m_vec[2].storeX( r+2 );
	m_real.m_vec[3].storeX( r+3 );
}

HK_FORCE_INLINE void hkMxReal::scatter(hkReal* base, hkUint32 byteAddressIncrement) const
{
	m_real.m_vec[0].storeX( hkAddByteOffset(base, 0*byteAddressIncrement) );
	m_real.m_vec[1].storeX( hkAddByteOffset(base, 1*byteAddressIncrement) );
	m_real.m_vec[2].storeX( hkAddByteOffset(base, 2*byteAddressIncrement) );
	m_real.m_vec[3].storeX( hkAddByteOffset(base, 3*byteAddressIncrement) );
}

HK_FORCE_INLINE void hkMxReal::scatter(hkReal* base, hkUint32 byteAddressIncrement, const hkInt32* indices) const
{
	m_real.m_vec[0].storeX( hkAddByteOffset(base, indices[0]*byteAddressIncrement) );
	m_real.m_vec[1].storeX( hkAddByteOffset(base, indices[1]*byteAddressIncrement) );
	m_real.m_vec[2].storeX( hkAddByteOffset(base, indices[2]*byteAddressIncrement) );
	m_real.m_vec[3].storeX( hkAddByteOffset(base, indices[3]*byteAddressIncrement) );
}

HK_FORCE_INLINE void hkMxReal::scatter(hkReal* base, hkUint32 byteAddressIncrement, const hkUint16* indices) const
{
	m_real.m_vec[0].storeX( hkAddByteOffset(base, indices[0]*byteAddressIncrement) );
	m_real.m_vec[1].storeX( hkAddByteOffset(base, indices[1]*byteAddressIncrement) );
	m_real.m_vec[2].storeX( hkAddByteOffset(base, indices[2]*byteAddressIncrement) );
	m_real.m_vec[3].storeX( hkAddByteOffset(base, indices[3]*byteAddressIncrement) );
}

HK_FORCE_INLINE void hkMxReal::compareLessThan(hkMxRealParameter v, hkMxMask4& mask) const
{
	mask.set( m_real.m_vec[0].compareLessThan4(v.m_real.m_vec[0]), 
			  m_real.m_vec[1].compareLessThan4(v.m_real.m_vec[1]),
			  m_real.m_vec[2].compareLessThan4(v.m_real.m_vec[2]),
			  m_real.m_vec[3].compareLessThan4(v.m_real.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxReal::compareLessThan(hkMxVector4Parameter v, hkMxMask4& mask) const
{
	mask.set( m_real.m_vec[0].compareLessThan4(v.m_vec.m_vec[0]), 
			  m_real.m_vec[1].compareLessThan4(v.m_vec.m_vec[1]),
			  m_real.m_vec[2].compareLessThan4(v.m_vec.m_vec[2]),
			  m_real.m_vec[3].compareLessThan4(v.m_vec.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxReal::compareLessThanEqual(hkMxRealParameter v, hkMxMask4& mask) const
{
	mask.set(	m_real.m_vec[0].compareLessThanEqual4(v.m_real.m_vec[0]), 
				m_real.m_vec[1].compareLessThanEqual4(v.m_real.m_vec[1]),
				m_real.m_vec[2].compareLessThanEqual4(v.m_real.m_vec[2]),
				m_real.m_vec[3].compareLessThanEqual4(v.m_real.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxReal::compareLessThanEqual(hkMxVector4Parameter v, hkMxMask4& mask) const
{
	mask.set(	m_real.m_vec[0].compareLessThanEqual4(v.m_vec.m_vec[0]), 
				m_real.m_vec[1].compareLessThanEqual4(v.m_vec.m_vec[1]),
				m_real.m_vec[2].compareLessThanEqual4(v.m_vec.m_vec[2]),
				m_real.m_vec[3].compareLessThanEqual4(v.m_vec.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxReal::compareGreaterThan(hkMxRealParameter v, hkMxMask4& mask) const
{
	mask.set( m_real.m_vec[0].compareGreaterThan4(v.m_real.m_vec[0]), 
			  m_real.m_vec[1].compareGreaterThan4(v.m_real.m_vec[1]),
			  m_real.m_vec[2].compareGreaterThan4(v.m_real.m_vec[2]),
			  m_real.m_vec[3].compareGreaterThan4(v.m_real.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxReal::compareGreaterThan(hkMxVector4Parameter v, hkMxMask4& mask) const
{
	mask.set(	m_real.m_vec[0].compareGreaterThan4(v.m_vec.m_vec[0]), 
				m_real.m_vec[1].compareGreaterThan4(v.m_vec.m_vec[1]),
				m_real.m_vec[2].compareGreaterThan4(v.m_vec.m_vec[2]),
				m_real.m_vec[3].compareGreaterThan4(v.m_vec.m_vec[3]) );
}

HK_FORCE_INLINE void hkMxReal::compareLessThan(hkMxSingleParameter v, hkMxMask4& mask) const
{
	mask.set(	m_real.m_vec[0].compareLessThan4(v.m_single), 
				m_real.m_vec[1].compareLessThan4(v.m_single),
				m_real.m_vec[2].compareLessThan4(v.m_single),
				m_real.m_vec[3].compareLessThan4(v.m_single) );
}

HK_FORCE_INLINE void hkMxReal::compareLessThanEqual(hkMxSingleParameter v, hkMxMask4& mask) const
{
	mask.set(	m_real.m_vec[0].compareLessThanEqual4(v.m_single), 
				m_real.m_vec[1].compareLessThanEqual4(v.m_single),
				m_real.m_vec[2].compareLessThanEqual4(v.m_single),
				m_real.m_vec[3].compareLessThanEqual4(v.m_single) );
}

HK_FORCE_INLINE void hkMxReal::compareGreaterThan(hkMxSingleParameter v, hkMxMask4& mask) const
{
	mask.set(	m_real.m_vec[0].compareGreaterThan4(v.m_single), 
				m_real.m_vec[1].compareGreaterThan4(v.m_single),
				m_real.m_vec[2].compareGreaterThan4(v.m_single),
				m_real.m_vec[3].compareGreaterThan4(v.m_single) );
}

HK_FORCE_INLINE void hkMxReal::setDot4xyz1(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	m_real.m_vec[0].setAll(v0.m_vec.m_vec[0].dot4xyz1(v1.m_vec.m_vec[0]));
	m_real.m_vec[1].setAll(v0.m_vec.m_vec[1].dot4xyz1(v1.m_vec.m_vec[1]));
	m_real.m_vec[2].setAll(v0.m_vec.m_vec[2].dot4xyz1(v1.m_vec.m_vec[2]));
	m_real.m_vec[3].setAll(v0.m_vec.m_vec[3].dot4xyz1(v1.m_vec.m_vec[3]));
}

HK_FORCE_INLINE void hkMxReal::setDot4xyz1(hkMxSingleParameter s, hkMxVector4Parameter v)
{
	m_real.m_vec[0].setAll(s.m_single.dot4xyz1(v.m_vec.m_vec[0]));
	m_real.m_vec[1].setAll(s.m_single.dot4xyz1(v.m_vec.m_vec[1]));
	m_real.m_vec[2].setAll(s.m_single.dot4xyz1(v.m_vec.m_vec[2]));
	m_real.m_vec[3].setAll(s.m_single.dot4xyz1(v.m_vec.m_vec[3]));
}

HK_FORCE_INLINE void hkMxReal::setDot3(hkMxVector4Parameter v0, hkMxVector4Parameter v1)
{
	m_real.m_vec[0].setAll(v0.m_vec.m_vec[0].dot3(v1.m_vec.m_vec[0]));
	m_real.m_vec[1].setAll(v0.m_vec.m_vec[1].dot3(v1.m_vec.m_vec[1]));
	m_real.m_vec[2].setAll(v0.m_vec.m_vec[2].dot3(v1.m_vec.m_vec[2]));
	m_real.m_vec[3].setAll(v0.m_vec.m_vec[3].dot3(v1.m_vec.m_vec[3]));
}

HK_FORCE_INLINE void hkMxReal::setDot3(hkMxSingleParameter s, hkMxVector4Parameter v)
{
	m_real.m_vec[0].setAll(s.m_single.dot3(v.m_vec.m_vec[0]));
	m_real.m_vec[1].setAll(s.m_single.dot3(v.m_vec.m_vec[1]));
	m_real.m_vec[2].setAll(s.m_single.dot3(v.m_vec.m_vec[2]));
	m_real.m_vec[3].setAll(s.m_single.dot3(v.m_vec.m_vec[3]));
}

HK_FORCE_INLINE void hkMxReal::select(hkMxMask4Parameter mask, hkMxRealParameter trueValue, hkMxRealParameter falseValue )
{
	m_real.m_vec[0].setSelect4(mask.get<0>(), trueValue.m_real.m_vec[0], falseValue.m_real.m_vec[0]);
	m_real.m_vec[1].setSelect4(mask.get<1>(), trueValue.m_real.m_vec[1], falseValue.m_real.m_vec[1]);
	m_real.m_vec[2].setSelect4(mask.get<2>(), trueValue.m_real.m_vec[2], falseValue.m_real.m_vec[2]);
	m_real.m_vec[3].setSelect4(mask.get<3>(), trueValue.m_real.m_vec[3], falseValue.m_real.m_vec[3]);
}

template <int I>
HK_FORCE_INLINE void hkMxReal::getBroadcast(hkVector4& vOut) const
{
	INDEX_CHECK;
	vOut = m_real.m_vec[I];
}

template <int I>
HK_FORCE_INLINE hkVector4 hkMxReal::getBroadcast() const
{
	INDEX_CHECK;
	return m_real.m_vec[I];
}

template <int I>
HK_FORCE_INLINE void hkMxReal::setBroadcast(hkReal r)
{
	INDEX_CHECK;
	m_real.m_vec[I].setAll(r);
}

HK_FORCE_INLINE void hkMxReal::storePacked(hkVector4& vOut) const
{
	vOut.setHorizX(m_real.m_vec[0], m_real.m_vec[1], m_real.m_vec[2], m_real.m_vec[3]);
}


HK_FORCE_INLINE void hkMxReal::setAdd(hkMxRealParameter v0, hkMxRealParameter v1)
{
	m_real.m_vec[0].setAdd4(v0.m_real.m_vec[0], v1.m_real.m_vec[0]);
	m_real.m_vec[1].setAdd4(v0.m_real.m_vec[1], v1.m_real.m_vec[1]);
	m_real.m_vec[2].setAdd4(v0.m_real.m_vec[2], v1.m_real.m_vec[2]);
	m_real.m_vec[3].setAdd4(v0.m_real.m_vec[3], v1.m_real.m_vec[3]);
}

HK_FORCE_INLINE void hkMxReal::setSub(hkMxRealParameter v0, hkMxRealParameter v1)
{
	m_real.m_vec[0].setSub4(v0.m_real.m_vec[0], v1.m_real.m_vec[0]);
	m_real.m_vec[1].setSub4(v0.m_real.m_vec[1], v1.m_real.m_vec[1]);
	m_real.m_vec[2].setSub4(v0.m_real.m_vec[2], v1.m_real.m_vec[2]);
	m_real.m_vec[3].setSub4(v0.m_real.m_vec[3], v1.m_real.m_vec[3]);
}

HK_FORCE_INLINE void hkMxReal::setMul(hkMxRealParameter v0, hkMxRealParameter v1)
{
	m_real.m_vec[0].setMul4(v0.m_real.m_vec[0], v1.m_real.m_vec[0]);
	m_real.m_vec[1].setMul4(v0.m_real.m_vec[1], v1.m_real.m_vec[1]);
	m_real.m_vec[2].setMul4(v0.m_real.m_vec[2], v1.m_real.m_vec[2]);
	m_real.m_vec[3].setMul4(v0.m_real.m_vec[3], v1.m_real.m_vec[3]);
}

HK_FORCE_INLINE void hkMxReal::setMax(hkMxRealParameter v0, hkMxRealParameter v1)
{
	m_real.m_vec[0].setMax4(v0.m_real.m_vec[0], v1.m_real.m_vec[0]);
	m_real.m_vec[1].setMax4(v0.m_real.m_vec[1], v1.m_real.m_vec[1]);
	m_real.m_vec[2].setMax4(v0.m_real.m_vec[2], v1.m_real.m_vec[2]);
	m_real.m_vec[3].setMax4(v0.m_real.m_vec[3], v1.m_real.m_vec[3]);
}

HK_FORCE_INLINE void hkMxReal::setMin(hkMxRealParameter v0, hkMxRealParameter v1)
{
	m_real.m_vec[0].setMin4(v0.m_real.m_vec[0], v1.m_real.m_vec[0]);
	m_real.m_vec[1].setMin4(v0.m_real.m_vec[1], v1.m_real.m_vec[1]);
	m_real.m_vec[2].setMin4(v0.m_real.m_vec[2], v1.m_real.m_vec[2]);
	m_real.m_vec[3].setMin4(v0.m_real.m_vec[3], v1.m_real.m_vec[3]);
}

HK_FORCE_INLINE void hkMxReal::add(hkMxRealParameter v)
{
	m_real.m_vec[0].add4(v.m_real.m_vec[0]);
	m_real.m_vec[1].add4(v.m_real.m_vec[1]);
	m_real.m_vec[2].add4(v.m_real.m_vec[2]);
	m_real.m_vec[3].add4(v.m_real.m_vec[3]);
}

HK_FORCE_INLINE void hkMxReal::sub(hkMxRealParameter v)
{
	m_real.m_vec[0].sub4(v.m_real.m_vec[0]);
	m_real.m_vec[1].sub4(v.m_real.m_vec[1]);
	m_real.m_vec[2].sub4(v.m_real.m_vec[2]);
	m_real.m_vec[3].sub4(v.m_real.m_vec[3]);
}

HK_FORCE_INLINE void hkMxReal::mul(hkMxRealParameter v)
{
	m_real.m_vec[0].mul4(v.m_real.m_vec[0]);
	m_real.m_vec[1].mul4(v.m_real.m_vec[1]);
	m_real.m_vec[2].mul4(v.m_real.m_vec[2]);
	m_real.m_vec[3].mul4(v.m_real.m_vec[3]);
}

HK_FORCE_INLINE void hkMxReal::setNeg(hkMxRealParameter v)
{
	m_real.m_vec[0].setNeg4(v.m_real.m_vec[0]);
	m_real.m_vec[1].setNeg4(v.m_real.m_vec[1]);
	m_real.m_vec[2].setNeg4(v.m_real.m_vec[2]);
	m_real.m_vec[3].setNeg4(v.m_real.m_vec[3]);
}


// 
// HK_FORCE_INLINE hkMxSingle::hkMxSingle(hkReal v)
// {
// 	m_single.setAll(v);
// }

HK_FORCE_INLINE hkMxSingle::hkMxSingle(hkSimdRealParameter v)
{
	m_single.setAll(v);
}

HK_FORCE_INLINE hkMxSingle::hkMxSingle(hkVector4Parameter v)
{
	m_single = v;
}

HK_FORCE_INLINE hkMxSingle::hkMxSingle(const hkQuadReal& q)
{
	m_single = q;
}

HK_FORCE_INLINE void hkMxSingle::getVector(hkVector4& out)
{
	out = m_single;
}

HK_FORCE_INLINE void hkMxSingle::setAdd(hkMxSingleParameter v0, hkMxSingleParameter v1)
{
	m_single.setAdd4(v0.m_single, v1.m_single);
}





HK_FORCE_INLINE void hkMxMask4::horizontalAnd4( hkVector4Comparison& andOut ) const
{
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED

#if defined(HK_COMPILER_HAS_INTRINSICS_NEON)
	hkVector4 v0; v0.getQuad() = m_comp[0];
	hkVector4 v1; v1.getQuad() = m_comp[1];
	hkVector4 v2; v2.getQuad() = m_comp[2];
	hkVector4 v3; v3.getQuad() = m_comp[3];
	HK_TRANSPOSE4( v0, v1, v2, v3 );
#else
	hkVector4 v0; v0.getQuad() = m_comp[0].m_mask;
	hkVector4 v1; v1.getQuad() = m_comp[1].m_mask;
	hkVector4 v2; v2.getQuad() = m_comp[2].m_mask;
	hkVector4 v3; v3.getQuad() = m_comp[3].m_mask;
	HK_TRANSPOSE4( v0, v1, v2, v3 );
#endif
	hkVector4Comparison t0; t0.setAnd( (const hkVector4Comparison&)v0, (const hkVector4Comparison&)v1 );
	hkVector4Comparison t1; t1.setAnd( (const hkVector4Comparison&)v2, (const hkVector4Comparison&)v3 );
	t0.setAnd( t0, t1 );
	andOut = t0;
#else
	int mask      = ( m_comp[0].allAreSet() ? hkVector4Comparison::MASK_X : hkVector4Comparison::MASK_NONE ) |
					( m_comp[1].allAreSet() ? hkVector4Comparison::MASK_Y : hkVector4Comparison::MASK_NONE ) |
					( m_comp[2].allAreSet() ? hkVector4Comparison::MASK_Z : hkVector4Comparison::MASK_NONE ) |
					( m_comp[3].allAreSet() ? hkVector4Comparison::MASK_W : hkVector4Comparison::MASK_NONE ) ;
	andOut.set( (hkVector4Comparison::Mask)mask );
#endif
}

HK_FORCE_INLINE void hkMxMask4::set( hkVector4ComparisonParameter c )
{
	m_comp[0] = c;
	m_comp[1] = c;
	m_comp[2] = c;
	m_comp[3] = c;
}

HK_FORCE_INLINE void hkMxMask4::set( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b, hkVector4ComparisonParameter c, hkVector4ComparisonParameter d )
{
	m_comp[0] = a;
	m_comp[1] = b;
	m_comp[2] = c;
	m_comp[3] = d;
}

template <int I>
HK_FORCE_INLINE const hkVector4Comparison& hkMxMask4::get() const
{
	INDEX_CHECK;
	return m_comp[I];
}

// HK_FORCE_INLINE void hkMxMask4::storeHorizontal( hkMaskStorage& out ) const
// {
// 	out.m_mask = m_comp[0].getMask(hkVector4Comparison::MASK_X) | m_comp[1].getMask(hkVector4Comparison::MASK_Y) | m_comp[2].getMask(hkVector4Comparison::MASK_Z) | m_comp[3].getMask(hkVector4Comparison::MASK_W) ;
// }

HK_FORCE_INLINE void hkMxMask4::setAnd(hkMxMask4Parameter a, hkMxMask4Parameter b)
{
	m_comp[0].setAnd(a.m_comp[0], b.m_comp[0]);
	m_comp[1].setAnd(a.m_comp[1], b.m_comp[1]);
	m_comp[2].setAnd(a.m_comp[2], b.m_comp[2]);
	m_comp[3].setAnd(a.m_comp[3], b.m_comp[3]);
}

HK_FORCE_INLINE hkBool32 hkMxMask4::anyIsSet() const
{
	return m_comp[0].anyIsSet() | m_comp[1].anyIsSet() | m_comp[2].anyIsSet() | m_comp[3].anyIsSet();
}





HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::transformPosition(const hkMxTransform& mat, hkMxVector4Parameter vIn, hkMxVector4& vOut)
{
	// the vectors in mat are the columns of a transform matrix
	hkVector4 m0,m1,m2,m3;
	{
		mat.getVector<0>(m0);
		mat.getVector<1>(m1);
		mat.getVector<2>(m2);
		mat.getVector<3>(m3);
	}

	hkVector4 in0,in1,in2,in3;
	{
		vIn.getVector<0>(in0);
		vIn.getVector<1>(in1);
		vIn.getVector<2>(in2);
		vIn.getVector<3>(in3);
	}

	hkVector4 xb,yb,zb;
	hkVector4 out;
	{
		xb.setBroadcast(in0, 0);
		yb.setBroadcast(in0, 1);
		zb.setBroadcast(in0, 2);

		out.setAddMul4( m3, xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<0>(out);
	}
	{
		xb.setBroadcast(in1, 0);
		yb.setBroadcast(in1, 1);
		zb.setBroadcast(in1, 2);

		out.setAddMul4( m3, xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<1>(out);
	}
	{
		xb.setBroadcast(in2, 0);
		yb.setBroadcast(in2, 1);
		zb.setBroadcast(in2, 2);

		out.setAddMul4( m3, xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<2>(out);
	}
	{
		xb.setBroadcast(in3, 0);
		yb.setBroadcast(in3, 1);
		zb.setBroadcast(in3, 2);

		out.setAddMul4( m3, xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<3>(out);
	}
}

HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::transformTransposePosition(const hkMxTransform& mat, hkMxVector4Parameter vIn, hkMxVector4& vOut)
{
	// the vectors in mat are the columns of a transform matrix
	hkVector4 m0,m1,m2,m3;
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	mat.getVector<0>(m0);
	mat.getVector<1>(m1);
	mat.getVector<2>(m2);
	mat.getVector<3>(m3);
	HK_TRANSPOSE4( m0, m1, m2, m3 );
#else
	m0.set( mat.m_vec.m_vec[0](0), mat.m_vec.m_vec[1](0), mat.m_vec.m_vec[2](0), mat.m_vec.m_vec[3](0) );
	m1.set( mat.m_vec.m_vec[0](1), mat.m_vec.m_vec[1](1), mat.m_vec.m_vec[2](1), mat.m_vec.m_vec[3](1) );
	m2.set( mat.m_vec.m_vec[0](2), mat.m_vec.m_vec[1](2), mat.m_vec.m_vec[2](2), mat.m_vec.m_vec[3](2) );
	m3.set( mat.m_vec.m_vec[0](3), mat.m_vec.m_vec[1](3), mat.m_vec.m_vec[2](3), mat.m_vec.m_vec[3](3) );
#endif

	hkVector4 in0,in1,in2,in3;
	{
		vIn.getVector<0>(in0);
		vIn.getVector<1>(in1);
		vIn.getVector<2>(in2);
		vIn.getVector<3>(in3);
	}

	hkVector4 xb,yb,zb;
	hkVector4 out;
	{
		xb.setBroadcast(in0, 0);
		yb.setBroadcast(in0, 1);
		zb.setBroadcast(in0, 2);

		out.setAddMul4( m3, xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<0>(out);
	}
	{
		xb.setBroadcast(in1, 0);
		yb.setBroadcast(in1, 1);
		zb.setBroadcast(in1, 2);

		out.setAddMul4( m3, xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<1>(out);
	}
	{
		xb.setBroadcast(in2, 0);
		yb.setBroadcast(in2, 1);
		zb.setBroadcast(in2, 2);

		out.setAddMul4( m3, xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<2>(out);
	}
	{
		xb.setBroadcast(in3, 0);
		yb.setBroadcast(in3, 1);
		zb.setBroadcast(in3, 2);

		out.setAddMul4( m3, xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<3>(out);
	}
}

HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::rotateDir( const hkMxTransform& mat, hkMxVector4Parameter vIn, hkMxVector4& vOut )
{
	// the vectors in mat are the columns of a transform matrix
	hkVector4 m0,m1,m2; 
	{
		mat.getVector<0>(m0);
		mat.getVector<1>(m1);
		mat.getVector<2>(m2);
	}

	hkVector4 in0,in1,in2,in3;
	{
		vIn.getVector<0>(in0);
		vIn.getVector<1>(in1);
		vIn.getVector<2>(in2);
		vIn.getVector<3>(in3);
	}

	hkVector4 xb,yb,zb;
	hkVector4 out;
	{
		xb.setBroadcast(in0, 0);
		yb.setBroadcast(in0, 1);
		zb.setBroadcast(in0, 2);

		out.setMul4( xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<0>(out);
	}
	{
		xb.setBroadcast(in1, 0);
		yb.setBroadcast(in1, 1);
		zb.setBroadcast(in1, 2);

		out.setMul4( xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<1>(out);
	}
	{
		xb.setBroadcast(in2, 0);
		yb.setBroadcast(in2, 1);
		zb.setBroadcast(in2, 2);

		out.setMul4( xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<2>(out);
	}
	{
		xb.setBroadcast(in3, 0);
		yb.setBroadcast(in3, 1);
		zb.setBroadcast(in3, 2);

		out.setMul4( xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<3>(out);
	}
}

HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::rotateInverseDir( const hkMxTransform& mat, hkMxVector4Parameter vIn, hkMxVector4& vOut )
{
	// the vectors in mat are the columns of a transform matrix
	hkVector4 m0,m1,m2; 
#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	mat.getVector<0>(m0);
	mat.getVector<1>(m1);
	mat.getVector<2>(m2);
	HK_TRANSPOSE3( m0, m1, m2 );
#else
	m0.set( mat.m_vec.m_vec[0](0), mat.m_vec.m_vec[1](0), mat.m_vec.m_vec[2](0), mat.m_vec.m_vec[3](0) );
	m1.set( mat.m_vec.m_vec[0](1), mat.m_vec.m_vec[1](1), mat.m_vec.m_vec[2](1), mat.m_vec.m_vec[3](1) );
	m2.set( mat.m_vec.m_vec[0](2), mat.m_vec.m_vec[1](2), mat.m_vec.m_vec[2](2), mat.m_vec.m_vec[3](2) );
#endif

	hkVector4 in0,in1,in2,in3;
	{
		vIn.getVector<0>(in0);
		vIn.getVector<1>(in1);
		vIn.getVector<2>(in2);
		vIn.getVector<3>(in3);
	}

	hkVector4 xb,yb,zb;
	hkVector4 out;
	{
		xb.setBroadcast(in0, 0);
		yb.setBroadcast(in0, 1);
		zb.setBroadcast(in0, 2);

		out.setMul4( xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<0>(out);
	}
	{
		xb.setBroadcast(in1, 0);
		yb.setBroadcast(in1, 1);
		zb.setBroadcast(in1, 2);

		out.setMul4( xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<1>(out);
	}
	{
		xb.setBroadcast(in2, 0);
		yb.setBroadcast(in2, 1);
		zb.setBroadcast(in2, 2);

		out.setMul4( xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<2>(out);
	}
	{
		xb.setBroadcast(in3, 0);
		yb.setBroadcast(in3, 1);
		zb.setBroadcast(in3, 2);

		out.setMul4( xb, m0 );
		out.addMul4( yb, m1 );
		out.addMul4( zb, m2 );
		vOut.setVector<3>(out);
	}
}

HK_FORCE_INLINE void HK_CALL hkMxVector4Util::transformPositions(hkMxVector4Parameter mat0, hkMxVector4Parameter mat1, hkMxVector4Parameter mat2, hkMxVector4Parameter mat3, hkMxVector4Parameter vIn, hkMxVector4& vOut)
{
	hkVector4 in0,in1,in2,in3;
	{
		vIn.getVector<0>(in0);
		vIn.getVector<1>(in1);
		vIn.getVector<2>(in2);
		vIn.getVector<3>(in3);
	}

	//	creates better assembly
	//	mat0 x in0
	hkVector4 m00,m10,m20,m30;
	hkVector4 xb0,yb0,zb0;
	hkVector4 out0;

	//	mat1 x in1
	hkVector4 m01,m11,m21,m31;
	hkVector4 xb1,yb1,zb1;
	hkVector4 out1;

	//	mat2 x in2
	hkVector4 m02,m12,m22,m32;
	hkVector4 xb2,yb2,zb2;
	hkVector4 out2;

	//	mat3 x in3
	hkVector4 m03,m13,m23,m33;
	hkVector4 xb3,yb3,zb3;
	hkVector4 out3;
	{
		// get matrix
		mat0.getVector<0>(m00);
		mat0.getVector<1>(m10);
		mat0.getVector<2>(m20);
		mat0.getVector<3>(m30);

		mat1.getVector<0>(m01);
		mat1.getVector<1>(m11);
		mat1.getVector<2>(m21);
		mat1.getVector<3>(m31);

		mat2.getVector<0>(m02);
		mat2.getVector<1>(m12);
		mat2.getVector<2>(m22);
		mat2.getVector<3>(m32);

		mat3.getVector<0>(m03);
		mat3.getVector<1>(m13);
		mat3.getVector<2>(m23);
		mat3.getVector<3>(m33);

		//	set vector
		xb0.setBroadcast(in0, 0);
		yb0.setBroadcast(in0, 1);
		zb0.setBroadcast(in0, 2);

		xb1.setBroadcast(in1, 0);
		yb1.setBroadcast(in1, 1);
		zb1.setBroadcast(in1, 2);

		xb2.setBroadcast(in2, 0);
		yb2.setBroadcast(in2, 1);
		zb2.setBroadcast(in2, 2);

		xb3.setBroadcast(in3, 0);
		yb3.setBroadcast(in3, 1);
		zb3.setBroadcast(in3, 2);

		//	execute
		out0.setAddMul4( m30, xb0, m00 );
		out0.addMul4( yb0, m10 );
		out0.addMul4( zb0, m20 );
		vOut.setVector<0>(out0);

		out1.setAddMul4( m31, xb1, m01 );
		out1.addMul4( yb1, m11 );
		out1.addMul4( zb1, m21 );
		vOut.setVector<1>(out1);

		out2.setAddMul4( m32, xb2, m02 );
		out2.addMul4( yb2, m12 );
		out2.addMul4( zb2, m22 );
		vOut.setVector<2>(out2);

		out3.setAddMul4( m33, xb3, m03 );
		out3.addMul4( yb3, m13 );
		out3.addMul4( zb3, m23 );
		vOut.setVector<3>(out3);
	}
}

HK_FORCE_INLINE void HK_CALL hkMxVector4Util::rotateDirs(hkMxVector4Parameter mat0, hkMxVector4Parameter mat1, hkMxVector4Parameter mat2, hkMxVector4Parameter mat3, hkMxVector4Parameter vIn, hkMxVector4& vOut)
{
	hkVector4 in0,in1,in2,in3;
	{
		vIn.getVector<0>(in0);
		vIn.getVector<1>(in1);
		vIn.getVector<2>(in2);
		vIn.getVector<3>(in3);
	}

	//	creates better assembly
	//	mat0 x in0
	hkVector4 m00,m10,m20;
	hkVector4 xb0,yb0,zb0;
	hkVector4 out0;

	//	mat1 x in1
	hkVector4 m01,m11,m21;
	hkVector4 xb1,yb1,zb1;
	hkVector4 out1;

	//	mat2 x in2
	hkVector4 m02,m12,m22;
	hkVector4 xb2,yb2,zb2;
	hkVector4 out2;

	//	mat3 x in3
	hkVector4 m03,m13,m23;
	hkVector4 xb3,yb3,zb3;
	hkVector4 out3;
	{
		// get matrix
		mat0.getVector<0>(m00);
		mat0.getVector<1>(m10);
		mat0.getVector<2>(m20);

		mat1.getVector<0>(m01);
		mat1.getVector<1>(m11);
		mat1.getVector<2>(m21);

		mat2.getVector<0>(m02);
		mat2.getVector<1>(m12);
		mat2.getVector<2>(m22);

		mat3.getVector<0>(m03);
		mat3.getVector<1>(m13);
		mat3.getVector<2>(m23);

		//	set vector
		xb0.setBroadcast(in0, 0);
		yb0.setBroadcast(in0, 1);
		zb0.setBroadcast(in0, 2);

		xb1.setBroadcast(in1, 0);
		yb1.setBroadcast(in1, 1);
		zb1.setBroadcast(in1, 2);

		xb2.setBroadcast(in2, 0);
		yb2.setBroadcast(in2, 1);
		zb2.setBroadcast(in2, 2);

		xb3.setBroadcast(in3, 0);
		yb3.setBroadcast(in3, 1);
		zb3.setBroadcast(in3, 2);

		//	execute
		out0.setMul4( xb0, m00 );
		out0.addMul4( yb0, m10 );
		out0.addMul4( zb0, m20 );
		vOut.setVector<0>(out0);

		out1.setMul4( xb1, m01 );
		out1.addMul4( yb1, m11 );
		out1.addMul4( zb1, m21 );
		vOut.setVector<1>(out1);

		out2.setMul4( xb2, m02 );
		out2.addMul4( yb2, m12 );
		out2.addMul4( zb2, m22 );
		vOut.setVector<2>(out2);

		out3.setMul4( xb3, m03 );
		out3.addMul4( yb3, m13 );
		out3.addMul4( zb3, m23 );
		vOut.setVector<3>(out3);
	}
}

template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::gatherUnpack(const hkHalf8* base, hkMxVector4& vOutFirst, hkMxVector4& vOutSecond)
{
	hkVector4 h1,h2;
	hkAddByteOffsetConst( base, 0 * byteAddressIncrement )->unpack(h1, h2); vOutFirst.setVector<0>(h1); vOutSecond.setVector<0>(h2);
	hkAddByteOffsetConst( base, 1 * byteAddressIncrement )->unpack(h1, h2); vOutFirst.setVector<1>(h1); vOutSecond.setVector<1>(h2);
	hkAddByteOffsetConst( base, 2 * byteAddressIncrement )->unpack(h1, h2); vOutFirst.setVector<2>(h1); vOutSecond.setVector<2>(h2);
	hkAddByteOffsetConst( base, 3 * byteAddressIncrement )->unpack(h1, h2); vOutFirst.setVector<3>(h1); vOutSecond.setVector<3>(h2);
}

template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::gatherUnpackFirst(const hkHalf8* base, hkMxVector4& vOut)
{
	hkVector4 h;
	hkAddByteOffsetConst( base, 0 * byteAddressIncrement )->unpackFirst(h); vOut.setVector<0>(h);
	hkAddByteOffsetConst( base, 1 * byteAddressIncrement )->unpackFirst(h); vOut.setVector<1>(h);
	hkAddByteOffsetConst( base, 2 * byteAddressIncrement )->unpackFirst(h); vOut.setVector<2>(h);
	hkAddByteOffsetConst( base, 3 * byteAddressIncrement )->unpackFirst(h); vOut.setVector<3>(h);
}

HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::gatherUnpackFirstSLOW(const hkHalf8* base, hkUint32 byteAddressIncrement, hkMxVector4& vOut)
{
	hkVector4 h;
	hkAddByteOffsetConst( base, 0 * byteAddressIncrement )->unpackFirst(h); vOut.setVector<0>(h);
	hkAddByteOffsetConst( base, 1 * byteAddressIncrement )->unpackFirst(h); vOut.setVector<1>(h);
	hkAddByteOffsetConst( base, 2 * byteAddressIncrement )->unpackFirst(h); vOut.setVector<2>(h);
	hkAddByteOffsetConst( base, 3 * byteAddressIncrement )->unpackFirst(h); vOut.setVector<3>(h);
}

HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::gatherUnpackFirst(const hkHalf8* HK_RESTRICT base, hkUint32 byteAddressIncrement, const hkInt32* HK_RESTRICT indices, hkMxVector4& vOut)
{
	hkVector4 h;
	hkAddByteOffsetConst( base, indices[0] * byteAddressIncrement )->unpackFirst(h); vOut.setVector<0>(h);
	hkAddByteOffsetConst( base, indices[1] * byteAddressIncrement )->unpackFirst(h); vOut.setVector<1>(h);
	hkAddByteOffsetConst( base, indices[2] * byteAddressIncrement )->unpackFirst(h); vOut.setVector<2>(h);
	hkAddByteOffsetConst( base, indices[3] * byteAddressIncrement )->unpackFirst(h); vOut.setVector<3>(h);
}

template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::gatherUnpackSecond(const hkHalf8* HK_RESTRICT base, hkMxVector4& vOut)
{
	hkVector4 h;
	hkAddByteOffsetConst( base, 0 * byteAddressIncrement )->unpackSecond(h); vOut.setVector<0>(h);
	hkAddByteOffsetConst( base, 1 * byteAddressIncrement )->unpackSecond(h); vOut.setVector<1>(h);
	hkAddByteOffsetConst( base, 2 * byteAddressIncrement )->unpackSecond(h); vOut.setVector<2>(h);
	hkAddByteOffsetConst( base, 3 * byteAddressIncrement )->unpackSecond(h); vOut.setVector<3>(h);
}

HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::gatherUnpackSecondSLOW(const hkHalf8* HK_RESTRICT base, hkUint32 byteAddressIncrement, hkMxVector4& vOut)
{
	hkVector4 h;
	hkAddByteOffsetConst( base, 0 * byteAddressIncrement )->unpackSecond(h); vOut.setVector<0>(h);
	hkAddByteOffsetConst( base, 1 * byteAddressIncrement )->unpackSecond(h); vOut.setVector<1>(h);
	hkAddByteOffsetConst( base, 2 * byteAddressIncrement )->unpackSecond(h); vOut.setVector<2>(h);
	hkAddByteOffsetConst( base, 3 * byteAddressIncrement )->unpackSecond(h); vOut.setVector<3>(h);
}

HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::gatherUnpackSecond(const hkHalf8* HK_RESTRICT base, hkUint32 byteAddressIncrement, const hkInt32* indices, hkMxVector4& vOut)
{
	hkVector4 h;
	hkAddByteOffsetConst( base, indices[0] * byteAddressIncrement )->unpackSecond(h); vOut.setVector<0>(h);
	hkAddByteOffsetConst( base, indices[1] * byteAddressIncrement )->unpackSecond(h); vOut.setVector<1>(h);
	hkAddByteOffsetConst( base, indices[2] * byteAddressIncrement )->unpackSecond(h); vOut.setVector<2>(h);
	hkAddByteOffsetConst( base, indices[3] * byteAddressIncrement )->unpackSecond(h); vOut.setVector<3>(h);
}

template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::scatterPack(hkMxVector4Parameter vFirst, hkMxVector4Parameter vSecond, hkHalf8* base)
{
	hkVector4 h1,h2;
	vFirst.getVector<0>(h1); vSecond.getVector<0>(h2); hkAddByteOffset( base, 0 * byteAddressIncrement )->pack(h1, h2);
	vFirst.getVector<1>(h1); vSecond.getVector<1>(h2); hkAddByteOffset( base, 1 * byteAddressIncrement )->pack(h1, h2);
	vFirst.getVector<2>(h1); vSecond.getVector<2>(h2); hkAddByteOffset( base, 2 * byteAddressIncrement )->pack(h1, h2);
	vFirst.getVector<3>(h1); vSecond.getVector<3>(h2); hkAddByteOffset( base, 3 * byteAddressIncrement )->pack(h1, h2);
}

template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::scatterPackFirst(hkMxVector4Parameter v, hkHalf8* HK_RESTRICT base)
{
	hkHalf8* HK_RESTRICT base0 = hkAddByteOffset( base, 0 * byteAddressIncrement );
	hkHalf8* HK_RESTRICT base1 = hkAddByteOffset( base, 1 * byteAddressIncrement );
	hkHalf8* HK_RESTRICT base2 = hkAddByteOffset( base, 2 * byteAddressIncrement );
	hkHalf8* HK_RESTRICT base3 = hkAddByteOffset( base, 3 * byteAddressIncrement );

	hkHalf8 h0 = *base0; 
	hkHalf8 h1 = *base1; 
	hkHalf8 h2 = *base2; 
	hkHalf8 h3 = *base3; 

	h0.packFirst( v.getVector<0>());
	h1.packFirst( v.getVector<1>());
	h2.packFirst( v.getVector<2>());
	h3.packFirst( v.getVector<3>());
	*base0 = h0;
	*base1 = h1;
	*base2 = h2;
	*base3 = h3;
}

HK_FORCE_INLINE /*static*/ void HK_CALL hkMxVector4Util::scatterPackFirst(hkMxVector4Parameter v, hkHalf8* HK_RESTRICT base, hkUint32 byteAddressIncrement, const hkInt32* HK_RESTRICT indices)
{
	hkHalf8* HK_RESTRICT base0 = hkAddByteOffset( base, indices[0] * byteAddressIncrement );
	hkHalf8* HK_RESTRICT base1 = hkAddByteOffset( base, indices[1] * byteAddressIncrement );
	hkHalf8* HK_RESTRICT base2 = hkAddByteOffset( base, indices[2] * byteAddressIncrement );
	hkHalf8* HK_RESTRICT base3 = hkAddByteOffset( base, indices[3] * byteAddressIncrement );

	hkHalf8 h0 = *base0; 
	hkHalf8 h1 = *base1; 
	hkHalf8 h2 = *base2; 
	hkHalf8 h3 = *base3; 

	h0.packFirst( v.getVector<0>());
	h1.packFirst( v.getVector<1>());
	h2.packFirst( v.getVector<2>());
	h3.packFirst( v.getVector<3>());
	*base0 = h0;
	*base1 = h1;
	*base2 = h2;
	*base3 = h3;
}


template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE void hkMxQuaternion::gather(const hkQuaternion* HK_RESTRICT base)
{
	m_vec.m_vec[0] = hkAddByteOffsetConst(base, 0*byteAddressIncrement)->m_vec;
	m_vec.m_vec[1] = hkAddByteOffsetConst(base, 1*byteAddressIncrement)->m_vec;
	m_vec.m_vec[2] = hkAddByteOffsetConst(base, 2*byteAddressIncrement)->m_vec;
	m_vec.m_vec[3] = hkAddByteOffsetConst(base, 3*byteAddressIncrement)->m_vec;
}

HK_FORCE_INLINE void hkMxQuaternion::gatherSLOW(const hkQuaternion* HK_RESTRICT base, hkUint32 byteAddressIncrement)
{
	m_vec.m_vec[0] = hkAddByteOffsetConst(base, 0*byteAddressIncrement)->m_vec;
	m_vec.m_vec[1] = hkAddByteOffsetConst(base, 1*byteAddressIncrement)->m_vec;
	m_vec.m_vec[2] = hkAddByteOffsetConst(base, 2*byteAddressIncrement)->m_vec;
	m_vec.m_vec[3] = hkAddByteOffsetConst(base, 3*byteAddressIncrement)->m_vec;
}

template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE void hkMxQuaternion::scatter(hkQuaternion* HK_RESTRICT base) const
{
	hkAddByteOffset(base, 0*byteAddressIncrement)->m_vec = m_vec.m_vec[0];
	hkAddByteOffset(base, 1*byteAddressIncrement)->m_vec = m_vec.m_vec[1];
	hkAddByteOffset(base, 2*byteAddressIncrement)->m_vec = m_vec.m_vec[2];
	hkAddByteOffset(base, 3*byteAddressIncrement)->m_vec = m_vec.m_vec[3];
}

HK_FORCE_INLINE void hkMxQuaternion::scatterSLOW(hkQuaternion* HK_RESTRICT base, hkUint32 byteAddressIncrement) const
{
	hkAddByteOffset(base, 0*byteAddressIncrement)->m_vec = m_vec.m_vec[0];
	hkAddByteOffset(base, 1*byteAddressIncrement)->m_vec = m_vec.m_vec[1];
	hkAddByteOffset(base, 2*byteAddressIncrement)->m_vec = m_vec.m_vec[2];
	hkAddByteOffset(base, 3*byteAddressIncrement)->m_vec = m_vec.m_vec[3];
}

template <hkUint32 byteAddressIncrement>
HK_FORCE_INLINE void hkMxQuaternion::convertAndScatter(hkRotation* HK_RESTRICT base) const
{
	hkAddByteOffset(base, 0*byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[0] );
	hkAddByteOffset(base, 1*byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[1] );
	hkAddByteOffset(base, 2*byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[2] );
	hkAddByteOffset(base, 3*byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[3] );
}

HK_FORCE_INLINE void hkMxQuaternion::convertAndScatterSLOW(hkRotation* HK_RESTRICT base, hkUint32 byteAddressIncrement) const
{
	hkAddByteOffset(base, 0*byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[0] );
	hkAddByteOffset(base, 1*byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[1] );
	hkAddByteOffset(base, 2*byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[2] );
	hkAddByteOffset(base, 3*byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[3] );
}

HK_FORCE_INLINE void hkMxQuaternion::convertAndScatter(hkRotation* HK_RESTRICT base, hkUint32 byteAddressIncrement, const hkInt32* indices ) const
{
	hkAddByteOffset(base, indices[0] * byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[0] );
	hkAddByteOffset(base, indices[1] * byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[1] );
	hkAddByteOffset(base, indices[2] * byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[2] );
	hkAddByteOffset(base, indices[3] * byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[3] );
}

HK_FORCE_INLINE void hkMxQuaternion::convertAndScatter(hkRotation* HK_RESTRICT base, hkUint32 byteAddressIncrement, const hkUint16* indices ) const
{
	hkAddByteOffset(base, indices[0] * byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[0] );
	hkAddByteOffset(base, indices[1] * byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[1] );
	hkAddByteOffset(base, indices[2] * byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[2] );
	hkAddByteOffset(base, indices[3] * byteAddressIncrement)->set( (const hkQuaternion&)m_vec.m_vec[3] );

}


HK_FORCE_INLINE void hkMxQuaternion::setMulQ(const hkMxQuaternion& p, const hkMxQuaternion& q)
{
	((hkQuaternion&)m_vec.m_vec[0]).setMul( (hkQuaternion&)p.m_vec.m_vec[0], (hkQuaternion&)q.m_vec.m_vec[0] );
	((hkQuaternion&)m_vec.m_vec[1]).setMul( (hkQuaternion&)p.m_vec.m_vec[1], (hkQuaternion&)q.m_vec.m_vec[1] );
	((hkQuaternion&)m_vec.m_vec[2]).setMul( (hkQuaternion&)p.m_vec.m_vec[2], (hkQuaternion&)q.m_vec.m_vec[2] );
	((hkQuaternion&)m_vec.m_vec[3]).setMul( (hkQuaternion&)p.m_vec.m_vec[3], (hkQuaternion&)q.m_vec.m_vec[3] );
}

HK_FORCE_INLINE void hkMxQuaternion::setMulQ(const hkMxQuaternion& p, hkMxSingleParameter s)
{
	((hkQuaternion&)m_vec.m_vec[0]).setMul( (hkQuaternion&)p.m_vec.m_vec[0], (hkQuaternion&)s.m_single );
	((hkQuaternion&)m_vec.m_vec[1]).setMul( (hkQuaternion&)p.m_vec.m_vec[1], (hkQuaternion&)s.m_single );
	((hkQuaternion&)m_vec.m_vec[2]).setMul( (hkQuaternion&)p.m_vec.m_vec[2], (hkQuaternion&)s.m_single );
	((hkQuaternion&)m_vec.m_vec[3]).setMul( (hkQuaternion&)p.m_vec.m_vec[3], (hkQuaternion&)s.m_single );
}


HK_FORCE_INLINE void hkMxHalf8::pack(hkMxVector4Parameter v0, hkMxVector4Parameter v1 )
{
	m_vec[0].pack(v0.m_vec.m_vec[0], v1.m_vec.m_vec[0]);
	m_vec[1].pack(v0.m_vec.m_vec[1], v1.m_vec.m_vec[1]);
	m_vec[2].pack(v0.m_vec.m_vec[2], v1.m_vec.m_vec[2]);
	m_vec[3].pack(v0.m_vec.m_vec[3], v1.m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxHalf8::reducePrecision(hkMxVector4& v )
{
	hkHalf8::reducePrecision(v.m_vec.m_vec[0]);
	hkHalf8::reducePrecision(v.m_vec.m_vec[1]);
	hkHalf8::reducePrecision(v.m_vec.m_vec[2]);
	hkHalf8::reducePrecision(v.m_vec.m_vec[3]);
}



HK_FORCE_INLINE void hkMxHalf8::unpack(hkMxVector4& v0, hkMxVector4& v1) const
{
	HK_ASSERT2(0xad342288, (hkUlong(&v0) + sizeof(hkMxVector4) <= hkUlong(this) || hkUlong(this) + sizeof(hkMxHalf8) <= hkUlong(&v0))
		                && (hkUlong(&v1) + sizeof(hkMxVector4) <= hkUlong(this) || hkUlong(this) + sizeof(hkMxHalf8) <= hkUlong(&v1)), "hkMxHalf8 overlaps with the vectors being packed.");
	hkMxVector4* HK_RESTRICT v0p = &v0;
	hkMxVector4* HK_RESTRICT v1p = &v1;
	m_vec[0].unpack(v0p->m_vec.m_vec[0], v1p->m_vec.m_vec[0]);
	m_vec[1].unpack(v0p->m_vec.m_vec[1], v1p->m_vec.m_vec[1]);
	m_vec[2].unpack(v0p->m_vec.m_vec[2], v1p->m_vec.m_vec[2]);
	m_vec[3].unpack(v0p->m_vec.m_vec[3], v1p->m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxHalf8::unpackFirst(hkMxVector4& v) const
{
	HK_ASSERT2(0xad342289, hkUlong(&v) + sizeof(hkMxVector4) <= hkUlong(this) || hkUlong(this) + sizeof(hkMxHalf8) <= hkUlong(&v), "hkMxHalf8 overlaps with the vectors being packed.");
	hkMxVector4* HK_RESTRICT vp = &v;
	m_vec[0].unpackFirst(vp->m_vec.m_vec[0]);
	m_vec[1].unpackFirst(vp->m_vec.m_vec[1]);
	m_vec[2].unpackFirst(vp->m_vec.m_vec[2]);
	m_vec[3].unpackFirst(vp->m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxHalf8::unpackSecond(hkMxVector4& v) const
{
	HK_ASSERT2(0xad34228a, hkUlong(&v) + sizeof(hkMxVector4) <= hkUlong(this) || hkUlong(this) + sizeof(hkMxHalf8) <= hkUlong(&v), "hkMxHalf8 overlaps with the vectors being packed.");
	hkMxVector4* HK_RESTRICT vp = &v;
	m_vec[0].unpackSecond(vp->m_vec.m_vec[0]);
	m_vec[1].unpackSecond(vp->m_vec.m_vec[1]);
	m_vec[2].unpackSecond(vp->m_vec.m_vec[2]);
	m_vec[3].unpackSecond(vp->m_vec.m_vec[3]);
}

HK_FORCE_INLINE void hkMxHalf8::setZero()
{
	m_vec[0].setZero();
	m_vec[1].setZero();
	m_vec[2].setZero();
	m_vec[3].setZero();
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
