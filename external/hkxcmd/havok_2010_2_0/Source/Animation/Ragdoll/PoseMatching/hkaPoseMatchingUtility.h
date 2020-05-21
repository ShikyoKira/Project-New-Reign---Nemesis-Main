/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_POSE_MATCHING_UTILITY_H
#define HK_POSE_MATCHING_UTILITY_H

class hkaAnimationBinding;

/// This utility class acts as a database of poses.
/// It can be searched efficiently to find as close a match as possible to any given pose.
/// This is particularly useful when trying to find an appropriate get up pose for a ragdoll.
class hkaPoseMatchingUtility
{

public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ANIMATION, hkaPoseMatchingUtility);

		/// Initialize the utility.
		/// The three bone indices here for the frame of reference when matching poses
		/// When matching rotation around the up axis is ignored.
	hkaPoseMatchingUtility( hkInt16 rootBoneIdx, hkInt16 otherBoneIdx, hkInt16 anotherBoneIdx, const hkVector4& upAxisMS );


		/// Find the best match for the input pose
		/// returns the index of the match in the array or -1 if no match found
		/// err gives and indication of how close this match is
	int findBestCandidatePoseIndex( const hkQsTransform* poseModelSpace , hkReal& err ) const;

		/// Add this pose as a getup pose
		/// The pose is sampled for a reference frame and the binding and time are stored with it
		/// The storage space for each pose is very small (quaternion + pointer + real)
		/// Returns the index associated with the newly added pose
	int addCandidatePose(const hkQsTransform* poseModelSpace, const hkaAnimationBinding* binding, hkReal time);

		/// Compute the world-from-model transform for a ragdoll in worldspace to best match a given pose in model space.
		/// The resultant ragdollWorldFromModel is the transform that when applied to the animation pose, aligns it with the ragdoll.
		/// The resultant animWorldFromModel is the same transform but with the rotation restricted to only be
		/// around the given axis (typically Y).
	void computeReferenceFrame(const hkQsTransform* animPoseModelSpace, const hkQsTransform* ragdollPoseWorldSpace, hkQsTransform& animWorldFromModel, hkQsTransform& ragdollWorldFromModel) const;

	struct CandidatePoseInfo
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaPoseMatchingUtility::CandidatePoseInfo );

		hkQuaternion m_reference;				// The key used to match on
		const hkaAnimationBinding* m_binding;	// A copy of the binding for the original animation
		hkReal m_time;							// The local time the animation was sampled at
	};

		/// A set of potential poses
	hkArray< CandidatePoseInfo > m_candidatePoses;
private:

	void computeOrientation(const hkQsTransform* poseModelSpace, hkQuaternion& out) const;

		// Rotation around this axis is always allowed. (Usually set to up)
	hkVector4 m_rotationAxis;

	hkInt16 m_rootBoneIdx;	  // Bone to match
	hkInt16 m_otherBoneIdx;	  // Main body axis
	hkInt16 m_anotherBoneIdx; // Another axis

};

#endif // HK_POSE_MATCHING_UTILITY_H

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
