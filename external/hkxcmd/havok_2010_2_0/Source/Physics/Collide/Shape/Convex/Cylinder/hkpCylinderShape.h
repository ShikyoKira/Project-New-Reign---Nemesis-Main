/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CYLINDER_SHAPE_H
#define HK_COLLIDE2_CYLINDER_SHAPE_H

#include <Physics/Collide/Shape/Convex/hkpConvexShape.h>

extern const hkClass hkpCylinderShapeClass;

	/// A cylinder defined by two points and a radius.
	/// The points define the centers of the circular ends of the cylinder.
	///
	/// Note, that with default settings the cylinder's radius must be not larger than ~35times the collision tolerance
	/// ( radius of 3.5 meters, with default collision tolerance). Otherwise jitter and 'popping' can be noticed for rolling cylinders.
	/// To workaround you can increase tessellation of cylinders (see below),
	/// or consider increasing collision tolerance (if you're using non-standard settings). (More details in HVK-5156.)
	///
	/// hkpCylinderShape is not modeled as an implicit geometry (hkpSphereShape and hkpCapsuleShape are implicitly modeled).
	/// hkpCylinderShape is represented by vertices generated on the cylinder's surface. Those vertices
	/// are efficiently generated on-the-fly, and do not take up extra memory.
	/// They are referred to as being virtual.
	///
	/// This virtual tessellation causes instability in collision response. For that reason
	/// hkpCylinderShape uses a specialized collision agent, hkPredGskCylinderAgent3.
	/// hkPredGskCylinderAgent3 uses an implicit capsule representation
	/// when the cylinder lies on its side or rolls on a flat surface.
	/// There is a simple mechanism that checks the cylinder's axis angle with the contact normal,
	/// once the angle passes a specified threshold, the object's representation is altered.
	///
	/// As hkPredGskCylinderAgent3 alters the geometrical representation of the cylinder, it reduces the contact information
	/// and this reduction may result in objects tunneling through each other. In a test where a 1cm-radius cylinder collided against
	/// a triangle, and the cylinder was constantly pushed towards the triangle's surface with very high impulses,
	/// the cylinder tunneled through after a few seconds of simulation. Generally, under 'normal' conditions, the hkPredGskCylinderAgent3
	/// yields stable results.
	///
	/// hkPredGskCylinderAgent3 is not used when an hkpCylinderShape is enclosed in an hkpShapeCollection
	/// hierarchy e.g., when an hkpCylinderShape is part of an hkpListShape.
	///
	/// A problem + work-around for such cases:
	///
	/// hkpCylinderShape is tessellated, not implicit like hkpSphereShape or hkpCapsuleShape. This leads
	/// to an issue with cylinders rolling on their sides which may cause them to penetrate other
	/// objects or have incorrect momentum properties. To work around this problem, use a list
	/// shape consisting of an hkpCylinderShape and an hkpCapsuleShape. The hkpCylinderShape ensures
	/// that the list shape can stand on its ends. The hkpCapsuleShape ensures that the list
	/// shape rolls properly. You will want to use a minimally tessellated cylinder sub-shape.
	///
	/// Note that this workaround does not work for cylinders that are flat e.g., coins. In these
	/// cases the capsule ends protrude from the faces of the coin.
	///
	/// Remember to disable hkPredGskCylinderAgent3 if you want to use this workaround for all cases.
	///
	/// You can vary the tessellation of cylinders by calling hkpCylinderShape::setNumberOfSideSegments().
	/// This indicates how many facets should make up the round side of the cylinder (see the function's description).
	///
	/// N.B. Changing this parameter affects ALL hkCylinderShapes. It should only be changed
	/// before creating the world. Changing it with hkCylinderShapes in the world can cause an assert in the
	/// convex collision agent.
