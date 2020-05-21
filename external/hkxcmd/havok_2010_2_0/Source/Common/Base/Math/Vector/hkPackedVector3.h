/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKMATH_MATH_PACKED_VECTOR3_H
#define HKMATH_MATH_PACKED_VECTOR3_H

#include <Common/Base/Math/Vector/hkIntVector.h>

/// Compressed 8 byte sized float vector3 with 1 sign bit, 15 bit mantissa and a shared 8 bit exponent.
/// 
/// This class is great to store positions or normals using an accuracy of 1.0f/32000
/// (e.g., a position which is 100meters off the origin has a precision of +-3mm.
/// pack() takes a little time (~100-200 cycles), unpack is pretty fast on SIMD platforms (6 assembly instructions)
struct hkPackedVector3
{
	public:

		HK_DECLARE_REFLECTION();

		/// pack a vector4 into this
		void pack( hkVector4Parameter v );

		// unpack
		HK_FORCE_INLINE void unpack( hkVector4& vOut ) const
		{
			// bring m_values into the high bits
			hkIntVector iv;		
			{
				iv.loadUnaligned4( (hkUint32*) m_values );
				hkIntVector zero; zero.setZero();
				iv.setCombineHead16To32( iv, zero );
			}

			// convert to integer
			hkVector4 v;		iv.convertS32ToF32( v );

			// calculate the exp correction
			hkIntVector iexp; 	iexp.setBroadcast<3>( iv );
			v.mul4( (hkVector4&)iexp );

			vOut = v;
		}

	public:
		HK_ALIGN( hkInt16 m_values[4], 4 );
};

#endif //HKMATH_MATH_PACKED_VECTOR3_H

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
