/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_MESH_HKXVERTEXBUFFER_HKCLASS_H
#define HKSCENEDATA_MESH_HKXVERTEXBUFFER_HKCLASS_H

#include <Common/SceneData/Mesh/hkxVertexDescription.h>

extern const class hkClass hkxVertexBufferVertexDataClass;

class hkxVertexDescription;

extern const class hkClass hkxVertexBufferClass;

/// The information needed to construct a vertex buffer. This structure binds a
/// chunk of memory to a vertex format
class hkxVertexBuffer : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
		HK_DECLARE_REFLECTION();
	
			/// Default constructor
		inline hkxVertexBuffer();
		hkxVertexBuffer( hkFinishLoadedObjectFlag f );

		inline int getNumVertices() const;
		void setNumVertices(int n, const hkxVertexDescription& format);

		const void* getVertexDataPtr(const hkxVertexDescription::ElementDecl& elem) const;
		void* getVertexDataPtr(const hkxVertexDescription::ElementDecl& elem);

		inline const hkxVertexDescription& getVertexDesc() const;
		
		void copy( const hkxVertexBuffer& other, bool resize);
		void copyVertex( const hkxVertexBuffer& other, int vertFrom, int vertTo);

		// Internal
	public:

		struct VertexData
		{
		public:

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, VertexData );
			HK_DECLARE_REFLECTION();

			VertexData() : m_numVerts(0), m_vectorStride(0), m_floatStride(0), m_uint32Stride(0), m_uint16Stride(0), m_uint8Stride(0) { }

			VertexData( hkFinishLoadedObjectFlag f ) : m_vectorData(f), m_floatData(f), m_uint32Data(f), m_uint16Data(f), m_uint8Data(f) {} 

			void clear();
			void setSize(int numVerts); // make sure strides are set first

			hkArray<hkVector4> m_vectorData; // just aligned floats, so could be in m_floatData. Vector data though is normally mutable data at runtime for skinning etc so handy to keep separate too if runtime supports loading as is.
			hkArray<hkReal> m_floatData; //32 bit, like hkUint32, but handy to keep as float for read-ability in XML dumps etc
			hkArray<hkUint32> m_uint32Data;
			hkArray<hkUint16> m_uint16Data;
			hkArray<hkUint8> m_uint8Data;

			hkUint32 m_numVerts;
			hkUint32 m_vectorStride;
			hkUint32 m_floatStride;
			hkUint32 m_uint32Stride;
			hkUint32 m_uint16Stride;
			hkUint32 m_uint8Stride;
		};

	protected:
		
		struct hkxVertexBuffer::VertexData m_data;
		class hkxVertexDescription m_desc;
};

#include <Common/SceneData/Mesh/hkxVertexBuffer.inl>

#endif // HKSCENEDATA_MESH_HKXVERTEXBUFFER_HKCLASS_H

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
