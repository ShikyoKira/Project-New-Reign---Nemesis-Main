/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_HK_POSE_H
#define HK_HK_POSE_H

#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

	/// This class may be used to store and manipulate poses, representing the state of an animation sample for both the transform and float data (where present).
	/// For the transform (bone) data it uses a dual representation of the poses : LOCAL SPACE, where the transform of each bone is relative to the transform
	/// of its parent, and MODEL SPACE, where the transform of each bone is absolute (respect to a global model space)
	/// Both representations are kept in sync lazily, using the hkaSkeleton hierarchy.
	/// For the float data it simply has an array of 'float slot values' into which the float data can be sampled - the skeleton hierarchy has no effect here.
	/// hkaPose objects are associated with skeletons on construction and can therefore only hold poses based on that skeleton.
	/// N.B. There is no necessity to use a hkaPose in order to get the results of sampling (you may simply pass
	/// pointers to reserved blocks of data for the sampling to write into) but this class facilitates model/local space syncing via the hkaSkeleton
	/// associated with it.
	/// N.B. There is a requirement when using animation jobs for sampling on SPU that the float slot array
	/// be allocated to a size (number of bytes) which is a multiple of 16. The hkaPose class handles this automatically
	/// by setting the m_floatSlotValues array to have a *capacity* rounded up to a multiple of 4. If you do not use this class
	/// you may wish to use the helper method
	/// hkaMultithreadedAnimationUtils::allocateFloatSlotsArrayRoundedUpToMultipleOf16()
