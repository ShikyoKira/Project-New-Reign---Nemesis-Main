/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ANIMATION_QUANTIZED_H
#define HK_ANIMATION_QUANTIZED_H

#include <Animation/Animation/Animation/hkaAnimation.h>
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Internal/Compression/hkaCompression.h>

extern const class hkClass hkaQuantizedAnimationTrackCompressionParamsClass;

/// hkaQuantizedAnimation meta information
extern const class hkClass hkaQuantizedAnimationClass;

/// Builder helper class
class hkaQuantizedAnimationBuildier;

/// Compresses animation data using a quantized approximation.
/// See Animation Compression section of the Userguide for details.
class hkaQuantizedAnimation : public hkaAnimation
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_COMPRESSED );
		HK_DECLARE_REFLECTION();


		//
		// Initialization interface classes
		//

		/// Compression settings for a single animation track
		class TrackCompressionParams
		{
			public:

				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_COMPRESSED, hkaQuantizedAnimation::TrackCompressionParams );
				HK_DECLARE_REFLECTION();

				/// Quantized approximation (compression) settings
				hkReal m_rotationTolerance;
				hkReal m_translationTolerance;
				hkReal m_scaleTolerance;
				hkReal m_floatingTolerance;

				/// Constructor with default parameters
				TrackCompressionParams();
		};

		/// This structure is used when specifying per track compression settings
		struct PerTrackCompressionParams
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaQuantizedAnimation::PerTrackCompressionParams);

			/// List of CompressionParams to enable per-bone compression settings
			/// On initialisation only a single element is allocated.
			hkArray< class hkaQuantizedAnimation::TrackCompressionParams > m_parameterPalette;

			/// An array of indices into the palette above
			hkArray< hkUint32 > m_trackIndexToPaletteIndex;
			hkArray< hkUint32 > m_floatTrackIndexToPaletteIndex;
		};


		//
		// Constructors
		//

		/// Constructor with given or default compression settings for each track
		hkaQuantizedAnimation(
			const hkaAnimationBinding& binding,
			const hkaSkeleton& skeleton,
			const TrackCompressionParams& Params = TrackCompressionParams() );

		/// Constructor allowing different compression settings for each track specified as a palette
		hkaQuantizedAnimation(
			const hkaAnimationBinding& binding,
			const hkaSkeleton& skeleton,
			const PerTrackCompressionParams& perTrackParams );

		/// Constructor allowing different compression settings for each track specified directly
		hkaQuantizedAnimation(
			const hkaAnimationBinding& binding,
			const hkaSkeleton& skeleton,
			const hkArray< TrackCompressionParams > transformParams,
			const hkArray< TrackCompressionParams > floatParams );


		virtual ~hkaQuantizedAnimation();

		/// Useful query returning the size of this compressed animation in bytes
		int getTotalSize() const;


		//
		// Functions for sampling the full pose
		//

		/// Scratch memory needed can be allocated internally, or passed in
		/// All output arrays must have their size rounded up to the nearest multiple of four
		/// Must call setSkeleton() at setup time
		///
		/// \param bonesOut                Output sampled bones
		/// \param boneWeightsOut          Output sampled bone weights
		/// \param floatsOut               Output sampled floats (if any)
		/// \param floatWeightsOut         Output sampled float weights
		/// \param time                    Local time to sample at
		/// \param useSlerp                Internally use slerp or lerp to blend between frames; lerp requires later normalization.
		/// \param frameSteppingTolerance  Tolerance for locking to the nearest frame; when set to 0 inter-frame interpolation always occurs. When set to 0.5 inter-frame interpolation never occurs.
		/// \param bones/FloatsScratch     Scratch buffers. Must be rounded up to nearest multiple of four times the number of bones/floats. If not provided, scratch memory will be allocated internally.
		///
		void sampleFullPose( hkQsTransform* bonesOut, hkReal* boneWeightsOut, hkReal* floatsOut, hkReal* floatWeightsOut, hkReal time, bool useSlerp = true, hkReal frameSteppingTolerance = 1.0e-3 ) const;
		void sampleFullPose( hkQsTransform* bonesOut, hkReal* floatsOut, hkReal* boneWeightsOut, hkReal* floatWeightsOut, hkQsTransform* bonesScratch0, hkQsTransform* bonesScratch1, hkReal* floatsScratch0, hkReal* floatsScratch1, hkReal time, bool useSlerp = true, hkReal frameSteppingTolerance = 1.0e-3 ) const;


		//
		// Inherited functions from hkaAnimation.
		// These functions will not perform as efficiently as the sampleFullPose interface presented above.
		//

		/// Get the pose at a given time
		/// Must call setSkeleton() at setup time
		virtual void sampleTracks( hkReal time, hkQsTransform* transformTracksOut, hkReal* floatTracksOut, hkaChunkCache* cache ) const;
			
		/// Get a subset of the first 'maxNumTracks' transform tracks (all tracks from 0 to maxNumTracks-1 inclusive), and the first 'maxNumFloatTracks' float tracks of a pose at a given time.
		/// Must call setSkeleton() at setup time
		virtual void samplePartialTracks( hkReal time,
										  hkUint32 maxNumTransformTracks, hkQsTransform* transformTracksOut,
										  hkUint32 maxNumFloatTracks, hkReal* floatTracksOut,
										  hkaChunkCache* cache ) const;

		/// Not yet implemented.
		virtual void sampleIndividualTransformTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkQsTransform* transformOut ) const;
		virtual void sampleIndividualFloatTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkReal* out ) const;

		/// Returns the number of original samples / frames of animation.
		virtual int getNumOriginalFrames() const;

		/// This function must be called at setup time if calling any of the non-static sample functions
		void setSkeleton( const hkaSkeleton* );


		//
		// Static interface
		// The following functions are designed for use on non-shared memory architectures.
		// Typically, these functions do not need to be called by the user directly
		//

		/// Non-static access to datastream
		HK_FORCE_INLINE const hkUint8* getData() const;

		/// Static query routines - typically not needed by the user
		HK_FORCE_INLINE static int HK_CALL getFrameOffset( const hkUint8* data, int frame );
		HK_FORCE_INLINE static int HK_CALL getFrameSize( const hkUint8* data );
		HK_FORCE_INLINE static void HK_CALL getFrameAndDelta( const hkUint8* data, hkReal time, hkUint32& frameOut, hkReal& delta );
		HK_FORCE_INLINE static hkUint32 getNumFrames( const hkUint8* data );
		HK_FORCE_INLINE static hkUint32 getNumBones( const hkUint8* data );
		HK_FORCE_INLINE static hkUint32 getNumFloats( const hkUint8* data );

		/// Static sampling; useful on platforms having a non-shared memory architecture
		static void HK_CALL sampleFullPose( const hkUint8* headerData, hkQsTransform* bonesOut, hkReal* floatsOut, hkReal* boneWeightsOut, hkReal* floatWeightsOut, const hkQsTransform* referencePose, const hkReal* referenceFloats, hkReal time, bool useSlerp = true, hkReal frameSteppingTolerance = 1.0e-3 );
		static void HK_CALL sampleFullPose( const hkUint8* headerData, hkQsTransform* bonesOut, hkReal* floatsOut, hkReal* boneWeightsOut, hkReal* floatWeightsOut, const hkQsTransform* referencePose, const hkReal* referenceFloats, hkQsTransform* bonesScratch0, hkQsTransform* bonesScratch1, hkReal* floatsScratch0, hkReal* floatsScratch1, hkReal time, bool useSlerp = true, hkReal frameSteppingTolerance = 1.0e-3 );
		static void HK_CALL sampleFullPose( const hkUint8* headerData, const hkUint8* frame0Data, const hkUint8* frame1Data, hkQsTransform* bonesOut, hkReal* floatsOut, hkReal* boneWeightsOut, hkReal* floatWeightsOut, const hkQsTransform* referencePose, const hkReal* referenceFloats, hkQsTransform* bonesScratch0, hkQsTransform* bonesScratch1, hkReal* floatsScratch0, hkReal* floatsScratch1, hkReal delta, bool useSlerp = true, hkReal frameSteppingTolerance = 1.0e-3 );

	protected:

		friend class hkaQuantizedAnimationBuilder;

	private:


		/// Endian handling
		void handleEndian();
		static void HK_CALL swizzleEndian16( void* valInOut, int n );
		static void HK_CALL swizzleEndian32( void* valInOut, int n );

		/// Utility functions for sampling
		static void HK_CALL sampleStaticScalars( hkReal* poseOut, const hkUint16* elements, const hkReal* values, int n );
		static void HK_CALL sampleDynamicScalars( hkReal* poseOut, const hkUint16* elements, const hkReal* minimums, const hkReal* spans, const hkUint16* values, int n );
		static void HK_CALL sampleRotations( hkQuaternion* poseOut, const hkUint16* elements, const hkUint16* values, int n );

		/// Data header description
		struct QuantizedAnimationHeader
		{
			/// Total size of header, including this struct, weights and static values/ranges
			hkUint16 m_headerSize;

			/// Description of this animation
			hkUint16 m_numBones;
			hkUint16 m_numFloats;
			hkUint16 m_numFrames;
			hkReal m_duration;

			hkUint16 m_numStaticTranslations;
			hkUint16 m_numStaticRotations;
			hkUint16 m_numStaticScales;
			hkUint16 m_numStaticFloats;

			hkUint16 m_numDynamicTranslations;
			hkUint16 m_numDynamicRotations;
			hkUint16 m_numDynamicScales;
			hkUint16 m_numDynamicFloats;

			/// Size of each frame in bytes
			hkUint16 m_frameSize;
			
			/// Element offsets
			hkUint16 m_staticElementsOffset;
			hkUint16 m_dynamicElementsOffset;

			/// Value offsets
			hkUint16 m_staticValuesOffset;
			hkUint16 m_dynamicRangeMinimumsOffset;
			hkUint16 m_dynamicRangeSpansOffset;

			/// Swizzle the endianness of this block
			void swizzleEndian();
		};

		HK_FORCE_INLINE const QuantizedAnimationHeader* getHeader() const;

		hkArray< hkUint8 > m_data;

		/// Endian
		hkUint32 m_endian;

		/// Associated skeleton. Only needed when calling inherited SampleTracks interface.
		const hkaSkeleton* m_skeleton; //+nosave +default(0)

		/// Table for converting bytes to reals
		static hkReal hkUint8toRealFraction[ 256 ];

	public:
		
		/// Constructor for initialisation of vtable fixup.
		HK_FORCE_INLINE hkaQuantizedAnimation( hkFinishLoadedObjectFlag flag ) :
			hkaAnimation( flag ),
			m_data( flag )
			{
				if ( flag.m_finishing ) handleEndian();
			}
};

