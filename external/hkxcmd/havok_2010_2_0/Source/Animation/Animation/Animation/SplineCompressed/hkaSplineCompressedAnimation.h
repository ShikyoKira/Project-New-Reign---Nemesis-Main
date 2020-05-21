/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKANIMATION_ANIMATION_SPLINE_HKSPLINEANIMATION_XML_H
#define HKANIMATION_ANIMATION_SPLINE_HKSPLINEANIMATION_XML_H

#include <Animation/Animation/Animation/hkaAnimation.h>
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Internal/Compression/hkaCompression.h>

extern const class hkClass hkaSplineCompressedAnimationAnimationCompressionParamsClass;

extern const class hkClass hkaSplineCompressedAnimationTrackCompressionParamsClass;

/// hkaSplineCompressedAnimation meta information
extern const class hkClass hkaSplineCompressedAnimationClass;


/// Compresses animation data using a spline approximation.
/// See Animation Compression section of the Userguide for details.
class hkaSplineCompressedAnimation : public hkaAnimation
{
public:

	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_COMPRESSED );
	HK_DECLARE_REFLECTION();

	static const int MAX_DEGREE = 3;
	static const int MAX_ORDER = MAX_DEGREE + 1;

	/// Compression settings for a single animation track
	struct TrackCompressionParams
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_COMPRESSED, hkaSplineCompressedAnimation::TrackCompressionParams );
		HK_DECLARE_REFLECTION();

		// Quantization enum declarations
		enum RotationQuantization
		{
			POLAR32 = 0, ///< 32 Bit polar quantization
			THREECOMP40 = 1, ///< 40 Bit "smallest three" quantization
			THREECOMP48 = 2, ///< 48 Bit "smallest three" quantization
			THREECOMP24 = 3, ///< 24 Bit "smallest three" quantization
			STRAIGHT16 = 4, ///< 16 Bit quantization
			UNCOMPRESSED = 5 ///< 128 Bit (no quantization)
		}; ///<  Rotation Quantization types

		enum ScalarQuantization
		{
			BITS8 = 0, ///< 8 Bit quantization
			BITS16 = 1 ///< 16 Bit quantization
		}; ///< Scalar value quantization types

		// Spline approximation (compression) settings
		hkReal m_rotationTolerance; ///< Rotation tolerance for the animation track
		hkReal m_translationTolerance; ///< Translation tolerance for the animation track
		hkReal m_scaleTolerance; ///< Scale tolerance for the animation track
		hkReal m_floatingTolerance; ///< Tolerance for floating data values
		hkUint16 m_rotationDegree; ///< Degree of polynomial approximation used for rotation data
		hkUint16 m_translationDegree; ///< Degree of polynomial approximation used for translation data
		hkUint16 m_scaleDegree; ///< Degree of polynomial approximation used for scale data
		hkUint16 m_floatingDegree; ///< Degree of polynomial approximation used for floating data

		// Quantization settings
		hkEnum< RotationQuantization, hkUint8 > m_rotationQuantizationType; ///< Quantization type for rotation data
		hkEnum< ScalarQuantization, hkUint8 > m_translationQuantizationType; ///< Quantization type for translation data
		hkEnum< ScalarQuantization, hkUint8 > m_scaleQuantizationType; ///< Quantization type for scale data
		hkEnum< ScalarQuantization, hkUint8 > m_floatQuantizationType; ///< Quantization type for float data

		static hkBool validQuantization( RotationQuantization ); ///< Debugging check if a valid rotation quantization was chosen
		static hkBool validQuantization( ScalarQuantization ); ///< Debugging check if a valid scalar quantization value was chosen

		// Validity test
		hkBool isOk() const;

		// Approximate quantization accuracy
		static hkReal HK_CALL approximateQuantizationAccuracy( RotationQuantization );
		static hkReal HK_CALL approximateQuantizationAccuracy( ScalarQuantization );

		// Constructor
		TrackCompressionParams(); ///< Constructor with default parameters
	};

	struct AnimationCompressionParams
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_COMPRESSED, hkaSplineCompressedAnimation::AnimationCompressionParams );
		HK_DECLARE_REFLECTION();

		// Per-animation settings
		hkUint16 m_maxFramesPerBlock; ///< Maximum number of frames per block (Typically only used for SPU animations)
		hkBool m_enableSampleSingleTracks; ///< Enable individual tracks to be sampled (can increase asset size)

		// Constructor
		AnimationCompressionParams(); ///< Constructor with default parameters
	};

		
	/// This structure is used when specifying per track compression settings
	struct PerTrackCompressionParams
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaSplineCompressedAnimation::PerTrackCompressionParams);
		/// Compression settings palette.
		/// List of CompressionParams to enable per-bone compression settings.
		/// On initialisation only a single element is allocated.
		hkArray< struct hkaSplineCompressedAnimation::TrackCompressionParams > m_parameterPalette;

		/// An array of indices into the palette above.
		/// Map for each track to the chosen compression parameter palette.
		hkArray<int> m_trackIndexToPaletteIndex;
		/// Map for each float track to the chosen compression parameter palette.
		hkArray<int> m_floatTrackIndexToPaletteIndex;

		/// Validity check
		hkBool isOk() const;
	};

	/// Constructor with uniform compression settings for each track
	hkaSplineCompressedAnimation(
		const hkaInterleavedUncompressedAnimation& raw,
		const TrackCompressionParams& trackParams,
		const AnimationCompressionParams& animationParams );

	/// Constructor allowing different compression settings for each track
	hkaSplineCompressedAnimation(
		const hkaInterleavedUncompressedAnimation& raw,
		const PerTrackCompressionParams& trackParams,
		const AnimationCompressionParams& animationParams );

				
	/// Get the tracks at a given time
	/// Note: If you are calling this method directly you may find some quantization error present in the rotations.
	/// The blending done in hkaAnimatedSkeleton is not sensitive to rotation error so rather than renormalize here
	/// we defer it until blending has been completed. If you are using this method directly you may want to call
	/// hkaSkeletonUtils::normalizeRotations() on the results.
	virtual void sampleTracks(hkReal time, hkQsTransform* transformTracksOut, hkReal* floatTracksOut, hkaChunkCache* cache) const;
		
	/// Get a subset of the first 'maxNumTracks' tracks of a pose at a given time (all tracks from 0 to maxNumTracks-1 inclusive).
	virtual void samplePartialTracks(hkReal time,
									 hkUint32 maxNumTransformTracks, hkQsTransform* transformTracksOut,
									 hkUint32 maxNumFloatTracks, hkReal* floatTracksOut,
									 hkaChunkCache* cache) const;

	/// Sample a individual animation tracks
	virtual void sampleIndividualTransformTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkQsTransform* transformOut ) const;

	/// Sample a individual floating tracks
	virtual void sampleIndividualFloatTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkReal* out ) const;


	/// Returns the number of original samples / frames of animation
	virtual int getNumOriginalFrames() const;

	/// Return the number of chunks of data required to sample a pose at time t
	virtual int getNumDataChunks(hkUint32 frame, hkReal delta) const;

	/// Return the chunks of data required to sample a pose at time t
	virtual void getDataChunks(hkUint32 frame, hkReal delta, DataChunk* dataChunks, int m_numDataChunks) const;

	/// Return the maximum total size of all combined chunk data which could be returned by getDataChunks for this animation.
	virtual int getMaxSizeOfCombinedDataChunks() const;

	/// Get a subset of the tracks a given time using data chunks. Sample is calculated using pose[frameIndex] * (1 - frameDelta) + pose[frameIndex+1] * frameDelta.
	static void HK_CALL samplePartialWithDataChunks(hkUint32 frameIndex, hkReal frameDelta, 
														hkUint32 maxNumTransformTracks, hkQsTransform* transformTracksOut,
														hkUint32 maxNumFloatTracks, hkReal* floatTracksOut,
														const DataChunk* dataChunks, int numDataChunks);

