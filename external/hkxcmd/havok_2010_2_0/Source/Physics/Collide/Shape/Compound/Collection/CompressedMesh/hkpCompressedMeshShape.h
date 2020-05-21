/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COMPRESSED_MESH_SHAPE_H
#define HK_COMPRESSED_MESH_SHAPE_H

#include <Physics/Collide/Shape/Compound/Collection/hkpShapeCollection.h>
#include <Physics/Collide/Shape/Compound/Collection/Mesh/hkpNamedMeshMaterial.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

extern const class hkClass hkpCompressedMeshShapeConvexPieceClass;

extern const class hkClass hkpCompressedMeshShapeBigTriangleClass;

extern const class hkClass hkpCompressedMeshShapeChunkClass;

extern const hkClass hkpCompressedMeshShapeClass;

class hkpMoppBvTreeShape;
class hkpCompressedPolytopeShape;
class hkpMeshMaterial;

#define HK_CMS_NULL_REF 0xffff

/// This shape is a shape container for triangle soups optimized for efficient memory storage.
/// Triangles are stored in chunks using both triangle strips and lists.
/// It also supports convex pieces and transformed instances (including scaling).
/// For building an hkpCompressedMeshShape from input geometries look at hkpCompressedMeshShapeBuilder.
class hkpCompressedMeshShape : public hkpShapeCollection
{
	//+hk.ReflectedFile("hkpCompressedMeshShape")
	//+version(9)
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Default constructor.
			/// The data for this shape is public, so simply fill in the
			/// member data after construction.
		hkpCompressedMeshShape( int bitsPerIndex = 17, hkReal radius = hkConvexShapeDefaultRadius );

		//
		// hkpShapeCollection interface
		//

			/// Get the first child shape key.
		virtual hkpShapeKey getFirstKey() const;

			/// Get the next child shape key.
		virtual hkpShapeKey getNextKey( hkpShapeKey oldKey ) const;

		hkpShapeKey checkShapeKey( hkpShapeKey key ) const;

		// hkpShapeCollection interface implementation.

			/// Returns an hkpTriangleShape or hkpConvexVerticesShape.
		HKP_SHAPE_VIRTUAL const hkpShape* getChildShapeImpl( HKP_SHAPE_VIRTUAL_THIS hkpShapeKey key, hkpShapeBuffer& buffer ) HKP_SHAPE_VIRTUAL_CONST;

		HKP_SHAPE_VIRTUAL hkUint32 getCollisionFilterInfoImpl( HKP_SHAPE_VIRTUAL_THIS hkpShapeKey key ) HKP_SHAPE_VIRTUAL_CONST;

			/// Gets the extra radius for every triangle.
		inline hkReal getRadius() const;

			/// Sets the extra radius for every triangle.
		inline void setRadius(hkReal r );

			// hkpShapeCollection interface implementation.
		virtual void setWeldingInfo( hkpShapeKey key, hkInt16 weldingInfo);

			// hkpShapeCollection interface implementation.
		virtual void initWeldingInfo( hkpWeldingUtility::WeldingType weldingType );

			/// Gets the material associated with the input child shape key
		const hkpMeshMaterial* getMaterial( hkpShapeKey childShapeKey ) const;

			/// Sets the number of bits out of 32 used to identify indices (triangles in chunks, big triangles or convex pieces)
		void setShapeKeyBitsPerIndex( int bitsPerIndex );

		//
		// hkpShape interface
		//

			// hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST;

        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;

			/// Returns the size of the shape class. The default implementation returns -1 which will force the shape onto the PPU.
		virtual int calcSizeForSpu(const CalcSizeForSpuInput& input, int spuBufferSizeLeft) const;


		// forward declarations
		class Chunk;
		struct ConvexPiece;

			/// Calculates the AABB of a given chunk
		void getChunkAabb( const Chunk& chunk, hkAabb& bounds );

			/// Calculates the AABB of a given convex piece
		void getConvexPieceAabb( const ConvexPiece& piece, hkAabb& bounds );

			/// Test function used to determine if the keys returned by the builder are valid
		int isValidShapeKey( hkpShapeKey key );

			/// Gets the transformed vertices of a big triangle specified by index
		HK_FORCE_INLINE void getBigTriangle( int index, hkVector4& v0, hkVector4& v1, hkVector4& v2, hkUint16* welding = HK_NULL ) const;


