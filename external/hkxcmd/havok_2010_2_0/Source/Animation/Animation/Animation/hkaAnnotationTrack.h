/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_ANIMATION_HKANIMATIONTRACK_HKCLASS_H
#define HKANIMATION_ANIMATION_HKANIMATIONTRACK_HKCLASS_H


extern const class hkClass hkaAnnotationTrackAnnotationClass;

extern const class hkClass hkaAnnotationTrackClass;

/// hkaAnnotationTrack meta information
extern const class hkClass hkAnimationTrackClass;

/// Annotation information for an animated bone track.
class hkaAnnotationTrack
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_DATA, hkaAnnotationTrack);
		HK_DECLARE_REFLECTION();

		hkaAnnotationTrack();

		hkaAnnotationTrack( const hkaAnnotationTrack& );
	
			/// The information needed to construct an simple animation annotation.
		struct Annotation
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaAnnotationTrack::Annotation );
			HK_DECLARE_REFLECTION();

			Annotation() : m_time( 0.0f ) {}

				/// The animation time associated with this annotation
			hkReal m_time;
			
				/// The annotation text, null terminated.
			hkStringPtr m_text;

			Annotation( hkFinishLoadedObjectFlag flag ) : m_text( flag ) {}
		};
		
		//
		// Members
		//
	public:
		
			/// The animation track name.
		hkStringPtr m_trackName;
		
			/// The annotations associated with this animation track.
		hkArray< struct Annotation > m_annotations;

	public:
		
		HK_FORCE_INLINE hkaAnnotationTrack( hkFinishLoadedObjectFlag flag ) : m_trackName( flag ), m_annotations( flag ) {}

};

#endif // HKANIMATION_ANIMATION_HKANIMATIONTRACK_HKCLASS_H

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
