/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SKELETON_UTILS_H
#define HK_SKELETON_UTILS_H

/// A user definable function useful for filtering strings before a name-matching operations.
/// This is easily implemented to remove/replace prefixes, replace characters, etc.
/// An implementation should return 0 if the strings should match
typedef int (HK_CALL* hkStringCompareFunc)(const char*, const char*);

class hkaSkeleton;
class hkAabb;

/// This utility class contains many static methods related to the manipulation of poses and skeletons
class hkaSkeletonUtils
{
	public:
			/// Converts a local space pose to a pose in model space
			/// This routine is alias safe if the indices are parent first order i.e. (parentIndices[i] < i)
		static void HK_CALL transformLocalPoseToModelPose ( int numTransforms, const hkInt16* parentIndices, const hkQsTransform* poseLocal, hkQsTransform* poseModelOut);

			/// Converts a bone transform from a local space to model space.
		static void HK_CALL transformLocalBoneToModelBone( int boneIndex, const hkInt16* parentIndices, const hkQsTransform* poseLocal, hkQsTransform& boneModelOut );

			/// Converts a model space pose to a pose in local space
			/// This routine is not alias safe.
		static void HK_CALL transformModelPoseToLocalPose ( int numTransforms, const hkInt16* parentIndices, const hkQsTransform* poseModel, hkQsTransform* poseLocalOut);

			/// Converts a model space pose to a pose in world space
			/// This simply multiplies through the model pose by the reference frame.
			/// The worldFromModel transform gives the reference frame in world space.
		static void HK_CALL transformModelPoseToWorldPose ( int numTransforms, const hkQsTransform& worldFromModel, const hkQsTransform* poseModel, hkQsTransform* poseWorldOut);

			/// Converts a world space pose to a pose in model space
			/// This simply multiplies through the world pose by the inverse of the reference frame.
			/// The worldFromModel transform gives the reference frame in world space.
		static void HK_CALL transformWorldPoseToModelPose ( int numTransforms, const hkQsTransform& worldFromModel, const hkQsTransform* poseWorld, hkQsTransform* poseModelOut);

			/// Converts a local space pose to a pose in world space
			/// This first converts to model space and then to world space using the reference frame
			/// The worldFromModel transform gives the reference frame in world space.
			/// This routine is alias safe if the indices are parent first order i.e. (parentIndices[i] < i)
		static void HK_CALL transformLocalPoseToWorldPose ( int numTransforms, const hkInt16* parentIndices, const hkQsTransform& worldFromModel, const hkQsTransform* poseLocal, hkQsTransform* poseWorldOut);

			/// Converts a local space pose to a pose in world space
			/// This first converts to model space and then to local space using the reference frame
			/// The worldFromModel transform gives the reference frame in world space.
			/// This routine is not alias safe.
		static void HK_CALL transformWorldPoseToLocalPose ( int numTransforms, const hkInt16* parentIndices, const hkQsTransform& worldFromModel, const hkQsTransform* poseWorld, hkQsTransform* poseLocalOut);

			/// Blends two (usually local) poses using per-bone weight array (0.0 = A, 1.0 = B)
		static void HK_CALL blendPoses( hkUint32 numBones, const hkQsTransform* poseA, const hkQsTransform* poseB, const hkReal* weights, hkQsTransform* poseOut );

			/// Blends two (usually local) poses using a single weight, where 0.0 = A and 1.0 = B
		static void HK_CALL blendPoses( hkUint32 numBones, const hkQsTransform* poseA, const hkQsTransform* poseB, const hkReal weight, hkQsTransform* poseOut );

			/// Blends two bone chain (usually local) poses using a single weight, where 0.0 = A and 1.0 = B
		static void HK_CALL blendPartialPoses( hkUint32 numBones, hkInt16* bones, const hkQsTransform* poseA, const hkQsTransform* poseB, const hkReal weight, hkQsTransform* poseOut );

			/// Given a local space pose, it will ensure that any bone constraint in the skeleton are enforced
			/// Currently the only bone constraint is "lockTranslation"
			/// This ensures the pose translations match the translations in the skeleton reference pose.
		static void HK_CALL enforceSkeletonConstraintsLocalSpace(const hkaSkeleton& skeleton, hkQsTransform* poseLocalInOut);