public:

	/// Gets the total size in bytes of the spline compressed animation.
	int getSizeInBytes() const;
		

public:
	
	// Constructor for initialisation of vtable fixup
	HK_FORCE_INLINE hkaSplineCompressedAnimation( hkFinishLoadedObjectFlag flag ) :
		hkaAnimation( flag ),
		m_blockOffsets( flag ),
		m_floatBlockOffsets( flag ),
		m_transformOffsets( flag ),
		m_floatOffsets( flag ),
		m_data( flag )
		{
			if ( flag.m_finishing ) handleEndian();
		}

	~hkaSplineCompressedAnimation();

private:

	// Sampling

	void sampleRotation( hkReal time, hkUint8 quantizedTime, hkUint8 mask, TrackCompressionParams::RotationQuantization type, const hkUint8*& dataInOut, hkQuaternion& out ) const;
	void sampleTranslation( hkReal time, hkUint8 quantizedTime, hkUint8 mask, TrackCompressionParams::ScalarQuantization type, const hkUint8*& dataInOut, hkVector4& out ) const;
	void sampleScale( hkReal time, hkUint8 quantizedTime, hkUint8 mask, TrackCompressionParams::ScalarQuantization type, const hkUint8*& dataInOut, hkVector4& out ) const;
	void sampleFloat( hkReal time, hkUint8 quantizedTime, hkUint8 mask, TrackCompressionParams::ScalarQuantization type, const hkUint8*& dataInOut, hkReal& out ) const;


	// Generic data I/O

	static void write8( hkUint8, hkArray< hkUint8 > & );
	static void write16( hkUint16, hkArray< hkUint8 > & );
	static void writeReal( hkReal, hkArray< hkUint8 > & );
	static void writeZeros( int, hkArray< hkUint8 > & );
	static void writeAlign( int, hkArray< hkUint8 > & );
	static void writeAlignQuaternion( TrackCompressionParams::RotationQuantization type, hkArray< hkUint8 > & );

	inline static hkUint8 read8( const hkUint8*& dataInOut );
	inline static hkUint16 read16( const hkUint8*& dataInOut );
	inline static hkReal readReal( const hkUint8*& dataInOut );
	inline static void readAlign( int, const hkUint8*& dataInOut );
	inline static void readAlignQuaternion( TrackCompressionParams::RotationQuantization type, const hkUint8*& dataInOut );

	inline static int bytesPerComponent( TrackCompressionParams::ScalarQuantization type );
	inline static int bytesPerQuaternion( TrackCompressionParams::RotationQuantization type );

	// NURBS Specific I/O

	void writeKnots( int n, int p, const hkArray< hkReal >& U );
	void writeRotation( hkUint8 mask, TrackCompressionParams::RotationQuantization type, const hkVector4& mean, int n, const hkArray< hkVector4 >& P );
	void writePoints( hkUint8 mask, TrackCompressionParams::ScalarQuantization type, const hkVector4& mean, const hkVector4& minp, const hkVector4& maxp, int n, hkArray< hkVector4 >& P );

	static void readNURBSCurve( const hkUint8*& dataInOut, hkUint8 quantizedTime, hkReal frameDuration, hkReal u, hkUint8 mask, TrackCompressionParams::ScalarQuantization scalarQuantizationType, hkVector4& I, hkVector4& out );
	static void readNURBSQuaternion( const hkUint8*& dataInOut, hkUint8 quantizedTime, hkReal frameDuration, hkReal u, hkUint8 mask, TrackCompressionParams::RotationQuantization type, hkQuaternion& out );
	static int readKnots( const hkUint8*& dataInOut, int* HK_RESTRICT n, int* HK_RESTRICT p, hkUint8 quantizedTime, hkReal frameDuration, hkReal U[ MAX_DEGREE * 2 ] );
	static void readPackedQuaternions( const hkUint8*& dataInOut, TrackCompressionParams::RotationQuantization type, int n, int p, int span, hkVector4 P[ MAX_ORDER ] );

	
	// NURBS Evaluation

	inline static int findSpan( int n, int p, hkUint8 u, const hkUint8* U );
	inline static void evaluate( hkReal time, int p, hkReal U[ MAX_DEGREE * 2 ], hkVector4 P[ MAX_ORDER ], hkVector4 & );

	static void evaluateSimple( hkReal time, int p, hkReal U[ MAX_DEGREE * 2 ], hkVector4 P[ MAX_ORDER ], hkVector4 & );
	static void evaluateSimple1( hkReal time, int p, hkReal U[ MAX_DEGREE * 2 ], hkVector4 P[ MAX_ORDER ], hkVector4 & );
	static void evaluateSimple2( hkReal time, int p, hkReal U[ MAX_DEGREE * 2 ], hkVector4 P[ MAX_ORDER ], hkVector4 & );
	static void evaluateSimple3( hkReal time, int p, hkReal U[ MAX_DEGREE * 2 ], hkVector4 P[ MAX_ORDER ], hkVector4 & );

	inline void getBlockAndTime( hkUint32 frame, hkReal delta, int& blockOut, hkReal& blockTimeOut, hkUint8& quantizedTimeOut ) const;


	// Initialization

	void initialize( const hkaInterleavedUncompressedAnimation& raw, const PerTrackCompressionParams& trackParams, const AnimationCompressionParams& animationParams );
	static void deinterleaveTransform( const hkQsTransform* xform, int numTrack, int numFrames, int offset, int track, hkArray< hkVector4 >& tOut,  hkArray< hkVector4 >& qOut,  hkArray< hkVector4 >& sOut );
	static void deinterleaveFloat( const hkReal* data, int numTrack, int numFrames, int offset, int track, hkArray< hkVector4 >& out );
	

	// Packing, quantization and endian handling

	static hkUint8 packQuantizationTypes( TrackCompressionParams::ScalarQuantization position, TrackCompressionParams::RotationQuantization rotation, TrackCompressionParams::ScalarQuantization scale );
	inline static void unpackQuantizationTypes( hkUint8 packedQuatizationTypes, TrackCompressionParams::ScalarQuantization& translationQuantization, TrackCompressionParams::RotationQuantization& rotationQuantization, TrackCompressionParams::ScalarQuantization& scaleQuantization );
	
	static hkUint8 packMaskAndQuantizationType( hkUint8 mask, TrackCompressionParams::ScalarQuantization floatQuatization );
	inline static void unpackMaskAndQuantizationType( hkUint8 packedMaskAndQuatizationType, hkUint8& mask, TrackCompressionParams::ScalarQuantization& floatQuantization );
	
	static hkUint8 pack8( hkReal minp, hkReal maxp, hkReal val );
	inline static hkReal unpack8( hkReal minp, hkReal maxp, hkUint8 val );

	static hkUint16 pack16( hkReal minp, hkReal maxp, hkReal val );
	inline static hkReal unpack16( hkReal minp, hkReal maxp, hkUint16 val );

	static void packQuaternion( TrackCompressionParams::RotationQuantization type, const hkQuaternion* in, hkUint8* out );
	inline static void unpackQuaternion( TrackCompressionParams::RotationQuantization type, const hkUint8* in, hkQuaternion* out );
	static void handleEndianQuaternion( TrackCompressionParams::RotationQuantization type, hkUint8* dataInOut );

	static hkUint8 isStatic( const hkVector4& mean, const hkVector4& minp, const hkVector4& maxp, hkReal tol, const hkVector4& I );
	static hkVector4 getMean( const hkArray< hkVector4 >& P );
	static hkVector4 getMin( const hkArray< hkVector4 >& P );
	static hkVector4 getMax( const hkArray< hkVector4 >& P );
	inline static void recompose( hkUint8 mask, const hkVector4& S, const hkVector4& I, hkVector4& out );

	void handleEndian();
	static void reverseEndian( int bytes, hkUint8*& dataInOut );
	static void handleEndianScalar( hkUint8 mask, TrackCompressionParams::ScalarQuantization type, hkUint8*& dataInOut );
	static void handleEndianRotation( hkUint8 mask, TrackCompressionParams::RotationQuantization type, hkUint8*& dataInOut );
	static int  handleEndianKnots( hkUint8 mask, hkUint8*& dataInOut );
	static void handleEndianScalarControlPoints( hkUint8 mask, int n, TrackCompressionParams::ScalarQuantization type, hkUint8*& dataInOut );
	static void handleEndianRotationControlPoints( hkUint8 mask, int n, TrackCompressionParams::RotationQuantization type, hkUint8*& dataInOut );

	// Utility
	static void Linspace( int m, hkArray< hkReal >& ub, hkReal a, hkReal b );

	// Member variables

	// Full animation data
	int m_numFrames;

	// Block info
	int m_numBlocks;
	int m_maxFramesPerBlock;
	int m_maskAndQuantizationSize;
	hkReal m_blockDuration;
	hkReal m_blockInverseDuration;
	hkReal m_frameDuration;

	// Data buffers
	hkArray< hkUint32 > m_blockOffsets; ///< Offset to data for block i, stream relative
	hkArray< hkUint32 > m_floatBlockOffsets; ///< Offset to the first byte of float data for block i, block relative
	hkArray< hkUint32 > m_transformOffsets; ///< Offset to transform data for track i, block relative, and in multiples of 4 bytes
	hkArray< hkUint32 > m_floatOffsets; ///< Offset to float data for track i, block relative, and in multiples of 4 bytes
	hkArray< hkUint8 > m_data;

	// Endian
	int m_endian;
};

#include <Animation/Animation/Animation/SplineCompressed/hkaSplineCompressedAnimation.inl>

#endif // HKANIMATION_ANIMATION_SPLINE_HKSPLINEANIMATION_XML_H

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
