/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MESH_SHAPE_H
#define HK_MESH_SHAPE_H


extern const class hkClass hkMeshSectionCinfoClass;

extern const class hkClass hkMeshSectionClass;

class hkMeshTexture;
class hkMeshVertexBuffer;
struct hkVertexFormat;

extern const hkClass hkMeshMaterialClass;

/// A material used with a mesh
///
/// This interface is used to represent a graphics engines specific implementation of a material. In this context a
/// 'material' means how a geometric primitive is rasterized in terms of lighting, texture mapping and so forth.
/// The mesh system doesn't need any direct details on how an engines material works, just a handle to tell an implementation
/// which internal material to use.
///
/// The interface has methods which allow determination of if vertex buffers/formats are suitable for a material. For example
/// a material may require vertex normals to render correctly - these methods provide a way for the material to communicate
/// with utilities about such requirements, in doing so allow such elements to be created as needed.
///
/// The createCompatibleVertexBuffer() is part of the interface as well as createCompatibleVertexFormat(), because an implementation
/// may have a method of rapidly converting a vertex buffer into an appropriate format for the material, or perhaps the vertex
/// buffer needs to have vertex elements generated which the standard tools do not know about. For example a material may require
/// a channel to hold a certain type of noise - the tools could not generate this but the material may be able to.
///
/// \sa hkMeshShape hkMeshMaterialRegistry
class hkMeshMaterial: public hkReferencedObject
{
	public:
		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// Default Ctor
		hkMeshMaterial() {}
			/// Serialization Ctor
		hkMeshMaterial( hkFinishLoadedObjectFlag flag ): hkReferencedObject(flag) {}

			/// Returns a vertex buffer format that is compatible with this material
		virtual hkResult createCompatibleVertexFormat(const hkVertexFormat& format, hkVertexFormat& compatibleFormat) = 0;

			/// Constructs a suitable vertex buffer based on the input.
			/// If the input is suitable, may return the input ref'd.
		virtual hkMeshVertexBuffer* createCompatibleVertexBuffer(hkMeshVertexBuffer* buffer) = 0;

			/// Returns true if this vertex buffer is suitable for this material
		virtual bool isCompatible(const hkMeshVertexBuffer* buffer) = 0;

			/// Sets the name of the material.
		virtual void setName(const char* name) = 0;
			/// Gets the name of the material. Returns HK_NULL if the material is not named.
		virtual const char* getName() const = 0;

			/// Number of textures used by the material
		virtual int getNumTextures() const = 0;
			/// Interface describing the texture at index
		virtual hkMeshTexture* getTexture(int index) const = 0;
			/// Adds a texture to the material
		virtual void addTexture(hkMeshTexture* texture) = 0;
};


/// A mesh section is a collection of render primitives which all use the same material and vertex buffer.
///
/// A mesh section is made up of a vertex buffer (potentially shared between multiple mesh sections, and shapes), a
/// (potentially shared) material and a list of primitives. Primitives are not shared, although they will typically
/// be shared with every hkMeshBody instantiation of a hkMeshShape.
///
/// A mesh section may or may not have indices associated with it. If there aren't indices associated then
/// the primitive will just use the vertices starting from m_vertexStartIndex to make up the primitive types.
/// Also note that it is possible for each mesh section to have its own transform - which can be independently set.
/// If the mesh is using an indexed transform the m_transformIndex will specify which transform will be used. The final
/// transform of the vertices will the indexed transform followed by the hkMeshBodys transform.
///
/// \sa hkMeshShape hkMeshBody
struct hkMeshSection
{
	//+version(1)
	HK_DECLARE_REFLECTION();
    HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkMeshSection );

		/// The different index types
	enum MeshSectionIndexType
	{
        INDEX_TYPE_NONE,                    ///< There isn't any indexing
		INDEX_TYPE_UINT16,					///< Uint16 indexing
		INDEX_TYPE_UINT32					///< Uint32 indexing
	};

		/// The different primitive types
	enum PrimitiveType
	{
		PRIMITIVE_TYPE_UNKNOWN,				///< Unknown primitive type
		PRIMITIVE_TYPE_POINT_LIST,			///< Point list
		PRIMITIVE_TYPE_LINE_LIST,			///< Line list (start index, end index etc)
		PRIMITIVE_TYPE_TRIANGLE_LIST,		///< Triangle list (3 vertices specified for each)
		PRIMITIVE_TYPE_TRIANGLE_STRIP 		///< Triangle strip
	};

	hkEnum<PrimitiveType,hkUint8> m_primitiveType;     ///< The primitive types
    int m_numPrimitives;                    ///< Total number of primitives of type m_primitiveType
    int m_numIndices;                       ///< Can be calculated from numPrims + primitive type, but convenient

    int m_vertexStartIndex;                 ///< This offset is used if no indices are specified (-1 if they are specified)

    int m_transformIndex;                   ///< Index into transform set used to position this section. -1 if unused.

    hkEnum<MeshSectionIndexType,hkUint8> m_indexType;  ///< The index type, that m_indices points to

		/// The indices (will be HK_NULL if there are no indices, or if a lockMeshSection didn't request the indices).
		/// NOTE the indices order and type may be different to that specified in the hkMeshSectionCinfo
    const void* m_indices;					//+serialized(false)

    hkMeshVertexBuffer* m_vertexBuffer;		///< Vertex buffer (will be HK_NULL if a lockMeshSection didn't request the vertex buffer)

    hkMeshMaterial* m_material;             ///< Material used on this mesh section

    int m_sectionIndex;                     ///< The index of this mesh section
};	

