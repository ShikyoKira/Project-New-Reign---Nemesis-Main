/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SKELETON_MAPPER_UTILS_H
#define HK_SKELETON_MAPPER_UTILS_H

class hkaSkeleton;

#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapperData.h>

/// This utility class matches two skeletons (A and B) using their reference pose in world and the (filtered) names of their bones
/// and, through the "createMapping" static method, creates the mapping data that can be used at run-time (by an hkaSkeletonMapper) to
/// map poses from A into B, and vice-versa.
/// One-to-one mappings and chains are autodetected, but can also be specified by the user
class hkaSkeletonMapperUtils
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaSkeletonMapperUtils);
			/// A chain, specified by the names of the start and end bones - used in Params::m_userChains.
			/// These names will be matched with the bone names (after filtering the names) of both skeletons.
		struct UserChain
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaSkeletonMapperUtils::UserChain );

				/// Start name.
			const char* m_start;
				/// End name.
			const char* m_end;
		};

			/// A explicit mapping (one-way only) - used in Params::m_userMappingsAtoB and Params::m_userMappingsBtoA.
			/// The names will be matched with the bones names (after filtering the names) of the skeleton (A or B).
		struct UserMapping
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaSkeletonMapperUtils::UserMapping );

				/// Bone in name.
			const char* m_boneIn;
				/// Bone out name.
			const char* m_boneOut;
		};

			/// This struct contains extracted motion parameters for animation mapping
		class ExtractedMotionParams
		{

		public:

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaSkeletonMapperUtils::ExtractedMotionParams );

			enum ExtractedMotionMode
			{
				HK_USE_BOUNDING_BOX_DIAGONAL = 0,
				HK_USE_BONE_LENGTHS = 1,
				HK_USE_FACTORS_DIRECTLY = 2
			};

			ExtractedMotionParams() :
				m_mode( HK_USE_BOUNDING_BOX_DIAGONAL ),
				m_extractedMotionBoneA( 0 ),
				m_extractedMotionBoneB( 0 ),
				m_extractedMotionFactorA( 1.0f ),
				m_extractedMotionFactorB( 1.0f )
			{
			}

				/// Computes appropriate transforms for extracted motion based on the settings
			void HK_CALL computeExtractedMotionTransforms( hkaSkeletonMapperData::MappingType type, const hkQsTransform* poseAMS, hkInt32 numBonesA, const hkQsTransform* poseBMS, hkInt32 numBonesB, hkQsTransform& aMotionOut, hkQsTransform& bMotionOut );


				/// Utility function to measure the bounding box of a character
			static void HK_CALL getPoseMinMax( const hkQsTransform* poseModelSpace, int numBones, hkVector4& minOut, hkVector4& maxOut );

				/// Mode to use for measuring the ratio of extracted motions
				/// Using character bounding box,
				/// Using specified bone lengths,
				/// Using specified numbers
			hkEnum< hkaSkeletonMapperUtils::ExtractedMotionParams::ExtractedMotionMode, hkInt32 > m_mode;

				/// Bone to measure for extracted motion
			hkInt32 m_extractedMotionBoneA;
			hkInt32 m_extractedMotionBoneB;

				/// Additional user-specified factor to scale
				/// extracted motion. Applied to either a bone
				/// length or bounding box calculation. (see
				/// m_extractedMotionBone)
			hkReal m_extractedMotionFactorA;
			hkReal m_extractedMotionFactorB;
		};

			/// These struct contains all the information used by "createMapping" in order to construct mapping data
		struct Params
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaSkeletonMapperUtils::Params );

				/// The first of the two skeletons involved
			const hkaSkeleton* m_skeletonA;
				/// The second of the two skeletons involved
			const hkaSkeleton* m_skeletonB;

				/// These filters are used to match names of bones from each skeleton
				/// It will use case insensitive compare if NULL and not using positional tolerance
			hkStringCompareFunc m_compareNames;

				/// If no string compare function is given and this tolerance is non zero,
				/// positions are used to match the bones in model space (with this tol)
				/// In retargeting mode, this value defines the length below which bones
				/// are considered to have zero length
			hkReal m_positionMatchTolerance;

				/// Used in retargeting mode only.
				/// Specifies the angle mismatch (in RADIANS) above which users are warned
				/// when mapping bones
			hkReal m_rotationMatchTolerance;
			
				/// Users can specify specific chains they want to enforce
			hkArray<UserChain> m_userChains;

				/// You can also force specific one-to-one mappings between bones.
				/// For example, you can map a "twist forearm" in A to a "forearm" in B.
			hkArray<UserMapping> m_userMappingsAtoB;

				/// You can also force specific one-to-one mappings between bones.
				/// For example, you can map a "twist forearm" in B to a "forearm" in A.
			hkArray<UserMapping> m_userMappingsBtoA;

				/// If true, the system will auto detect simple mappings
				/// This is normally desired unless you have completely specified
				/// your mapping using user maps.
			hkBool m_autodetectSimple; 

				/// If true, the system will try to autodetect chains by looking at unmapped bones
				/// located in between mapped bones. The chain has to be of length >=3 in at least one of the skeletons.
			hkBool m_autodetectChains;	

				/// The type of mapper... model space for ragdolls, or local space for animations
			hkEnum< hkaSkeletonMapperData::MappingType, hkInt32 > m_mappingType;

				/// Params for extracted motion
			ExtractedMotionParams m_motionParams;

				/// Copy constructor
			Params ( const Params& other) : 
				m_skeletonA(other.m_skeletonA), m_skeletonB(other.m_skeletonB), 
				m_compareNames(other.m_compareNames), 
				m_positionMatchTolerance (other.m_positionMatchTolerance),
				m_rotationMatchTolerance (other.m_rotationMatchTolerance),
				m_autodetectSimple (other.m_autodetectSimple), m_autodetectChains (other.m_autodetectChains),
				m_mappingType(other.m_mappingType),
				m_motionParams(other.m_motionParams)
				{
					m_userChains = other.m_userChains;
					m_userMappingsAtoB = other.m_userMappingsAtoB;
					m_userMappingsBtoA = other.m_userMappingsBtoA;
				}

			Params () :
				m_skeletonA(HK_NULL), m_skeletonB(HK_NULL), m_compareNames(HK_NULL), 
				m_positionMatchTolerance(0.0f), m_rotationMatchTolerance( 30.0f * HK_REAL_DEG_TO_RAD ),
				m_autodetectSimple(true), m_autodetectChains(true),
				m_mappingType(hkaSkeletonMapperData::HK_RAGDOLL_MAPPING)
			{}
		};


			/// Creates the mapping data (used by hkaSkeletonMapper) used to map poses from skeleton A to skeleton B
			/// and vice-versa.
		static void HK_CALL createMapping (const Params& params, hkaSkeletonMapperData& aToB, hkaSkeletonMapperData& bToA);

			/// Searches and returns a pointer to the simple mapping associated with the given bone (of skeleton A) inside
			/// the given mapping data.
		static const struct hkaSkeletonMapperData::SimpleMapping* HK_CALL findSimpleMapping(hkInt16 boneA, const hkaSkeletonMapperData& mapperdata );

			/// Given mapping data between two skeletons, it automatically locks translations on the appropriate bones of both skeletons.
			/// The bones at the top-most simple mapping (and their ancestors) are left unlocked - the rest of bones are locked.
		static hkResult HK_CALL lockTranslationsAutomatically (const hkaSkeletonMapperData& mapperData);

			/// Given a skeleton mapping, scales the translations of all transforms.
			/// Useful when scaling ragdolls or animations
		static void HK_CALL scaleMapping( hkaSkeletonMapper& mapper, hkReal scale );


	private:

			// Utilities used to compute mapping transforms

		static void HK_CALL computeMappingTransform( hkaSkeletonMapperData::MappingType type, const hkQsTransform* poseA, hkInt16 boneA, const hkQsTransform* poseB, hkInt16 boneB, hkQsTransform& mapOut, hkReal tolerance = 1.0e-5f, hkReal defaultScale = 1.0f );

		static void HK_CALL computeMappingTransform( hkaSkeletonMapperData::MappingType type, const hkQsTransform& a, const hkQsTransform& b, hkQsTransform& mapOut, hkReal tolerance, hkReal defaultScale );

		static void HK_CALL computeChainMapping( hkaSkeletonMapperData::MappingType type, const hkaSkeleton* skeletonA, const hkQsTransform* poseA, hkInt16 startA, hkInt16 endA, const hkaSkeleton* skeletonB, const hkQsTransform* poseB, hkInt16 startB, hkInt16 endB, hkaSkeletonMapperData::ChainMapping& mapAFromBOut );

		static void HK_CALL computeChainLengthAndPosition( const hkaSkeleton* skeleton, const hkQsTransform* pose, hkInt16 start, hkInt16 end, hkReal& lengthOut, hkVector4& posOut );

		static bool HK_CALL checkRotation( const Params& params, hkInt16 boneA, hkInt16 boneB, const hkQsTransform& aToBTransform, const hkQsTransform& bToATransform );
};



#endif // HK_SKELETON_MAPPER_UTILS_H

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
