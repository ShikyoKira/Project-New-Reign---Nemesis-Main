/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#define HK_INT_VECTOR_NO_VECTOR_UNIT

//
// Clearing, loading, and storing
//

inline void hkIntVector::setZero()
{
	hkQuadUintUnion qu;
	qu.u[0] = 0;
	qu.u[1] = 0;
	qu.u[2] = 0;
	qu.u[3] = 0;
	m_quad = qu.q;
}

inline int hkIntVector::get() const
{
	return reinterpret_cast<const hkInt32*>(&m_quad)[0];
}

inline void hkIntVector::set(int i)
{
	reinterpret_cast<hkInt32*>(&m_quad)[0] = (hkInt32)i;
}

inline void hkIntVector::loadAligned1x(const hkUint32* p)
{
	HK_ASSERT2(0x19df6be3, ((hkUlong)p % 4) == 0, "pointer for hkIntVector::loadAligned1x must be 4-byte aligned");
	m_quad = *(const hkQuadUint*)p;
}

inline hkVector4Comparison hkIntVector::compareLessThanS32( hkIntVectorParameter a, hkIntVectorParameter b )
{
	const hkInt32* sa = reinterpret_cast<const hkInt32*>(&a.getQuad());
	const hkInt32* sb = reinterpret_cast<const hkInt32*>(&b.getQuad());
	const int msk = (sa[0] < sb[0] ? hkVector4Comparison::MASK_X : 0) |
					(sa[1] < sb[1] ? hkVector4Comparison::MASK_Y : 0) |
					(sa[2] < sb[2] ? hkVector4Comparison::MASK_Z : 0) |
					(sa[3] < sb[3] ? hkVector4Comparison::MASK_W : 0);
	hkVector4Comparison	cmp; cmp.set((hkVector4Comparison::Mask)msk);
	return cmp;
}

inline void hkIntVector::loadUnaligned4(const hkUint32* p)
{
//	HK_ASSERT2(0x612f84c7, ((hkUlong)p & 3) == 0, "pointer for hkIntVector::loadUnaligned4 must be 4-byte aligned");
	hkQuadUintUnion qu;
	qu.u[0] = p[0];
	qu.u[1] = p[1];
	qu.u[2] = p[2];
	qu.u[3] = p[3];
	this->m_quad = qu.q;
}

inline void hkIntVector::storeUnaligned4(void* p)
{
	hkString::memCpy(p, &m_quad, 16);
}

//
// Logical operations
//

inline void hkIntVector::setNot(hkIntVectorParameter a)
{
	hkQuadUintUnion qu, qa;
	qa.q = a;

	qu.u[0] = ~qa.u[0];
	qu.u[1] = ~qa.u[1];
	qu.u[2] = ~qa.u[2];
	qu.u[3] = ~qa.u[3];

	this->m_quad = qu.q;
}

HK_FORCE_INLINE hkBool32 hkIntVector::isNegativeAssumingAllValuesEqual(  ) const
{
	hkQuadUintUnion qu;	qu.q = this->m_quad;
	hkUint32 x = qu.u[0];
	return x & 0x80000000;
}

inline void hkIntVector::setOr(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadUintUnion qu, qa, qb;
	qa.q = a;
	qb.q = b;

	qu.u[0] = qa.u[0] | qb.u[0];
	qu.u[1] = qa.u[1] | qb.u[1];
	qu.u[2] = qa.u[2] | qb.u[2];
	qu.u[3] = qa.u[3] | qb.u[3];

	this->m_quad = qu.q;
}

inline void hkIntVector::setAnd(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadUintUnion qu, qa, qb;
	qa.q = a;
	qb.q = b;

	qu.u[0] = qa.u[0] & qb.u[0];
	qu.u[1] = qa.u[1] & qb.u[1];
	qu.u[2] = qa.u[2] & qb.u[2];
	qu.u[3] = qa.u[3] & qb.u[3];

	this->m_quad = qu.q;
}

inline void hkIntVector::setXor(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadUintUnion qu, qa, qb;
	qa.q = a;
	qb.q = b;

	qu.u[0] = qa.u[0] ^ qb.u[0];
	qu.u[1] = qa.u[1] ^ qb.u[1];
	qu.u[2] = qa.u[2] ^ qb.u[2];
	qu.u[3] = qa.u[3] ^ qb.u[3];

	this->m_quad = qu.q;
}