class hkaQuantizedAnimationBuilder
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaQuantizedAnimationBuilder);
		hkaQuantizedAnimationBuilder( hkaQuantizedAnimation& anim );

		/// Common initialization member function used by constructors
		void initialize(
			const hkaAnimationBinding& binding,
			const hkaSkeleton& skeleton,
			const hkArray< hkaQuantizedAnimation::TrackCompressionParams >& transformParams,
			const hkArray< hkaQuantizedAnimation::TrackCompressionParams >& floatParams );

	public:

		struct Range
		{
			hkReal m_minimum;
			hkReal m_span;
		};

		struct StaticScalar
		{
			hkReal m_value;
			hkUint16 m_boneElement;
		};

		struct DynamicScalar
		{
			Range m_range;
			hkUint16 m_trackElement;
			hkUint16 m_boneElement;
		};

		struct StaticRotation
		{
			hkQuaternion m_value;
			hkUint16 m_boneElement;
		};

		struct DynamicRotation
		{
			hkUint16 m_trackElement;
			hkUint16 m_boneElement;
		};

		/// Tests if the track-to-bone order of binding is monotonically increasing, only monotonically increasing
		/// bindings are supported when using the inherited SampleTracks interfaces
		static bool HK_CALL isBindingMonotonic( const hkaAnimationBinding& );

		/// Determines the maximum and minimum value of a real value in an array
		static void HK_CALL getRange( Range& rangeOut, const hkReal* base, int stride, int n );
		static void HK_CALL getRange( hkReal& minimumOut, hkReal& spanOut, const hkReal* base, int stride, int n );

		static void HK_CALL identifyScalarElements( hkUint16 trackElement, hkUint16 boneElement,
											const Range* rangeBase, const hkReal* referenceBase, hkReal tolerance,
											hkArray< StaticScalar >& staticElementsOut, hkArray< DynamicScalar >& dynamicElementsOut );

		static void HK_CALL identifyRotations( hkUint16 track, hkUint16 bone, const Range* rangeBase, const hkReal* referenceBase, hkReal tolerance,
									   hkArray< StaticRotation >& staticElementsOut, hkArray< DynamicRotation >& dynamicElementsOut );		///< Utility function for appending data to the datastream during construction

		/// Mostly handles alignment
		static void HK_CALL align( hkUint32& numInOut, int size );
		void align( const int size );
		void appendData( const void* data, int size );
		void appendData( const hkUint16& val );
		void appendData( const hkUint32& val );
		void appendData( const hkReal& val );
		void appendData( const hkArray< hkUint8 >& data );
		void appendElements( const hkArray< StaticScalar >& data );
		void appendElements( const hkArray< StaticRotation >& data );
		void appendElements( const hkArray< DynamicScalar >& data );
		void appendElements( const hkArray< DynamicRotation >& data );
		void appendValues( const hkArray< StaticScalar >& data );
		void appendValues( const hkArray< StaticRotation >& data );
		void appendRangeMinimums( const hkArray< DynamicScalar >& data );
		void appendRangeSpans( const hkArray< DynamicScalar >& data );
		void appendValues( const hkArray< DynamicScalar >& data, const hkReal* base );
		void appendValues( const hkArray< DynamicRotation >& data, const hkQuaternion* base );

		/// Quaternion quantization
		static hkUint16 HK_CALL quantize16( hkReal value, const Range& range );
		static void HK_CALL quantizeQuaternion( hkUint16* dst, const hkQuaternion& src );

		/// hkaQuantizedAnimation data members
		hkArray< hkUint8 >& m_data;
		hkUint32& m_endian;
		const hkaSkeleton*& m_skeleton;
		hkEnum<hkaAnimation::AnimationType, hkInt32>& m_type;
};


#include <Animation/Animation/Animation/Quantized/hkaQuantizedAnimation.inl>

#endif // HK_ANIMATION_QUANTIZED_H

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
