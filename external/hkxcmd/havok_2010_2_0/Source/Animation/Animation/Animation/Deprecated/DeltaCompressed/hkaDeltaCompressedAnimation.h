/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_ANIMATION_DELTACOMPRESSED_HKDELTACOMPRESSEDANIMATION_XML_H
#define HKANIMATION_ANIMATION_DELTACOMPRESSED_HKDELTACOMPRESSEDANIMATION_XML_H

#include <Animation/Animation/Animation/hkaAnimation.h>

extern const class hkClass hkaDeltaCompressedAnimationQuantizationFormatClass;

/// hkaDeltaCompressedAnimation meta information
extern const class hkClass hkaDeltaCompressedAnimationClass;

class hkaInterleavedUncompressedAnimation;

/// Compresses animation data using a delta transform.
/// See Animation Compression section of the Userguide for details.
class hkaDeltaCompressedAnimation : public hkaAnimation
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_COMPRESSED );
		HK_DECLARE_REFLECTION();

			/// Compression parameters
		struct CompressionParams
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_COMPRESSED, hkaDeltaCompressedAnimation::CompressionParams );

			/// Bits used for float quantization - default 8, range [2,16]
			hkUint16 m_quantizationBits;
			/// Block size - default 65535
			hkUint16 m_blockSize;

			/// TrackAnalysis absolute position tolerance. See the "Compression Overview" section of the Userguide for details - default 0.0
			hkReal m_absolutePositionTolerance;	// Set to 0 to use only relative tolerance
			/// TrackAnalysis relative position tolerance. See the "Compression Overview" section of the Userguide for details - default 0.01
			hkReal m_relativePositionTolerance; // Set to 0 to use only abs tolerance
			/// TrackAnalysis rotation position tolerance. See the "Compression Overview" section of the Userguide for details - default 0.001
			hkReal m_rotationTolerance;
			/// TrackAnalysis scale position tolerance. See the "Compression Overview" section of the Userguide for details - default 0.01
			hkReal m_scaleTolerance;
			/// TrackAnalysis float position tolerance. See the "Compression Overview" section of the Userguide for details - default 0.001
			hkReal m_absoluteFloatTolerance;

			CompressionParams();
		};

			/// This structure is used when specifying per track compression settings
		struct PerTrackCompressionParams
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_COMPRESSED, hkaDeltaCompressedAnimation::PerTrackCompressionParams );

			/// List of CompressionParams to enable per-bone compression settings
			hkArray<struct hkaDeltaCompressedAnimation::CompressionParams> m_parameterPalette;

			/// An array of indices into the palette above for transform tracks
			hkArray<int> m_trackIndexToPaletteIndex;

			/// An array of indices into the palette above for float tracks
			hkArray<int> m_floatTrackIndexToPaletteIndex;			
		};

			/// Constructor compresses data
		hkaDeltaCompressedAnimation(const hkaInterleavedUncompressedAnimation& rawData, const CompressionParams& params, hkBool useThreeComponentQuaternions = true );

		    /// Constructor allowing different compression settings for each track in the animation
		hkaDeltaCompressedAnimation(const hkaInterleavedUncompressedAnimation& rawData, const PerTrackCompressionParams& params, hkBool useThreeComponentQuaternions = true );
		
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


			/// Sample individual animation tracks
		virtual void sampleIndividualTransformTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkQsTransform* transformOut ) const;

			/// Sample a individual floating tracks
		virtual void sampleIndividualFloatTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkReal* out ) const;

			/// Get a key for use with the cache
		virtual hkUint32 getFullCacheKey( hkUint32 poseIdx ) const;

			/// Clear the cache of all keys associated with this animation - use to 'unload' an animation from the cache
		virtual void clearAllCacheKeys(hkaChunkCache* cache) const;

			/*
			* Block decompression
			*/

			/// Returns the number of original samples / frames of animation
		virtual int getNumOriginalFrames() const;

			/// Return the number of chunks of data required to sample the tracks at time t
		virtual int getNumDataChunks(hkUint32 frame, hkReal delta) const;

			/// Return the chunks of data required to sample the track at time t
		virtual void getDataChunks(hkUint32 frame, hkReal delta, DataChunk* dataChunks, int numDataChunks) const;

			/// Return the maximum total size of all combined chunk data which could be returned by getDataChunks for this animation.
		virtual int getMaxSizeOfCombinedDataChunks() const;

			/// Get a subset of the tracks at a given time using data chunks. Sample is calculated using pose[frameIndex] * (1 - frameDelta) + pose[frameIndex+1] * frameDelta.
		static void HK_CALL samplePartialWithDataChunks(hkUint32 frameIndex, hkReal frameDelta, 
														hkUint32 maxNumTransformTracks, hkQsTransform* transformTracksOut,
														hkUint32 maxNumFloatTracks, hkReal* floatTracksOut,
														const DataChunk* dataChunks, int numDataChunks);

		void getBlockDataAndSize(int blockNum, int numBlocks, DataChunk& dataChunkOut) const;

	public:
		
			/// The number of samples encoded in the animation.
		int m_numberOfPoses;
		
			/// The number of tracks in each encoded block
		int m_blockSize;
		
		// INTERNAL 
		struct QuantizationFormat
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaDeltaCompressedAnimation::QuantizationFormat );
			HK_DECLARE_REFLECTION();

			QuantizationFormat( ) { }
			QuantizationFormat( hkFinishLoadedObjectFlag flag ) {} 

			// Backwards compatibility only - pre-per-track compression.
			hkUint8 m_maxBitWidth;

			// Always 1 for delta since first float is stored exactly (for accurate reconstruction when applying inverse delta transform).
			hkUint8 m_preserved;

			// Number of dynamic tracks that are quantized and stored
			hkUint32 m_numD;

			// Index into the data buffer for the quantization offsets
			hkUint32 m_offsetIdx;

			/// Index into the data buffer for the quantization scales
			hkUint32 m_scaleIdx;

			// Index into the data buffer for the quantization bidwidths
			hkUint32 m_bitWidthIdx;
		};

		/// Quantization Description
		struct QuantizationFormat m_qFormat;

		/// Index into the data buffer for the Quantization Data
		hkUint32 m_quantizedDataIdx;

		/// Size of the Quantization Data (stored as hkUint8)
		hkUint32 m_quantizedDataSize;


		/// Index into the data buffer for the Track Mask
		hkUint32 m_staticMaskIdx;

		/// Size of the Track Mask (stored as hkUint16)
		hkUint32 m_staticMaskSize;


		/// Index into the data buffer for the Static DOF Data
		hkUint32 m_staticDOFsIdx;

		/// Size of the Static DOF Data (stored as hkReal)
		hkUint32 m_staticDOFsSize;

		/// Number Static Transform DOFs
		hkUint32 m_numStaticTransformDOFs;

		/// Number Dynamic Transform DOFs
		hkUint32 m_numDynamicTransformDOFs;

		/// Total size of each block in bytes (except for the last block, which can be smaller)
		/// Multiply by the block number to get the index into that block
		hkUint32 m_totalBlockSize;

		/// Total size of the last block (in bytes)
		/// Store this so that we don't have to compute it on the fly
		hkUint32 m_lastBlockSize;

		/// The data buffer where compressed and static data is kept
		hkArray< hkUint8 > m_dataBuffer;

	public:
	
			// Constructor for initialisation of vtable fixup
		HK_FORCE_INLINE hkaDeltaCompressedAnimation( hkFinishLoadedObjectFlag flag ) : hkaAnimation(flag), m_dataBuffer(flag)
			{ if (flag.m_finishing) handleEndian(); }

	private:
			/// Initialize the animation with construction info
		void initialize(const hkaInterleavedUncompressedAnimation& rawData, const PerTrackCompressionParams& params, hkBool useThreeComponentQuaternions );

			/// Swap the endianness in the data buffer as appropriate
		void handleEndian();
};



#endif // HKANIMATION_ANIMATION_DELTACOMPRESSED_HKDELTACOMPRESSEDANIMATION_XML_H

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
