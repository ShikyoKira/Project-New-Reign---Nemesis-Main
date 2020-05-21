/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COMPRESSED_MESH_SHAPE_BUILDER_H
#define HK_COMPRESSED_MESH_SHAPE_BUILDER_H

#include <Physics/Collide/Shape/Compound/Collection/CompressedMesh/hkpCompressedMeshShape.h>
#include <Common/Base/Types/Geometry/hkGeometry.h>
#include <Physics/Collide/Shape/Compound/Collection/CompressedMesh/hkTjunctionDetector.h>


class hkpCompressedMeshShapeBuilder
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpCompressedMeshShapeBuilder);
		//
		// User interface
		//

			/// Static method that wraps the functionality of the builder in a simple call.
			/// You need to specify a quantization error and a material type (none, 1, 2 or 4 bytes per triangle).
		static hkpCompressedMeshShape* HK_CALL createMeshFromGeometry( const hkGeometry& geometry, hkReal quantizationError, 
			hkpCompressedMeshShape::MaterialType materialType );

			/// Default constructor
		hkpCompressedMeshShapeBuilder() : 
			m_overlapRatio( 0.4f ),
			m_stripperPasses( 1 ),
			m_weldVertices( false ),
			m_preserveTjunctions( true ),
			m_TjunctionTolerance( 1e-4f ),
			m_weldTolerance( 1e-3f ),
			m_createMapping( false )
		{

		}

			/// Creates an hkpCompressedMeshShape object and performs basic initialization for both the shape and the builder.
			/// You need to specify a quantization error and a material type (none, 1, 2 or 4 bytes per triangle).
			/// Optionally you can specify the number of bits in the shape key used for storing chunk triangles indices (default is 17).
		hkpCompressedMeshShape* createMeshShape( hkReal quantizationError, hkpCompressedMeshShape::MaterialType materialType, int bitsPerIndex = 17 );

			/// Appends an input geometry along with its transform to the compressed mesh shape.
			/// The transform is applied before quantization.
			/// Optional, you can get a mapping between the initial triangles and their corresponding shape keys.
		void addGeometry( const hkGeometry& geometry, const hkMatrix4& transform, hkpCompressedMeshShape* compressedMesh );

			/// Appends a convex piece (an array of vertices) to the compressed mesh shape.
			/// Optional, you can get the convex geometry constructed from the vertices.
		bool addConvexPiece( const hkArray<hkVector4>& vertices, hkpCompressedMeshShape* compressedMesh );
		bool addConvexPiece( const hkGeometry& geometry, hkpCompressedMeshShape* compressedMesh );

			/// Marks the beginning of a new subpart (the following geometries and convex pieces to be added)
			/// Returns the ID of the subpart to be used with addInstance
		int beginSubpart( hkpCompressedMeshShape* compressedMesh );

			/// Marks the end of the current subpart. Make sure to call it after beginSuppart.
		void endSubpart( hkpCompressedMeshShape* compressedMesh );

			/// Adds a new instance given the referenced geometry/subpart and a transform (translation and rotation)
		void addInstance( int subpartID, const hkMatrix4& transform, hkpCompressedMeshShape* compressedMesh, hkArray<hkpShapeKey>* shapeKeyMap = HK_NULL );

			/// Fills in all the fields in the m_statistics data member.
		void gatherStatistics( hkpCompressedMeshShape* meshShape );

		//
		// Internal functions ( not guaranteed to work separate from the builder )
		//

		// forward declaration
		class MappingTree;

			/// Splits a given geometry into two output geometries and one left over geometry
			/// \param dir the longest axis of the AABB wrapping the geometry
			/// \param half the median point along that axis
			/// \param mapping optional parameter for build the mapping tree to the original triangles
		void splitGeometry( const hkGeometry& source, hkGeometry& out1, hkGeometry& out2,
			int dir, hkReal half, MappingTree* mapping );

			/// Splits a given geometry into two output geometries and one left over geometry
		void splitGeometry( const hkGeometry& source, hkGeometry& out1, hkGeometry& out2, hkGeometry& leftOver );

			/// Snaps the geometry to a grid given by the quantization error and preserves T-junctions
		void snapGeometry( MappingTree* mapping );

		//
		// Static helper functions
		//

			///// Converts an hkpCompressedMeshShape to an hkGeometry (obsolete)
		//static hkGeometry* HK_CALL createGeometry( hkpCompressedMeshShape* shape );
		

			/// Converts a chunk to an hkGeometry given its index in the compressed mesh
		static void HK_CALL chunkToGeometry( hkpCompressedMeshShape* compressedMesh, int index, hkGeometry& geometry );

			/// Converts a convex piece to an hkGeometry given its index in the compressed mesh
		static void HK_CALL convexPieceToGeometry( const hkpCompressedMeshShape* compressedMesh, int index, hkGeometry& geometry );

			/// Converts all big triangles in the compressed mesh to an hkGeometry
		static void HK_CALL bigTrianglesToGeometry( const hkpCompressedMeshShape* compressedMesh, hkGeometry& geometryOut );

			/// Snaps the geometry to a grid given by the quantization error
		static void HK_CALL snapGeometry( hkGeometry& geometry, hkReal quantizationError );

			/// Snaps a vector to the grid given by the quantization error
		static void HK_CALL snapToGrid( hkVector4& v, hkReal error );

			/// Appends a geometry to a destination one
		void appendGeometry( hkGeometry& dest, const hkGeometry& geom );

			/// Quantizes a set of vertices given a quantization error and the grid origin
		static void HK_CALL quantizeVertices( hkReal quantization, const hkVector4& offset, const hkArray<hkVector4>& verticesIn, 
			hkArray<hkUint16>& verticesOut );
	public:

		// Input data - these data members should be set before building the compressed mesh shape
		
			/// The quantization error
		hkReal m_error;

			/// The fraction of max extent permitted for a chunk triangle
		hkReal m_overlapRatio;

			/// The number of search passes for the triangle stripper
		int m_stripperPasses;

			/// Switch that enables welding of the vertices before quantization
		bool m_weldVertices;

			/// Switch that enables preserving of T-junctions
		bool m_preserveTjunctions;

			/// The tolerance used to detect T-junctions
		hkReal m_TjunctionTolerance;

			/// The tolerance used to detect/weld vertices
		hkReal m_weldTolerance;

			///// The way materials are stored in the compressed mesh
		//hkpCompressedMeshShape::MaterialType m_materialType;


		// Output data - these can be inspected after the mesh was built

			/// The left over geometry (i.e., the big triangles)
		hkGeometry m_leftOver;

			/// Array used to store the T-junctions
		hkArray<hkTjunctionDetector::ProximityInfo> m_Tjunctions;

			/// Array used to store the welded vertices (vertices that are very close to each other before the quantization)
		hkArray<hkVector4> m_weldedVertices;

			/// Structure used to gather statistics data
		struct Statistics
		{
				/// The number of vertices in the input geometry
			int m_numVertices;

				/// The number of triangles in the input geometry
			int m_numTriangles;

				/// The maximum extent of a chunk allowed by the quantization error
			hkReal m_maxExtent;

				/// The maximum index that can be stored in the triangles
			int m_maxIndex;

				/// The quantization error
			hkReal m_error;

				/// The number of big left over triangles
			int m_numBigTriangles;

				/// The number of vertices used to represent the big triangles
			int m_numBigVertices;

				/// The total number of excess (duplicated) vertices
			int m_numExcessVertices;

				/// The number of mesh chunks
			int m_numChunks;

				/// The size in bytes of the mesh
			int m_size;

				/// The ratio between size and a reference size (uncompressed triangle lists)
			hkReal m_compressRatio;

				/// The total number of triangle strips
			int m_numStrips;

				/// The maximum overlap between two chunks
			hkReal m_maxOverlap;

				/// The number of degenerate triangles
			int m_numDegenerate;

			int m_chunksSize;

			int m_convexPiecesSize;

			int m_bigTrianglesSize;

			int m_numChunkClones;

			Statistics() : m_numVertices( 0 ), 				
				m_numTriangles( 0 ),
				m_maxExtent( 0.0f ),
				m_maxIndex( 0 ),
				m_error( 0.0f ),
				m_numBigTriangles( 0 ),
				m_numBigVertices( 0 ),
				m_numExcessVertices( 0 ),
				m_numChunks( 0 ),
				m_size( 0 ),
				m_compressRatio( 0.0f ),
				m_numStrips( 0 ),
				m_maxOverlap( HK_REAL_MIN ),
				m_numDegenerate( 0 ),
				m_numChunkClones( 0 )
			{
			}
		};

			/// Internal field used to gather some of the statistics data
		Statistics m_statistics;

		struct TriangleMapping
		{
				/// The index of the triangle in the original mesh
			int m_originalIndex;

				/// The offset of the first index of the triangle
			int m_triangleIndex;
				
				/// The index in the final chunk list
			int m_chunkIndex;

				/// The winding of the triangle
			int m_winding;

				/// The resulting shape key
			hkpShapeKey m_key;

				/// Default constructor for member initialization
			TriangleMapping(): m_originalIndex(-1), m_triangleIndex(-1), m_chunkIndex(-1), 
				m_winding(-1), m_key(HK_INVALID_SHAPE_KEY)
			{
			}
		};

			/// Intermediate structure used to store the mapping between the original triangles and the resulting ones
		class MappingTree
		{
			public:

				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCompressedMeshShapeBuilder::MappingTree );

					/// Default constructor
				MappingTree() : m_left( HK_NULL ), m_right( HK_NULL ) 
				{
				}

					/// Destructor (the tree does its own memory management)
				~MappingTree()
				{
					if ( m_left )
						delete m_left;
					if ( m_right )
						delete m_right;
				}

					/// Builds the mapping array ( original index -> shape key )
				void getKeys( hkArray<hkpShapeKey>& keys, const int keysBase );

				int getNumKeys()
				{
					int numKeys = m_triangles.getSize();
					if ( m_left != HK_NULL && m_right != HK_NULL )
					{
						numKeys += m_left->getNumKeys();
						numKeys += m_right->getNumKeys();
					}
					return numKeys; 
				}

			public:

					/// Mapping data per triangle
				hkArray<TriangleMapping> m_triangles;

					/// Left sub-tree
				MappingTree* m_left;

					/// Right sub-tree
				MappingTree* m_right;

		};

			/// The mapping table for the left over triangles
		hkArray<TriangleMapping> m_bigMapping;

		struct Subpart
		{
			// number of instances
			int m_numInstances;

			// chunk sequence
			int m_chunkOffset, m_numChunks;

			// big triangles sequence
			int m_bigOffset, m_numBigTriangles;

			// convex pieces sequence
			int m_convexOffset, m_numConvexPieces;

			// shape key mapping sequence
			int m_geomOffset, m_numTriangles;

		};

			/// Structures used for creating instances
		hkArray<Subpart> subparts;

			/// Optional mapping between original triangles and resulting shape keys
		hkArray<hkpShapeKey> m_shapeKeys;

			/// Activates creation of shape keys mapping
		bool m_createMapping;

 	protected:

			/// Recursive function that adds geometries to the chunk list and stores the big triangles in the left over geometry
		void addChunk( const hkGeometry& geometry, hkArray<hkpCompressedMeshShape::Chunk>& chunks, MappingTree* mapping );

			/// Creates a chunk from a given geometry and offset
		void createChunk( const hkGeometry& geometry, const hkVector4& offset, hkpCompressedMeshShape::Chunk& chunk,
			MappingTree* mapping );

			/// Split criterion 2: mid point test
		int splitCriterion( const hkVector4& v1, const hkVector4& v2, const hkVector4& v3, hkReal half, int dir );

			/// Adds the big (left over) triangles and snaps to grid their vertices
		int addBigTriangles( hkpCompressedMeshShape* meshShape );

#ifdef HK_DEBUG
			/// Helper function used to test an intermediate mapping array
		void testMapping( const hkGeometry& testGeom, const hkArray<TriangleMapping>& triMap );

			/// Used by testMapping or separately
		void testVertices( int originalIndex, const hkArray<hkVector4>& vertices, int a, int b, int c);
#endif // HK_DEBUG

			/// Filters unwanted triangles from the input geometry
		void filterGeometry( MappingTree* mapping );

			/// Adds a triangle to the global list
		void addLeftOverTriangle( const hkVector4& v0, const hkVector4& v1, const hkVector4& v2, int material, TriangleMapping* map );

		void addMaterial( hkpCompressedMeshShape::Chunk& chunk, const int material );

	protected:
		// Internal data

			/// The source geometry (used to test the mappings)
		hkGeometry m_geometry;

		HK_ON_DEBUG( hkGeometry m_originalGeometry );

		hkpCompressedMeshShape* m_mesh;

		int m_TjunctionsBase;

};

#endif //HK_COMPRESSED_MESH_SHAPE_BUILDER_H

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
