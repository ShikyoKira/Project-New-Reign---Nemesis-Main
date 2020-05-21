/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


// Interpolates n hkReal pairs with a single alpha
void hkaBlender::blend( hkReal* HK_RESTRICT dst, const hkReal* HK_RESTRICT srcL, const hkReal* HK_RESTRICT srcR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode )
{
	hkaBatchBlenderUtilities::blendBase( dst, HK_NULL, srcL, HK_NULL, srcR, HK_NULL, alpha, n, blendMode, LERP );
}

// Interpolates n hkVector4 pairs with a single alpha
void hkaBlender::blend( hkVector4* HK_RESTRICT dst, const hkVector4* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode )
{
	hkaBatchBlenderUtilities::blendBase( dst, HK_NULL, srcL, HK_NULL, srcR, HK_NULL, alpha, n, blendMode, LERP );
}

// Interpolates n hkQuaternion pairs with a single alpha
void hkaBlender::blend( hkQuaternion* HK_RESTRICT dst, const hkQuaternion* HK_RESTRICT srcL, const hkQuaternion* HK_RESTRICT srcR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode, ROTATION_MODE rotationMode )
{
	hkaBatchBlenderUtilities::blendBase( dst, HK_NULL, srcL, HK_NULL, srcR, HK_NULL, alpha, n, blendMode, rotationMode );
}

// Interpolates n hkQsTransform pairs with a single alpha
void hkaBlender::blend( hkQsTransform* HK_RESTRICT dst, const hkQsTransform* HK_RESTRICT srcL, const hkQsTransform* HK_RESTRICT srcR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode, ROTATION_MODE rotationMode )
{
	hkaBatchBlenderUtilities::blendBase( dst, HK_NULL, srcL, HK_NULL, srcR, HK_NULL, alpha, n, blendMode, rotationMode );
}


// Interpolates n hkReal pairs with n unique weight pairs and a single alpha
void hkaBlender::blend( hkReal* HK_RESTRICT dst, hkReal* HK_RESTRICT weightsOut, const hkReal* HK_RESTRICT srcL, const hkReal* HK_RESTRICT weightL, const hkReal* HK_RESTRICT srcR, const hkReal* HK_RESTRICT weightR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode )
{
	hkaBatchBlenderUtilities::blendBase( dst, weightsOut, srcL, weightL, srcR, weightR, alpha, n, blendMode, LERP );
}

// Interpolates n hkVector4 pairs with n unique weight pairs and a single alpha
void hkaBlender::blend( hkVector4* HK_RESTRICT dst, hkReal* HK_RESTRICT weightsOut, const hkVector4* HK_RESTRICT srcL, const hkReal* HK_RESTRICT weightL, const hkVector4* HK_RESTRICT srcR, const hkReal* HK_RESTRICT weightR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode )
{
	hkaBatchBlenderUtilities::blendBase( dst, weightsOut, srcL, weightL, srcR, weightR, alpha, n, blendMode, LERP );
}

// Interpolates n TYPE pairs with n unique weight pairs and a single alpha
void hkaBlender::blend( hkQuaternion* HK_RESTRICT dst, hkReal* HK_RESTRICT weightsOut, const hkQuaternion* HK_RESTRICT srcL, const hkReal* HK_RESTRICT weightL, const hkQuaternion* HK_RESTRICT srcR, const hkReal* HK_RESTRICT weightR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode, ROTATION_MODE rotationMode )
{
	hkaBatchBlenderUtilities::blendBase( dst, weightsOut, srcL, weightL, srcR, weightR, alpha, n, blendMode, rotationMode );
}

// Interpolates n TYPE pairs with n unique weight pairs and a single alpha
void hkaBlender::blend( hkQsTransform* HK_RESTRICT dst, hkReal* HK_RESTRICT weightsOut, const hkQsTransform* HK_RESTRICT srcL, const hkReal* HK_RESTRICT weightL, const hkQsTransform* HK_RESTRICT srcR, const hkReal* HK_RESTRICT weightR, const hkSimdReal& alpha, int n, BLEND_MODE blendMode, ROTATION_MODE rotationMode )
{
	hkaBatchBlenderUtilities::blendBase( dst, weightsOut, srcL, weightL, srcR, weightR, alpha, n, blendMode, rotationMode );
}