	public:

			/// Number of bits in the key reserved for a triangle index
		int m_bitsPerIndex;
			
			/// Number of bits in the key reserved for a triangle index and its winding
		int m_bitsPerWIndex;

			/// Mask used to get the triangle index and winding from a shape key
		int m_wIndexMask;

			/// Mask used to get the triangle index from a shape key
		int m_indexMask;

			/// The radius of the storage mesh shape. It is initialized to .05
		hkReal m_radius;

			/// A a welding type per triangle
		hkEnum<hkpWeldingUtility::WeldingType, hkUint8> m_weldingType; // +default(hkpWeldingUtility::WELDING_TYPE_NONE)

		enum MaterialType
		{
			MATERIAL_NONE,
			MATERIAL_SINGLE_VALUE_PER_CHUNK,
			MATERIAL_ONE_BYTE_PER_TRIANGLE,
			MATERIAL_TWO_BYTES_PER_TRIANGLE,
			MATERIAL_FOUR_BYTES_PER_TRIANGLE
		};

			/// Type indicating how materials are stored ( see the MaterialType enum)
		hkEnum<MaterialType, hkUint8> m_materialType;

			/// 4 byte material indices array
		hkArray<hkUint32> m_materials;

			/// 2 byte material indices array
		hkArray<hkUint16> m_materials16;

			/// 1 byte material indices array
		hkArray<hkUint8> m_materials8;

			/// A transform table used by the chunks and convex pieces for instancing
		hkArray<hkQsTransform> m_transforms;

			/// Class used to store the geometry information corresponding to a compressed mesh chunk.
		class Chunk
		{
			public:

				//+version(4)
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCompressedMeshShape::Chunk );
				HK_DECLARE_REFLECTION();
			
					/// Offset vector of the chunk
				hkVector4 m_offset;

					/// Quantized vertices
				hkArray<hkUint16> m_vertices;

					/// Triangle (strips and list) indices
				hkArray<hkUint16> m_indices;

					/// Array used to iterate through strips
				hkArray<hkUint16> m_stripLengths;

					/// Welding info per triangle
				hkArray<hkUint16> m_weldingInfo;

					/// Material info (depending on material type of the compressed mesh)
				hkUint32 m_materialInfo;

					/// Reference to another chunk - an index in the chunks list
				hkUint16 m_reference; 

					/// An index in the transform table
				hkUint16 m_transformIndex;

			public:

				Chunk()
				{
					m_reference = HK_CMS_NULL_REF;
					m_transformIndex = HK_CMS_NULL_REF;
					m_offset.setZero4();
					m_materialInfo = 0;
				}

				Chunk( const Chunk& other )
				{
					m_offset = other.m_offset;
					m_vertices = other.m_vertices;
					m_indices = other.m_indices;
					m_stripLengths = other.m_stripLengths;
					m_weldingInfo = other.m_weldingInfo;
					m_materialInfo = other.m_materialInfo;
					m_reference = other.m_reference;
					m_transformIndex = other.m_transformIndex;
				}

				Chunk( hkFinishLoadedObjectFlag flag ) : m_vertices( flag ),
					m_indices( flag ),
					m_stripLengths( flag ),
					m_weldingInfo( flag )
				{
				}

					/// Re-samples the vertices of a triangle given by its first vertex index and using the provided quantization error
				void getTriangle( int index, hkReal error, hkVector4& v1, hkVector4& v2, hkVector4& v3 ) const;

					/// Re-samples the vertices of a triangle given its vertex indices (multiplied by 3)
				void getTriangle( int a, int b, int c, hkReal error, hkVector4& v1, hkVector4& v2, hkVector4& v3 ) const;

					/// Returns the next valid triangle index after the one given and its associated winding
				int getNextIndex( int index, int& winding ) const;

					/// Calculates the number of triangles in the chunk
				int getNumTriangles() const;

					/// Re-samples all the vertices in the chunk
				void getVertices( hkReal quantization, const hkQsTransform& transform, hkArray<hkVector4>& vertices ) const;