inline void hkIntVector::setAndC(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadUintUnion qu, qa, qb;
	qa.q = a;
	qb.q = b;

	qu.u[0] = qa.u[0] & ~qb.u[0];
	qu.u[1] = qa.u[1] & ~qb.u[1];
	qu.u[2] = qa.u[2] & ~qb.u[2];
	qu.u[3] = qa.u[3] & ~qb.u[3];

	this->m_quad = qu.q;
}

//
// Arithmetic operations
//

inline void hkIntVector::setSubU32( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadUintUnion qu, qa, qb;
	qa.q = a; qb.q = b;
	qu.u[0] = qa.u[0] - qb.u[0];
	qu.u[1] = qa.u[1] - qb.u[1];
	qu.u[2] = qa.u[2] - qb.u[2];
	qu.u[3] = qa.u[3] - qb.u[3];
	m_quad = qu.q;
}

inline void hkIntVector::setAddU32( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadUintUnion qu, qa, qb;
	qa.q = a; qb.q = b;
	qu.u[0] = qa.u[0] + qb.u[0];
	qu.u[1] = qa.u[1] + qb.u[1];
	qu.u[2] = qa.u[2] + qb.u[2];
	qu.u[3] = qa.u[3] + qb.u[3];
	m_quad = qu.q;
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
	hkQuadShortUnion qu, qa, qb;
	qa.q = a; qb.q = b; qu.q = a;

	const int max = +32767;
	const int min = -32768;

	for(int i = 0 ; i < 8 ; i++)
	{
		qu.u[i] = (hkInt16)hkMath::clamp((int)(qa.u[i] + qb.u[i]), min, max);
	}

	m_quad = qu.q;
}

inline void hkIntVector::setAddSaturateU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadUshortUnion qu, qa, qb;
	qa.q = a; qb.q = b; qu.q = a;

	const hkUint16 max = hkUint16(-1);
	for (int i=0; i<8; i++)
	{
		qu.u[i] = (qa.u[i] < max - qb.u[i]) ? (qa.u[i] + qb.u[i]) : max; 
	}

	m_quad = qu.q;
}


inline void hkIntVector::setSubSaturateU32( hkIntVectorParameter a, hkIntVectorParameter b )
{
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
	hkQuadShortUnion qu, qa, qb;
	qa.q = a; qb.q = b; qu.q = a;

	const int min = -32768;
	const int max = +32767;
	for(int i = 0 ; i < 8 ; i++)
	{
		qu.u[i] = (hkInt16)hkMath::clamp((int)(qa.u[i] - qb.u[i]), min, max); 
	}

	m_quad = qu.q;
}

inline void hkIntVector::setSubSaturateU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadUshortUnion qu, qa, qb;
	qa.q = a; qb.q = b; qu.q = a;

	for (int i=0; i<8; i++)
	{
		qu.u[i] = (qa.u[i] > qb.u[i]) ? (qa.u[i] - qb.u[i]) : 0; 
	}

	m_quad = qu.q;
}

inline void hkIntVector::setMinU8( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadUcharUnion qu, qa, qb;
	qa.q = a;
	qb.q = b;
	qu.q = a;
	
	for(int i = 0; i < 16; i++)
	{
		qu.u[i] = hkMath::min2(qa.u[i], qb.u[i]);  
	}
	
	m_quad = qu.q;
}


//
// Shift operations
//

template<int shift> inline void hkIntVector::setShiftLeft32( hkIntVectorParameter a)
{
	hkQuadUintUnion qu, qa;
	qa.q = a;

	qu.u[0] = qa.u[0] << shift;
	qu.u[1] = qa.u[1] << shift;
	qu.u[2] = qa.u[2] << shift;
	qu.u[3] = qa.u[3] << shift;

	m_quad = qu.q;
}

template<int shift> inline void hkIntVector::setShiftLeft128( hkIntVectorParameter a)
{
	hkQuadUcharUnion qu, qa;
	qa.q = a;
	qu.q = a;

	for(int j = 16 - shift ; j < 16 ; j++)
	{
		qu.u[j] = 0;
	}

	for(int i = 15 - shift ; i >= 0 ; i--)
	{
		qu.u[i] = qa.u[i + shift];
	}

	m_quad =  qu.q;
}

template<int shift> inline void hkIntVector::setShiftRight128( hkIntVectorParameter a)
{
	hkQuadUcharUnion qu, qa;
	qa.q = a;
	qu.q = a;

	for(int i = 15, j = 15 - shift ; i >= shift ; i--, j--)
	{
		qu.u[i] = qa.u[j];
	}
	for(int j = shift - 1 ; j >= 0 ; j--)
	{
		qu.u[j] = 0;
	}

	m_quad =  qu.q;
}