void hkaBlender::mul( hkReal* HK_RESTRICT dst, const hkReal* HK_RESTRICT srcL, const hkReal* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x0cfb4e7f, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x0efb3b2e, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x18d6b5c3, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	// Treat the multiples of 16 as hkVector4 in batches of 4
	// handle the rest as individual hkVector4 in batches of 1
	const int offset = n & ~0xF;
	const int num16 = offset >> 2;
	const int num4 = ( ( n & 0x0F ) + 3 ) >> 2;

	mul( reinterpret_cast< hkVector4* >( dst ),
		 reinterpret_cast< const hkVector4* >( srcL ),
		 reinterpret_cast< const hkVector4* >( srcR ),
		 num16 );

	hkVector4* dstp = reinterpret_cast< hkVector4* >( dst + offset );
	const hkVector4* srcLp = reinterpret_cast< const hkVector4* >( srcL + offset );
	const hkVector4* srcRp = reinterpret_cast< const hkVector4* >( srcR + offset );

	for ( int i = 0; i < num4; ++i )
	{
		dstp[ i ].setMul4( srcLp[ i ], srcRp[ i ] );
	}		
}

void hkaBlender::mul( hkVector4* HK_RESTRICT dst, const hkVector4* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x1e9b8626, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x1a68a9c8, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x16035782, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		// Perform multiplication
		hkaBatchBlenderUtilities::mul4( dst[ 0 ], dst[ 1 ], dst[ 2 ], dst[ 3 ],
										srcL[ 0 ], srcL[ 1 ], srcL[ 2 ], srcL[ 3 ],
										srcR[ 0 ], srcR[ 1 ], srcR[ 2 ], srcR[ 3 ] );
			

		// Increment the pointers
		dst += 4;
		srcL += 4;
		srcR += 4;
	}
}

void hkaBlender::mul( hkQuaternion* HK_RESTRICT dst, const hkQuaternion* HK_RESTRICT srcL, const hkQuaternion* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x0eb9ae52, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x0b0191f5, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x060c16cf, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		// Perform multiplication
		hkaBatchBlenderUtilities::mul4( dst[ 0 ], dst[ 1 ], dst[ 2 ], dst[ 3 ],
										srcL[ 0 ], srcL[ 1 ], srcL[ 2 ], srcL[ 3 ],
										srcR[ 0 ], srcR[ 1 ], srcR[ 2 ], srcR[ 3 ] );
			

		// Increment the pointers
		dst += 4;
		srcL += 4;
		srcR += 4;
	}
}

void hkaBlender::mul( hkQsTransform* HK_RESTRICT dst, const hkQsTransform* HK_RESTRICT srcL, const hkQsTransform* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x07363922, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x037e1cc6, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x01035f33, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	
	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		// Perform multiplication
		hkaBatchBlenderUtilities::mul4( dst[ 0 ], dst[ 1 ], dst[ 2 ], dst[ 3 ],
										srcL[ 0 ], srcL[ 1 ], srcL[ 2 ], srcL[ 3 ],
										srcR[ 0 ], srcR[ 1 ], srcR[ 2 ], srcR[ 3 ] );
			

		// Increment the pointers
		dst += 4;
		srcL += 4;
		srcR += 4;
	}
}

void hkaBlender::mulInv( hkQuaternion* HK_RESTRICT dst, const hkQuaternion* HK_RESTRICT srcL, const hkQuaternion* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x0eb9ae52, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x0b0191f5, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x060c16cf, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		// Perform multiplication
		hkaBatchBlenderUtilities::mulInv4( dst[ 0 ], dst[ 1 ], dst[ 2 ], dst[ 3 ],
										   srcL[ 0 ], srcL[ 1 ], srcL[ 2 ], srcL[ 3 ],
										   srcR[ 0 ], srcR[ 1 ], srcR[ 2 ], srcR[ 3 ] );
			

		// Increment the pointers
		dst += 4;
		srcL += 4;
		srcR += 4;
	}
}

void hkaBlender::mulInv( hkQsTransform* HK_RESTRICT dst, const hkQsTransform* HK_RESTRICT srcL, const hkQsTransform* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x07363922, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x037e1cc6, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x01035f33, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		// Perform multiplication
		hkaBatchBlenderUtilities::mulInv4( dst[ 0 ], dst[ 1 ], dst[ 2 ], dst[ 3 ],
										   srcL[ 0 ], srcL[ 1 ], srcL[ 2 ], srcL[ 3 ],
										   srcR[ 0 ], srcR[ 1 ], srcR[ 2 ], srcR[ 3 ] );
			

		// Increment the pointers
		dst += 4;
		srcL += 4;
		srcR += 4;
	}
}

void HK_CALL hkaBlender::normalize( hkQuaternion* quaternionsInOut, int n )
{
	HK_ASSERT2( 0x05864e6a, ( hkUlong( quaternionsInOut ) & 0x0F ) == 0, "Input must be 16 byte aligned" );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		hkaBatchBlenderUtilities::normalize4( quaternionsInOut[ 0 ],
											  quaternionsInOut[ 1 ],
											  quaternionsInOut[ 2 ],
											  quaternionsInOut[ 3 ] );

		quaternionsInOut += 4;
	}
}


