/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DETECT_RAGDOLL_PENETRATION_H
#define HK_DETECT_RAGDOLL_PENETRATION_H

#include <Common/Base/hkBase.h>


/// This interface class defines a single method, castRay(), and it's used by the hkaDetectRagdollPenetration object to cast rays
/// into the application's world.
class hkaRagdollRaycastInterface
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaRagdollRaycastInterface);
		virtual ~hkaRagdollRaycastInterface() { }

			/// Abstract method, should be implemented by the user in a derived class. Given two points, "from" and "to", specified in
			/// world space, and AABB phantom (encapsulates ragdoll), it should return whether a ray between those two points intersects the scene or not. If it does (if it returns true),
			/// the hitPoint and the hitNormal are returned.
		virtual hkBool castRay (const class hkpAabbPhantom* phantomIn,  const hkVector4& fromWS, const hkVector4& toWS, hkVector4& hitPointWS, hkVector4& hitNormalWS ) = 0;

};

/// hkaDetectRagdollPenetration class detects penetration of ragdoll with landscape. The method takes as input actual hkaPose and performs
/// raycasting of all bones with landscape. Output is the array of the penetrated bones. Each output for penetrated bone contains the begin and
/// end indexes of the bone, penetration point in world coordinates and the normal of penetrated landscape.
/// Raycasting is defined in hkaRagdollRaycastInterface (mainly correct landscape layer);
/// The class also contains very important static utility method isBonePenetratedOrDescendant for checking particular bone status in input
/// ragdoll according to results of penetration detection. The method returns three states for the bone: no penetrated, penetrated or descendant
/// of penetrated bone.
class hkaDetectRagdollPenetration : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME );

		/// This structure is passed on construction of an hkaDetectRagdollPenetration, and contains information about
		/// the ragdoll skeleton, the interface to raycast and the AABB phantom
		struct Setup
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaDetectRagdollPenetration::Setup );

			/// Ragdoll skeleton
			const class hkaSkeleton* m_ragdollSkeleton;

			/// An interface to ray cast functionality. The implementation should only hit objects that the user wants to
			/// check for collisions (usually landscape). For example, it may ignore debris objects.
			class hkaRagdollRaycastInterface* m_raycastInterface;

			/// pointer to hkpAabbPhantom which encapsulates ragdoll
			class hkpAabbPhantom* m_ragdollPhantom;

			/// Constructor. Sets some defaults.
			Setup(){}

		};

		/// This structure is filled by the detection routine, and contains information about the start
		/// and end point of penetrated bone and about hit point and hit normal
		struct BonePenetration
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, BonePenetration);

			/// Index of the bone (from)
			hkInt16 m_boneBeginIndex;

			/// Index of the next bone (to)
			hkInt16 m_boneEndIndex;

			/// Penetration point in world space
			hkVector4 m_penetrationPoint;

			/// Penetration normal in world space
			hkVector4 m_penetrationNormal;

			/// Constructor. Sets some defaults.
			BonePenetration();

			/// Destructor. Requested for hkArray
			~BonePenetration();			
					
		};

		/// Penetration Status for particular bone return by static method isInPenetrationOrDescendant(bone, boneArray)
		enum BonePenetrationStatus
		{
			HK_NOP = 0,	// No penetration
			HK_YESP = 1, // Yes, bone penetrates (penetration point lies on this bone)
			HK_YESP_DESCENDANT = 2	// Yes, bone is descendant of penetrated bones
		};

		/// Output structure holds array of BonePenetration objects
		struct Output
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaDetectRagdollPenetration::Output );

			hkArray<BonePenetration> m_penetrations;
			
		};
		
		/// Constructor input is setup structure.
		hkaDetectRagdollPenetration ( const Setup& setup );


		/// Function detects penetrated bones stored in output array structure,
		hkBool detectPenetration( const class hkaPose& ragdollPoseIn, Output& output );
				
		/// Set m_poseReset to ensure reset of bonesStates after ragdoll keyframed state
		void resetBoneStates();

		/// Get array of bones states (not penetrated, penetrated or descendant)
		const hkArray<BonePenetrationStatus>& getBoneStatusArray() {return m_boneStates;}

	private:

        Setup m_setup;

		hkInt32 m_numBones;

		/// array with bone states (not penetrated, penetrated or descendant)
		hkArray<BonePenetrationStatus> m_boneStates;
		
		/// No penetration on all bones?
		hkBool m_ragdollIsOut;
		
        		
};

#endif // HK_DETECT_RAGDOLL_PENETRATION_H

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
