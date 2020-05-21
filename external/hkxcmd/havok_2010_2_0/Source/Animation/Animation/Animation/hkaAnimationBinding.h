/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_ANIMATION_HKANIMATIONBINDING_HKCLASS_H
#define HKANIMATION_ANIMATION_HKANIMATIONBINDING_HKCLASS_H

#include <Animation/Animation/Animation/hkaAnimation.h>

/// hkaAnimationBinding meta information
extern const class hkClass hkaAnimationBindingClass;

/// Describes how an animation is bound to a skeleton
/// This class is useful when we have partial animations which only animate a subset of the skeleton
/// or when we have reordered the tracks to aid in LOD. All hkaAnimationControl objects
/// (added to a hkaAnimatedSkeleton) require a hkaAnimationBinding to map from tracks to bones/slots
/// for blending of multiple animations.
class hkaAnimationBinding : public hkReferencedObject
{
	public:
		// +version(1)
		// +vtable(true)
	
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_DATA );
		HK_DECLARE_REFLECTION();
		
		//
		// Members
		//
	public:

			/// Default constructor.
		HK_FORCE_INLINE hkaAnimationBinding();

			/// Copy constructor.
		HK_FORCE_INLINE hkaAnimationBinding(const hkaAnimationBinding& other);

			/// (Optional) The name of the skeleton from which the binding was constructed.
		hkStringPtr m_originalSkeletonName; 

			/// A handle to the animation bound to the skeleton.
		hkRefPtr<hkaAnimation> m_animation;
		
			/// A mapping from animation track indices to bone indices.
		hkArray< hkInt16 > m_transformTrackToBoneIndices;


			/// A mapping from animation float track indices to float slot indices.
		hkArray< hkInt16 > m_floatTrackToFloatSlotIndices;


			/// A hint to indicate how this animation should be blended -
			/// See hkaAnimatedSkeleton for a description of how this affects animation blending.
		enum BlendHint
		{
				/// Normal
			NORMAL = 0,
				/// Additive
			ADDITIVE = 1
		};

			/// The blend hint.
		hkEnum<BlendHint, hkInt8> m_blendHint; //+default(0/*hkaAnimationBinding::NORMAL*/)

			// Helper functions to find tracks given bones or slots
		hkInt16 findTrackIndexFromBoneIndex( hkInt16 boneIndex ) const;
		hkInt16 findTrackIndexFromSlotIndex( hkInt16 slotIndex ) const;
		static hkInt16 HK_CALL findTrackIndexFromBoneIndex( hkInt16 boneIndex, hkInt32 numTracks, const hkInt16* trackToBoneIndices );

	public:

			// Constructor for initialisation of vtable fixup
		HK_FORCE_INLINE hkaAnimationBinding( hkFinishLoadedObjectFlag flag );
};

#include <Animation/Animation/Animation/hkaAnimationBinding.inl>

#endif // HKANIMATION_ANIMATION_HKANIMATIONBINDING_HKCLASS_H

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