			/// Given a model space pose, it will ensure that any bone constraint in the skeleton are enforced
			/// Currently the only bone constraint is "lockTranslation"
			/// This ensures the pose translations match the translations in the skeleton reference pose.
		static void HK_CALL enforceSkeletonConstraintsModelSpace(const hkaSkeleton& skeleton, hkQsTransform* poseModelInOut);

			/// Given a model space pose, it will ensure that any bone constraint in the skeleton are enforced
			/// Currently the only bone constraint is "lockTranslation"
			/// This ensures the pose translations match the translations in the constraint pose (given in local space).
		static void HK_CALL enforcePoseConstraintsLocalSpace (const hkaSkeleton& skeleton, const hkQsTransform* constraintsLocal, hkQsTransform* poseInOut);

			/// Given a model space pose, it will ensure that any bone constraint in the skeleton are enforced
			/// Currently the only bone constraint is "lockTranslation"
			/// This ensures the pose translations match the translations in the constraint pose (given in local space).
		static void HK_CALL enforcePoseConstraintsModelSpace (const hkaSkeleton& skeleton, const hkQsTransform* constraintsLocal, hkQsTransform* poseModelInOut);

			/// Looks for a bone with the specified name (possibly filtering it). Returns -1 if not found, or the bone ID if found
		static hkInt16 HK_CALL findBoneWithName (const hkaSkeleton& skeleton, const char* name, hkStringCompareFunc compare = HK_NULL);

			/// Locks the translation of all bones in the given skeleton (except roots)
		static void HK_CALL lockTranslations (hkaSkeleton& skeleton, hkBool exceptRoots = true);

			/// Unlocks all the translations for the given skeleton
		static void HK_CALL unlockTranslations (hkaSkeleton& skeleton);

			/// Fills bonesOut with the chain between start bone and end bone (where end bone is deeper in the hierarchy).
			/// Returns false if there is no possible chain.
			/// This method possibly allocates space in the array (since its length is unknown by the caller).
		static hkBool HK_CALL getBoneChain(const hkaSkeleton& skeleton, hkInt16 startBone, hkInt16 endBone, hkArray<hkInt16>& bonesOut);

			/// Fills bonesOut with the id of all bones descending from the given bone
			/// This methods possibly allocates space in the array (since its length is unknown by the caller)
		static void HK_CALL getDescendants(const hkaSkeleton& skeleton, hkInt16 startBone, hkArray<hkInt16>& bonesOut, bool includeStart = false );


			/// Given an array of boolean skeleton->m_numBones big, the array will be marked true for all bones which are
			/// descendants of the start bone and false otherwise.
		static void HK_CALL markDescendants( const hkaSkeleton* skeleton, int startBone, bool* out, bool includeStart = false );

		/// Fills bonesOut with the id of all bones in the given skeleton with no children
		static void HK_CALL getLeaves(const hkaSkeleton& skeleton, hkArray<hkInt16>& leavesOut);

			/// This method checks the mapping in the binding and returns true if all bone indices are valid for this skeleton
		static hkBool HK_CALL isBindingOk( const hkaSkeleton& skeleton, const class hkaAnimationBinding& binding );

			/// Given a local pose, gets the scale in model space of the associated bone
		static void HK_CALL getModelSpaceScale (const hkaSkeleton& skeleton, const hkQsTransform* poseLocal, int boneId, hkVector4& scaleOut);

			/// This method iterates through the array and normalizes the quaternions in the array of hkQsTransforms
		static void HK_CALL normalizeRotations(hkQsTransform* transformsInOut, int numTransforms );

			/// This method uses the bone names from a skeleton and animation track names from an animation
			/// to fill in the animation track to bone mapping for an animation binding.
			/// A string comparison routine is passed which allows you to strip prefixes etc. the strings are passed in the order (bone name , track name)
			/// The m_transformTrackToBoneIndices array must be appropriately initialized
			/// (remember to set m_numTransformTrackToBoneIndices and allocate memory)
		static hkResult HK_CALL bindByName(const hkaSkeleton& skeleton, const class hkaAnimation& anim, hkStringCompareFunc compare, class hkaAnimationBinding& bindingOut);

			/// Calculate the AABB containing the given pose in worldspace
		static void HK_CALL calcAabb(hkUint32 numBones, const hkQsTransform* poseLocal, const hkInt16* parentIndices, const hkQsTransform& worldFromModel, hkAabb& aabbOut );
};


#endif // HK_SKELETON_UTILS_H

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
