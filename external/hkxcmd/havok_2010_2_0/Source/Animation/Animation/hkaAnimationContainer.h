/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_HKANIMATIONCONTAINER_HKCLASS_H
#define HKANIMATION_HKANIMATIONCONTAINER_HKCLASS_H

#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Animation/hkaAnimation.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Rig/hkaBoneAttachment.h>
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

	/// hkaAnimationContainer meta information
extern const class hkClass hkaAnimationContainerClass;

	/// This is the top level container class for animation data
class hkaAnimationContainer : public hkReferencedObject
{
	public:
		//+version(1)
		//+vtable(true)
		
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_DATA );
		HK_DECLARE_REFLECTION();

			/// Constructor.
		HK_FORCE_INLINE hkaAnimationContainer();

			/// Copy constructor.
		HK_FORCE_INLINE hkaAnimationContainer(const hkaAnimationContainer& other);

			/// Finish constructor.
		HK_FORCE_INLINE hkaAnimationContainer(hkFinishLoadedObjectFlag f);

			/// Assignment operator.
		HK_FORCE_INLINE hkaAnimationContainer& operator=(const hkaAnimationContainer& other);
		
		//
		// Members
		//
	public:
		
			/// The rigs in the scene
		hkArray< hkRefPtr<hkaSkeleton> > m_skeletons;
		
			/// The animations in the scene
		hkArray< hkRefPtr<hkaAnimation> > m_animations;
		
			/// The bindings from an animations to a set of bone numbers. Check the skeleton you
			/// apply them to fits.
		hkArray< hkRefPtr<hkaAnimationBinding> > m_bindings;
		
			/// The attachments in the scene
		hkArray< hkRefPtr<hkaBoneAttachment> > m_attachments;
		
			/// The skins in the scene that reference the above skeletons.
		hkArray< hkRefPtr<hkaMeshBinding> > m_skins;

		//
		// Utility methods
		//

			/// Finds the given skeleton in the animation container
		hkaSkeleton* findSkeletonByName (const char* name) const;

			/// Finds the given bone attachment in the animation container
		hkaBoneAttachment* findBoneAttachmentByName (const char* name) const;
};

#include <Animation/Animation/hkaAnimationContainer.inl>

#endif // HKANIMATION_HKANIMATIONCONTAINER_HKCLASS_H

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
