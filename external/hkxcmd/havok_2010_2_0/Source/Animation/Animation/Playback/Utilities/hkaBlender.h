/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ANIMATION_BLENDER_H
#define HK_ANIMATION_BLENDER_H

namespace hkaBlender
{
	// Blend modes
	enum BLEND_MODE
	{
		NORMAL = 0,
		ADDITIVE = 1,
		SUBTRACTIVE = -1
	};

	// Rotation interpolation modes
	enum ROTATION_MODE
	{
		LERP = 0,
		SLERP = 1
	};

	//
	// The following high-level functions blend and interpolate
	// reals, vectors, quaternions and transforms.  Each function provides
	// the appropriate analogue of the interpolation function:
	// dst = ( 1 - alpha ) * srcL + ( alpha ) * srcR.
	//
	// Each of these functions requires that all inputs and outputs
	// be 16 byte aligned, and have an allocation size which is a
	// multiple of four times the size of the given type.
	//
	// All functions below are alias safe for outputPtr == inputPtr, but may
	// not be safe for more general cases of overlap.
	//

	// Interpolates n TYPE pairs with a single alpha
	void HK_CALL blend( hkReal* HK_RESTRICT dstOut, const hkReal* HK_RESTRICT srcL, const hkReal* HK_RESTRICT srcR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode );
	void HK_CALL blend( hkVector4* HK_RESTRICT dstOut, const hkVector4* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode );
	void HK_CALL blend( hkQuaternion* HK_RESTRICT dstOut, const hkQuaternion* HK_RESTRICT srcL, const hkQuaternion* HK_RESTRICT srcR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode, ROTATION_MODE rotationMode );
	void HK_CALL blend( hkQsTransform* HK_RESTRICT dstOut, const hkQsTransform* HK_RESTRICT srcL, const hkQsTransform* HK_RESTRICT srcR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode, ROTATION_MODE rotationMode );

	//
	// Weighted blending proceeds by computing a new blend parameter beta, and new weight w,
	// that are a function of the original blend parameter alpha and the given left and right weights
	//
	// If weightL == weightR then want beta = alpha.  Let beta = alpha.
	// If weightL >  weightR then want beta < alpha.  Let beta = alpha * weightR / weightL.
	// If weightR >  weightL then want (1-beta) < (1-alpha).  Let (1-beta) = (1-alpha) * weightL / weightR.
	//
	// Lastly, handle zero weights...
	//
	//               |  weightL > 0                      |  weightL == 0       
	// ------------------------------------------------------------------------
	// weightR > 0   |  beta = (see logic above)         |  w = weightR        
	//               |  w = blend(weightL,weightR,beta)  |  beta = 1           
	// ------------------------------------------------------------------------
	// weightR == 0  |  w = weightL                      |  w = 0              
    //               |  beta = 0                         |  beta = alpha
	//

	// Interpolates n TYPE pairs with n unique weight pairs and a single alpha
	void HK_CALL blend( hkReal* HK_RESTRICT dstOut, hkReal* HK_RESTRICT weightsOut, const hkReal* HK_RESTRICT srcL, const hkReal* HK_RESTRICT weightL, const hkReal* HK_RESTRICT srcR, const hkReal* HK_RESTRICT weightR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode );
 	void HK_CALL blend( hkVector4* HK_RESTRICT dstOut, hkReal* HK_RESTRICT weightsOut, const hkVector4* HK_RESTRICT srcL, const hkReal* HK_RESTRICT weightL, const hkVector4* HK_RESTRICT srcR, const hkReal* HK_RESTRICT weightR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode );
	void HK_CALL blend( hkQuaternion* HK_RESTRICT dstOut, hkReal* HK_RESTRICT weightsOut, const hkQuaternion* HK_RESTRICT srcL, const hkReal* HK_RESTRICT weightL, const hkQuaternion* HK_RESTRICT srcR, const hkReal* HK_RESTRICT weightR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode, ROTATION_MODE rotationMode );
 	void HK_CALL blend( hkQsTransform* HK_RESTRICT dstOut, hkReal* HK_RESTRICT weightsOut, const hkQsTransform* HK_RESTRICT srcL, const hkReal* HK_RESTRICT weightL, const hkQsTransform* HK_RESTRICT srcR, const hkReal* HK_RESTRICT weightR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode, ROTATION_MODE rotationMode );		

	// Multiplies n TYPE pairs
	void HK_CALL mul( hkReal* HK_RESTRICT dstOut, const hkReal* HK_RESTRICT srcL, const hkReal* HK_RESTRICT srcR, int n );
	void HK_CALL mul( hkVector4* HK_RESTRICT dstOut, const hkVector4* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n );
	void HK_CALL mul( hkQuaternion* HK_RESTRICT dstOut, const hkQuaternion* HK_RESTRICT srcL, const hkQuaternion* HK_RESTRICT srcR, int n );
	void HK_CALL mul( hkQsTransform* HK_RESTRICT dstOut, const hkQsTransform* HK_RESTRICT srcL, const hkQsTransform* HK_RESTRICT srcR, int n );

	// Multiplies n TYPE pairs, inverting the right
	void HK_CALL mulInv( hkQuaternion* HK_RESTRICT dstOut, const hkQuaternion* HK_RESTRICT srcL, const hkQuaternion* HK_RESTRICT srcR, int n );
	void HK_CALL mulInv( hkQsTransform* HK_RESTRICT dstOut, const hkQsTransform* HK_RESTRICT srcL, const hkQsTransform* HK_RESTRICT srcR, int n );

	// Normalizes n transform rotations
	void HK_CALL normalize( hkQuaternion* transformsInOut, int n );
	void HK_CALL normalize( hkQsTransform* transformsInOut, int n );

	// Transforms n hkVector4 instances
	void HK_CALL rotate( hkVector4* HK_RESTRICT dstOut, const hkQuaternion* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n );
	void HK_CALL rotateInv( hkVector4* HK_RESTRICT dstOut, const hkQuaternion* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n );
	void HK_CALL transform( hkVector4* HK_RESTRICT dstOut, const hkQsTransform* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n );
	void HK_CALL transformInv( hkVector4* HK_RESTRICT dstOut, const hkQsTransform* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n );

	// Convert local-model or local-world
	void HK_CALL localFromModel( hkQsTransform* poseLocalOut, hkQsTransform* poseModelIn, const hkInt16* parentIndices, int numBones, const hkQsTransform& worldFromModel = hkQsTransform::getIdentity() );
	void HK_CALL modelFromLocal( hkQsTransform* poseModelOut, hkQsTransform* poseLocalIn, const hkInt16* parentIndices, int numBones, const hkQsTransform& worldFromModel = hkQsTransform::getIdentity() );

}

// Choose the platform-dependent blender implementation
// Define HK_USE_BATCH_BLENDER as 0 or 1 to override
#ifndef HK_USE_BATCH_BLENDER

// (Allow for user override)
#define HK_USE_BATCH_BLENDER ( HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED )

#endif // HK_USE_BATCH_BLENDER


#if HK_USE_BATCH_BLENDER

// Include the SIMD-optimized implementation
#include <Animation/Animation/Playback/Utilities/hkaBatchBlenderUtils.inl>

#else

// Include the traditional implementation
#include <Animation/Animation/Playback/Utilities/hkaIndividualBlenderUtils.inl>

#endif // HK_USE_BATCH_BLENDER


#endif // HK_ANIMATION_BLENDER_H

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
