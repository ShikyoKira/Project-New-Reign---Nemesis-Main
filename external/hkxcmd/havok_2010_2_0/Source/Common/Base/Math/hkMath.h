/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATH_MATH_H
#define HK_MATH_MATH_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Common/Base/Config/hkConfigSimd.h>

// Temporarily disable the alignment warning for AMD64
#if defined(HK_PLATFORM_X64)
#	pragma warning( disable: 4328 )
#endif

typedef float hkReal;


	/// A list of constants.
	/// \warning Do not change the order of the following constants and keep synchronized with value array: g_vectorConstants[] because algorithms rely on this order.
enum hkVectorConstant
{
	HK_QUADREAL_MINUS1,
	HK_QUADREAL_0,
	HK_QUADREAL_1,
	HK_QUADREAL_2,
	HK_QUADREAL_3,
	HK_QUADREAL_4,
	HK_QUADREAL_5,
	HK_QUADREAL_6,
	HK_QUADREAL_7,
	HK_QUADREAL_15,
	HK_QUADREAL_16,
	HK_QUADREAL_255,
	HK_QUADREAL_256,
	HK_QUADREAL_2_POW_23,

	HK_QUADREAL_INV_0,	///< 0.0f, 0.0f, 0.0f
	HK_QUADREAL_INV_1,	///< 1 1 1 1
	HK_QUADREAL_INV_2,	///< .5 .5 .5 .5
	HK_QUADREAL_INV_3,	///< 1/3 1/3 1/3 1/3
	HK_QUADREAL_INV_4,
	HK_QUADREAL_INV_5,
	HK_QUADREAL_INV_6,
	HK_QUADREAL_INV_7,
	HK_QUADREAL_INV_15,		///< 1/15 x 4
	HK_QUADREAL_INV_255,	///< 1/255 x 4
	HK_QUADREAL_1000,
	HK_QUADREAL_0100,
	HK_QUADREAL_0010,
	HK_QUADREAL_0001,
	HK_QUADREAL_MAX,
	HK_QUADREAL_EPS,
	HK_QUADREAL_EPS_SQRD, ///< Eps squared, really really tiny number
	HK_QUADREAL_m11m11, ///< -1,1,-1,1
	HK_QUADREAL_8421, ///< 8,4,2,1
	HK_QUADREAL_PACK_HALF, ///< Used to pack hkHalfs
	HK_QUADREAL_PACK16_UNIT_VEC, ///< Used to compress unit vectors
	HK_QUADREAL_UNPACK16_UNIT_VEC, ///< Used to compress unit vectors

	HK_QUADREAL_END
};

#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
#	if defined(HK_COMPILER_HAS_INTRINSICS_IA32)
#		include <Common/Base/Math/Types/Sse/hkSseMathTypes.inl>
#	elif defined(HK_PLATFORM_XBOX360)
#		include <Common/Base/Math/Types/Xbox360/hkXbox360MathTypes.inl>
#	elif defined(HK_ARCH_PS3) || defined(HK_ARCH_PS3SPU)
#		define HK_PLATFORM_SUPPORTS_MANY_SIMD_REGISTERS
#		include <Common/Base/Math/Types/Ps3/hkPs3MathTypes.inl>
#	elif defined(HK_COMPILER_HAS_INTRINSICS_NEON)
#		define HK_PLATFORM_SUPPORTS_MANY_SIMD_REGISTERS
#		include <Common/Base/Math/Types/Neon/hkNeonMathTypes.inl>
#	elif defined(HK_PLATFORM_LRB)
#		include <Common/Base/Math/Types/Lrb/hkLrbMathTypes.inl>
#	else // simd
#		error Unknown platform for SIMD
#	endif
#else // not simd
#		include <Common/Base/Math/Types/Fpu/hkFpuMathTypes.inl>
#endif
#include <Common/Base/Math/Functions/hkFpuMathFuncs.inl>

extern const hkQuadReal hkQuadReal0000;
extern const hkQuadReal hkQuadReal1111;
extern const hkQuadReal hkQuadRealMinus1111;
extern const hkQuadReal hkQuadReal1000;
extern const hkQuadReal hkQuadReal0100;
extern const hkQuadReal hkQuadReal0010;
extern const hkQuadReal hkQuadReal0001;
extern const hkQuadReal hkQuadReal3333;
extern const hkQuadReal hkQuadRealHalf;
extern const hkQuadReal hkQuadRealMinusHalf;
extern const hkQuadReal hkQuadRealMax;
extern const hkQuadReal hkQuadRealEps;
extern const hkQuadReal hkQuadRealMinusEps;

extern const hkQuadReal g_vectorConstants[HK_QUADREAL_END];

extern HK_ALIGN16(const hkUint32 hkQuadRealNegMask[4 * 16]);

class hkVector4;
class hkQuaternion;
class hkMatrix3;
class hkRotation;
class hkTransform;
class hkQTransform;
class hkQsTransform;

#include <Common/Base/Math/Vector/hkVector4.h>
#include <Common/Base/Math/Quaternion/hkQuaternion.h>
#include <Common/Base/Math/Matrix/hkMatrix3.h>
#include <Common/Base/Math/Matrix/hkRotation.h>
#include <Common/Base/Math/Matrix/hkTransform.h>
#include <Common/Base/Math/SweptTransform/hkSweptTransform.h>
#include <Common/Base/Math/QsTransform/hkQsTransform.h>
#include <Common/Base/Math/QTransform/hkQTransform.h>
#include <Common/Base/Math/Matrix/hkMatrix4.h>

#if HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
#	if defined(HK_COMPILER_HAS_INTRINSICS_IA32)
#		include <Common/Base/Math/Vector/Sse/hkSseVector4.inl>
#	elif defined(HK_COMPILER_HAS_INTRINSICS_PS2)
#		include <Common/Base/Math/Vector/Ps2/hkPs2Vector4.inl> 
#	elif defined(HK_PLATFORM_XBOX360)
#		include <Common/Base/Math/Vector/Xbox360/hkXbox360Vector4.inl>
#	elif defined(HK_PLATFORM_PS3_PPU) || defined(HK_PLATFORM_PS3_SPU)
#		include <Common/Base/Math/Vector/Ps3/hkPs3Vector4.inl>
#	elif defined(HK_COMPILER_HAS_INTRINSICS_NEON)
#		include <Common/Base/Math/Vector/Neon/hkNeonVector4.inl>
#	elif defined(HK_PLATFORM_LRB)
#		include <Common/Base/Math/Vector/Lrb/hkLrbVector4.inl>
#	else
#		error Dont know how to do simd on this platform
#	endif
#else // no SIMD
#		include <Common/Base/Math/Vector/Fpu/hkFpuVector4.inl>
#endif // HK_CONFIG_SIMD


	// common implementations after inline definitions
#include <Common/Base/Math/Vector/hkVector4.inl>

#include <Common/Base/Math/Matrix/hkMatrix3.inl>
#include <Common/Base/Math/Quaternion/hkQuaternion.inl>
#include <Common/Base/Math/Matrix/hkTransform.inl>
#include <Common/Base/Math/Matrix/hkMatrix4.inl>
#include <Common/Base/Math/QsTransform/hkQsTransform.inl>
#include <Common/Base/Math/QTransform/hkQTransform.inl>
#include <Common/Base/Math/SweptTransform/hkSweptTransform.inl>


#endif // HK_MATH_MATH_H

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
