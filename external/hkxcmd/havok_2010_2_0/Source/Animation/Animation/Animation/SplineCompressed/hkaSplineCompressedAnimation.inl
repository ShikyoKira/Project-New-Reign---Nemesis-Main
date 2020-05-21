/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


/// Copies dynamic and static values into their respective components, based on a mask
/// \param mask Mask describing the dynamic and static components of the data
/// \param S Static values
/// \param I Identity values
/// \param inOut Output with static/identity values overwritten
void hkaSplineCompressedAnimation::recompose( hkUint8 mask, const hkVector4& S, const hkVector4& I, hkVector4& inOut )
{

	int stat = mask & 0x0F;
	int iden = ~mask & ( ~mask >> 4 ) & 0x0F;

	int shift = 0x01;

	for ( int i = 0; i < 4; i++ )
	{
		if ( stat & shift )
		{
			inOut( i ) = S( i );
		}
		else if ( iden & shift )
		{
			inOut( i ) = I( i );
		}

		shift <<= 1;
	}

}


/// Reads 8 bits from an internal buffer
/// \param dataInOut Buffer which is incremented
hkUint8 hkaSplineCompressedAnimation::read8( const hkUint8*& dataInOut )
{
	return *dataInOut++;
}


/// Reads 16 bits from an internal buffer
/// \param dataInOut Buffer which is incremented
hkUint16 hkaSplineCompressedAnimation::read16( const hkUint8*& dataInOut )
{
	return *reinterpret_cast< const hkUint16*&  >( dataInOut )++;
}


/// Reads a real value from an internal buffer
/// \param dataInOut Buffer which is incremented
hkReal hkaSplineCompressedAnimation::readReal( const hkUint8*& dataInOut )
{
	return *reinterpret_cast< const hkReal*&  >( dataInOut )++;
}


/// Update a pointer to be aligned
/// \param align Number of bytes to align (must be a multiple of 2)
void hkaSplineCompressedAnimation::readAlign( int align, const hkUint8*& dataInOut )
{
	dataInOut = reinterpret_cast< const hkUint8 * > ( HK_NEXT_MULTIPLE_OF( align, reinterpret_cast< hk_size_t >( dataInOut ) ) );
}


/// Align a point for quaternions (varies based on the number of bytes per quaternion)
/// \param type The type of quaternion compression to use
/// \param dataInOut Pointer to byte buffer (updated)
void hkaSplineCompressedAnimation::readAlignQuaternion( TrackCompressionParams::RotationQuantization type, const hkUint8*& dataInOut )
{
	HK_ASSERT2( 0x3aa3eb74, TrackCompressionParams::validQuantization( type ), "Spline data corrupt." );

	// Bit mask for byte alignment
	// Each type has it's own unique byte alignment
	static const int align[6] = { 4, 1, 2, 1, 2, 4 };

	readAlign( align[ type ], dataInOut );
}


/// \return The number of bytes required for the current quaternion packing scheme
/// \param type The type of quaternion compression to use
int hkaSplineCompressedAnimation::bytesPerQuaternion( TrackCompressionParams::RotationQuantization type )
{
	HK_ASSERT2( 0x3aa3eb74, TrackCompressionParams::validQuantization( type ), "Spline data corrupt." );

	static const int size[6] = { 4, 5, 6, 3, 2, 16 };

	return size[ type ];
}


/// \return The number of bytes required per component for the current packing scheme
/// \param type The type of quaternion compression to use
int hkaSplineCompressedAnimation::bytesPerComponent( TrackCompressionParams::ScalarQuantization type )
{
	HK_ASSERT2( 0x3aa3eb74, TrackCompressionParams::validQuantization( type ), "Spline data corrupt." );

	static const int size[2] = { 1, 2 };

	return size[ type ];
}


/// \return A packed floating point value expanded
/// \param minp Minimum expected value
/// \param maxp Maximum expected value
/// \param Packed value to expand
hkReal hkaSplineCompressedAnimation::unpack16( hkReal minp, hkReal maxp, hkUint16 val )
{
	const hkReal span = 65535.0f;

	return ( static_cast< hkReal >( val ) / span ) * ( maxp - minp ) + minp;
}



/// Unpack the representation of the given quantization types
/// \param translation The type of translation quantization given
/// \param rotation The type of rotation quantization given
/// \param scale The type of scale quantization given
void hkaSplineCompressedAnimation::unpackQuantizationTypes( hkUint8 packedQuatizationTypes, TrackCompressionParams::ScalarQuantization& translation, TrackCompressionParams::RotationQuantization& rotation, TrackCompressionParams::ScalarQuantization& scale )
{
	translation = static_cast< TrackCompressionParams::ScalarQuantization >( ( packedQuatizationTypes >> 0 ) & 0x03 );
	rotation = static_cast< TrackCompressionParams::RotationQuantization >( ( packedQuatizationTypes >> 2 ) & 0x0F );
	scale = static_cast< TrackCompressionParams::ScalarQuantization >( ( packedQuatizationTypes >> 6 ) & 0x03 );

	HK_ASSERT2( 0x3aa3eb74, TrackCompressionParams::validQuantization( translation ), "Spline data corrupt." );
	HK_ASSERT2( 0x3aa3eb74, TrackCompressionParams::validQuantization( rotation ), "Spline data corrupt." );
	HK_ASSERT2( 0x3aa3eb74, TrackCompressionParams::validQuantization( scale ), "Spline data corrupt." );
}


