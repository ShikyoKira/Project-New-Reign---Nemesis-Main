/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_TRIANGLE_SHAPE_H
#define HK_COLLIDE2_TRIANGLE_SHAPE_H

#include <Physics/Collide/Shape/Convex/hkpConvexShape.h>
#include <Physics/Collide/Util/Welding/hkpWeldingUtility.h>

extern const hkClass hkpTriangleShapeClass;

class hkpGskCache;

/// A triangle shape with its details stored as an hkGeometry::Triangle.
/// This shape is typically created at runtime, for example from the hkpMeshShape. You should use the hkpMeshShape, or
/// a variant on it to store a permanent collection of triangles.
class hkpTriangleShape : public hkpConvexShape
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		HK_DECLARE_GET_SIZE_FOR_SPU(hkpTriangleShape);

			/// Default constructor
		HK_FORCE_INLINE hkpTriangleShape( hkReal radius = hkConvexShapeDefaultRadius, hkUint16 weldingInfo = 0, hkpWeldingUtility::WeldingType type = hkpWeldingUtility::WELDING_TYPE_NONE);

			/// Constructor that sets the points of the triangle.
		HK_FORCE_INLINE hkpTriangleShape(const hkVector4& v0, const hkVector4& v1, const hkVector4& v2, hkReal radius = hkConvexShapeDefaultRadius );

			/// Get a pointer to the vertices of the triangle.
			/// Returns the hkGeometry::Triangle.
		HK_FORCE_INLINE const hkVector4* getVertices() const;

			/// Get a non const reference to a vertex.
			/// The parameter "i" must be 0, 1 or 2
		HK_FORCE_INLINE hkVector4& getVertex(int i);

			/// Get a const reference to a vertex.
			/// The parameter "i" must be 0, 1 or 2
		HK_FORCE_INLINE const hkVector4& getVertex(int i) const;

			/// Set a vertex
			/// The parameter "i" must be 0, 1 or 2
		HK_FORCE_INLINE void setVertex(int i, const hkVector4& vertex);

			//
			// Welding Info
			//

			/// Get the welding info for this triangle
		HK_FORCE_INLINE hkUint16 getWeldingInfo() const;

			/// Set the welding info for this triangle
		HK_FORCE_INLINE void setWeldingInfo( hkUint16 info );



			/// Get the welding type for the triangle
		HK_FORCE_INLINE hkpWeldingUtility::WeldingType getWeldingType() const;

			/// Set the welding type for the triangle
		HK_FORCE_INLINE void setWeldingType( hkpWeldingUtility::WeldingType type );


		HK_FORCE_INLINE bool isExtruded() const;

		HK_FORCE_INLINE const hkVector4& getExtrusion() const;

		HK_FORCE_INLINE void setExtrusion( const hkVector4& extrusion );



		//
		// hkpConvexShape implementation
		//

			// hkpConvexShape::getSupportingVertex() interface implementation.
		HKP_SHAPE_VIRTUAL void getSupportingVertexImpl( HKP_SHAPE_VIRTUAL_THIS hkVector4Parameter direction, hkpCdVertex& supportingVertexOut ) HKP_SHAPE_VIRTUAL_CONST;

			// hkpConvexShape::convertVertexIdsToVertices() interface implementation.
		HKP_SHAPE_VIRTUAL void convertVertexIdsToVerticesImpl( HKP_SHAPE_VIRTUAL_THIS const hkpVertexId* ids, int numIds, hkpCdVertex* verticesOut) HKP_SHAPE_VIRTUAL_CONST;

			// hkpConvexShape interface implementation.
		HKP_SHAPE_VIRTUAL int weldContactPointImpl( HKP_SHAPE_VIRTUAL_THIS hkpVertexId* featurePoints, hkUint8& numFeaturePoints, hkVector4& contactPointWs, const hkTransform* thisObjTransform, const hkpConvexShape* collidingShape, const hkTransform* collidingTransform, hkVector4& separatingNormalInOut ) HKP_SHAPE_VIRTUAL_CONST;

			// hkpConvexShape interface implementation.
		HKP_SHAPE_VIRTUAL void getCentreImpl( HKP_SHAPE_VIRTUAL_THIS hkVector4& centreOut ) HKP_SHAPE_VIRTUAL_CONST;

		//
		// hkpSphereRepShape implementation
		//

			// hkpSphereRepShape interface implementation.
		HKP_SHAPE_VIRTUAL int getNumCollisionSpheresImpl( HKP_SHAPE_VIRTUAL_THIS2 ) HKP_SHAPE_VIRTUAL_CONST;

			// hkpSphereRepShape interface implementation.
		HKP_SHAPE_VIRTUAL const hkSphere* getCollisionSpheresImpl( HKP_SHAPE_VIRTUAL_THIS hkSphere* sphereBuffer ) HKP_SHAPE_VIRTUAL_CONST;

		//
		// hkpShape implementation
		//
			// hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST;

			//	hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL hkBool castRayImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results) HKP_SHAPE_VIRTUAL_CONST;

		//	hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL hkVector4Comparison castRayBundleImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayBundleCastInput& input, hkpShapeRayBundleCastOutput& results, hkVector4ComparisonParameter mask ) HKP_SHAPE_VIRTUAL_CONST;
		
			//	hkpConvexShape interface implementation.
		virtual void getFirstVertex(hkVector4& v) const;

		//
		// hkpShape implementation
		//

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerSimulationFunctions )( ShapeFuncs& sf );
			
			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerCollideFunctions_Narrowphase )( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerCollideQueryFunctions )( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerRayCastFunctions )( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerGetAabbFunction )( ShapeFuncs& sf );

        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;


	protected:

		hkUint16 m_weldingInfo; //+default(0)
		hkEnum<hkpWeldingUtility::WeldingType, hkUint8> m_weldingType; // +default(hkpWeldingUtility::WELDING_TYPE_NONE)
		hkUint8 m_isExtruded;

		hkVector4 m_vertexA;
		hkVector4 m_vertexB;
		hkVector4 m_vertexC;

		hkVector4 m_extrusion;

	public:

		void setType() { m_type = HK_SHAPE_TRIANGLE; }
		hkpTriangleShape( hkFinishLoadedObjectFlag flag ) : hkpConvexShape( flag ) { m_type = HK_SHAPE_TRIANGLE; }

};

#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.inl>

#endif // HK_COLLIDE2_TRIANGLE_SHAPE_H

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
