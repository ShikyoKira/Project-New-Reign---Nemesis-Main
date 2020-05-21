/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULT_CONTROL_H
#define HK_DEFAULT_CONTROL_H

#include <Animation/Animation/Playback/Control/hkaAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Animation/hkaAnimation.h>

class hkaDefaultAnimationControlListener;
class hkaSkeletonMapper;

/// This class represents a simple animation control.
/// It provides basic support for varying playback speed, fading and looping
class hkaDefaultAnimationControl : public hkaAnimationControl
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		/// Create a new hkaDefaultAnimationControl.
		/// \param binding The binding (which contains an hkaAnimation) to which this control is attached
		/// \param startEasedIn True if the animation is to be started fully eased in, false if fully eased out
		/// \param maxCycles Maximum number of cycles to play this animation (1,2, etc.). Negative values signify infinite looping.
		hkaDefaultAnimationControl( const hkaAnimationBinding* binding, hkBool startEasedIn = true, hkInt32 maxCycles = -1 );
			
		hkaDefaultAnimationControl(const hkaDefaultAnimationControl& other);

			/// Destructor
		~hkaDefaultAnimationControl();

			/*
			 * Control interface
			 */

			/// Advance the local clock by the specified delta time and return the new state
		void update( hkReal stepDelta );


			/// Returns the local time of the control in the future.
			/// Local time is guaranteed to lie in the range 0..duration.
			/// If the stepDelta overflows / underflows the end of the animation then loopsOut is positive / negative.
		virtual void getFutureTime( hkReal stepDelta, hkReal& localTimeOut, int& loopsOut) const;


			/*
			 * Basic Control
			 */

			/// Return the master weight for the control
		inline hkReal getMasterWeight() const;

			/// Set the master weight for this playing animation
		inline void setMasterWeight( hkReal weight );

			/// Return the current playback speed
		inline hkReal getPlaybackSpeed() const;

			/// Set the playback speed.
			/// If speed is set to a negative value the animation will run backwards.
		inline void setPlaybackSpeed( hkReal speed );

			/*
			 * Loop counting
			 */

			/// Return how many times this control has looped passed the end of the animation - never reset unless by user using setOverflowCount().
		inline hkUint32 getOverflowCount() const;

			/// Reset (to zero) how many times this control has looped passed the end of the animation.
		inline void setOverflowCount( hkUint32 count );

			/// Return how many times this control has looped passed the start of the animation - never reset unless by user using setUnderflowCount().
		inline hkUint32 getUnderflowCount() const;

			/// Reset (to zero) how many times this control has looped passed the start of the animation.
		inline void setUnderflowCount( hkUint32 count );

			/*
			 * Ease Curve Control
			 */

			/// Set the ease in curve. The curve is a Bezier defined by
			/// B(t) = t^3(-p0 + 3p1 - 3p2 + p3) + t^2(3p0 - 6p1 + 3p2) + t^1(-3p0 + 3p1) + t^0(p0)
		inline void setEaseInCurve(hkReal y0, hkReal y1, hkReal y2, hkReal y3);

			/// Set the ease out curve. The curve is a Bezier defined by
			/// B(t) = t^3(-p0 + 3p1 - 3p2 + p3) + t^2(3p0 - 6p1 + 3p2) + t^1(-3p0 + 3p1) + t^0(p0)
		inline void setEaseOutCurve(hkReal y0, hkReal y1, hkReal y2, hkReal y3);

			/// Ease in the control according to the curve.
			/// \param duration The length of time to perform the ease
			/// \return The time at which it will be fully eased in (may not be time specified if the control is already in the middle of easing).
		inline hkReal easeIn(hkReal duration);

			/// Ease out the control according to the curve.
			/// \param duration The length of time to perform the ease
			/// \return The time at which it will be fully eased out (may not be time specified if the control is already in the middle of easing).
		inline hkReal easeOut(hkReal duration);

			/// Ease the control in or out
			/// \param duration The length of time to perform the ease
			/// \param easeIn If true the control will be eased in. If false the control will be eased out
			/// \return The time at which it will be fully eased out (may not be time specified if the control is already in the middle of easing).
		inline hkReal ease(hkReal duration, hkBool easeIn);

			/// Ease status.
		enum EaseStatus
		{
				/// easeIn has been called, and ease is not yet complete
			EASING_IN,
				/// easeIn was called and ease has completed (or controller constructed in eased in state)
			EASED_IN,
				/// easeOut has been called, and ease is not yet complete
			EASING_OUT,
				/// easeOut was called and ease has completed (or controller constructed in eased out state)
			EASED_OUT
		};

			/// Returns the status of the easing for this control
		inline enum EaseStatus getEaseStatus() const;

			/*
			 * Listener interface
			 */

			/// Add a listener
		void addDefaultControlListener(hkaDefaultAnimationControlListener* listener);

			/// Remove a listener
		void removeDefaultControlListener(hkaDefaultAnimationControlListener* listener);

			/*
			 * Cropping
			 */

			/// Set the amount (in local seconds) to crop the start of the animation.
			/// This number should always be between 0 and the duration of the animation
		inline void setCropStartAmountLocalTime( hkReal cropStartAmountLocalTime );

			/// Set the amount (in local seconds) to crop the end of the animation.
			/// This number should always be between 0 and the duration of the animation
			/// For example, to crop the last half second from an animation of any length,
			/// pass in 0.5f.
		inline void setCropEndAmountLocalTime( hkReal cropEndAmountLocalTime );

			/// Get the amount (in local seconds) to crop the start of the animation.
		inline hkReal getCropStartAmountLocalTime();

			/// Get the amount (in local seconds) to crop the end of the animation.
		inline hkReal getCropEndAmountLocalTime();

			/// Get the skeleton mapper used for this control (HK_NULL if no mapper is assigned)
		const hkaSkeletonMapper* getSkeletonMapper() const;

			/// Set the (optional) skeleton mapper for this control
		void setSkeletonMapper( const hkaSkeletonMapper* mapper );

			/// Returns the number of transform tracks associated with this animation
		hkInt32 getNumberOfTransformTracks() const;

			/// Returns the number of float tracks associated with this animation
		hkInt32 getNumberOfFloatTracks() const;

			/// Returns the number of transform trackToBone indices.
		hkInt32 getNumTransformTrackToBoneIndices() const;

			/// Returns the transform trackToBone indices. May return null.
		const hkInt16* getTransformTrackToBoneIndices() const;

			/// Get the pose at the control local time
		virtual void sampleTracks( hkQsTransform* transformTracksOut, hkReal* floatTracksOut, hkaChunkCache* cache) const;
		
			/// Get a subset of the first 'maxNumTracks' transform tracks (all tracks from 0 to maxNumTracks-1 inclusive), and the first 'maxNumFloatTracks' float tracks of a pose at a given time.
		virtual void samplePartialTracks(hkUint32 maxNumTransformTracks, hkQsTransform* transformTracksOut,
										 hkUint32 maxNumFloatTracks, hkReal* floatTracksOut,
										 hkaChunkCache* cache) const;

			/// Query extracted motion from this control
			/// Sets output to identity if extracted motion is not present
		virtual void getExtractedMotionDeltaReferenceFrame( hkReal deltaTime, hkQsTransform& deltaMotionOut ) const;


	protected:

		// Master weight for this control
		hkReal	m_masterWeight;

		// Speed Control
		hkReal m_playbackSpeed;

		// loop counters
		hkUint32 m_overflowCount;
		hkUint32 m_underflowCount;
		hkInt32 m_maxCycles;

		// Ease controls
		hkVector4 m_easeInCurve;	// 4 control pts 
		hkVector4 m_easeOutCurve;	// 4 control pts
		hkReal	m_easeInvDuration;	// 1/duration of the ease
		hkReal	m_easeT;			// Current parameterisation for the ease curve
		EaseStatus  m_easeStatus;			// Direction in/out

		// cropping amounts (in positive local seconds from the respective endpoint of the clip)
		hkReal m_cropStartAmountLocalTime;
		hkReal m_cropEndAmountLocalTime;

			// Control listeners
		hkArray<hkaDefaultAnimationControlListener*> m_defaultListeners;

			// Animation retargeting mapping (optional)
		class hkaDefaultAnimationControlMapperData* m_mapper;
};

class hkaDefaultAnimationControlMapperData : public hkReferencedObject
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

	hkaDefaultAnimationControlMapperData( const hkaSkeletonMapper* mapper, const hkaAnimationBinding* binding );
	~hkaDefaultAnimationControlMapperData();

	const class hkaSkeletonMapper* m_mapper;

	hkArray< hkInt16 > m_srcBoneToTrackIndices;
	hkArray< hkInt16 > m_dstBoneToTrackIndices;
	hkArray< hkInt16 > m_dstTrackToBoneIndices;
};

#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.inl>

#endif // HK_DEFAULT_CONTROL_H

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