/// Structure for describing mesh sections for construction
///
/// A hkMeshShape is made up of one or more mesh sections. The hkMeshSectionCinfo structure describes each mesh
/// section for construction of the shape.
///
/// A mesh section can consist of a vertex buffer (potentially shared), a material (potentially shared) and a number of
/// primitives - indexed or not. Note that the vertex buffer can only be a vertex buffer constructed on the hkMeshSystem
/// where the shape is being constructed. Also note that vertex buffers returned from a hkMeshBody should NOT be used
/// to construct a hkMeshShape either.
///
/// A primitive type specifies what kind of primitive this mesh section will draw. Currently only points, lines and
/// triangles are supported.
///
/// Each mesh section can be associated with an indexed transform which can be set on the hkMeshBody. To use this feature
/// ALL of the mesh sections associated with the hkMeshShape must have a m_transformIndex >= 0.
///
/// It is possible to define a mesh section without indices - set the m_indices member to HK_NULL, and set the m_vertexStartIndex
/// to the vertex start index for the primitive to be drawn. The primitive will use vertex indices increasing from this value for
/// each additional point.
///
/// \sa hkMeshShape
struct hkMeshSectionCinfo
{
	//+version(1)
	HK_DECLARE_REFLECTION();

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkMeshSectionCinfo );
	HK_DECLARE_POD_TYPE();

	hkMeshVertexBuffer* m_vertexBuffer;					///< Vertex buffer that will be used
	hkMeshMaterial* m_material;							///< Material used on this mesh section

		/// The primitive types
	hkEnum<hkMeshSection::PrimitiveType, hkUint8> m_primitiveType;
		/// Total number of primitives of type m_primitiveType
	int m_numPrimitives;
		/// The index type used (if NONE, then m_indices must be equal to NULL)
	hkEnum<hkMeshSection::MeshSectionIndexType,hkUint8>	m_indexType;
		/// The indices (if HK_NULL then indices are not used, only vertex order).
	const void* m_indices; // +nosave
		/// This offset is used if indices aren't specified (ignored if indices are set)
	int m_vertexStartIndex;
		/// Index into transform set used to position this section. -1 if per section transform isn't used.
	int m_transformIndex;
};


extern const hkClass hkMeshShapeClass;

/// This interface defines geometrically how a graphics entity will appear.
///
/// A mesh shape consists of one or more mesh sections. A mesh section consists of a
///   - reference to a vertex buffer
///   - a material
///   - triangle indices.
/// Basic information about a mesh section can be determined by calling lockMeshSection.
///
/// It is implementation specific how index information is stored internally. Therefore it is NOT guaranteed that the
/// index, order, primitive type will be the same once the hkMeshShape has been constructed. This allows an implementation to
/// use indexing information for best memory usage/performance.
///
/// If a mesh section does not have any indices ( this can be determined if m_vertexStartIndex >= 0 in the hkMeshSection
/// structure returned by getMeshSection), then the m_indices member of hkMeshSection will be HK_NULL.
///
/// In order to make access of the primitives more simple there are utilities such as hkMeshPrimitiveUtil which will
/// convert a mesh sections primitives (if possible) into an indexed triangle list more simple processing.
///
/// To draw a mesh, you need an instance of a hkMeshBody referencing the hkMeshShape.
/// In order for the body (and therefore shape) to be drawn the hkMeshBody has to be
/// added to the hkMeshSystem.
///
/// \sa hkMeshBody hkMeshMaterial hkMeshSystem
class hkMeshShape: public hkReferencedObject
{
	public:
		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

		hkMeshShape() {}
		hkMeshShape( hkFinishLoadedObjectFlag flag ): hkReferencedObject(flag) {}

        enum AccessFlags
        {
            ACCESS_INDICES = 0x1,               ///< This flag is set if on the lock access to indices is required
            ACCESS_VERTEX_BUFFER = 0x2          ///< This flag is set if on the lock access to the vertex buffer is required
        };

			/// Returns the amount of sections
		virtual int getNumSections() const = 0;

			/// Gets information about a section
			///
			/// Must be balanced with unlockSection calls. NOTE there can be multiple locks active at one time.
			/// Forgetting to call unlockSection may lead to memory leaks, and other problems.
			/// Also the contents of the hkMeshSection structure are only guaranteed to be valid in between
			/// lock/unlockSection. To keep a vertex buffer in scope - you can addReference(). The only exception
			/// is the hkMeshMaterial which will remain in scope.
		virtual void lockSection(int sectionIndex, hkUint8 accessFlags, hkMeshSection& sectionOut) const = 0;

			/// Unlocks a mesh section. Must be given exactly the same structure contents as was returned from a lockSection
			/// otherwise behavior is undefined.
		virtual void unlockSection(const hkMeshSection& section) const = 0;

			/// Returns an optional name of the mesh shape
		virtual const char* getName() const { return HK_NULL; }

			/// Sets an optional name
		virtual void setName(const char* n) { }

};

#endif // HK_MESH_SHAPE_H

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
