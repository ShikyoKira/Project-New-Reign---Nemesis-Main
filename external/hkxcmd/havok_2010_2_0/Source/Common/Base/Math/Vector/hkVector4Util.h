/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATH_VECTOR3_UTIL_H
#define HK_MATH_VECTOR3_UTIL_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

class hkStringBuf;

union hkIntUnion64
{
	hkQuadReal quad;
	hkInt64 i64;
	hkUint64 u64;
	hkInt32 i32[2];
	hkUint32 u32[2];
	hkInt16 i16[4];
	hkUint16 u16[4];
	hkInt8 i8[8];
	hkUint8 u8[8];
};

namespace hkVector4Util
{
		/// Sets the calling vector to be the normal to the 2 vectors (b-a) and (c-a).
		///
		/// NOTE: The calculated result is not explicitly normalized.
		/// This function is particularly fast on PlayStation(R)2.
		/// Result = (b-a) cross (c-a)
	HK_FORCE_INLINE void HK_CALL setNormalOfTriangle(hkVector4& result, const hkVector4 &a, const hkVector4& b,const hkVector4& c);

		/// Multiplies sign bits from signsSource onto inout leaving the mantissa and exponent untouched.
	HK_FORCE_INLINE void HK_CALL mulSigns4( hkVector4& inout, const hkVector4& signsSource);

		/// 
	HK_FORCE_INLINE void HK_CALL convertToUint16( const hkVector4& in, const hkVector4& offset, const hkVector4& scale, hkIntUnion64& out );

		/// 
	HK_FORCE_INLINE void HK_CALL convertToUint16WithClip( const hkVector4& in, const hkVector4& offset, const hkVector4& scale, const hkVector4& min, const hkVector4& max, hkIntUnion64& out);

		/// calculates a value x so that convertToUint16WithClip( out, in + x/scale, ... ) == out = int(floor( (in+offset)*scale
	hkReal HK_CALL getFloatToInt16FloorCorrection();

		/// 
	HK_FORCE_INLINE void HK_CALL convertToUint32( const hkVector4& in, const hkVector4& offset, const hkVector4& scale, hkUint32* out );

		///
	HK_FORCE_INLINE void HK_CALL convertToUint32WithClip( const hkVector4& in, const hkVector4& offset, const hkVector4& scale, const hkVector4& min, const hkVector4& max, hkUint32* out);

		/// calculates a value x so that convertToUint32WithClip( out, in + x/scale, ... ) == out = int(floor( (in+offset)*scale
	hkReal HK_CALL getFloatToInt32FloorCorrection();

		/// Converts a floating-point AABB to an integer AABB
	HK_FORCE_INLINE void HK_CALL convertAabbToUint32( const hkAabb& aabb, const hkVector4& offsetLow, const hkVector4& offsetHigh, const hkVector4& scale, hkAabbUint32& aabbOut );

		/// Converts an integer AABB to a floating-point AABB
	HK_FORCE_INLINE void HK_CALL convertAabbFromUint32( const hkAabbUint32& aabbIn, const hkVector4& offsetLow, const hkVector4& scale, hkAabb& aabbOut);

		/// Store the difference between a big expandedAabbInt and a smaller, enclosed unexpandedAabbInt_InOut in m_expansionXXX components of unexpandedAabbInt_InOut.
	HK_FORCE_INLINE void HK_CALL compressExpandedAabbUint32(const hkAabbUint32& expandedAabbInt, hkAabbUint32& unexpandedAabbInt_InOut);

		/// Expand a compressed unexpandedAabbInt to an expandedAabbOut using the information stored in the m_expansionXXX components.
	HK_FORCE_INLINE void HK_CALL uncompressExpandedAabbUint32(const hkAabbUint32& unexpandedAabbInt, hkAabbUint32& expandedAabbOut);

		/// Finds a vector that is perpendicular to a line segment.
		///
		/// Achieved by constructing a vector from the segment vector with the following properties:
		/// (Segment vector is any vector parallel to the line segment.)
		/// 1) component with the smallest index is set to 0
		/// 2) the remaining two component are copied into the new vector but are swapped in position
		/// 3) one of the copied components is multiplied by -1.0 (has its sign flipped!)
		///
		/// leaving: (for example)
		/// segmentVector = (x, y, z), with let's say y as the smallest component
		/// segmentNormal = (-z, 0 , x) or (z, 0, -x), either will do.
		///
		/// This will in fact be orthogonal as (in the example) the dot product will be zero.
		/// i.e., x*-z + y*0 + z*x = 0
		///
	HK_FORCE_INLINE void HK_CALL calculatePerpendicularVector(const hkVector4& vectorIn, hkVector4& biVectorOut);