class hkaPose
{
public:
	
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaPose );

	/*
	** Construction
	*/

		/// This enum is used for the construction of an hkaPose instance described either in model or in local space
	enum PoseSpace
	{
		MODEL_SPACE,
		LOCAL_SPACE
	};
	
		/// Constructor (Note : it doesn't initialize the pose).
	HK_FORCE_INLINE hkaPose (const hkaSkeleton* skeleton);

		/// Constructor; uses the specified preallocated memory for internal arrays (Note : it doesn't initialize the pose).
		/// Use the static method getRequiredMemorySize() to determine the required size of this buffer.
	HK_FORCE_INLINE hkaPose (const hkaSkeleton* skeleton, void* preallocatedMemory);

		/// Returns the memory size needed to hold an hkaPose object based on the given skeleton.
		/// You can preallocate a buffer (possibly local) of this size and then use the hkaPose(skeleton, preallocatedMemory) constructor
		/// to create an hkaPose instance that doesn't dynamically allocate memory.
	HK_FORCE_INLINE static int getRequiredMemorySize (const hkaSkeleton* skeleton);

		/// Constructor; initializes the pose from an array of transforms in Local Space or Model Space (depending on the value of "space"). The float slots are set to 0.0.
	hkaPose (PoseSpace space, const hkaSkeleton* skeleton, const hkArrayBase<hkQsTransform>& pose);
	hkaPose (PoseSpace space, const hkaSkeleton* skeleton, const hkQsTransform* pose, int numBones);

	/*
	** Get/set/access methods
	*/

		/// This enum is passes to methods that change a bone in model space, specifying whether the change propagates or not,
		/// i.e., whether the children keep their local (propagate) or model (don't propagate) transform.
	enum PropagateOrNot
	{
		DONT_PROPAGATE = 0,
		PROPAGATE = 1
	};

		/// Returns the skeleton associated with this pose.
	HK_FORCE_INLINE const hkaSkeleton* getSkeleton() const;

		/// Returns a const-reference to an array representing the pose in Local Space.
	const hkArray<hkQsTransform>& getSyncedPoseLocalSpace() const;

		/// Returns a const-reference to an array representing the pose in Model Space.
	const hkArray<hkQsTransform>& getSyncedPoseModelSpace() const;

		/// Returns a const-reference to an array representing the current values in the float slots.
	HK_FORCE_INLINE const hkArray<hkReal>& getFloatSlotValues() const;

		/// Overwrites this pose with a new one described as an array of transforms in Local Space.
	void setPoseLocalSpace (const hkArrayBase<hkQsTransform>& poseLocal);
	
		/// Overwrites this pose with a new one described as an array of transforms in Model Space.
	void setPoseModelSpace (const hkArrayBase<hkQsTransform>& poseModel);

		/// Overwrites the values in the float slots.
	HK_FORCE_INLINE void setFloatSlotValues (const hkArrayBase<hkReal>& floatSlotValues);
	
		/// Retrieves the transform associated with one bone in Local Space.
		/// May synchronize ancestors in model space
	HK_FORCE_INLINE const hkQsTransform& getBoneLocalSpace (int boneIdx) const;

		/// Retrieves the transform associated with one bone in Model Space.
		/// May synchronize ancestors in model space
	HK_FORCE_INLINE const hkQsTransform& getBoneModelSpace (int boneIdx) const;

		/// Retrieves the value in a given float slot.
	HK_FORCE_INLINE hkReal getFloatSlotValue (int floatSlotIdx) const;

		/// Sets the transform of a particular bone in Local Space. The transform in Local Space of the descendants is kept (therefore their
		/// transform in Model Space is not).
	HK_FORCE_INLINE void setBoneLocalSpace (int boneIdx, const hkQsTransform& boneLocal);

		/// Sets the transform of a particular bone in Model Space. The transform in Model Space of the descendants is kept,
		/// unless propagateOrNot is set to PROPAGATE. In that case, the Local Space transform of the descendants is kept (and the Model Space transform is not)
	HK_FORCE_INLINE void setBoneModelSpace (int boneIdx, const hkQsTransform& boneModel, PropagateOrNot propagateOrNot);

			/// Sets the value in a given float slot.
	HK_FORCE_INLINE void setFloatSlotValue (int floatSlotIdx, hkReal value);

		/// Ensures all Local Space transforms stored are in sync (since they are evaluated lazily, they may not always be in sync). It is useful if the
		/// application is going to repeatedly access most of these Local Space transforms (without modifying them in Model Space)
	void syncLocalSpace () const;

		/// Ensures all Model Space transforms stored are in sync (since they are evaluated lazily, they may not always be in sync). It is useful if the
		/// application is going to repeatedly access most of these Model Space transforms (without modifying them in Local Space)
	void syncModelSpace () const;

		/// Synchronizes both representations (calls both syncLocalSpace and syncModelSpace).
	inline void syncAll () const;


	/*
	** Bone Pose Access methods, non-const : they sync one representation and dirty the other one. Use with caution.
	*/

		/// Returns a non-const reference to the bone in local space. This reference should only be used for a limited scope (it shouldn't be used
		/// once the hkaPose is further modified). Useful in order to modify individual components of the transform without creating a temporary object.
		/// Use this method with caution.
	hkQsTransform& accessBoneLocalSpace (int boneIdx);

		/// Returns a non-const reference to the bone in model space. This reference should only be used for a limited scope (it shouldn't be used
		/// once the hkaPose is further modified). Useful in order to modify individual components of the transform without creating a temporary object.
		/// Usage of propagateOrNot is as in setBoneModelSpace(). Use this method with caution.
	hkQsTransform& accessBoneModelSpace (int boneIdx, PropagateOrNot propagateOrNot = DONT_PROPAGATE);

		/// Returns a non-const reference to the pose as an array of transforms in Local Space. It fully syncs the Local Space representation of the pose,
		/// and fully invalidates the Model Space representation. This is useful in order to pass the pose to methods that only modify this Local Space representation.
		/// No other methods in hkaPose should be called until this reference goes out of scope or is no longer used.
	hkArray<hkQsTransform>& accessSyncedPoseLocalSpace();

		/// Returns a non-const reference to the pose as an array of transforms in Model Space. It fully syncs the Model Space representation of the pose,
		/// and fully invalidates the Local Space representation. This is useful in order to pass the pose to methods that only modify this Model Space representation.
		/// No other methods in hkaPose should be called until this reference goes out of scope or is no longer used.
	hkArray<hkQsTransform>& accessSyncedPoseModelSpace();

	/*
	** Write access methods, non-const: they don't sync the representation; the user (caller) is expected to ignore current values and set new values.
	*/

		/// Returns a non-const reference to the pose as an array of transforms in local space. The caller is expected to set all of these transforms.
		/// These values should be only written to, not read. No other methods
		/// in hkaPose should be called until this reference goes out of scope or is no longer used. Use this method with caution.
	hkArray<hkQsTransform>& accessUnsyncedPoseLocalSpace();

		/// Returns a non-const reference to the pose as an array of transforms in model space. The caller is expected to set all of these transforms.
		/// These values should only be written to, not read. No other methods
		/// in hkaPose should be called until this reference goes out of scope or is no longer used. Use this method with caution.
	hkArray<hkQsTransform>& accessUnsyncedPoseModelSpace();

		/// Returns a non-const reference to the float slot values as an array of floats.
	HK_FORCE_INLINE hkArray<hkReal>& getFloatSlotValues();

	/*
	** Utility methods
	*/

		/// Set the pose to be the reference pose (no reference pose data exits for the float slots, so they are set to 0.0.
	void setToReferencePose ();

		/// Ensure that any bone constraint in the skeleton is enforced. Currently the only bone constraint is "lockTranslation".
		/// This method operates in local space.
	void enforceSkeletonConstraintsLocalSpace ();

		/// Ensure that any bone constraint in the skeleton is enforced. Currently the only bone constraint is "lockTranslation".
		/// This method operates in model space.
	void enforceSkeletonConstraintsModelSpace ();

		/// Returns the AABB in model space that contains all bones positions in the pose.
	void getModelSpaceAabb (class hkAabb& aabbOut) const;

	/*
	** Operators
	*/

		/// Assignment operator.
	inline hkaPose& operator= ( const hkaPose& other );


