/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_ANIMATION_HKSKELETALANIMATION_XML_H
#define HKANIMATION_ANIMATION_HKSKELETALANIMATION_XML_H

#include <Common/Base/hkBase.h>
#include <Animation/Animation/Animation/hkaAnnotationTrack.h>
#include <Animation/Animation/Motion/hkaAnimatedReferenceFrame.h>

class hkaChunkCache;

/// hkaAnimation meta information
extern const class hkClass hkaAnimationClass;

/// The base class for animation storage.
/// All skeletal animation have a finite period specified in seconds.
/// The interface implies that each implementation can be sampled continuously over the period.
class hkaAnimation : public hkReferencedObject
{
	public:
	
		//+version(1)

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_UNCOMPRESSED );
		HK_DECLARE_REFLECTION();

			/// Default constructor
		inline hkaAnimation();

			/// Type information
		enum AnimationType {

				///
			HK_UNKNOWN_ANIMATION = 0,

				/// Interleaved
			HK_INTERLEAVED_ANIMATION,
				/// Delta compressed
			HK_DELTA_COMPRESSED_ANIMATION,
				/// Wavelet compressed
			HK_WAVELET_COMPRESSED_ANIMATION,
				/// Mirrored
			HK_MIRRORED_ANIMATION,
				/// Spline
			HK_SPLINE_COMPRESSED_ANIMATION,
				/// Quantized
			HK_QUANTIZED_COMPRESSED_ANIMATION
		};

		
			/// Get AnimationType.
		HK_FORCE_INLINE hkaAnimation::AnimationType getType() const;

			/// Get the pose at a given time
		virtual void sampleTracks(hkReal time, hkQsTransform* transformTracksOut, hkReal* floatTracksOut, hkaChunkCache* cache) const = 0;
		
			/// Get a subset of the first 'maxNumTracks' transform tracks (all tracks from 0 to maxNumTracks-1 inclusive), and the first 'maxNumFloatTracks' float tracks of a pose at a given time.
		virtual void samplePartialTracks(hkReal time,
										 hkUint32 maxNumTransformTracks, hkQsTransform* transformTracksOut,
										 hkUint32 maxNumFloatTracks, hkReal* floatTracksOut,
										 hkaChunkCache* cache) const;

			/// Clear the cache of all keys associated with this animation - use to 'unload' an animation from the cache
		virtual void clearAllCacheKeys(hkaChunkCache* cache) const;


			/// Sample a single animation track.
		inline void sampleSingleTransformTrack( hkReal time, hkInt16 track, hkQsTransform* transformOut ) const;

			/// Sample a single floating track.
		inline void sampleSingleFloatTrack( hkReal time, hkInt16 track, hkReal* out ) const;

			/// Sample individual animation tracks.
		virtual void sampleIndividualTransformTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkQsTransform* transformOut ) const = 0;

			/// Sample a individual floating tracks.
		virtual void sampleIndividualFloatTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkReal* out ) const = 0;



			/// Returns the number of original samples / frames of animation.
		virtual int getNumOriginalFrames() const = 0;

			/*
			* Block decompression
			*/

			// INTERNAL
		struct DataChunk
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ANIM_RUNTIME, DataChunk);

			const void* m_data;
			hkUint32	m_size;
			hkUint8     m_offset; // the amount we need to add to the data to bring it to the "real" data
			DataChunk() : m_data(HK_NULL), m_size(0), m_offset(0) {}	// Only exisits on PPU for error checking
			inline void setDataChunkWith16ByteAlignmentCorrection(const void* dataPtrIn, int numBytes)
			{
				hkUlong actualStart = (hkUlong) dataPtrIn;
				m_offset = hkUint8(actualStart & 0xF);
				actualStart &= 0xFFFFFFF0;
				m_data = (const void*) actualStart;

				m_size = numBytes;
				m_size += m_offset;
				m_size = HK_NEXT_MULTIPLE_OF(16, m_size);
			}

			inline void init()
			{
				m_data = HK_NULL;
				m_size = 0;
				m_offset = 0;
			}
		};

			/// Return the number of chunks of data required to sample a pose at time t, expressed as a frame index and delta to eliminate ambiguity
		virtual int getNumDataChunks(hkUint32 frame, hkReal delta) const;

			/// Return the chunks of data required to sample a pose at time t, expressed as a frame index and delta to eliminate ambiguity
		virtual void getDataChunks(hkUint32 frame, hkReal delta, DataChunk* dataChunks, int m_numDataChunks) const;

			/// Return the maximum total size of all combined chunk data which could be returned by getDataChunks for this animation.
		virtual int getMaxSizeOfCombinedDataChunks() const;

			/// Returns the motion stored (previously extracted from the animation) at time t.
			/// This motion represents the absolute offset from the start of the animation.
		virtual void getExtractedMotionReferenceFrame(hkReal time, hkQsTransform& motionOut) const;
	
			/// Returns the change in reference frame between two times for extracted motion.
		virtual void getExtractedMotionDeltaReferenceFrame( hkReal time, hkReal nextTime, int loops, hkQsTransform& deltaMotionOut, hkReal cropStartAmount, hkReal cropEndAmount ) const;

			/// Test for presence of extracted motion
		hkBool hasExtractedMotion() const;

			/// Set the extracted motion of this animation
		void setExtractedMotion( const hkaAnimatedReferenceFrame* extractedMotion );
				
			/// Get the frame/sample index and delta between frames. So 0 <= frameOut < getNumOriginalFrames() and 0 <= delta < 1.0
		inline void getFrameAndDelta(hkReal time, hkUint32& frameOut, hkReal& deltaOut) const;

			/*
			 * Annotation support
			 */

			/// A structure to hold annotation results
			/// The ID is remapped from an animation track ID to a skeleton bone ID
			/// This class MUST have the same declaration as hkaAnimatedSkeleton::BoneAnnotation
		struct TrackAnnotation
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaAnimation::TrackAnnotation );

			/// The bone which is annotated
			hkUint16 m_trackID;

			/// Annotation data
			hkaAnnotationTrack::Annotation m_annotation;
		};

			/// Get the number of annotations for which: startTime <= annotation < startTime + deltaTime
			/// Useful for allocating correctly-sized output arrays (see getAnnotations)
		virtual hkUint32 getNumAnnotations( hkReal startTime, hkReal deltaTime ) const;

			/// Get all annotations for which: startTime <= annotation < startTime + deltaTime
			/// \param annotations Output array. Must be pre-allocated to at least maxAnnotations in size, (see getNumAnnotations)
			/// \param maxAnnotations maximum number of annotations to find
			/// \return The number of annotations found
		virtual hkUint32 getAnnotations( hkReal startTime, hkReal deltaTime, TrackAnnotation* annotationsOut, hkUint32 maxAnnotations = HK_INT32_MAX ) const;


	protected:
		
			/// Copy constructor.
		hkaAnimation( const hkaAnimation& );

			/// AnimationType.
		hkEnum<hkaAnimation::AnimationType, hkInt32> m_type;
	
	public:

			/// The length of the animation cycle in seconds
		hkReal m_duration;
		
			/// The number of bone tracks to be animated.
		int m_numberOfTransformTracks;

			/// The number of float tracks to be animated
		int m_numberOfFloatTracks;

	protected:
			/// An hkaAnimatedReferenceFrame instance containing extracted motion.
		hkRefPtr< const class hkaAnimatedReferenceFrame > m_extractedMotion;
		
	public:
			/// The annotation tracks associated with this skeletal animation.
		hkArray< class hkaAnnotationTrack > m_annotationTracks;
	
			// Constructor for initialisation of vtable fixup
		HK_FORCE_INLINE hkaAnimation( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag), m_extractedMotion(flag), m_annotationTracks(flag) {}
};

#include <Animation/Animation/Animation/hkaAnimation.inl>

#endif // HKANIMATION_ANIMATION_HKSKELETALANIMATION_XML_H

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