void HK_CALL hkaBlender::normalize( hkQsTransform* transformsInOut, int n )
{
	HK_ASSERT2( 0x05864e6a, ( hkUlong( transformsInOut ) & 0x0F ) == 0, "Input must be 16 byte aligned" );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		hkaBatchBlenderUtilities::normalize4( transformsInOut[ 0 ].m_rotation,
											  transformsInOut[ 1 ].m_rotation,
											  transformsInOut[ 2 ].m_rotation,
											  transformsInOut[ 3 ].m_rotation );

		transformsInOut += 4;
	}
}



void hkaBlender::rotate( hkVector4* HK_RESTRICT dst, const hkQuaternion* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x07363922, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x037e1cc6, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x01035f33, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		// Perform rotation
		hkaBatchBlenderUtilities::rotate4( dst[ 0 ], dst[ 1 ], dst[ 2 ], dst[ 3 ],
										   srcL[ 0 ], srcL[ 1 ], srcL[ 2 ], srcL[ 3 ],
										   srcR[ 0 ], srcR[ 1 ], srcR[ 2 ], srcR[ 3 ] );
			

		// Increment the pointers
		dst += 4;
		srcL += 4;
		srcR += 4;
	}
}

void hkaBlender::rotateInv( hkVector4* HK_RESTRICT dst, const hkQuaternion* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x07363922, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x037e1cc6, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x01035f33, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		// Perform rotation
		hkaBatchBlenderUtilities::rotateInv4( dst[ 0 ], dst[ 1 ], dst[ 2 ], dst[ 3 ],
											  srcL[ 0 ], srcL[ 1 ], srcL[ 2 ], srcL[ 3 ],
											  srcR[ 0 ], srcR[ 1 ], srcR[ 2 ], srcR[ 3 ] );
			

		// Increment the pointers
		dst += 4;
		srcL += 4;
		srcR += 4;
	}
}

void hkaBlender::transform( hkVector4* HK_RESTRICT dst, const hkQsTransform* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x0c168cbe, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x099bcf2b, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x07211198, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		// Perform transformation
		hkaBatchBlenderUtilities::transform4( dst[ 0 ], dst[ 1 ], dst[ 2 ], dst[ 3 ],
											  srcL[ 0 ], srcL[ 1 ], srcL[ 2 ], srcL[ 3 ],
											  srcR[ 0 ], srcR[ 1 ], srcR[ 2 ], srcR[ 3 ] );

		// Increment the pointers
		dst += 4;
		srcL += 4;
		srcR += 4;
	}
}


void hkaBlender::transformInv( hkVector4* HK_RESTRICT dst, const hkQsTransform* HK_RESTRICT srcL, const hkVector4* HK_RESTRICT srcR, int n )
{
	HK_ASSERT2( 0x0c168cbe, ( hkUlong( dst ) & 0x0F ) == 0, "dst must be 16 byte aligned." );
	HK_ASSERT2( 0x099bcf2b, ( hkUlong( srcL ) & 0x0F ) == 0, "srcL must be 16 byte aligned." );
	HK_ASSERT2( 0x07211198, ( hkUlong( srcR ) & 0x0F ) == 0, "srcR must be 16 byte aligned." );

	const int num = ( n + 3 ) / 4;

	for ( int i = 0; i < num; i++ )
	{
		// Perform transformation
		hkaBatchBlenderUtilities::transformInv4( dst[ 0 ], dst[ 1 ], dst[ 2 ], dst[ 3 ],
												 srcL[ 0 ], srcL[ 1 ], srcL[ 2 ], srcL[ 3 ],
												 srcR[ 0 ], srcR[ 1 ], srcR[ 2 ], srcR[ 3 ] );

		// Increment the pointers
		dst += 4;
		srcL += 4;
		srcR += 4;
	}
}

void HK_CALL hkaBlender::localFromModel( hkQsTransform* poseLocalOut, hkQsTransform* poseModelIn, const hkInt16* parentIndices, int numBones, const hkQsTransform& worldFromModel )
{
	for ( int i = numBones; i--; )
	{
		int p = parentIndices[ i ];

		const hkQsTransform modelFromBone = poseModelIn[ i ];
		const hkQsTransform modelFromParent = p >= 0 ? poseModelIn[ p ] : worldFromModel;
		
		poseLocalOut[ i ].setMulInverseMul( modelFromParent, modelFromBone );
	}
}

void HK_CALL hkaBlender::modelFromLocal( hkQsTransform* poseModelOut, hkQsTransform* poseLocalIn, const hkInt16* parentIndices, int numBones, const hkQsTransform& worldFromModel )
{
	for ( int i = 0; i < numBones; i++ )
	{
		int p = parentIndices[ i ];

		const hkQsTransform parentFromBone = poseLocalIn[ i ];
		const hkQsTransform modelFromParent = p >= 0 ? poseModelOut[ p ] : worldFromModel;
		
		poseModelOut[ i ].setMul( modelFromParent, parentFromBone );
	}
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
