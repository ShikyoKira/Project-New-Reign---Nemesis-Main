/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MESHWELDINGUTILITY_H
#define HK_MESHWELDINGUTILITY_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Physics/Collide/Util/Welding/hkpWeldingUtility.h>

class hkpBvTreeShape;
class hkpTriangleShape;
class hkpShapeCollection;


	/// Utility functions for building runtime welding information for triangular meshes
class hkpMeshWeldingUtility
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpMeshWeldingUtility);
			/// specify consistency tests
		enum WindingConsistency
		{
			WINDING_IGNORE_CONSISTENCY,		///< ignore consistency
			WINDING_TEST_CONSISTENCY,		///< test consistency and warn if inconsistent triangle winding
		};

			/// The single shape version of computeWeldingInfoMultiShape. See comments for that function for details. This function
			/// will not take multiple meshes into account and will not weld across edges between meshes.
		static void HK_CALL computeWeldingInfo( hkpShapeCollection* collection, const hkpBvTreeShape* bvTree, hkpWeldingUtility::WeldingType weldingType, 
			bool weldOpenEdges = true, bool disableEdges = false );

			
		struct ShapeInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpMeshWeldingUtility::ShapeInfo);
			hkTransform m_transform;
			const hkpBvTreeShape* m_shape;
		};
	
			/// Computes welding information for the mesh while taking into account other mesh shapes. This will compute welding
			/// information so that welding works across the mesh shapes provided in 'allShapes'. 'allShapes' should include an
			/// entry for the mesh being passed in. Only triangles can be welded. Therefore only triangle collections
			/// (hkpSimpleMeshShape, hkpExtendedMeshShape, hkpTriSampledHeightFieldBvTreeShape and hkpCompressedMeshShape)
			/// are handled. This method must be called after all child shapes (subparts or triangles) have
			/// been added to the collection. The hkpMoppBvTreeShape you pass in must be built referencing this collection.
			/// Welding information adds an additional 2 bytes per triangle storage overhead. This is an expensive call,
			/// and should be done off line, and the resultant collection serialized, to save the runtime overhead of
			/// computing the welding info.
		static void HK_CALL computeWeldingInfoMultiShape( const hkTransform& meshTransform, hkpShapeCollection* mesh, hkpWeldingUtility::WeldingType weldingType, 
			hkArray< ShapeInfo >& allShapes, bool weldOpenEdges = true, bool disableEdges = false );		


	public:


			// Helper to extract a single bitcode for a specific edge
		static int HK_CALL calcSingleEdgeBitcode(hkUint16 triangleEdgesBitcode, int edgeIndex );

			// Helper for calcWeldingInfoForTriangle
		static hkUint16 HK_CALL modifyCombinedEdgesBitcode(hkUint16 combinedBitcode, int edgeIndex, int bitcode);

			// Helper for calcWeldingInfoForTriangle
		static int HK_CALL calcEdgeAngleBitcode(hkReal angle);

			// Helper for calcWeldingInfoForTriangle
		static void HK_CALL calcAntiClockwiseTriangleNormal(const hkVector4& vertex0, const hkVector4& vertex1, const hkVector4& vertex2, hkVector4& normal);

			// Helper for computeWeldingInfo 
		static hkReal HK_CALL calcAngleForEdge(hkVector4Parameter edgeNormal, hkVector4Parameter edgeOrtho, hkVector4Parameter triangleNormal);

			// Helper for computeWeldingInfoMultiShape
		static hkResult HK_CALL testWindingConsistency( hkVector4Parameter localVertex0, hkVector4Parameter localEdgeOrtho, hkVector4Parameter localNormal, const hkpTriangleShape* adjacentTriangle, hkVector4Parameter adjacentNormal );


	public:

		//
		// DEPRECATED FUNCTIONS
		//

			/// This function is deprecated, use computeWeldingInfo instead
			/// A function to generate welding information for a shape in a mesh. This welding should be
			/// stored by the mesh and set as the welding information in the triangle returned by getChildShape() with that shape key.
		static hkResult HK_CALL calcWeldingInfoForTriangle( hkpShapeKey shapeKey, const hkpBvTreeShape* bvTreeShape, WindingConsistency testConsistency, hkUint16& info );

			/// This function is deprecated, use computeWeldingInfo instead
			/// A function to check if winding is consistent between a triangle and its neighbors.
		static hkBool HK_CALL isTriangleWindingValid( hkpShapeKey shapeKey, const hkpBvTreeShape* bvTreeShape );

			// Helper for calcWeldingInfoForTriangle
		static hkResult HK_CALL calcBitcodeForTriangleEdge(const hkpBvTreeShape* bvTreeShape, const hkpTriangleShape* triangleShape, hkpShapeKey triangleShapeKey, int edgeIndex, WindingConsistency testConsistency, hkInt16& combinedBitcodesOut );

			// Helper for calcWeldingInfoForTriangle
		static int HK_CALL createSingularVertexArray(const hkVector4 *vertices0, const hkVector4 *vertices1, int edgeIndex, hkVector4* vertexArrayOut, int orderedEdgeVerticesOnTriangle1[2]);
		
			// Helper for calcWeldingInfoForTriangle
		static int HK_CALL calcEdgeAngleBitcode(const hkVector4* vertices);

			// Helper for calcWeldingInfoForTriangle
		static hkReal HK_CALL calcAngleFromVertices(const hkVector4* vertices, hkReal& sinAngleOut, hkReal& cosAngleOut);

};

#endif // HK_MESHWELDINGUTILITY_H

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
