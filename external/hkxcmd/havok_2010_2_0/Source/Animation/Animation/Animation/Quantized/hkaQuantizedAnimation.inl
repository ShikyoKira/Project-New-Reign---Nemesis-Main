/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE const hkUint8* hkaQuantizedAnimation::getData() const
{
	// Return a pointer to the data buffer
	return m_data.begin();
}

HK_FORCE_INLINE int hkaQuantizedAnimation::getFrameOffset( const hkUint8* data, int frame )
{
	const QuantizedAnimationHeader* header = reinterpret_cast< const QuantizedAnimationHeader* >( data );

	return header->m_headerSize + ( frame * header->m_frameSize );
}

HK_FORCE_INLINE int hkaQuantizedAnimation::getFrameSize( const hkUint8* data )
{
	const QuantizedAnimationHeader* header = reinterpret_cast< const QuantizedAnimationHeader* >( data );
	return header->m_frameSize;
}

HK_FORCE_INLINE const hkaQuantizedAnimation::QuantizedAnimationHeader* hkaQuantizedAnimation::getHeader() const
{
	return reinterpret_cast< const QuantizedAnimationHeader* >( getData() );
}

HK_FORCE_INLINE void hkaQuantizedAnimation::getFrameAndDelta( const hkUint8* data, hkReal time, hkUint32& frameOut, hkReal& deltaOut )
{
	const QuantizedAnimationHeader* header = reinterpret_cast< const QuantizedAnimationHeader* >( data );
	
	const hkUint32 maxFrameIndex = header->m_numFrames - 1;
	
	const hkReal frameFloat = ( time / header->m_duration ) * maxFrameIndex;
	hkUint32 frame = static_cast<hkUint32>( frameFloat );

	// Ensure frame < maxFrameIndex - 1.  Otherwise delta = 1.0f
	bool lastFrame = frame > maxFrameIndex - 1;

	if( lastFrame )
	{
		deltaOut = 1.f;
		frameOut = maxFrameIndex - 1;
	}
	else
	{
		frameOut = frame;
		const hkReal frameDelta = frameFloat - frame;

		// Clamp deltaOut to frameOut[0,1].  Cant use hkMath::clamp due to SNC bug.
		deltaOut = frameDelta < 0 ? 0.f : (frameDelta > 1.f ? 1.f : frameDelta);
	}
}


HK_FORCE_INLINE hkUint32 hkaQuantizedAnimation::getNumFrames( const hkUint8* data )
{
	const QuantizedAnimationHeader* header = reinterpret_cast< const QuantizedAnimationHeader* >( data );

	return header->m_numFrames;
}

HK_FORCE_INLINE hkUint32 hkaQuantizedAnimation::getNumBones( const hkUint8* data )
{
	const QuantizedAnimationHeader* header = reinterpret_cast< const QuantizedAnimationHeader* >( data );

	return header->m_numBones;
}

HK_FORCE_INLINE hkUint32 hkaQuantizedAnimation::getNumFloats( const hkUint8* data )
{
	const QuantizedAnimationHeader* header = reinterpret_cast< const QuantizedAnimationHeader* >( data );

	return header->m_numFloats;
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