		/// Transforms an array of points.
	HK_FORCE_INLINE void HK_CALL transformPoints( const hkTransform& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut );
	HK_FORCE_INLINE void HK_CALL transformPoints( const hkQTransform& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut );

		/// Transforms an array of points including the .w component
	HK_FORCE_INLINE void HK_CALL mul4xyz1Points( const hkTransform& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut );

		/// Transforms an array of spheres (keeps the .w component as the radius)
	HK_FORCE_INLINE void HK_CALL transformSpheres( const hkTransform& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut );

		/// Transforms an array of plane equations
	HK_FORCE_INLINE void HK_CALL transformPlaneEquations( const hkTransform& t, const hkVector4* planesIn, int numPlanes, hkVector4* planesOuts );




		/// Rotate an array of points.
	HK_FORCE_INLINE void HK_CALL rotatePoints( const hkMatrix3& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut );

		/// Invert rotate an array of points.
	HK_FORCE_INLINE void HK_CALL rotateInversePoints( const hkRotation& t, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut );

		/// set aTcOut = aTb * bTc
	HK_FORCE_INLINE void HK_CALL setMul( const hkMatrix3& aTb, const hkMatrix3& bTc, hkMatrix3& aTcOut );

		/// set aTcOut = bTa^1 * bTc
	HK_FORCE_INLINE void HK_CALL setInverseMul( const hkRotation& bTa, const hkMatrix3& bTc, hkMatrix3& aTcOut );

		/// set aTcOut = aTb * bTc
	HK_FORCE_INLINE void HK_CALL transpose( hkMatrix3& m );

		/// m += add
	HK_FORCE_INLINE void HK_CALL add( hkMatrix3& m, const hkMatrix3& add );


		/// Sets this vector components: this(0) = a0.dot3(b0), this(1) = a1.dot3(b1) ...
	HK_FORCE_INLINE void HK_CALL dot3_3vs3( const hkVector4& a0, const hkVector4& b0, const hkVector4& a1, const hkVector4& b1, const hkVector4& a2, const hkVector4& b2, hkVector4& dotsOut);

		/// Sets this vector components: this(0) = a0.dot3(b0) ... this(3) = a3.dot3(b3)
	HK_FORCE_INLINE void HK_CALL dot3_4vs4( const hkVector4& a0, const hkVector4& b0, const hkVector4& a1, const hkVector4& b1, const hkVector4& a2, const hkVector4& b2, const hkVector4& a3, const hkVector4& b3, hkVector4& dotsOut);

		/// Sets this vector components: this(0) = a0.dot4(b0) ... this(3) = a3.dot4(b3)
	HK_FORCE_INLINE void HK_CALL dot4_4vs4( const hkVector4& a0, const hkVector4& b0, const hkVector4& a1, const hkVector4& b1, const hkVector4& a2, const hkVector4& b2, const hkVector4& a3, const hkVector4& b3, hkVector4& dotsOut);

		/// Sets this vector components: this(i) = vector.dot3(AI) for i=0..3
	HK_FORCE_INLINE void HK_CALL dot3_1vs4( const hkVector4& vectorIn, const hkVector4& a0, const hkVector4& a1, const hkVector4& a2, const hkVector4& a3, hkVector4& dotsOut);

		/// Sets this vector components: this(i+j) = AI.dot3(bj)
	HK_FORCE_INLINE void HK_CALL dot3_2vs2( const hkVector4& a0, const hkVector4& a2, const hkVector4& b0, const hkVector4& b1, hkVector4& dotsOut);


		/// build an orthonormal matrix, where the first column matches the parameter dir.
		/// Note: Dir must be normalized
	HK_FORCE_INLINE void HK_CALL buildOrthonormal( const hkVector4& dir, hkMatrix3& out );

		/// build an orthonormal matrix, where the first column matches the parameter dir and the second
		/// column matches dir2 as close as possible
		/// Note: Dir must be normalized
	HK_FORCE_INLINE void HK_CALL buildOrthonormal( const hkVector4& dir, const hkVector4& dir2, hkMatrix3& out );

		/// out = in^-1, in must be symmetric and invertible
	HK_FORCE_INLINE void HK_CALL invertSymmetric( const hkMatrix3& in, hkReal eps, hkMatrix3& out );
		/// Resets the fpu after using MMX instructions on x86. No-op for other architectures.
	HK_FORCE_INLINE void HK_CALL exitMmx();

		/// Returns the squared distance from p to the line segment ab
	HK_FORCE_INLINE hkSimdReal HK_CALL distToLineSquared( const hkVector4& a, const hkVector4& b, const hkVector4& p );

		/// Set the of 'out' to the maximum of 'a','b','c' and 'd'
	HK_FORCE_INLINE void HK_CALL setMax44(const hkVector4& a, const hkVector4& b, const hkVector4& c, const hkVector4& d, hkVector4& out);

