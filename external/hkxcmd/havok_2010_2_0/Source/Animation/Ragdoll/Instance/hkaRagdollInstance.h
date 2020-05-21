/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_HKRAGDOLL_INSTANCE
#define INC_HKRAGDOLL_INSTANCE

#include <Common/Base/hkBase.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/Constraint/hkpConstraintInstance.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>

extern const hkClass hkaRagdollInstanceClass;

	/// An hkaRagdollInstance encapsulates the rigid bodies, constraints and associated skeleton for a physical ragdoll.
	/// It also provides utilities to get / set poses, add and remove from world, cloning, and handling rigid body transforms.
class hkaRagdollInstance : public hkReferencedObject
{
	public:

		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ANIMATION);

			/// Constructor, requires a properly ordered set of rigid bodies and constraints.
			/// The must represent a tree (n rigid bodies, n-1 constraints), and the local
			/// transformation of the rigid bodies must match the pivot of the bone they represent.
			/// You can use hkaRagdollUtils::prepareSystemForRagdoll() in order to ensure this.
			/// The skeleton contains information about hierarchy, bone names, initial pose and bone constraints.
			/// You can generate an hkaSkeleton from a physics system by using the utility hkaRagdollUtils::constructSkeletonFromRagdoll().
		
		hkaRagdollInstance ( const hkArrayBase<hkpRigidBody*>& rigidBodies, const hkArrayBase<hkpConstraintInstance*>& constraints, const hkaSkeleton* skeleton );
		
			/// Same as the above constructor, but the extra bone-to-rigidbody map allows the ragdoll skeleton to have extra bones
		hkaRagdollInstance ( const hkArrayBase<hkpRigidBody*>& rigidBodies, const hkArrayBase<hkpConstraintInstance*>& constraints, const hkaSkeleton* skeleton , const hkArrayBase<int>& boneToRigidBody);

			/// Destructor.
		virtual ~hkaRagdollInstance();

			/// Cloning - it returns a new hkaRagdollInstance cloned from this one.
			///
			/// When using the hkConstraintInstane::CLONE_DATAS_WITH_MOTORS mode
			/// this method clones 'powered' hkConstraintDatas, therefore it also
			/// clones motors, what lets you set their targets independently of those of
			/// the original ragdoll's.
		virtual hkaRagdollInstance* clone(hkpConstraintInstance::CloningMode mode = hkpConstraintInstance::CLONE_FORCE_SHALLOW) const;

		//
		// Accessors
		//

			/// Number of bones.
		inline int getNumBones () const;

			/// Access the rigid body associated with the i-th bone.
		inline hkpRigidBody* getRigidBodyOfBone (int i) const;

			/// Get the bone index of the rigid body, or -1 if it is not a body in the ragdoll.
		inline int getBoneIndexOfRigidBody (hkpRigidBody* rb) const;

			/// Access the constraint associated with the i-th bone. The constraint associated with a bone is the constraint that links that bone with its parent.
			/// Notice, therefore, that getConstraint(0) returns HK_NULL.
		inline hkpConstraintInstance* getConstraintOfBone (int i) const;

			/// Access the associated skeleton
		inline const hkaSkeleton* getSkeleton () const;

			/// Retrieves the ID of the parent of a bone. -1 for the root (bone 0).
		inline int getParentOfBone (int i) const;

			/// Returns a (const) array of rigid bodies representing the bones
		inline const hkArray<hkpRigidBody*>& getRigidBodyArray() const;

			/// Returns a (const) array of constraint representing the joints. Notice that, since the root (bone 0) has no parent,
			/// the size of this array is n-1, where n is the number of bones.
		inline const hkArray<hkpConstraintInstance*>& getConstraintArray() const;

			/// Returns true if the ragdoll instance has more bones than rigid bodies. From 4.6, Ragdolls can have extra terminals bones,
			/// which can be useful in order to perform some operations.
		inline hkBool hasNonRigidBodyBones () const;

		//
		// Get/Set pose methods
		//
			/// Gets the current pose of the rag doll in world space.
		void getPoseWorldSpace (hkQsTransform* poseWorldSpaceOut) const;

			/// Gets the current pose of the rag doll in world space
			/// The user may optionally specify a local space pose for unmapped bones. If NULL, these bones are filled in with the reference pose.
		void getPoseWorldSpace (hkQsTransform* poseWorldSpaceOut, const hkQsTransform& worldFromModel, const hkQsTransform* localSpacePoseForUnmappedBonesOptional = HK_NULL ) const;

			// Use this to get the pose of the ragdoll in world space when doing asynchronous stepping.
			// The pose is evaluated at time getWorld()->getCurrentTime().
		void getApproxCurrentPoseWorldSpace (hkQsTransform* poseWorldSpaceOut) const;

			// Use this to get the pose of the ragdoll in world space when doing asynchronous stepping.
			// You can specify the time at which to evaluate the pose.
		void getApproxPoseWorldSpaceAt (hkTime time, hkQsTransform* poseWorldSpaceOut) const;

			/// Sets the current pose of the rag doll using a pose in world space - WARNING: This is for 'posing' the ragdoll and should not be called every frame - use controllers to drive a ragdoll.
		void setPoseWorldSpace (const hkQsTransform* poseWorldSpaceIn);

			/// Gets the current pose of the rag doll in model space
			/// This is similar to getPoseWorldSpace but it uses the worldFromModel transform to transform the result to model space
		void getPoseModelSpace (hkQsTransform* poseModelSpaceOut, const hkQsTransform& worldFromModel) const;

		/// Sets the current pose of the rag doll using a pose in model space - WARNING: This is for 'posing' the ragdoll and should not be called every frame - use controllers to drive a ragdoll.
		void setPoseModelSpace (const hkQsTransform* poseModelSpaceIn, const hkQsTransform& worldFromModel);

			/// Sets the current pose and velocities of the rag doll using two poses in model space, separated by a given timestep.
			/// The rigid bodies are set to the positions and orientations of poseModelSpaceB, and the velocities are set to those
			/// that would have been required to bring the rigid bodies from poseModelSpaceA to poseModelSpaceB
		void setPoseAndVelocitiesModelSpace ( const hkQsTransform* poseModelSpaceA, const hkQsTransform& worldFromModelA, 
											const hkQsTransform* poseModelSpaceB, const hkQsTransform& worldFromModelB, hkReal timestep);


		//
		// World add remove methods
		//

			/// Add to world; returns failure if the ragdoll is already in the world.
			/// The 'updateFilter' parameter can be used to force a filter update after both the bodies *and the constraints* have been added.
			/// This will only be necessary if the user has implemented filter logic (their own custom filter) based on the addition
			/// of constraints in the world - it will not be necessary for the supplied Havok filters:
			/// hkpGroupFilter, hkpCollisionFilterList, hkpNullCollisionFilter, hkpPairCollisionFilter, hkpPairwiseCollisionFilter - because they don't depend on constraints
		hkResult addToWorld (hkpWorld* world, hkBool updateFilter) const;

			/// Removes from world; returns failure if the ragdoll wasn't part of a world.
		hkResult removeFromWorld () const;

			/// Returns the world where the ragdoll has been added to (HK_NULL if none).
		inline hkpWorld* getWorld() const;

		//
		// Utility methods
		//

			/// Returns the current transform (worldFromBone) of the i-th bone (rigid body).
			/// Position and orientation are taken from the rb, scale is taken from the reference pose of the skeleton
		void getWorldFromBoneTransform (int i, hkQsTransform& worldFromBoneOut) const;

			/// Returns the current transform (bone-to-world) of the i-th bone (rigid body).
			/// Position and orientation are taken from the rb, scale is taken from the reference pose of the skeleton
			/// This method uses hkpRigidBody::approxTransformAt and is useful if you are stepping animation and physics independently.
		void getApproxWorldFromBoneTransformAt (int i, hkTime time, hkQsTransform& worldFromBoneOut) const;

	public:

			/// The rigid bodies associated with this rag doll.
		/*const*/ hkArray<hkpRigidBody*> m_rigidBodies;
			/// The constraints associated with this rag doll
		/*const*/ hkArray<hkpConstraintInstance*> m_constraints;

			/// For ragdolls with non-rigid-body bones, we need this map
		/*const*/ hkArray<int> m_boneToRigidBodyMap; // HKA-773

			/// The skeleton associated with this rag doll.
		hkRefPtr<const hkaSkeleton> m_skeleton;

	public:

		hkaRagdollInstance( hkFinishLoadedObjectFlag f );

	private:

		void commonInit (const hkArrayBase<hkpRigidBody*>& rigidBodies, const hkArrayBase<hkpConstraintInstance*>& constraints, const hkaSkeleton* skeleton);
};

#include <Animation/Ragdoll/Instance/hkaRagdollInstance.inl>

#endif //INC_HKRAGDOLL_INSTANCE

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