template<int shift> inline void hkIntVector::setShiftLeft16( hkIntVectorParameter a)
{
	hkQuadUshortUnion qu, qa;
	qa.q = a;

	qu.u[0] = qa.u[0] << shift;
	qu.u[1] = qa.u[1] << shift;
	qu.u[2] = qa.u[2] << shift;
	qu.u[3] = qa.u[3] << shift;
	qu.u[4] = qa.u[4] << shift;
	qu.u[5] = qa.u[5] << shift;
	qu.u[6] = qa.u[6] << shift;
	qu.u[7] = qa.u[7] << shift;

	m_quad = qu.q;
}

template<int shift> inline void hkIntVector::setShiftRight16( hkIntVectorParameter a)
{
	hkQuadUshortUnion qu, qa;
	qa.q = a;

	qu.u[0] = qa.u[0] >> shift;
	qu.u[1] = qa.u[1] >> shift;
	qu.u[2] = qa.u[2] >> shift;
	qu.u[3] = qa.u[3] >> shift;
	qu.u[4] = qa.u[4] >> shift;
	qu.u[5] = qa.u[5] >> shift;
	qu.u[6] = qa.u[6] >> shift;
	qu.u[7] = qa.u[7] >> shift;

	m_quad = qu.q;
}

template<int shift> inline void hkIntVector::setShiftRight32( hkIntVectorParameter a)
{

	hkQuadUintUnion qu, qa;
	qa.q = a;

	qu.u[0] = qa.u[0] >> shift;
	qu.u[1] = qa.u[1] >> shift;
	qu.u[2] = qa.u[2] >> shift;
	qu.u[3] = qa.u[3] >> shift;

	m_quad = qu.q;
}

inline void hkIntVector::setShiftLeft32( hkIntVectorParameter a, hkIntVectorParameter shift )
{

	hkQuadUintUnion qu, qa, qshift;
	qa.q = a;
	qshift.q = shift;

	qu.u[0] = qa.u[0] << qshift.u[0];
	qu.u[1] = qa.u[1] << qshift.u[1];
	qu.u[2] = qa.u[2] << qshift.u[2];
	qu.u[3] = qa.u[3] << qshift.u[3];

	m_quad = qu.q;
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
}

//
// Merge operations
//
inline void hkIntVector::setMergeHead32(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadUintUnion qu, qa, qb;
	qa.q = a; qb.q = b;

	qu.u[0] = qa.u[0]; 
	qu.u[1] = qb.u[0];
	qu.u[2] = qa.u[1];
	qu.u[3] = qb.u[1];
	m_quad = qu.q;
}

inline void hkIntVector::setMergeTail32(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadUintUnion qu, qa, qb;
	qa.q = a; qb.q = b;

	qu.u[0] = qa.u[2];
	qu.u[1] = qb.u[2];
	qu.u[2] = qa.u[3];
	qu.u[3] = qb.u[3];
	m_quad = qu.q;
}

inline void hkIntVector::setMergeHead16(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadUshortUnion qu, qa, qb;
	qa.q = a; qb.q = b; qu.q = a;

	for (int i=0, j=0; i<8; i+=2, j++)
	{
		qu.u[ i ] = qa.u[j];
		qu.u[i+1] = qb.u[j];
	}

	m_quad = qu.q;
}

inline void hkIntVector::setMergeTail16(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadUshortUnion qu, qa, qb;
	qa.q = a; qb.q = b; qu.q = a;

	for (int i=0, j=4; i<8; i+=2, j++)
	{
		qu.u[ i ] = qa.u[j];
		qu.u[i+1] = qb.u[j];
	}

	m_quad = qu.q;
}

inline void hkIntVector::setMergeHead8(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadCharUnion qu, qa, qb;
	qa.q = a; qb.q = b; qu.q = a;

	for (int i=0, j=0; i<16; i+=2, j++)
	{
		qu.u[ i ] = qa.u[j];
		qu.u[i+1] = qb.u[j];
	}

	m_quad = qu.q;
}

inline void hkIntVector::setMergeTail8(hkIntVectorParameter a, hkIntVectorParameter b)
{
	hkQuadCharUnion qu, qa, qb;
	qa.q = a; qb.q = b; qu.q = a;

	for (int i=0, j=8; i<16; i+=2, j++)
	{
		qu.u[ i ] = qa.u[j];
		qu.u[i+1] = qb.u[j];
	}

	m_quad = qu.q;
}