private:

	// Private constructor helper
	void init(PoseSpace space, const hkaSkeleton* skeleton, const hkArrayBase<hkQsTransform>& pose);

	typedef hkUint32 hkaPoseFlag;

	const hkaSkeleton* m_skeleton;

	mutable hkArray<hkQsTransform> m_localPose;
	mutable hkArray<hkQsTransform> m_modelPose;
	mutable hkArray<hkaPoseFlag> m_boneFlags;

	mutable hkBool m_modelInSync;
	mutable hkBool m_localInSync;

	hkArray<hkReal> m_floatSlotValues;

	// Bone Flags
	enum 
	{
		F_BONE_LOCAL_DIRTY = 0x1,
		F_BONE_MODEL_DIRTY = 0x2,
		F_BONE_INTERNAL_FLAG1 = 0x4, // internally used (enforceConstraints, makeAllChildrenLocal)
		F_BONE_INTERNAL_FLAG2 = 0x8, // internally used (calculateBoneModelSpace)
		F_BONE_INTERNAL_FLAG3 = 0x10, // internally used
	};

	// Masks
	enum
	{
		M_BONE_INTERNAL_FLAGS = F_BONE_INTERNAL_FLAG1 | F_BONE_INTERNAL_FLAG2 | F_BONE_INTERNAL_FLAG3
	};

	HK_FORCE_INLINE int isFlagOn (int boneIdx, hkaPoseFlag enumToCheck) const;
	HK_FORCE_INLINE void setFlag (int boneIdx, hkaPoseFlag enumToCheck) const;
	HK_FORCE_INLINE void clearFlag (int boneIdx, hkaPoseFlag enumToCheck) const;

	HK_FORCE_INLINE int isFlagOnExplicit (hkaPoseFlag flag, hkaPoseFlag enumToCheck) const;
	HK_FORCE_INLINE void setFlagExplicit (hkaPoseFlag& flag, hkaPoseFlag enumToCheck) const;
	HK_FORCE_INLINE void clearFlagExplicit (hkaPoseFlag& flag, hkaPoseFlag enumToCheck) const;

	HK_FORCE_INLINE void clearInternalFlags() const;

	HK_FORCE_INLINE void makeAllChildrenLocalSpace   (int boneIdx) const;
	HK_FORCE_INLINE void makeFirstChildrenModelSpace (int boneIdx) const;
	
	// These methods are called when the value stored is dirty
	const hkQsTransform& calculateBoneModelSpace (int boneIdx) const;

	// DEBUG ONLY METHODS
	void setNonInitializedFlags();
	hkBool checkInternalFlagIsClear(hkaPoseFlag flag) const;

public:

	// For unit testing

	// Checks that the internals of hkaPose are working properly
	hkBool checkPoseValidity () const;

	// Checks that the pose contains only valid hkQsTransforms
	hkBool checkPoseTransformsValidity() const;

};

#include <Animation/Animation/Rig/hkaPose.inl>

#endif

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
