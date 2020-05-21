/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATH_MATH_UTIL_H
#define HK_MATH_MATH_UTIL_H

#include <Common/Base/hkBase.h>

namespace hkMathUtil
{
	// [row0col0, row0col1, row1col0, row1col1]
	inline hkBool HK_CALL invert2x2Matrix(const hkReal* m, hkReal* out, hkReal tolerance);

	/// Structure returned by decompose4x4ColTransform
	struct Decomposition
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MATH, Decomposition );
		hkVector4 m_translation;	// T
		hkQuaternion m_rotation;	// R
		hkVector4 m_scale;			// S

		hkBool m_hasScale;			// True if S !=(1,1,1)
		hkBool m_hasSkew;			// True if T*R*S != M (original Matrix)
		hkBool m_flips;				// True if Det(M)<0.0f

		hkMatrix4 m_scaleAndSkew;	// M = T*R * scaleAndSkew
		hkMatrix4 m_skew;			// M = T*R*S * skew

		hkRotation m_basis;			// Rotation matrix, may involve flipping
		// If m_flips=false, then m_basis == m_rotation
	};

	/// Decomposes M (4x4 matrix) as M == T * R * Scale * Skew == T * R * ScaleAndSkew
	/// Useful for conversion both to orthogonal (no skew) and orthonormal (no scale or skew) transforms
	void HK_CALL decomposeMatrix (const class hkMatrix4& matrixIn, Decomposition& decomposition);

	/// Decomposes M (3x3 matrix). Same as decomposeMatrix for Matrix4, but translation component is Zero
	void HK_CALL decomposeMatrix (const class hkMatrix3& matrixIn, Decomposition& decomposition);

	/// Decomposes M (4x4 matrix, column-major) as M == T * R * Scale * Skew == T * R * ScaleAndSkew
	/// Useful for conversion both to orthogonal (no skew) and orthonormal (no scale or skew) transforms
	void HK_CALL decompose4x4ColTransform (const hkReal* matrixIn, Decomposition& decomposition);

	/// Calculates the greatest common divisor of A and B (using the Euclidean algorithm)
	inline int HK_CALL calculateGcd (int a, int b);

	/// Calculates the least common multiple of A and B (using the Euclidean algorithm)
	inline int HK_CALL calculateLcm (int a, int b);
}


hkBool HK_CALL hkMathUtil::invert2x2Matrix(const hkReal* m, hkReal* out, hkReal tolerance)
{
	const hkReal *co0 = m;
	const hkReal *co1 = co0 + 2;

	hkReal D =  (co0[0] * co1[1]) - (co0[1] * co1[0]);

	if(D*D < tolerance * tolerance){
		return false;  // may be an ordinary result
	}

	hkReal DI = 1.0f/D;
	hkReal a1 =  co1[1] * DI;
	hkReal b1 = -co0[1] * DI;
	hkReal a2 = -co1[0] * DI;
	hkReal b2 =  co0[0] * DI;

	hkReal *o_co0 = out;
	hkReal *o_co1 = o_co0 + 2;

	o_co0[0] = a1;
	o_co1[0] = b1;
	o_co0[1] = a2;
	o_co1[1] = b2;

	return true;
}

/// Calculates the greatest common divisor of A and B (using the Euclidean algorithm)
inline int HK_CALL hkMathUtil::calculateGcd (int a, int b)
{
	int t;
	while (b!=0)
	{
		t=b;
		b = a % b;
		a=t;
	}

	return a;
}

/// Calculates the least common multiple of A and B (using the Euclidean algorithm)
inline int HK_CALL hkMathUtil::calculateLcm (int a, int b)
{
	return (a*b) / calculateGcd (a,b);
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