				void decompressVertex( hkInt16 index, const hkSimdReal& error, hkVector4& v );
		};

			/// Structure used to store big triangle information (e.g., triangles that don't fit the maximum size).
		struct BigTriangle
		{
			//+version(2)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCompressedMeshShape::BigTriangle );
			HK_DECLARE_REFLECTION();
			
				/// First vertex index
			hkUint16 m_a;

				/// Second vertex index
			hkUint16 m_b;

				/// Third vertex index
			hkUint16 m_c;

				/// Up to 4 bytes for material info
			hkUint32 m_material;
			
				/// Welding info
			hkUint16 m_weldingInfo; // +default(0)

				/// An index in the transform table
			hkUint16 m_transformIndex;

			BigTriangle() : m_material( 0 ), 
				m_weldingInfo( 0 ), 
				m_transformIndex( HK_CMS_NULL_REF )
			{
			}
		};

			/// Structure used to store a compressed convex piece.
		struct ConvexPiece
		{
			//+version(3)
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCompressedMeshShape::ConvexPiece );

				/// The origin of the local space
			hkVector4 m_offset;

				/// The array of quantized vertices
			hkArray<hkUint16> m_vertices;

				/// The vertex indices describing the faces
			hkArray<hkUint8> m_faceVertices;

				/// The offsets of each face in the indices array
			hkArray<hkUint16> m_faceOffsets;

				/// Reference to another convex piece
			hkUint16 m_reference; 

				/// Index in the transform table
			hkUint16 m_transformIndex;

			ConvexPiece()
			{
				m_reference = HK_CMS_NULL_REF;
				m_transformIndex = HK_CMS_NULL_REF;
				m_offset.setZero4();
			}

				/// Copy constructor
			ConvexPiece( const ConvexPiece& other )
			{
				m_offset = other.m_offset;
				m_vertices = other.m_vertices;
				m_faceVertices = other.m_faceVertices;
				m_faceOffsets = other.m_faceOffsets;
				m_reference = other.m_reference;
				m_transformIndex = other.m_transformIndex;
			}

				/// Finish constructor
			ConvexPiece( hkFinishLoadedObjectFlag flag ) : m_vertices( flag ),
				m_faceVertices( flag ),
				m_faceOffsets( flag )
			{
			}

				/// Return the decompressed vertices of the convex piece
			void getVertices( hkReal quantization, const hkQsTransform& transform, hkArray<hkVector4>& vertices ) const;

			static void getFaceNormals(const void* object, hkArray<hkVector4>& normals);
		};
		
			/// Vertices that form the big triangles (snapped to quantization grid)
		hkArray<hkVector4> m_bigVertices;
		
			/// Big triangles left over after chunking and quantization
		hkArray<struct BigTriangle> m_bigTriangles;

			/// The list of mesh chunks
		hkArray<class Chunk> m_chunks;

			/// The list of convex pieces
		hkArray<struct ConvexPiece> m_convexPieces;

			/// The quantization error
		hkReal m_error;

			/// Field used to store the AABB of the mesh
		class hkAabb m_bounds;

			/// Collision filter info used if there is no material set.
			/// Note if you set this value to hkpGroupFilter::USE_COLLIDABLE_FILTER_INFO, and no material info
			/// is set, than the hkpGroupFilter collision filter info will be taken from the rootCollidable
		hkUint32 m_defaultCollisionFilterInfo;

			/// Pointer to hkpMeshMaterial (or derived) table.
		hkpMeshMaterial* m_meshMaterials; //+nosave +default(0)

			/// The striding in the material table.
		hkUint16 m_materialStriding;

			/// The number of materials
		hkUint16 m_numMaterials;

			/// Array of named materials used by the export filter.
			/// If materials are exported, the m_meshMaterial pointer will point
			/// to the contents of this array.
		hkArray<class hkpNamedMeshMaterial> m_namedMaterials;

		static const int MAX_CONVEX_VERTICES = 32;
		static const int MAX_CONVEX_FACES = 64;

		HK_ALIGN16( static hkVector4 m_spuPlanes[MAX_CONVEX_FACES] );

		static hkpGetFaceNormalsFunc m_getFaceNormalsFunc;

	public:

			/// Finish constructor
		hkpCompressedMeshShape( hkFinishLoadedObjectFlag flag );

};


#include <Physics/Collide/Shape/Compound/Collection/CompressedMesh/hkpCompressedMeshShape.inl>

#endif //HK_COMPRESSED_MESH_SHAPE_H

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
