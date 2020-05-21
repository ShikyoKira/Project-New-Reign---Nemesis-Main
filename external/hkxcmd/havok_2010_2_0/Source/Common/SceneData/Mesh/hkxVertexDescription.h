/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_MESH_HKXVERTEXDESCRIPTION__H
#define HKSCENEDATA_MESH_HKXVERTEXDESCRIPTION__H


extern const class hkClass hkxVertexDescriptionElementDeclClass;

/// hkxVertexDescription meta information
extern const class hkClass hkxVertexDescriptionClass;

/// This structure describes the memory layout and format of a vertex buffer.
class hkxVertexDescription
{
	//+version(1)
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkxVertexDescription );
		HK_DECLARE_REFLECTION();

			/// Default constructor
		inline hkxVertexDescription();
			/// Finish constructor
		hkxVertexDescription(hkFinishLoadedObjectFlag f);
		
		enum DataType
		{
			HKX_DT_NONE = 0,
			HKX_DT_UINT8, // only used for four contiguous hkUint8s, hkUint8[4]
			HKX_DT_INT16, // used for bone indices when there are more than 255 bones. Used for old style quantized tcoords (0x7fff maps to 10.0f), so div by 3276.7f to get the float tcoords (Deprecated). 
			HKX_DT_UINT32,
			HKX_DT_FLOAT,
		};

		enum DataUsage
		{
			HKX_DU_NONE = 0,
			HKX_DU_POSITION = 1,
			HKX_DU_COLOR = 2,    // first color always can be assumed to be per vertex Diffuse, then per vertex Specular (rare)
			HKX_DU_NORMAL = 4,
			HKX_DU_TANGENT = 8,
			HKX_DU_BINORMAL = 16, // aka BITANGENT
			HKX_DU_TEXCOORD = 32, // in order, 0,1,2, etc of the texture channels. Assumed to be 2D, [u,v], in most cases
			HKX_DU_BLENDWEIGHTS = 64,  // usually 4 weights, but 3 can be stored with 1 implied. Can be stored as 4*uint8, so quantized where 1.0f => 0xff (255),
			HKX_DU_BLENDINDICES = 128, // usually 4 hkUint8s in a row. So can reference 256 blend transforms (bones)
			HKX_DU_USERDATA = 256
		};

		struct ElementDecl
		{
			//+version(2)

			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkxVertexDescription::ElementDecl );

			ElementDecl() : m_byteOffset(0), m_type(HKX_DT_NONE), m_usage(HKX_DU_NONE), m_byteStride(0) { }
			ElementDecl(DataUsage u, DataType t, hkUint8 numElements) : m_byteOffset(0), m_type(t), m_usage(u), m_byteStride(0), m_numElements(numElements) { }

			hkUint32					m_byteOffset; // from start of the buffer, set by the hkxVertexBuffer 
			hkEnum<DataType,  hkUint16>	m_type;
			hkEnum<DataUsage, hkUint16>	m_usage;
			hkUint32					m_byteStride; // between each vert, set by the hkxVertexBuffer 
			hkUint8					    m_numElements; // between each vert, set by the hkxVertexBuffer 
		};


		inline hkUint32 getUsageCount( DataUsage usage ) const; // 0 if not used.
		
		inline const ElementDecl* getElementDeclByIndex( int index ) const; //Will return NULL on error
		inline const ElementDecl* getElementDecl( DataUsage usage, int index ) const; // index == cardinal of that usage, eg TexCoord0, TexCoord1 etc. Will return NULL on error		
		inline ElementDecl* getElementDeclByIndex( int index ); //Will return NULL on error
		inline ElementDecl* getElementDecl( DataUsage usage, int index ); // index == cardinal of that usage, eg TexCoord0, TexCoord1 etc. Will return NULL on error
		
			// as used quite often:
		inline hkUint32 getElementStride( DataUsage usage, int index ) const; 
		inline DataType getElementType( DataUsage usage, int index ) const; 
		
		inline hkUint32 getMask() const;

		bool operator== (const hkxVertexDescription& other ) const;

	public:
		
		hkArray<struct ElementDecl> m_decls;
};

#include <Common/SceneData/Mesh/hkxVertexDescription.inl>

#endif // HKSCENEDATA_MESH_HKXVERTEXDESCRIPTION__H

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
