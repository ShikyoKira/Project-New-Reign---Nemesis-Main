/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CONVEX_VERTICES_SHAPE_H
#define HK_COLLIDE2_CONVEX_VERTICES_SHAPE_H

#include <Physics/Collide/Shape/Convex/hkpConvexShape.h>

extern const class hkClass hkpConvexVerticesShapeFourVectorsClass;

extern const hkClass hkpConvexVerticesShapeClass;

struct hkStridedVertices;
class hkpConvexVerticesConnectivity;

/// You can use this shape class to create a convex geometric object by specifying a set of vertices.
/// Specify the vertices in the shape's local space. You must also provide the planes of the convex hull, which
/// can be computed using the hkGeometryUtility::createConvexGeometry function (see the SimpleShapesDemo for an example).
class hkpConvexVerticesShape : public hkpConvexShape
{
	//+hk.ReflectedFile("hkpCompressedMeshShape")
	//+version(3)

	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();
		HK_DECLARE_GET_SIZE_FOR_SPU(hkpConvexVerticesShape);

		// 4 vectors stored transposed in the "columns" not the rows
		struct FourVectors
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpConvexVerticesShape::FourVectors );
			HK_DECLARE_REFLECTION();

			HK_DECLARE_POD_TYPE();

			hkVector4 m_x;
			hkVector4 m_y;
			hkVector4 m_z;
		};

		/// Build configuration
		struct BuildConfig
		{
			BuildConfig();
			hkBool	m_createConnectivity;							///< Create connectivity (default: false).
			hkBool	m_shrinkByConvexRadius;							///< Shrink vertices by convex radius (default: true), so that raycast and physics collisions are consistent.
			hkBool	m_useOptimizedShrinking;						///< Optimized shrinking such as the number of vertices after shrinking is less or equal to the unshrunk shape vertices count.
			hkReal	m_convexRadius;									///< Convex radius (default: hkConvexShapeDefaultRadius).
			int		m_maxVertices;									///< Maximum vertices, used for simplification (default: 0, no simplification).
			hkReal	m_maxRelativeShrink;							///< Maximum relative shrink factor (default: 0.05) (relative to the AABB of the object).
			hkReal	m_maxShrinkingVerticesDisplacement;				///< Maximum vertices displacement due to shrinking (default: 0.07).
			hkReal  m_maxCosAngleForBevelPlanes;					///< If the cos angle (== dot product) of two neighboring face normals is smaller than this, a bevel plane is inserted to optimize raycast behavior (default -0.1f)
		};

	public:

			/// Construct the shape from the given vertices and matching plane equations.
			/// These are plane equations of the convex hull and can be generated
			/// using the hkGeometryUtility::createConvexGeometry method.
			/// This constructor makes an internal copy of the vertices.
			/// You should take care of not passing in unnecessary vertices, e.g., inner vertices or
			/// duplicated vertices. hkGeometryUtility::createConvexGeometry will also give
			/// you back a clean list of vertices to use. See our Havok demos
		hkpConvexVerticesShape(const hkStridedVertices& vertsIn, const hkArray<hkVector4>& planeEquations, hkReal radius = hkConvexShapeDefaultRadius);

		hkpConvexVerticesShape(	hkReal radius = hkConvexShapeDefaultRadius );
			
			/// Create from precomputed data.
			/// Note that numVertices is the actual number of vertices, not the
			/// number of FourVectors structures.
		hkpConvexVerticesShape( FourVectors* rotatedVertices, int numVertices,
				hkVector4* planes, int numPlanes,
				const hkAabb& aabb, hkReal radius = hkConvexShapeDefaultRadius );			

			/// Create a shape from the given vertices, automatically set planes equations and connectivity.
			/// Also shrink the shape vertices so that physics and raycast are consistent.
			/// Notes: This is the recommended way to create a convex vertices shape.
		hkpConvexVerticesShape(	const hkStridedVertices& vertices, const BuildConfig& config=BuildConfig());

		hkpConvexVerticesShape( hkFinishLoadedObjectFlag flag );

			/// Dtor
		~hkpConvexVerticesShape();

			/// The hkpConvexVerticesShape stores the vertices in optimized form.
			/// This function will retrieve them into the vertices array.
			/// It copies the vertices so is able to be a const method.
		void getOriginalVertices( hkArray<hkVector4>& vertices ) const;

			/// Returns the plane equations passed into the constructor
		const hkArray<hkVector4>& getPlaneEquations() const;
			
			/// Overwrite plane equations passed into the constructor
		void setPlaneEquations( const hkArray<hkVector4>& planes );

			/// convenience function to convert this shape to a new space
		void transformVerticesAndPlaneEquations( const hkTransform& t );

			/// Overwrite only the convex radius without caring about plane equations or shape shrinking.
		void setRadiusUnchecked( hkReal radius ) { m_radius = radius; }
		
		//
		// hkpConvexShape implementation
		//

			// hkpConvexShape::getSupportingVertex() interface implementation.
		HKP_SHAPE_VIRTUAL void getSupportingVertexImpl( HKP_SHAPE_VIRTUAL_THIS hkVector4Parameter direction, hkpCdVertex& supportingVertexOut ) HKP_SHAPE_VIRTUAL_CONST;

			// hkpConvexShape interface implementation.
			/// Note: You must be able to supply one valid vertex corresponding to ID 0.
			/// This is a requirement because getFirstVertex() is not supported on SPU.
		HKP_SHAPE_VIRTUAL void convertVertexIdsToVerticesImpl( HKP_SHAPE_VIRTUAL_THIS const hkpVertexId* ids, int numIds, hkpCdVertex* verticesOut) HKP_SHAPE_VIRTUAL_CONST;

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

			// hkpConvexShape interface implementation
		void getFirstVertex(hkVector4& v) const;

			// Get the connectivity. This is optional for this shape. Returns HK_NULL if connectivity is not present.
		inline const hkpConvexVerticesConnectivity* getConnectivity() const { return m_connectivity; }

			// Set the connectivity. Setting to HK_NULL will remove connectivity information.
            // The connectivity information is a 'cache' of information - so can be used to modify a const shape
        void setConnectivity(const hkpConvexVerticesConnectivity* connect, bool sort=true);

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

		void copyVertexData(const float* vertexIn, int byteStriding, int numVertices);

	protected:

		void sortPlanes(void);

	public:

		hkVector4	m_aabbHalfExtents;
		hkVector4	m_aabbCenter;

	//protected:

		//hkInplaceArray<FourVectors, 3> m_rotatedVertices;
		hkArray<struct FourVectors> m_rotatedVertices;
		hkInt32		m_numVertices;
		
		// a pointer to compressed convex data
		const void* m_externalObject;  //+default(0) +nosave

		hkpGetFaceNormalsFunc m_getFaceNormals; //+default(0) +nosave

		// declared mutable so they can be computed on the fly inside castRay()
		mutable hkArray<hkVector4> m_planeEquations;

		// Optional data that provides the vertex connectivity that defines this shape
		// set to HK_NULL if connectivity information is not present
		// (Ref counted)
        mutable const hkpConvexVerticesConnectivity* m_connectivity;
		
};


#endif // HK_COLLIDE2_CONVEX_VERTICES_SHAPE_H

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
