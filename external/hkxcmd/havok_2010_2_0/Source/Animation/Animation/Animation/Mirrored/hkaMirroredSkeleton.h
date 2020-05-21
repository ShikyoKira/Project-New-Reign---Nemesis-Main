/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKANIMATION_ANIMATION_HKMIRROREDSKELETON_H
#define HKANIMATION_ANIMATION_HKMIRROREDSKELETON_H

#include <Common/Base/Container/StringMap/hkStringMap.h>
#include <Animation/Animation/Animation/hkaAnimation.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>

class hkaSkeleton;

	/// This is a helper class for the hkaMirroredAnimation which encapsulates the 'mirroring' functionality
	/// and works at a per-bone level (rather than the per-track level of a hkaAnimation).
class hkaMirroredSkeleton : public hkReferencedObject
{

	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RIG );

		/// Constructor for hkaMirroredAnimation
		/// \param skeleton Rig hierarchy
		hkaMirroredSkeleton( const hkaSkeleton *skeleton );

		/// Destructor
		~hkaMirroredSkeleton();

		// MIRRORING
		
		/// Replaces qInOut with its mirrored equivalent for non-additive animations
		void mirrorRegularBone( hkQsTransform &qInOut, int bone ) const;

		/// Replaces qInOut with its mirrored equivalent for additive animations
		void mirrorAdditiveBone( hkQsTransform &qInOut, int bone ) const;

	 	/// Replaces qInOut with its mirrored equivalent for extracted motion
		void mirrorExtractedMotion( hkQsTransform &qInOut ) const;

	 	/// Replaces qInOut with its mirrored equivalent
		void mirrorPairedBone( hkQsTransform &qInOut, int bone, bool isAdditive ) const;


		// MIRRORING SETUP

		/// Given a worldAxis (typically X=[1,0,0,0] Y=[0,1,0,0]
		/// Z=[0,0,1,0]) and an initially symmetric reference pose,
		/// compute all necessary mirroring setup. Note that bone
		/// pairing setup must be complete before this function
		/// should be called.
		void setAllBoneInvariantsFromReferencePose( const hkQuaternion& worldAxis, hkReal tolerance );

		/// Given a worldAxis (typically X=[1,0,0,0] Y=[0,1,0,0]
		/// Z=[0,0,1,0]) and an initially symmetric pose, compute all
		/// necessary mirroring setup. Note that bone pairing setup
		/// must be complete before this function should be called.
		void setAllBoneInvariantsFromSymmetricPose( const hkQuaternion& worldAxis, hkReal tolerance, const class hkaPose & );

		/// Given a worldAxis (typically X=[1,0,0,0] Y=[0,1,0,0]
		/// Z=[0,0,1,0]) and an initially symmetric pose, compute all
		/// necessary mirroring setup. Note that bone pairing setup
		/// must be complete before this function should be called.
		void setAllBoneInvariantsFromSymmetricPose( const hkQuaternion& worldAxis, hkReal tolerance, const hkQsTransform *poseModelSpace );

		/// Low-level access to the bone invariants
		const hkQuaternion& getBoneInvariant( int bone ) const;

		/// Low-level access to the bone invariants. Typically computed
		/// by calling one of the setAllBoneInvariants*() member
		/// functions on a symmetric example pose.
		void setBoneInvariant( int bone, const hkQuaternion& axis );


		// BONE PAIRING

		/// Allows the user to set bone pairing information
		/// \param bi Bone to pair with bj
		/// \param bj Bone to pair with bi
		void setBonePair( hkInt16 bi, hkInt16 bj );

		/// Allows the user to set all of the bone pairing information at once
		/// \param bonePairMap The mapping of bones to their mirrored partners
		void setBonePairMap( const hkInt16* bonePairMap, int numBonePairMap );

		/// Allows the user to query bone pairing information
		/// \return The paired bone for bi
		/// \param bi Bone to query pair information for
		hkInt16 getBonePair( hkInt16 bi ) const;


		// ANNOTATION PAIRING

		/// Set annotation pair
		/// \param ai Annotation to pair with aj
		/// \param aj Annotation to pair with AI
		void setAnnotationPair( hkaAnnotationTrack::Annotation* ai, hkaAnnotationTrack::Annotation* aj );

		/// Set annotation name pair, strings must persist throughout the scope of this class
		/// \param ai Name of annotation to pair with aj
		/// \param aj Name of annotation to pair with AI
		void setAnnotationNamePair( const char* namei, const char* namej );

		/// Allows the user to query annotation pairing information
		/// \return The paired annotation
		const char* getAnnotationNamePair( const char* ) const;

		/// Clears all existing annotation pairs
		void clearAllAnnotationPairs();

		/// Computes bone pairing information from unique substrings of bone names
		/// \param ltag Array of strings uniquely identifying left bones
		/// \param ltag Array of strings uniquely identifying right bones
		void computeBonePairingFromNames( const hkArrayBase< hkStringPtr >& ltag, const hkArrayBase< hkStringPtr >& rtag );

		/// Computes bone pairing information from unique substrings of bone names
		/// \param ltag Array of strings uniquely identifying left bones
		/// \param ltag Array of strings uniquely identifying right bones
		/// \param skeleton The skeleton that is being mirrored
		/// \param bonePairMap The buffer for the resultant bone map
		static void HK_CALL computeBonePairingFromNames( 
			const hkArrayBase< hkStringPtr >& ltag, 
			const hkArrayBase< hkStringPtr >& rtag,
			const hkaSkeleton* skeleton,
			hkInt16* bonePairMap );


		// NOT FOR USER USE Accessors for static method hkaMirroredAnimation::samplePartialWithDataChunks
		static int HK_CALL getNumDataChunks();
		void getDataChunks( hkaAnimation::DataChunk* chunksOut ) const;
		static const hkaMirroredSkeleton* HK_CALL convertDataChunks( hkaAnimation::DataChunk* chunksInOut );


		// Static computation and accessors for non-shared memory architectures
		static void HK_CALL mirrorBonesNormal( hkQsTransform* qInOut, const hkInt16* parentIndices, const hkQuaternion* invariants, const hkInt16* bonePairMap, int numBones );
		static void HK_CALL mirrorBonesAdditive( hkQsTransform* qInOut, const hkInt16* parentIndices, const hkQuaternion* invariants, const hkInt16* bonePairMap, int numBones );
		const hkInt16* getBonePairMap() const;
		const hkQuaternion* getInvariants() const;
	

	private:

			/// Returns the parent bone index
		int getBoneParent( int bone ) const;

			/// Sets all bone axes to canonical (multiples of pi/2) rotations
   		void canonicalize( hkQuaternion &qInOut, hkReal tolerance );

			/// Swap bone transforms with their pair
		static void HK_CALL inplaceSwap( hkQsTransform* qInOut, const hkInt16* bonePairMap, int numBones );

			/// Bone pairing information
		hkInt16* m_bonePairMap;

			/// Returns the number of bones
		int numBones() const;


			/// Skeleton, used to get the parent bone information
		const hkaSkeleton *m_skeleton;

			/// Parent indices from the original skeleton
			/// This pointer points to the values in the original skeleton.
			/// No new memory is allocated (motivated by data chunks).
		const hkInt16 *m_parentIndices; // = m_skeleton->m_parentIndices.begin()

			/// Number of bones from the original skeleton
		hkInt32 m_numBones; // = m_skeleton->m_bones.getSize()

			/// Rotations for each bone
		hkQuaternion* m_boneInvariants;

			/// Map of mirrored annotation names
		hkStringMap< const char* > m_annotationNameMap;
};

#endif // HKANIMATION_ANIMATION_HKMIRROREDSKELETON_H

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
