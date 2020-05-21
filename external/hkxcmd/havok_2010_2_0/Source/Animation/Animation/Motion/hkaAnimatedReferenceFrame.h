/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_MOTION_ANIMATED_REFERENCE_FRAME_H
#define HKANIMATION_MOTION_ANIMATED_REFERENCE_FRAME_H

/// hkaAnimatedReferenceFrame meta information
extern const class hkClass hkaAnimatedReferenceFrameClass;


/// This class represents the motion extracted from an animation. Check the Motion Extraction section in the Havok Animation User Guide for details.
class hkaAnimatedReferenceFrame : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_MOTION );
		HK_DECLARE_REFLECTION();
		
			/// Default constructor
		inline hkaAnimatedReferenceFrame() { }
		
			/// Returns the motion stored (previously extracted from the animation) at time t.
			/// This motion represents the absolute offset from the start of the animation.
		virtual void getReferenceFrame(hkReal time, hkQsTransform& motionOut) const = 0;

			/// Returns the change in reference frame between two times.
		virtual void getDeltaReferenceFrame( hkReal time, hkReal nextTime, int loops, hkQsTransform& deltaMotionOut, hkReal cropStartAmount, hkReal cropEndAmount ) const = 0;

            /// Returns the length of time this reference frame is animated for.
		virtual hkReal getDuration() const = 0;

	public:
	
			// Constructor for initialisation of vtable fixup
		HK_FORCE_INLINE hkaAnimatedReferenceFrame( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) {}
};

#endif // HKANIMATION_MOTION_ANIMATED_REFERENCE_FRAME_H

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
