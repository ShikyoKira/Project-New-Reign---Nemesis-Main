/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TRACK_ANALYSIS_H
#define HK_TRACK_ANALYSIS_H

#include <Animation/Animation/Animation/hkaAnimation.h>

// INTERNAL
class hkaTrackAnalysis
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaTrackAnalysis);
			/// The input structure for analysing animation tracks
			/// Tolerances are conservative, i.e., if both absolute and relative
			/// tolerances are specified then the position must be within both
		struct AnalysisInput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaTrackAnalysis::AnalysisInput );

			const hkQsTransform* m_boneData;
			hkUint32 m_numberOfTransformTracks;
			hkUint32 m_numberOfPoses;

			hkReal m_absolutePositionTolerance;	// Set to 0 to use only relative tolerance
			hkReal m_relativePositionTolerance; // Set to 0 to use only abs tolerance
			hkReal m_rotationTolerance;
			hkReal m_scaleTolerance;
			hkBool m_useThreeComponentQuaternions;
		};

		struct PerTransformTrackAnalysisInput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaTrackAnalysis::PerTransformTrackAnalysisInput );

			PerTransformTrackAnalysisInput()
				: m_boneData(HK_NULL), m_numberOfTransformTracks(0), m_numberOfPoses(0)
			{}
			

			const hkQsTransform* m_boneData;
			hkUint32 m_numberOfTransformTracks;
			hkUint32 m_numberOfPoses;

			hkArray<hkReal> m_absolutePositionTolerance;	// Set to 0 to use only relative tolerance
			hkArray<hkReal> m_relativePositionTolerance; // Set to 0 to use only abs tolerance
			hkArray<hkReal> m_rotationTolerance;
			hkArray<hkReal> m_scaleTolerance;
			hkArray<hkBool> m_useThreeComponentQuaternions;
		};

		struct PerFloatTrackAnalysisInput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaTrackAnalysis::PerFloatTrackAnalysisInput );

			PerFloatTrackAnalysisInput()
				: m_floatData(HK_NULL), m_numberOfFloatTracks(0), m_numberOfPoses(0)
			{}

			const hkReal*	m_floatData;
			hkUint32		m_numberOfFloatTracks;
			hkUint32		m_numberOfPoses;

			hkArray<hkReal> m_absoluteTolerance;
		};
		
		enum TrackType 
		{
			HK_TRACK_DYNAMIC = 0x00, // This track should use the dynamic value
			HK_TRACK_STATIC	 = 0x01, // This track should use the static value
			HK_TRACK_CLEAR   = 0x02	 // This track can be cleared / set to the identity
		};

			/// This method analyzes the input animation to find and
			/// record static degrees of freedom in the animation.
			/// The method fills data into the arrays staticMask and staticDofs.
			/// Both must be preallocated.
			///
			/// The staticMask array must be m_numberOfTransformTracks long, 16 bits of mask
			/// info for each transform track as follows:
			///     - Bits 0..1 = Track Type for Position
			///     - Bits 2..3 = Track Type for Rotation
			///     - Bits 4..5 = Track Type for Scale
			///     - Bits 6..8 = Z,Y,X flags for dynamic positions
			///     - Bits 9..12 = W,Z,Y,X flags for dynamic rotations
			///     - Bits 13..15 = Z,Y,X flags for dynamic scales
			///
			/// The staticDofs array should be 10*m_numberOfTransformTracks long as
			/// it may need to store 10 static values for each transform (of all 10 DOFs are static for
			/// every track).
			///
			/// Sub-tracks (Position/Rotation/Scale) are considered Clear if *all* components are clear,
			/// Static if *all* are static, and Dynamic if *any* are dynamic.
			/// Individual DOFs can be either static (0) or dynamic (1).
		static void HK_CALL analyze( const AnalysisInput& input, hkUint16* staticMask, hkReal* staticDOFs );

			/// This version of the analysis function takes per-track settings, both for transform tracks and float tracks.
		static void HK_CALL analyze( const PerTransformTrackAnalysisInput& input, const PerFloatTrackAnalysisInput& finput, hkUint16* staticMask, hkReal* staticDOFs, hkBool displayStatsReport = true);

			/// An output structure used to gather statistics on an analysed track mask
		struct AnalysisStats
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaTrackAnalysis::AnalysisStats );

			AnalysisStats()
				:	m_staticPosDOF(0),
					m_staticRotDOF(0),
					m_staticSclDOF(0),
					m_staticFloatDOF(0),
					m_clearPosDOF(0),
					m_clearRotDOF(0),
					m_clearSclDOF(0),
					m_dynamicPosDOF(0),
					m_dynamicRotDOF(0),
					m_dynamicSclDOF(0),
					m_dynamicFloatDOF(0),
					m_numTransformDOFS(0),
					m_numFloatDOFS(0),
					m_ratio(0.0f) 
			{
			}

			void displayReport();

			// Number of static tracks
			hkUint32 m_staticPosDOF;
			hkUint32 m_staticRotDOF;
			hkUint32 m_staticSclDOF;

			hkUint32 m_staticFloatDOF;

			// Number of clear tracks
			hkUint32 m_clearPosDOF;
			hkUint32 m_clearRotDOF;
			hkUint32 m_clearSclDOF;

			// Number of dynamic degrees of freedom
			hkUint32 m_dynamicPosDOF;
			hkUint32 m_dynamicRotDOF;
			hkUint32 m_dynamicSclDOF;

			hkUint32 m_dynamicFloatDOF;

			hkUint32 m_numTransformDOFS;
			hkUint32 m_numFloatDOFS;

			// Ratio of total tracks to dynamic tracks
			hkReal	 m_ratio; 
		};

			/// This method computes the statistics in the AnalysisStats struct for a given static mask.
		static void HK_CALL calcStats(const hkUint16* staticMaskT, const hkUint16* staticMaskF, hkUint32 numTracksT, hkUint32 numTracksF, AnalysisStats& out);

			/// This method computes information for the dynamic tracks
		static void HK_CALL calcDynamicStats(const hkUint16* staticMaskT, const hkUint16* staticMaskF, hkUint32 numTracksT, hkUint32 numFTracksF, AnalysisStats& out);


		struct AnalysisSplit
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaTrackAnalysis::AnalysisSplit );

			AnalysisSplit()
				:	m_data(HK_NULL), m_numberOfTransformTracks(0), m_floatData(HK_NULL), m_numberOfFloatTracks(0),
					m_numberOfPoses(0), m_channelAlign(0), m_staticMask(HK_NULL)
			{}

			const hkQsTransform* m_data;
			hkUint32 m_numberOfTransformTracks;
			const hkReal* m_floatData;
			hkUint32 m_numberOfFloatTracks;
			hkUint32 m_numberOfPoses;
			hkUint32 m_channelAlign;
			const hkUint16* m_staticMask;
		};

		static void HK_CALL channelSplit(const AnalysisSplit& input, hkReal* channelData);

		struct AnalysisCompose
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaTrackAnalysis::AnalysisCompose );

			hkUint32 m_numberOfTransformTracks;
			hkUint32 m_numberOfFloatTracks;
			const hkUint16* m_staticMaskTransform;
			const hkUint16* m_staticMaskFloat;
			const hkReal* m_staticDOFsTransform;
			const hkReal* m_staticDOFsFloat;
			const hkReal* m_dynamicDOFsTransform;
			const hkReal* m_dynamicDOFsFloat;
		};

		static void HK_CALL recompose(const AnalysisCompose& input, hkQsTransform* outputPose, hkReal* outputFloatTracks);


	private:
			/// This method computes the *qstransform only* statistics in the AnalysisStats struct for a given static mask,
		static void HK_CALL calcStatsQsTransform(const hkUint16* staticMask, hkUint32 numTracks, AnalysisStats& out);

			/// This method computes information for the *qstransform only* dynamic tracks
		static void HK_CALL calcDynamicStatsQsTransform(const hkUint16* staticMask, hkUint32 numTracks, AnalysisStats& out);

			/// This version of the analysis function takes per-track settings, for *qstransform only* tracks.
		static void HK_CALL analyzeQsTransform( const PerTransformTrackAnalysisInput& input, hkUint16* staticMask, hkReal* staticDOFs, hkBool displayStatsReport = true);

};


#endif // HK_TRACK_ANALYSIS_H

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
