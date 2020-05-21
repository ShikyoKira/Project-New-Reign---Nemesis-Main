/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULT_EXTRACTED_MOTION_H
#define HK_DEFAULT_EXTRACTED_MOTION_H

#include <Common/Base/hkBase.h>
#include <Animation/Animation/Rig/hkaBone.h>
#include <Animation/Animation/Motion/hkaAnimatedReferenceFrame.h>

class hkaInterleavedUncompressedAnimation;
extern const hkClass hkaDefaultAnimatedReferenceFrameClass;

/// Default implementation for the extraction and storage of motion from an animation.
/// Multiple samples can be used for motions with variable velocity. Motion can also be
/// constrained to different axis, and optionally include a yaw (turn) factor.
/// Objects of this class are usually generated during toolchain operations (motion extraction)
/// If required at runtime, the static method "extractMotionFrom" constructs an instance of this
/// class by extracting the motion from an hkaInterleavedUncompressedAnimation.
class hkaDefaultAnimatedReferenceFrame : public hkaAnimatedReferenceFrame
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		/// Options for the extractMotionFrom() utility
		struct MotionExtractionOptions
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_MOTION, hkaDefaultAnimatedReferenceFrame::MotionExtractionOptions );

				/// This represents the frame of reference to use when extracting motion
				/// Typically this will be something like the root track for an animation
			const hkQsTransform* m_referenceFrameTransforms;

				/// The number of reference transforms above.
			int m_numReferenceFrameTransforms;

				/// The duration of the motion represented by the reference frame.
			hkReal m_referenceFrameDuration;

				/// Number of samples to be taken from the original animation in order
				/// to calculate and store the motion. Minimum (and default) number is two.
				/// A value of -1 will be interpreted as "take as many samples as the original animation".
				/// Generally, use 2 samples if motion is constant linear, use multiple samples otherwise.
			hkInt32 m_numSamples;

				/// If true, vertical movements in the root bone of the animation will be considered as
				/// motion of the character (and therefore extracted from the animation track). Default is false.
			hkBool m_allowUpDown;

				/// If true, forward/backward displacements in the root bone will be considered part of the
				/// motion of the character (and therefore extracted from the animation track). Default is true.
			hkBool m_allowFrontBack;

				/// If true, sideways displacements of the root bone will be considered part of the motion of
				/// the character (and therefore extracted from the animation track). Default is false.
			hkBool m_allowRightLeft;

				/// If true, changes of orientation (yaw) around the vertical axis will be considered part of the
				/// motion of the character (and therefore extracted from the animation track). Default is false.
			hkBool m_allowTurning;

				/// Specified which direction in world (root track) is up. Default is (0,0,1) [Z]
			hkVector4 m_up;

				/// Specified which direction in world (root track) is forward. Default is (1,0,0) [X]
			hkVector4 m_forward;

				/// Constructs an instance of this struct with default values
			MotionExtractionOptions ();

		};

			/// Constructs an hkDefaultExtractMotion object by sampling the root node of an interleaved animation
			/// Notice that it doesn't modify the animation passed as a parameter. In order to remove the extracted
			/// motion of an animation and associate the motion to it, you can use the hkaAnimatedReferenceFrameUtils class.
		hkaDefaultAnimatedReferenceFrame( const MotionExtractionOptions& options );

			/// Overridden from hkaDefaultAnimatedReferenceFrame, this method returns the motion extracted from the
			/// from the animation up to the given time. See hkaAnimatedReferenceFrame::getReferenceFrame() for details.
		virtual void getReferenceFrame (hkReal time, hkQsTransform& motionOut) const;

			/// Returns the change in motion between two times.
			/// The default implementation assumes the animation is a looping animation.
		virtual void getDeltaReferenceFrame( hkReal time, hkReal nextTime, int loops, hkQsTransform& output, hkReal cropStartAmount, hkReal cropEndAmount ) const;

			/// Returns the length of time this reference frame is animated for.
		virtual hkReal getDuration() const;

	public:


		// Primary attributes
		/// Up vector.
		hkVector4 m_up;
		/// Forward vector.
		hkVector4 m_forward;

		/// The duration of the animated reference frame
		hkReal m_duration;

		/// We store the motion track as a vector4 since we only need a translation and a rotational (w) component
		/// around the up direction.
		hkArray< hkVector4 > m_referenceFrameSamples;

		/// Used internally (for every sample) by extractMotionFrom()
		static void HK_CALL project(const MotionExtractionOptions& options, const hkQsTransform& rootMotion, const hkVector4& previousProjection, hkVector4& projectionOut);

	public:

		// Constructor for initialisation of vtable fixup
		HK_FORCE_INLINE hkaDefaultAnimatedReferenceFrame( hkFinishLoadedObjectFlag flag ) : hkaAnimatedReferenceFrame(flag), m_referenceFrameSamples(flag) {}

};


#endif // HK_DEFAULT_EXTRACTED_MOTION_H

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