//
// Pack operations
//
inline void hkIntVector::setConvertU32ToU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadShortUnion qu;
	qu.u[0] = (hkUint16)a.getU32(0);	// shifts are faster than load, as the input values are already in registers
	qu.u[1] = (hkUint16)a.getU32(1);
	qu.u[2] = (hkUint16)a.getU32(2);
	qu.u[3] = (hkUint16)a.getU32(3);

	qu.u[4] = (hkUint16)b.getU32(0);
	qu.u[5] = (hkUint16)b.getU32(1);
	qu.u[6] = (hkUint16)b.getU32(2);
	qu.u[7] = (hkUint16)b.getU32(3);
	
	m_quad = qu.q;
}

inline void hkIntVector::setConvertSaturateS32ToU16( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadUshortUnion qu;
	hkQuadIntUnion qa, qb;
	qa.q = a;	qb.q = b;

	const hkInt32 min = 0;
	const hkInt32 max = 0x10000;
	qu.u[0]		= (hkUint16)hkMath::clamp(qa.u[0], min, max);
	qu.u[1]		= (hkUint16)hkMath::clamp(qa.u[1], min, max);
	qu.u[2]		= (hkUint16)hkMath::clamp(qa.u[2], min, max);
	qu.u[3]		= (hkUint16)hkMath::clamp(qa.u[3], min, max);

	qu.u[4]		= (hkUint16)hkMath::clamp(qb.u[0], min, max);
	qu.u[5]		= (hkUint16)hkMath::clamp(qb.u[1], min, max);
	qu.u[6]		= (hkUint16)hkMath::clamp(qb.u[2], min, max);
	qu.u[7]		= (hkUint16)hkMath::clamp(qb.u[3], min, max);

	m_quad = qu.q;
}

inline void hkIntVector::setConvertSaturateS16ToU8( hkIntVectorParameter a, hkIntVectorParameter b )
{
	hkQuadUcharUnion qu;
	hkQuadShortUnion qa, qb;
	qa.q = a;	qb.q = b;

	const hkInt16 min = 0;
	const hkInt16 max = 255;
	qu.u[0]		= (hkUint8)hkMath::clamp(qa.u[0], min, max);
	qu.u[1]		= (hkUint8)hkMath::clamp(qa.u[1], min, max);
	qu.u[2]		= (hkUint8)hkMath::clamp(qa.u[2], min, max);
	qu.u[3]		= (hkUint8)hkMath::clamp(qa.u[3], min, max);
	qu.u[4]		= (hkUint8)hkMath::clamp(qa.u[4], min, max);
	qu.u[5]		= (hkUint8)hkMath::clamp(qa.u[5], min, max);
	qu.u[6]		= (hkUint8)hkMath::clamp(qa.u[6], min, max);
	qu.u[7]		= (hkUint8)hkMath::clamp(qa.u[7], min, max);

	qu.u[8]		= (hkUint8)hkMath::clamp(qb.u[0], min, max);
	qu.u[9]		= (hkUint8)hkMath::clamp(qb.u[1], min, max);
	qu.u[10]	= (hkUint8)hkMath::clamp(qb.u[2], min, max);
	qu.u[11]	= (hkUint8)hkMath::clamp(qb.u[3], min, max);
	qu.u[12]	= (hkUint8)hkMath::clamp(qb.u[4], min, max);
	qu.u[13]	= (hkUint8)hkMath::clamp(qb.u[5], min, max);
	qu.u[14]	= (hkUint8)hkMath::clamp(qb.u[6], min, max);
	qu.u[15]	= (hkUint8)hkMath::clamp(qb.u[7], min, max);

	m_quad = qu.q;
}

//
// Shuffle
//
template<int A, int B, int C, int D> HK_FORCE_INLINE void hkIntVector::setShuffle( hkIntVectorParameter a )
{
	hkQuadUintUnion qu, qa;
	qa.q = a;

	qu.u[0] = qa.u[A];
	qu.u[1] = qa.u[B];
	qu.u[2] = qa.u[C];
	qu.u[3] = qa.u[D];

	m_quad = qu.q;
}

