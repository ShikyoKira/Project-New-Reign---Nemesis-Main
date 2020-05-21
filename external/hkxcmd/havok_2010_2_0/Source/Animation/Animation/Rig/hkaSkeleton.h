/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_RIG_HKSKELETON_HKCLASS_H
#define HKANIMATION_RIG_HKSKELETON_HKCLASS_H

#include <Common/Base/Types/Geometry/LocalFrame/hkLocalFrame.h>
#include <Animation/Animation/Rig/hkaBone.h>

extern const class hkClass hkaSkeletonLocalFrameOnBoneClass;

/// hkaSkeleton meta information
extern const class hkClass hkaSkeletonClass;

/// The class that represents a combination of a character rig and arbitrary float data.
/// The bone/transform data is designed to represent a skeletal animation rig - bone(transform) names, hierarchy and reference pose.
/// The float slot data is designed to store arbitrary floating point data.
///
/// Either bones or float slots may be empty (but not both). For example:
///     - A character rig might have only bones.
///     - A character rig with auxiliary animated data might have both bones and float slots.
///     - A morph target 'rig' might have only float slots.
///
/// Note that size of m_bones.getSize()==m_parentIndices.getSize()==m_referencePose.getSize().
class hkaSkeleton: public hkReferencedObject
{
	public:

		//+version(3)
		//+vtable(true)

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RIG );
		HK_DECLARE_REFLECTION();

			/// A local frame and the index of the bone it is attached to.
		struct LocalFrameOnBone
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RIG, LocalFrameOnBone );
			HK_DECLARE_REFLECTION();

				/// The local frame attached to a bone.
			hkRefPtr<hkLocalFrame> m_localFrame;

				/// The index of the bone that the frame is attached to.
			int m_boneIndex;

			HK_FORCE_INLINE LocalFrameOnBone();
			HK_FORCE_INLINE LocalFrameOnBone( hkFinishLoadedObjectFlag f );
		};

			/// A constructor for null initialization
		HK_FORCE_INLINE hkaSkeleton();

			/// Copy the given skeleton into this one.
		HK_FORCE_INLINE hkaSkeleton( const hkaSkeleton& skel );

			/// Get the local frame that is attached to a bone. Returns HK_NULL if there isn't one.
		hkLocalFrame* getLocalFrameForBone( int boneIndex ) const;

		//
		// Members
		//

			/// A user name to aid in identifying the skeleton
		hkStringPtr m_name;

			/// Parent relationship
		hkArray<hkInt16> m_parentIndices;

			/// Bones for this skeleton
		hkArray<class hkaBone> m_bones;

			/// The reference pose for the bones of this skeleton. This pose is stored in local space.
		hkArray<hkQsTransform> m_referencePose;

			/// The reference values for the float slots of this skeleton. This pose is stored in local space.
		hkArray<hkReal> m_referenceFloats;

			/// Floating point track slots. Often used for auxiliary float data or morph target parameters etc.
			/// This defines the target when binding animations to a particular rig.
		hkArray<hkStringPtr> m_floatSlots;

			/// The local frames that are attached to the bones of this skeleton. These must be ordered
			/// in order of increasing bone index.
		hkArray<struct LocalFrameOnBone> m_localFrames;

			// Constructor for initialisation of vtable fixup
		HK_FORCE_INLINE hkaSkeleton( hkFinishLoadedObjectFlag f );

};

#include <Animation/Animation/Rig/hkaSkeleton.inl>

#endif // HKANIMATION_RIG_HKSKELETON_HKCLASS_H

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