		/// Set the of 'out' to the minimum of 'a','b','c' and 'd'
	HK_FORCE_INLINE void HK_CALL setMin44(const hkVector4& a, const hkVector4& b, const hkVector4& c, const hkVector4& d, hkVector4& out);

		/// Helper function for use with hkAlgorithm::findMinimumIndex, etc.
		/// Sample usage (finding the index of the element of "vectors" that is closest to "point"):
		/// \code int minIndex = hkAlgorithm::findMinimumIndex( vectors.begin(), vectors.getSize(), hkVector4Util::DistanceToPoint( point ) ); \endcode
	struct DistanceToPoint
	{
		HK_FORCE_INLINE DistanceToPoint( hkVector4Parameter p ) : m_p(p) { }
		HK_FORCE_INLINE hkReal operator() (hkVector4Parameter p) const { return m_p.distanceToSquared3(p).getReal(); }

		hkVector4 m_p;
	};

		/// check for free registers for computers with lot's of registers (e.g. PlayStation(R)2)
	HK_FORCE_INLINE void HK_CALL checkRegisters( hkUint32 registerMask );

		/// reserve registers for computers with lots of registers (e.g. PlayStation(R)2)
	HK_FORCE_INLINE void HK_CALL reserveRegisters( hkUint32 registerMask );

		/// release these registers
	HK_FORCE_INLINE void HK_CALL releaseRegisters( hkUint32 registerMask );

		//
		//	convertions
		//

		/// convert an hkVector4 to a string representing the x,y,z components in that order.
	const char* HK_CALL toString3(const hkVector4& x, hkStringBuf& s, char separator=',');

		/// convert an hkVector4 to a string representing the x,y,z,w components in that order.
	const char* HK_CALL toString4(const hkVector4& x, hkStringBuf& s, char separator=',');

		//
		//	compression
		//

		// packs a normalized quaternion into a single 4*8 bit integer. The error is roughly 0.01f per component
	hkUint32 HK_CALL packQuaternionIntoInt32( hkVector4Parameter qin);

		// unpack an 32 bit integer into quaternion. Note: The resulting quaternion is not normalized ( |q.length4()-1.0f| < 0.04f )
	HK_FORCE_INLINE void HK_CALL unPackInt32IntoQuaternion( hkUint32 ivalue, hkVector4& qout );

	HK_FORCE_INLINE void memClear16( void* dest, int numQuads);

	extern hkUint32 m_reservedRegisters;

	enum { AABB_UINT32_MAX_FVALUE = 0x7ffe0000 };
	extern hkQuadReal hkAabbUint32MaxVal;
}

// For internal use. Define this to check that register allocations
// between functions do not clash. Disabled by default
// because it does not work in a multithreaded environment.
//#	define HK_CHECK_REG_FOR_PS2
HK_FORCE_INLINE void HK_CALL hkVector4Util::reserveRegisters( hkUint32 registerMask )
{
#ifdef HK_CHECK_REG_FOR_PS2
	HK_ASSERT2(0x23c4e825,  (registerMask & m_reservedRegisters) == 0, "Register clash");
	m_reservedRegisters |= registerMask;
#endif
}

HK_FORCE_INLINE void HK_CALL hkVector4Util::checkRegisters( hkUint32 registerMask )
{
#ifdef HK_CHECK_REG_FOR_PS2
	HK_ASSERT2(0x2459dd7d,  (registerMask & m_reservedRegisters) == 0, "Register clash");
#endif
}

HK_FORCE_INLINE void HK_CALL hkVector4Util::releaseRegisters( hkUint32 registerMask )
{
#ifdef HK_CHECK_REG_FOR_PS2
	m_reservedRegisters &= ~registerMask;
#endif
}

#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
#	if defined(HK_COMPILER_HAS_INTRINSICS_IA32)
#		include <Common/Base/Math/Vector/Sse/hkSseVector4Util.inl>
#	elif defined(HK_COMPILER_HAS_INTRINSICS_NEON)
#		include <Common/Base/Math/Vector/Neon/hkNeonVector4Util.inl> 
#	elif defined(HK_PLATFORM_PS3_SPU) || defined(HK_PLATFORM_PS3_PPU)
#		include <Common/Base/Math/Vector/Ps3/hkPs3Vector4Util.inl>
#	elif defined(HK_PLATFORM_XBOX360)
#		include <Common/Base/Math/Vector/Xbox360/hkXbox360Vector4Util.inl>
#	endif
#endif // HK_CONFIG_SIMD

#include <Common/Base/Math/Vector/hkVector4Util.inl>

#endif // HK_MATH_VECTOR3_UTIL_H

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