class hkpCylinderShape : public hkpConvexShape
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		HK_DECLARE_GET_SIZE_FOR_SPU(hkpCylinderShape);

			/// Creates a new hkpCylinderShape using the specified points and radius
		hkpCylinderShape(	const hkVector4& vertexA,
							const hkVector4& vertexB,
							hkReal cylinderRadius,
							hkReal paddingRadius = hkConvexShapeDefaultRadius );

			/// Gets the pointer to the first vertex. This casts the corresponding hkSphere (m_vertexA) to a hkVector4*.
			/// You can then index by 0 or 1, to get the first or second vertex respectively.
		inline const hkVector4* getVertices() const;

			/// Gets a vertex given an index "i". "i" can be 0 or 1. This casts the corresponding hkSphere to a hkVector4.
		HK_FORCE_INLINE const hkVector4& getVertex(int i) const;

			/// Sets a vertex given an index "i". "i" can be 0 or 1.
		HK_FORCE_INLINE void setVertex(int i, const hkVector4& position );

			/// Returns cylinder radius (don't use getRadius)
		hkReal getCylinderRadius() const;

			/// Returns cylinder radius (don't use setRadius)
		void setCylinderRadius(const hkReal radius);

			/// Tells by how much the spheres representing the circumference of each of the
			/// cylinder's basis are moved towards the cylinder's axis of symmetry.
			///
			/// This is only used for collisions against heightfield.
			///
			/// When colliding against a heightfield, the sphere representation of the shape is used.
			/// The cylinder shape is represented by 16 spheres distributed uniformly on the basis (8 spheres/per base)
			/// and two extra spheres -- one in the center of each base. They all have the radius of equal to the
			/// bounding m_radius of the shape.
			///
			/// If the cylinder is long enough to fit a sphere with the radius equal to the radius of the cylinder
			/// (i.e., e.g., not in the case of a coin), than the two spheres in the centers of the bases are 'blown up'.
			/// Their radius is equal to the radius of the cylinder (i.e., equal to m_cylRadius + m_radius), and their centers
			/// are moved inwards just enough that they don't stick out of the cylinder.
			/// Those large spheres allow better rolling behavior.
			///
			/// The cylinder-base-radius factor is useful when a cylinder rolls over an uneven landscape heightfield.
			/// In such cases, due to the landscape's curvature, the cylinder may easily rest on
			/// the edges of its basis. This will make it harder to roll (because the two large spheres
			/// used for rolling are not toughing the landscape, and the base edge's representation uses
			/// only 8 spheres).
			///
			/// Meaning of the parameter's value:
			/// Factor of 1.0f means that the spheres are not moved.
			/// Factor of 0.0f means that all the spheres representing a base are moved
			/// onto the symmetry axis of the cylinder (effectively converting the cylinder
			/// The spheres are only shifted in the plane perpendicular to the cylinder's axis.
		hkReal getCylinderBaseRadiusFactorForHeightfieldCollisions(hkReal radiusFactor) { return m_cylBaseRadiusFactorForHeightFieldCollisions; }

			/// Tells by how much the spheres representing the circumference of each of the
			/// cylinder's basis are moved towards the axis.
			///
			/// See hkReal getCylinderBaseRadiusFactorForSphereCollisionInfo(hkReal radiusFactor) for
			/// detailed info.
		inline void setCylinderBaseRadiusFactorForHeightfieldCollisions(hkReal radiusFactor) { m_cylBaseRadiusFactorForHeightFieldCollisions = radiusFactor; }


		//
		// hkpConvexShape implementation
		//

			// hkpConvexShape::getSupportingVertex() interface implementation.
		HKP_SHAPE_VIRTUAL void getSupportingVertexImpl( HKP_SHAPE_VIRTUAL_THIS hkVector4Parameter direction, hkpCdVertex& supportingVertexOut ) HKP_SHAPE_VIRTUAL_CONST;

			// hkpConvexShape interface implementation.
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

			// hkpConvexShape interface implementation.
			/// Returns the first vertex of this shape. This is only used for initialization of collision detection data.
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

			/// Sets the number of side segments (facets) for the cylinder. This parameter is set globally for all cylinders.
			/// Note that virtual tessellation is not uniform.
			/// Input value is rounded down to a multiple of 8. Values between 8 and 128 are accepted.
		static void HK_CALL setNumberOfVirtualSideSegements(int numSegments);

	public:
			/// Returns a unit vector perpendicular to the axis of the cylinder.
		void presetPerpendicularVector();

	private:

		HK_FORCE_INLINE static void HK_CALL initRoundUpThreshold();

		HK_FORCE_INLINE	// don't inline on an spu
		void decodeVertexId(hkpVertexId code, hkVector4& result) const;

	protected:
		// The cylinder's radius (as opposed to the bounding volume radius associated with hkpConvexShape
		hkReal    m_cylRadius;

		hkReal m_cylBaseRadiusFactorForHeightFieldCollisions; //+default(0.8f)

		// The line's first point.
		hkVector4 m_vertexA;

		// The line's second point.
		hkVector4 m_vertexB;

		// Arbitrary vector perpendicular to the axis
		hkVector4 m_perpendicular1;
		hkVector4 m_perpendicular2;

	public:

		hkpCylinderShape( hkFinishLoadedObjectFlag flag );


		enum VertexIdEncoding
		{
			VERTEX_ID_ENCODING_IS_BASE_A_SHIFT     = 7,
			VERTEX_ID_ENCODING_SIN_SIGN_SHIFT      = 6,
			VERTEX_ID_ENCODING_COS_SIGN_SHIFT      = 5,
			VERTEX_ID_ENCODING_IS_SIN_LESSER_SHIFT = 4,
			VERTEX_ID_ENCODING_VALUE_MASK          = 0x0f
		};

	private:
		static hkReal s_virtualTesselationParameter;
		static hkReal s_virtualTesselationParameterInv;
		static hkReal s_intRoundUpThreshold;
};

#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.inl>

#endif // HK_COLLIDE2_CYLINDER_SHAPE_H

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
