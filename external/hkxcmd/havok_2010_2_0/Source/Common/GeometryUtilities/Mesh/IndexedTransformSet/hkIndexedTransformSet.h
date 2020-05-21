/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_INDEXED_TRANSFORM_SET_H
#define HK_INDEXED_TRANSFORM_SET_H


extern const class hkClass hkMeshBoneIndexMappingClass;

extern const class hkClass hkIndexedTransformSetClass;

/// Array of index mappings that specify how the bones in a skeleton are bound to a skin. This is used in the
/// same way as hkaMeshBinding::Mapping
struct hkMeshBoneIndexMapping
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkMeshBoneIndexMapping );			
	HK_DECLARE_REFLECTION();

	hkArray<hkInt16> m_mapping;
	hkMeshBoneIndexMapping(){}
	hkMeshBoneIndexMapping(hkFinishLoadedObjectFlag f) : m_mapping(f) {}
	hkMeshBoneIndexMapping(const hkMeshBoneIndexMapping& other) { m_mapping = other.m_mapping; }	
};

/// hkIndexedTransformSetCinfo describes transforms needed by a hkMeshBody that has multiple transforms (over and
/// above the basic position transform)
///
/// A hkMeshBody needs to have indexed transforms, if all of its shapes' mesh sections have an index. Such a mesh shape
/// is constructed when the method createCompoundShape is called on hkMeshShape. Each of the meshes is given a transform
/// index, the first shape has index 0, the next 1 and so forth.
/// When a hkMeshBody is constructed from the mesh shape, the positions of the 'child' meshes can be controlled via
/// indexed transforms. This structure holds information about the child transforms.
///
/// The other situation where indexed transforms are used is when skinning is in operation. Skinning occurs when
/// the vertex buffer for a mesh section has skinning information defined on it.
/// The actual sequence of transforms performed of a vertex will be as follows (the displayTransform
/// is the main transform, set via setTransform on the hkMeshBody):
///
/// vertex -> inverse -> matrix -> displayTransform
///
/// or as column major matrix multiplication (Havok style)
///
/// vertexOut = displayTransform * matrix * inverse * vertexIn
///
/// Only the displayMatrix and the matrices can be modified on a per body basis - the inverses are immutable throughout
/// the life of the hkMeshBody.
///
/// The inverse transforms are there for skinning and are the 'bone space' transform. Setting the value to HK_NULL
/// will indicate that inverse transforms are not needed, and may provide a significant speed improvement.
///
/// \sa hkMeshBody
struct hkIndexedTransformSetCinfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkIndexedTransformSetCinfo );

	hkIndexedTransformSetCinfo();

	const hkMatrix4* m_inverseMatrices;			///< Pointer to the matrices to transform from vertex buffer space to bone space (if HK_NULL assumed all are identity - reducing memory usage, and improving performance)
	const hkMatrix4* m_matrices;                ///< Pointer to matrices to transform from bone space into 'model' space (if HK_NULL all are assumed to be the identity)
	const hkInt16* m_matricesOrder;				///< Optional order of the matrices. For example to represent a skeleton hierarchy. Use -1 from roots.
	const hkStringPtr* m_matricesNames;			///< Optional name of each matrix. For example to represent skeleton bone names.
	int m_numMatrices;							///< The number of matrices
	hkBool m_allTransformsAreAffine;			///< Set to true if all the transforms are affine (no scaling/skew). Allows for faster skinning.

	const struct hkMeshBoneIndexMapping* m_indexMappings;		///< Bone indices that apply to this mesh
	int m_numIndexMappings;									///< The number of mappings
};


/// The hkIndexedTransformSet is designed make implementation transforms associated with hkMeshBody objects simpler
/// to implement.
///
/// If a hkMeshBody is holding indexed matrix information (identified by the hkIndexedTransformSetCinfo being passed
/// into createBody on hkMeshSystem) - then that data needs to be held somehow. Although an implementation is free to
/// choose how to do this - using the hkIndexedTransformSet makes it simpler.
///
/// To use in an implementation of a hkMeshBody - hold a pointer to a hkIndexedTransformSet. If the body doesn't
/// have any indexed transformed this member can just be left as HK_NULL. If it does construct a hkIndexedTranformSet
/// using the hkIndexedTransformSetCinfo value passed into the createBody method. Finding transforms used for rendering
/// is straight forward using the calculateMatrixes/calculateMatrix method. The class also has optimizations to stop
/// unneeded calculations if for example the inverse transforms are all the identity.
class hkIndexedTransformSet: public hkReferencedObject
{
    public:        
        //+version(1)
        //+vtable(true)
		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

            /// Ctor
        hkIndexedTransformSet(const hkIndexedTransformSetCinfo& info);

			/// Get the total amount of matrices stored in this set
        HK_FORCE_INLINE int getNumMatrices() const { return m_matrices.getSize(); }

            /// Set a range of matrices
        void setMatrices(int startIndex, const hkMatrix4* matrices, int numMatrices);

            /// Get a range of matrices
        void getMatrices(int startIndex, hkMatrix4* matrices, int numMatrices) const;

            /// Get a range of inverse matrices
        void getInverseMatrices(int startIndex, hkMatrix4* matrices, int numMatrices) const;

            /// Get the matrices
        HK_FORCE_INLINE const hkArray<hkMatrix4>& getMatrices() const { return m_matrices; }

            /// Get the inverse matrices. If there are no inverses (i.e., effectively all identity) will return with a size of 0
        HK_FORCE_INLINE const hkArray<hkMatrix4>& getInverseMatrices() const { return m_inverseMatrices; }

            /// Returns true if it has inverse transforms
        HK_FORCE_INLINE bool hasInverseMatrices() const { return m_inverseMatrices.getSize() > 0; }

			/// Gets the index mappings associated with the mesh.
		HK_FORCE_INLINE const hkArray<hkMeshBoneIndexMapping>& getIndexMappings() const { return m_indexMappings; }

            /// Works out the transforms from model space to world/parent space
        void calculateMatrices(hkArray<hkMatrix4>& matrices) const;

            /// Works out a series of transforms
        void calculateMatrices(const hkMatrix4& parentToWorld,  hkArray<hkMatrix4>& matrices) const;

            /// Works out a single transform matrix
        void calculateMatrix(int index, hkMatrix4& matrixOut) const;

            /// Works out the transforms (taking into account the parentToWorld transform)
        void calculateMatrix(const hkMatrix4& parentToWorld, int index, hkMatrix4& matrixOut) const;            

			/// Serialization Ctor
		hkIndexedTransformSet( hkFinishLoadedObjectFlag flag );

	public:
		//
		// Members
		//

		hkArray<hkMatrix4> m_matrices;						///< the matrices
        hkArray<hkMatrix4> m_inverseMatrices;               ///< the inverted matrices (if size is 0 means there are no inverse transforms)
		hkArray<hkInt16> m_matricesOrder;					///< order of the matrices, can be empty
		hkArray<hkStringPtr> m_matricesNames;				///< names of the matrices, can be empty
        hkArray<struct hkMeshBoneIndexMapping> m_indexMappings;	///< array of indices for the mesh
		hkBool m_allMatricesAreAffine;                      ///< Set if all the matrices are affine
};



#endif // HK_INDEXED_TRANSFORM_SET_H

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
