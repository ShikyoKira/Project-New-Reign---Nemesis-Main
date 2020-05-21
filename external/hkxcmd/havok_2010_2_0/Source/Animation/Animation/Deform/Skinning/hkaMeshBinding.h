/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_DEFORM_SKINNING_HKMESHBINDING_HKCLASS_H
#define HKANIMATION_DEFORM_SKINNING_HKMESHBINDING_HKCLASS_H


extern const class hkClass hkaMeshBindingMappingClass;

class hkxMesh;
class hkaBone;
class hkaSkeleton;

/// hkaMeshBinding meta information
extern const class hkClass hkaMeshBindingClass;

/// A link between a set of bones and an index mesh (the indices referring to the
/// bones, in order).
class hkaMeshBinding : public hkReferencedObject
{
	public:
		// +version(1)
		// +vtable(true)

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_DATA );
		HK_DECLARE_REFLECTION();

			/// Mapping structure
		struct Mapping
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIMATION, hkaMeshBinding::Mapping );
			HK_DECLARE_REFLECTION();

				/// Mapping indices
			hkArray<hkInt16> m_mapping;
			Mapping(){}
			Mapping(hkFinishLoadedObjectFlag f) : m_mapping(f) {}
			Mapping(const Mapping& other) { m_mapping = other.m_mapping; }
		};

		//
		// Members
		//
	public:

			/// A constructor for null initialization
		hkaMeshBinding();

			///
		~hkaMeshBinding();
		
			/// The mesh (hopefully indexed)
		hkRefPtr<hkxMesh> m_mesh;

			/// (Optional) The name of the skeleton from which the binding was constructed.
		hkStringPtr m_originalSkeletonName; 

			/// The skeleton that holds the bones.
		hkRefPtr<hkaSkeleton> m_skeleton;
		
			/// The bone indices that apply to this mesh, as seen by the skeleton.
			/// If there is no mapping, then the mapping is trivial in that the
			/// indices in the mesh represent the bone number directly.
			/// If there is only one mapping then all sections in the mesh use the
			/// same indexed pallet.
			/// If there is more than one mapping then it implies that there
			/// is a mapping per mesh section's index buffer, in order. This can happen
			/// due to breaking up exported sections and reindexing to keep within bone
			/// influence limits imposed by hardware skinning for instance.
		hkArray<struct Mapping> m_mappings;
		
			/// This array contains a transformation for each bone in the skeleton. The
			/// transformation is the inverse of the original bone transform in its
			/// T-pose or bind pose, multiplied by the initial world transform of the skin.
			/// By storing this we only have to store vertices once in skin space, rather than
			/// multiple times in the local space of each bone.
			/// Therefore, the transformations stored are bone-from-skinMesh transformations.
		hkArray<hkTransform> m_boneFromSkinMeshTransforms;

	public:

			// Constructor for initialisation of vtable fixup
		hkaMeshBinding( hkFinishLoadedObjectFlag f );
};

#endif // HKANIMATION_DEFORM_SKINNING_HKMESHBINDING_HKCLASS_H

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