inline void hkIntVector::setPermuteU8(hkIntVectorParameter a, hkIntVectorParameter mask)
{
	hkQuadUcharUnion qu, qa, qi;
	qa.q = a;
	qu.q = a;
	qi.q = mask;

	for(int i = 0; i < 16; i++)
	{
		qu.u[i] = qa.u[qi.u[i]];
	}
	m_quad = qu.q;
}

inline void hkIntVector::setPermuteU8(hkIntVectorParameter a, hkIntVectorParameter b, hkIntVectorParameter mask)
{
	hkQuadUcharUnion qa, qb, qi, qu;
	qa.q = a;
	qb.q = b;
	qi.q = mask;
	qu.q = a;
	const hkUint8* v[2] = {qa.u, qb.u};
	const hkUint8* idx	= qi.u;

	for(int i = 0; i < 16; i++)
	{
		qu.u[i] = v[idx[i] >> 4][idx[i] & 0xF];
	}

	m_quad = qu.q;
}

//
// Splat
//
template<int val> inline void hkIntVector::splatImmediate32()
{
	HK_COMPILE_TIME_ASSERT( val <=  15 );
	HK_COMPILE_TIME_ASSERT( val >= -16 );

	hkQuadIntUnion qu;
	qu.u[0] = val;
	qu.u[1] = val;
	qu.u[2] = val;
	qu.u[3] = val;
	m_quad = qu.q;
}

template<int val> inline void hkIntVector::splatImmediate16()
{
	HK_COMPILE_TIME_ASSERT( val <=  15 );
	HK_COMPILE_TIME_ASSERT( val >= -16 );

	hkQuadUshortUnion qu; 
	qu.q = m_quad; // avoid uninitialized variable warning
	for (int i=0; i<8; i++)
	{
		qu.u[i] = (hkUint16) val;
	}
	m_quad = qu.q;
}

template<int val> inline void hkIntVector::splatImmediate8()
{
	HK_COMPILE_TIME_ASSERT( val <=  15 );
	HK_COMPILE_TIME_ASSERT( val >= -16 );

	hkQuadCharUnion qu; 
	qu.q = m_quad; // avoid uninitialized variable warning
	for (int i=0; i<16; i++)
	{
		qu.u[i] = (hkUint8) val;
	}
	m_quad = qu.q;
}

template<int idx> inline void hkIntVector::setBroadcast(hkIntVectorParameter a)
{
	hkQuadUintUnion qu, qa;
	qa.q = a; 

	qu.u[0] = qa.u[idx];
	qu.u[1] = qa.u[idx];
	qu.u[2] = qa.u[idx];
	qu.u[3] = qa.u[idx];
	
	m_quad = qu.q;
}


//
// int <-> float conversion
//
inline void hkIntVector::convertU32ToF32(hkVector4& vOut) const
{
	hkQuadUintUnion qu;
	qu.q = m_quad;

	vOut(0) = (hkReal) qu.u[0];
	vOut(1) = (hkReal) qu.u[1];
	vOut(2) = (hkReal) qu.u[2];
	vOut(3) = (hkReal) qu.u[3];
}

inline void hkIntVector::convertS32ToF32(hkVector4& vOut) const
{
	hkQuadIntUnion qu;
	qu.q = m_quad;

	vOut(0) = (hkReal) qu.u[0];
	vOut(1) = (hkReal) qu.u[1];
	vOut(2) = (hkReal) qu.u[2];
	vOut(3) = (hkReal) qu.u[3];
}

inline void hkIntVector::setConvertF32toU32(hkVector4Parameter vIn)
{
	hkQuadUintUnion qu;

	qu.u[0] = (hkUint32) vIn(0);
	qu.u[1] = (hkUint32) vIn(1);
	qu.u[2] = (hkUint32) vIn(2);
	qu.u[3] = (hkUint32) vIn(3);

	m_quad = qu.q;
}

inline void hkIntVector::setConvertF32toS32(hkVector4Parameter vIn)
{
	hkQuadIntUnion qu;

	qu.u[0] = (hkInt32) vIn(0);
	qu.u[1] = (hkInt32) vIn(1);
	qu.u[2] = (hkInt32) vIn(2);
	qu.u[3] = (hkInt32) vIn(3);

	m_quad = qu.q;
}

// The following functions are valid for all platforms (there wont' be many of these)
inline void hkIntVector::loadAligned(const hkUint32* p)
{
#ifndef HK_ARCH_ARM
	HK_ASSERT2(0x3a8619dc, ((hkUlong)p & 0xf) == 0, "pointer for hkIntVector::loadAligned must be 16-byte aligned");
#endif
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
