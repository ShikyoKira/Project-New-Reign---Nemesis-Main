/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkaAnimation::hkaAnimation() : m_type(HK_UNKNOWN_ANIMATION)
{
	m_duration = 0.0f;
	m_numberOfTransformTracks = 0;
	m_numberOfFloatTracks = 0;
	m_extractedMotion = HK_NULL;
}

inline hkaAnimation::AnimationType hkaAnimation::getType() const
{
	return m_type;
}

inline void hkaAnimation::samplePartialTracks(hkReal time, hkUint32 maxNumTransformTracks, hkQsTransform* transformTracksOut, hkUint32 maxNumFloatTracks, hkReal* floatTracksOut, hkaChunkCache* cache) const
{
#ifndef HK_PLATFORM_IS_SPU
	HK_ERROR(0x54e32123, "samplePartialPose not implemented for this type of animation");
#endif
}

inline int hkaAnimation::getNumDataChunks(hkUint32 frame, hkReal delta) const
{
	return 0;
}

inline void hkaAnimation::getDataChunks(hkUint32 frame, hkReal delta, DataChunk* dataChunks, int numDataChunks) const
{
#ifndef HK_PLATFORM_IS_SPU
	HK_ERROR(0x54e32124, "getDataChunks not implemented for this type of animation");
#endif
}

inline int hkaAnimation::getMaxSizeOfCombinedDataChunks() const
{
	return 0;
}

inline void hkaAnimation::getFrameAndDelta( hkReal time, hkUint32& frameOut, hkReal& deltaOut ) const
{
	const hkUint32 maxFrameIndex = getNumOriginalFrames() - 1;
	
	const hkReal frameFloat = (time / m_duration) * maxFrameIndex;
	frameOut = static_cast<hkUint32>( frameFloat );

	// Handle any roundoff error: We are always interpolating between pose[frameOut] and pose[frameOut+1]
    if(frameOut > maxFrameIndex - 1)
	{
		frameOut = maxFrameIndex - 1;
		deltaOut = 1.0f;
		return;
	}

	deltaOut = hkMath::clamp(frameFloat - frameOut, 0.0f, 1.0f);
	
}

inline hkBool hkaAnimation::hasExtractedMotion() const
{
	return m_extractedMotion != HK_NULL;
}

inline void hkaAnimation::setExtractedMotion( const hkaAnimatedReferenceFrame* extractedMotion )
{
	m_extractedMotion = extractedMotion;
}

inline void hkaAnimation::getExtractedMotionReferenceFrame(hkReal time, hkQsTransform& motionOut) const
{
	return m_extractedMotion->getReferenceFrame( time, motionOut );
}

inline void hkaAnimation::getExtractedMotionDeltaReferenceFrame( hkReal time, hkReal nextTime, int loops, hkQsTransform& deltaMotionOut, hkReal cropStartAmount, hkReal cropEndAmount ) const
{
	return m_extractedMotion->getDeltaReferenceFrame( time, nextTime, loops, deltaMotionOut, cropStartAmount, cropEndAmount );
}

/// Sample a single animation track
inline void hkaAnimation::sampleSingleTransformTrack( hkReal time, hkInt16 track, hkQsTransform* transformOut ) const
{
	return sampleIndividualTransformTracks( time, &track, 1, transformOut );
}

/// Sample a single floating track
inline void hkaAnimation::sampleSingleFloatTrack( hkReal time, hkInt16 track, hkReal* out ) const
{
	return sampleIndividualFloatTracks( time, &track, 1, out );
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
