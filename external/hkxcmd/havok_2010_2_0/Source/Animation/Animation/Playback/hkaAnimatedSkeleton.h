/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ANIMATED_SKELETON_H
#define HK_ANIMATED_SKELETON_H

#include <Common/Base/hkBase.h>
#include <Animation/Animation/Playback/Control/hkaAnimationControlListener.h>
#include <Animation/Animation/Animation/hkaAnnotationTrack.h>
#include <Animation/Animation/Animation/hkaAnimation.h>

class hkaSkeleton;
class hkaChunkCache;
class hkaAnimationControl;
class hkaPose;

/// This class manages the runtime instance of each skeleton.
/// It is responsible for maintaining the currently active animation controls.
/// It also performs animation blending and mixing.
/// In general controls are sampled and blended by adding their contributions and renormalizing.
/// If an animation binding has a blend hint of additive then this is layered on after other controls
/// have been accumulated and renormalized.
class hkaAnimatedSkeleton : public hkReferencedObject, protected hkaAnimationControlListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ANIM_RUNTIME);

			/// The skeleton below is not reference counted and must persist for the lifetime of this hkaAnimatedSkeleton instance
		hkaAnimatedSkeleton( const hkaSkeleton* skeleton );

		~hkaAnimatedSkeleton();

			/// Iterates through all controls and advances each of their local clocks
		virtual void stepDeltaTime( hkReal time );

			/// This method is the core of the blending and binding.
			/// It queries all active controls and samples their animations by
			/// iterating over the skeleton extracting bound animation track info.
			/// It then accumulates or blends this data across tracks into a single local
			/// pose. An optional cache can be passed to improve decompression performance.
		virtual void sampleAndCombineAnimations( hkQsTransform* poseLocalSpaceOut, hkReal* floatSlotsOut, hkaChunkCache* cache = HK_NULL ) const;

			/// This method is the core of the blending and binding.
			/// It queries all active controls and samples their animations by
			/// iterating over the skeleton extracting bound animation track info.
			/// It then accumulates or blends this data across tracks into a single local
			/// pose. An optional cache can be passed to improve decompression performance.
			/// This method will sample only the first maxBones bones and the first maxFloatSlots slots
			/// in the skeleton, to assist in LODing animations.
		virtual void sampleAndCombinePartialAnimations( hkQsTransform* poseLocalSpaceOut, hkUint32 maxBones, hkReal* floatSlotsOut, hkUint32 maxFloatSlots, hkaChunkCache* cache = HK_NULL ) const;

			/// This method allows the user to sample the local space transform of a single bone
			/// \param localSpaceOut Output transform to store the local transform of the bone in
			/// \param bone Index of the requested bone (must be valid for the associated skeleton)
			/// \param cache Cache provided (if any)
		virtual void sampleAndCombineSingleBone( hkQsTransform* localSpaceOut, hkInt16 bone, hkaChunkCache* cache = HK_NULL ) const;

			/// This method allows the user to sample the value of a single float slot
			/// \param floatOut Output value to store the current value of the slot in
			/// \param slot Index of the requested slot (must be valid for the associated skeleton)
			/// \param cache Cache provided (if any)
		virtual void sampleAndCombineSingleSlot( hkReal* floatOut, hkInt16 slot, hkaChunkCache* cache = HK_NULL ) const;

			/// This method allows the user to sample the local space transforms of a series of bones
			/// \param localSpaceOut Output transform array to store the local transforms of the bone in (must be numBones in size)
			/// \param bones Array of indices of the requested bones (must be numBones in size)
			/// \param numBones Number of bones to sample
			/// \param cache Cache provided (if any)
		virtual void sampleAndCombineIndividualBones( hkQsTransform* localSpaceOut, hkInt16* bones, hkUint32 numBones, hkaChunkCache* cache = HK_NULL ) const;

			/// This method allows the user to sample the values of a series of float slots
			/// \param floatOut Output value array to store the values of the requested slots in (must be numSlots in size)
			/// \param slots Array of indices of the requested slots (must be numSlots in size)
			/// \param numSlots Number of float slots to sample
			/// \param cache Cache provided (if any)
		virtual void sampleAndCombineIndividualSlots( hkReal* floatOut, hkInt16* slots, hkUint32 numSlots, hkaChunkCache* cache = HK_NULL ) const;

			/// Access to the original skeleton
		inline const hkaSkeleton* getSkeleton() const;

			/// If the total weight on any bone falls below the reference pose weight threshold
			/// we begin to blend in the reference pose.
		inline hkReal getReferencePoseWeightThreshold() const;

			/// If the total weight on any bone falls below the reference pose threshold
			/// we begin to blend in the reference pose.
		void setReferencePoseWeightThreshold( hkReal val );

			/// Calculates the change in root position and orientation for a given timestep.
			/// This combines the weighted contributions from each of then playing animations.
		void getDeltaReferenceFrame(hkReal deltaTimestep, hkQsTransform& deltaMotionOut) const;

			/*
			 * Animation controls
			 */

			/// Add an animation control.
			/// If the weight on this control is > 0 then the associated animation
			/// will be blended and combined.
		void addAnimationControl( hkaAnimationControl*  animation );

			/// Remove an animation control
		void removeAnimationControl( hkaAnimationControl* animation );

			/// Return the number of added controls
		inline int getNumAnimationControls() const;

			/// Access to an animation control.
		inline hkaAnimationControl* getAnimationControl( int i ) const ;


			/*
			 * Annotation support
			 */

			/// A structure to hold annotation results
			/// The ID is remapped from an animation track ID to a skeleton bone ID
			/// This class MUST have the same declaration as hkaAnimation::TrackAnnotation
		struct BoneAnnotation
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaAnimatedSkeleton::BoneAnnotation );

			/// The bone which is annotated
			hkUint16 m_boneID;

			/// Annotation data
			hkaAnnotationTrack::Annotation m_annotation;
		};
			/// Examine all animation controls and counts the annotations that occur
			/// in the timestep. Usually you query this in advance of stepping the controls
		hkUint32 getNumAnnotations( hkReal deltaTime ) const;

			/// Fill annotationsOut with annotations which reside in the current timestep for all animations
			/// annotationsOut must be pre-allocated. Up to the first maxNumAnnotations are returned (see getNumAnnotations)
			/// \return The number of annotations found
		hkUint32 getAnnotations(hkReal deltaTime, BoneAnnotation* annotationsOut, hkUint32 maxNumAnnotations = HK_INT32_MAX ) const;

			/// Check whether this hkaAnimatedSkeleton instance consists wholly of hkaAnimationControls pointing to hkaQuantizedAnimation instances
		bool hasOnlyQuantizedAnimations() const;

	protected:

			/// Array of controls.
		HK_ALIGN16( hkArray<hkaAnimationControl*> m_animationControls );

			/// The skeleton associated with all animations/controls.
		const hkaSkeleton* m_skeleton;

			/// The fill threshold below which the reference pose will be blending in when the sum of the control weights is small.
		hkReal m_referencePoseWeightThreshold;

			/// An hkaAnimatedSkeleton consisting of only hkaQuantizedAnimation can be executed more efficiently. Keep track of the number here.
		hkInt32 m_numQuantizedAnimations;

			/// Animation Control listener interface
		virtual void controlDeletedCallback(hkaAnimationControl* control);

		void sampleAndCombineInternal( hkQsTransform* poseLocalSpaceOut, hkUint32 maxBones, hkReal* floatSlotsOut, hkUint32 maxFloatSlots, hkaChunkCache* cache, bool partial ) const;
		void sampleAndCombineIndividual( hkUint32 numBones, const hkInt16* individualBones, hkQsTransform* poseLocalSpaceOut, hkUint32 numFloatSlots, const hkInt16* individualSlots, hkReal* floatSlotsOut, hkaChunkCache* cache ) const;
};

#include <Animation/Animation/Playback/hkaAnimatedSkeleton.inl>

#endif // HK_ANIMATED_SKELETON_H

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
