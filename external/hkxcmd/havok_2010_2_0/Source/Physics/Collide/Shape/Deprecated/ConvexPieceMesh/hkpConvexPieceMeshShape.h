/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CONVEXPIECE_MESH_SHAPE_H
#define HK_COLLIDE2_CONVEXPIECE_MESH_SHAPE_H

#include <Physics/Internal/Collide/ConvexPieceMesh/hkpConvexPieceStreamData.h>

#include <Physics/Collide/Shape/Compound/Collection/hkpShapeCollection.h>
#include <Common/Base/Reflection/hkTypeInfo.h>

extern hkReal hkConvexShapeDefaultRadius;
extern const hkClass hkpConvexPieceMeshShapeClass;

/// DEPRECATED. This shape will be removed in future releases of Havok Physics.
class hkpConvexPieceMeshShape: public hkpShapeCollection
{
	//+hk.ReflectedFile("hkpConvexPieceMeshShape")
	public:

		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

			/// Constructs a new hkpConvexPieceMeshShape.
			///
			/// The inputMesh should be any hkpShapeCollection that returns hkpTriangleShape children.
			///
			/// The convexPieceStream must be created by the hkpConvexPieceMeshBuilder::convexifyLandscape method.
		hkpConvexPieceMeshShape( const hkpShapeCollection* inputMesh, const hkpConvexPieceStreamData* convexPieceStream, hkReal radius = hkConvexShapeDefaultRadius );

		~hkpConvexPieceMeshShape();

		//
		// hkpShape Collection interface
		//

			/// hkpShapeCollection interface implementation.
			/// Gets the first child shape key.
		virtual hkpShapeKey getFirstKey() const;

			/// This function implements hkpShapeCollection::getNextKey
			/// Gets the next child shape key.
		virtual hkpShapeKey getNextKey( hkpShapeKey oldKey ) const;


			///	hkpShapeCollection interface implementation.
		const hkpShape* getChildShape( hkpShapeKey key, hkpShapeBuffer& buffer ) const;

			/// Tests if the vertex given by vertexId is set in the vertex bitstream
			/// of stream.
		const hkBool vertexIsSet( const hkUint32* stream, int vertexId ) const;

			/// hkpShapeCollection interface implementation
		virtual hkUint32 getCollisionFilterInfo( hkpShapeKey key ) const;

		//
		// hkpShape interface
		//

			/// Used to pre-calculate the AABB for this shape.
		void calcAabb();

			///	hkpShape interface implementation.
			/// A precise but not very fast implementation of getting an AABB.
 		void getAabbImpl( const hkTransform& localToWorld, hkReal tolerance, class hkAabb& out  ) const;

			/// Calculates memory statistics
        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;

		//
		// Statistics retrieval interface
		//

			/// Contains statistical information about the structure of the convex piece mesh.
		struct Stats
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpConvexPieceMeshShape::Stats );

				/// The total number of triangles in the mesh.
			int m_numTriangles;

				/// The total number of convex pieces in the mesh.
			int m_numConvexPieces;

				/// The average number of triangles per convex piece.
				/// This should be used as the metric to determine how
				/// effective the builder algorithm is - the higher this value
				/// is the better.
			hkReal m_avgNumTriangles;

				/// The maximum number of triangles in any single convex piece.
			int m_maxTrianglesPerConvexPiece;

			Stats() : m_numTriangles(0), m_numConvexPieces(0), m_avgNumTriangles(0), m_maxTrianglesPerConvexPiece(0) {}
		};

			/// Fills statsOut to contains info about this mesh shape.
		void getStats( Stats& statsOut );

	public:

			/// The mesh, divided into convex pieces which are then stored in a
			/// bit stream.
		const hkpConvexPieceStreamData* m_convexPieceStream;

			/// The underlying meshShape
		const hkpShapeCollection* m_displayMesh;

		hkpConvexPieceMeshShape( hkFinishLoadedObjectFlag flag ) : hkpShapeCollection( flag )
		{
			if( flag.m_finishing )
			{
				m_type = HK_SHAPE_COLLECTION;
			}
		}

	protected:
			/// The radius used in the construction of the convex pieces.
			/// This can only be set on construction.
		hkReal m_radius;

};


#endif // HK_COLLIDE2_CONVEXPIECE_MESH_SHAPE_H

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
