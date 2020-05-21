/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


// Return the local time for the control
inline hkReal hkaAnimationControl::getLocalTime() const
{
	return m_localTime;
}

// Set the local time for the animation
inline void hkaAnimationControl::setLocalTime( hkReal lTime )
{
	m_localTime = lTime;
}

inline hkReal hkaAnimationControl::getWeight() const
{
	return m_weight;
}

inline const hkaAnimationBinding* hkaAnimationControl::getAnimationBinding() const
{
	return m_binding;
}

inline hkUint8 hkaAnimationControl::getTransformTrackWeight(hkUint32 track) const
{
	return m_transformTrackWeights[track];
}

inline hkUint8 hkaAnimationControl::getFloatTrackWeight(hkUint32 track) const
{
	return m_floatTrackWeights[track];
}

inline void hkaAnimationControl::setTransformTrackWeight(hkUint32 track, hkUint8 weight)
{	
	m_transformTrackWeights[track] = weight;
}

inline void hkaAnimationControl::setFloatTrackWeight(hkUint32 track, hkUint8 weight)
{	
	m_floatTrackWeights[track] = weight;
}

// Get the weight values for all tracks (non-const access)
inline const hkArray<hkUint8>& hkaAnimationControl::getTransformTracksWeights() const
{
	return m_transformTrackWeights;
}

// Get the weight values for all tracks (non-const access)
inline const hkArray<hkUint8>& hkaAnimationControl::getFloatTracksWeights() const
{
	return m_floatTrackWeights;
}

// Get the motion track weight for this control
inline hkReal hkaAnimationControl::getMotionTrackWeight() const
{
	return m_motionTrackWeight;
}

// Set the motion track weight for this control
inline void hkaAnimationControl::setMotionTrackWeight(hkReal w)
{
	m_motionTrackWeight = w;
}

/// Const access to the blend hint
inline hkaAnimationBinding::BlendHint hkaAnimationControl::getBlendHint() const
{
	HK_ASSERT2( 0x0584c1f0, m_binding != HK_NULL, "Attempt to use null binding." );

	return m_binding->m_blendHint;
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