void hkaSplineCompressedAnimation::unpackMaskAndQuantizationType( hkUint8 packedMaskAndQuatizationType, hkUint8& mask, TrackCompressionParams::ScalarQuantization& floatQuantization )
{
	// Read in the floatQuantization from the 1st (not 0th) bit
	floatQuantization = static_cast< TrackCompressionParams::ScalarQuantization >( ( packedMaskAndQuatizationType >> 1 ) & 0x03 );
	mask = packedMaskAndQuatizationType & ~0x06;

	HK_ASSERT2( 0x3aa3eb74, TrackCompressionParams::validQuantization( floatQuantization ), "Spline data corrupt." );
	HK_ASSERT2( 0x3aa3eb74,  ( mask & 0x06 ) == 0, "Spline data corrupt." );
}


/// \return A packed floating point value expanded
/// \param minp Minimum expected value
/// \param maxp Maximum expected value
/// \param Packed value to expand
hkReal hkaSplineCompressedAnimation::unpack8( hkReal minp, hkReal maxp, hkUint8 val )
{
	const hkReal span = 255.0f;

	return ( static_cast< hkReal >( val ) / span ) * ( maxp - minp ) + minp;
}


/// Expands a quantized quaternion
/// \param type The type of quaternion compression to use
/// \param in Input buffer to read from
/// \param out Quaternion to store the result
void hkaSplineCompressedAnimation::unpackQuaternion( TrackCompressionParams::RotationQuantization type, const hkUint8* in, hkQuaternion* out )
{
	static void ( HK_CALL * unpackfunc[6] )( const hkUint8* in, hkQuaternion* out ) = { unpackSignedQuaternion32, unpackSignedQuaternion40, unpackSignedQuaternion48, unpackSignedQuaternion24, unpackSignedQuaternion16, unpackSignedQuaternion128 };

	HK_ASSERT2( 0x3aa3eb74, TrackCompressionParams::validQuantization( type ), "Spline data corrupt." );

	(* unpackfunc[ type ] )( in, out );
}


/// Evaluate the spline at a given time.  Chooses from several optimized function implementations
/// \param u Time to evaluate at
/// \param p Degree of the curve
/// \param U Array of knot values for the given time
/// \param P Array of control point values for the given time
/// \param out Output value
void hkaSplineCompressedAnimation::evaluate( hkReal u, int p, hkReal U[ MAX_DEGREE * 2 ], hkVector4 P[ MAX_ORDER ], hkVector4& out )
{
	static void (* evaluateFunction[4] )( hkReal u, int p, hkReal U[ MAX_DEGREE * 2 ], hkVector4 P[ MAX_ORDER ], hkVector4& out ) =
		{ HK_NULL, evaluateSimple1, evaluateSimple2, evaluateSimple3 };

	HK_ASSERT2( 0x3aa3eb74,  p >= 1 && p <= 3, "Spline data corrupt." );

	return evaluateFunction[ p ]( u, p, U, P, out );
}


/// Algorithm A2.1 The NURBS Book p68 - Determine the knot span index
/// \return The index i such that U[i] <= u < U[i+1]
/// \param n Max control point index
/// \param p Degree
/// \param u Knot value to find span for as byte
/// \param U Array of knots as bytes
int hkaSplineCompressedAnimation::findSpan( int n, int p, hkUint8 u, const hkUint8* U )
{
	// Bounds protect
	// Splines can extrapolate, so times (slightly) outside the range are OK.
	if ( u >= U[ n+1 ] ) return n;
	if ( u <= U[0] ) return p;

	// Search
	int low = p;
	int high = n + 1;
	int mid = ( low + high ) / 2;
	while ( u < U[mid] || u >= U[mid+1] )
	{
		if ( u < U[mid] ) high = mid;
		else low = mid;
		mid = ( low + high ) / 2;
	}
	return mid;
}

/// Find the local time within a block
/// Provides an unambiguous result for the blockOut when
/// multithreading on processors with different numeric precision.
/// The blockTimeOut may be subject to differences in precision.
/// This avoids discrepencies in boundary cases where the block
/// index is computed differently on different processors.
/// \param time Time to query the animation
/// \param blockOut Which block the local time lies within
/// \param blockTimeOut Local time within the block
/// \param quantizedTimeOut Time expressed as integer within the block
void hkaSplineCompressedAnimation::getBlockAndTime( hkUint32 frame, hkReal delta, int& blockOut, hkReal& blockTimeOut, hkUint8& quantizedTimeOut ) const
{
	// Find the appropriate block
	blockOut = frame / ( m_maxFramesPerBlock - 1 );

	// Clamp the block
	blockOut = hkMath::max2( blockOut, 0 );
	blockOut = hkMath::min2( blockOut, m_numBlocks-1 );

	// Find the local time
	const hkUint32 firstFrameOfBlock = blockOut * ( m_maxFramesPerBlock - 1 );
	const hkReal realFrame = static_cast< hkReal >( frame - firstFrameOfBlock ) + delta;
	blockTimeOut = realFrame * m_frameDuration;

	// Find the truncated time
	quantizedTimeOut = static_cast< hkUint8 >( ( blockTimeOut * m_blockInverseDuration ) * ( m_maxFramesPerBlock - 1 ) );
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
