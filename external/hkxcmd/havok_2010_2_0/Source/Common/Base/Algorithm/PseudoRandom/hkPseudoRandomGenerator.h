/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MATH_PSEUDORANDOMGENERATOR_H
#define HK_MATH_PSEUDORANDOMGENERATOR_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>

// This is a Pseudorandom Number generator from  Num. Recip. p284:  Knuth-Lewis "quick and dirty" rand,
// otherwise known as randq1(). It's not great, but it's fast. Don't use it for "serious" work.
class hkPseudoRandomGenerator
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkPseudoRandomGenerator);

		// Seed MUST be passed in on construction (to force you to think about it!)
	hkPseudoRandomGenerator(int s)
	{
		m_seed = hkUint32(s);
		m_current = m_seed;
	}

		// Can also reseed if desired. It wipes over  the last used, so effectively "restarts" the RNG
	void setSeed(int s)
	{
		m_seed = hkUint32(s);
		m_current = m_seed;
	}

		// Get seed used by generator. This may be useful to determine how the RNG was "started".
	int getSeed()
	{
		return int(m_seed);
	}

		// Get current value (NOT equal to getRand32), but will allow you to "monitor" the RNG so you
		// can reseed it later. 
	int getCurrent()
	{
		return int(m_current);
	}

		// Get random number as unsigned int
	hkUint32 getRand32()
	{
		m_current = 1664525U * m_current + 1013904223U;		
		return m_current;
	}

		// Get random integer in range [0-X) as unsigned int, with X<=256. Suitable for small values.
	hkUint32 getRandChar(int x)
	{
		HK_ASSERT(0x777504aa,  x > 0 && x <= 256 );
		m_current = 1664525L * m_current + 1013904223L;	
		
		hkUint32 temp = (hkUint32)m_current;
		temp >>= 13;
		temp = temp % x;

		HK_ASSERT(0x3443045a, temp < 256 );
		return temp;
	}

		// Get random integer in range [0-X) as unsigned int, with X<=65536. Suitable for medium values.
	hkUint32 getRandInt16(int x)
	{
		HK_ASSERT(0x777504ab,  x > 0 && x <= 65536 );
		m_current = 1664525L * m_current + 1013904223L;	

		hkUint32 temp = (hkUint32)m_current;
		temp >>= 11;
		temp = temp % x;

		HK_ASSERT(0x3443045b, temp < 65536 );
		return temp;
	}

		// Get random real in range [0,1] using getRand32()
	hkReal getRandReal01()
	{	
		const hkReal v = getRand32()*( 1.0f / 0xffffffff);
		return v;
	}

		// Get random real in range [-1,1] using getRand32()
	hkReal getRandReal11()
	{	
		return 2*getRandReal01() - 1.0f;
	}

		// Get random real in range [min,max] using getRand01()
	hkReal getRandRange( hkReal min, hkReal max )
	{
		return getRandReal01() * ( max - min ) + min;
	}

	void getRandomRotation( hkRotation& rotOut )
	{
		hkVector4 v;
		v(0) = getRandReal11();
		v(1) = getRandReal11();
		v(2) = getRandReal11();
		v(3) = getRandReal11();
		v.normalize4();
		hkQuaternion q;
		q.m_vec = v;
		rotOut.set( q );
	}

	void getRandomRotation( hkQuaternion& rotOut )
	{
		hkVector4 v;
		v(0) = getRandReal11();
		v(1) = getRandReal11();
		v(2) = getRandReal11();
		v(3) = getRandReal11();
		v.normalize4();
		rotOut.m_vec = v;
	}

		/// sets xyzw randomly between -1 and 1
	void getRandomVector11( hkVector4& vecOut )
	{
		vecOut(0) = getRandReal11();
		vecOut(1) = getRandReal11();
		vecOut(2) = getRandReal11();
		vecOut(3) = getRandReal11();
	}

		/// sets xyzw randomly between 0 and 1
	void getRandomVector01( hkVector4& vecOut )
	{
		vecOut(0) = getRandReal01();
		vecOut(1) = getRandReal01();
		vecOut(2) = getRandReal01();
		vecOut(3) = getRandReal01();
	}

	void getRandomVectorRange( hkVector4Parameter min, hkVector4Parameter max, hkVector4& vecOut)
	{
		hkVector4 t; getRandomVector01(t);
		hkVector4 delta; delta.setSub4(max, min);
		vecOut.setAddMul4(min, delta, t);
	}

	template<typename T>
	void shuffle( T* t, int num )
	{
		for( int n = num; n >= 2; --n )
		{
			int k = getRandInt16(n);
			hkAlgorithm::swap( t[k], t[n-1] );
		}
	}

private:
	hkUint32 m_seed;
	hkUint32 m_current;
};

#endif // HK_MATH_PSEUDORANDOMGENERATOR_H

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
