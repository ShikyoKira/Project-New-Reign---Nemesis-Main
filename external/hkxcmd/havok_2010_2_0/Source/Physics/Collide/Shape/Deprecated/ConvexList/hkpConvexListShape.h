/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CONVEX_LIST_SHAPE_H
#define HK_COLLIDE2_CONVEX_LIST_SHAPE_H

#include <Physics/Collide/Shape/hkpShapeContainer.h>
#include <Physics/Collide/Shape/Convex/hkpConvexShape.h>

extern const hkClass hkpConvexListShapeClass;

/// A simple static list of hkpShapes. You can use this shape class to create compound bodies.
/// A list shape can hold a mix of different shape types, however the radius of all shapes must be
/// identical (see below) which may limit its use in some cases.
/// The convex list shape is very good for smaller objects, which are made of a few convex pieces.
/// You will get a very good CPU for this convex list, as long as the hkpConvexListShape is
/// only colliding with its convex hull. A typical example would be a car made of several parts.
/// If your object looks more like a spaceship, where other bodies may frequently be 'inside' the hull
/// you should consider using hkpListShape wrapped in a hkpMoppBvTreeShape.
/// N.B. In either case, there is still a potential for a performance impact hit with complex compound bodies, so
/// it is advisable to contact Havok support to discuss optimum representation.
/// Note: This implementation has some limitations:
///     - the number of child objects is restricted to 255
///     - the number of vertices of each child object is restricted to 255 (see user guide)
///     - per-childshape collision filtering is not possible (for either collisions or raycasts)
///     - The radius of all child objects must be equal. For this reason you cannot usually combine
///       hkpSphereShapes, hkpCapsuleShapes and hkpConvexVerticesShapes in hkpConvexListShapes.
///
/// Please see the user guide for more details on the hkpConvexListShape
class hkpConvexListShape : public hkpConvexShape, public hkpShapeContainer
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Constructs a list shape with an array of pointers to shapes.
		hkpConvexListShape( const hkpConvexShape*const* shapeArray, int numShapes );

			/// The destructor removes references to child shapes.
		~hkpConvexListShape();

			/// Set whether you want to use a cached version of the AABB for getAabb().
			/// The default is true.
			/// If this is set to true a cached AABB of the children's AABBs will be used,
			/// otherwise getAabb will query all children and combine their AABBs.
			/// If true, the AABB returned is bigger, but is much faster to evaluate.
		void setUseCachedAabb( bool useCachedAabb );

			/// Get whether you want to use a cached version of the AABB for getAabb().
			/// The default is true.
			/// If this is set to true a cached AABB of the children's AABBs will be used,
			/// otherwise getAabb will query all children and combine their AABBs.
			/// If true, the AABB returned is bigger, but is much faster to evaluate.
		bool getUseCachedAabb();


			/// Returns the i'th child shape.
		inline const hkpShape* getChildShapeInl(int i) const { return m_childShapes[i]; }

		//
		// hkpConvexShape interface
		//

			// hkpConvexShape::getSupportingVertex() interface implementation.
		void getSupportingVertexImpl( hkVector4Parameter dir, hkpCdVertex& supportingVertexOut ) const;

			// Implemented method of hkpConvexShape
		void convertVertexIdsToVerticesImpl( const hkpVertexId* ids, int numIds, hkpCdVertex* verticesOut) const;

			// Implemented method of hkpConvexShape
		virtual void getFirstVertex(hkVector4& v) const;

		//
		// hkpSphereRepShape interface
		//

			// Implemented method of hkpSphereRepShape
		int getNumCollisionSpheresImpl( )  const;

			// Implemented method of hkpSphereRepShape
		const hkSphere* getCollisionSpheresImpl( hkSphere* sphereBuffer ) const;



		//
		// hkpShapeContainer interface
		//

			// hkpShapeContainer interface implementation.
		virtual int getNumChildShapes() const;

			/// Get the first child shape key.
		virtual hkpShapeKey getFirstKey() const;

			/// Get the next child shape key.
		virtual hkpShapeKey getNextKey( hkpShapeKey oldKey ) const;

			/// hkpShapeCollection interface implementation. Always returns 0, and warns, because you cannot filter hkConvexListShapes.
		virtual hkUint32 getCollisionFilterInfo( hkpShapeKey key ) const;

			/// See hkpShapeCollection::getChildShape().
			/// Note that a hkpListShape does not use the char* buffer for its returned shape.
		HKP_SHAPE_VIRTUAL const hkpShape* getChildShape( HKP_SHAPE_VIRTUAL_THIS hkpShapeKey key, hkpShapeBuffer& buffer ) HKP_SHAPE_VIRTUAL_CONST;


		//
		// hkpShape interface
		//

			// Implemented method of hkpShape
		virtual void getAabbImpl( const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) const;

			// Implemented method of hkpShape
		virtual hkBool castRayImpl( const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results ) const;

			// Implemented method of hkpShape
		virtual void castRayWithCollectorImpl( const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const;

			// Implemented method of hkpShape
		virtual const hkpShapeContainer* getContainer() const;
			// Implemented method of hkReferencedObject
        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;



	protected:

		void setShapesAndRadius( const hkpConvexShape*const* shapeArray, int numShapes );

	public:

		hkpConvexListShape( class hkFinishLoadedObjectFlag flag ) : hkpConvexShape(flag), m_childShapes(flag) { m_type = HK_SHAPE_CONVEX_LIST; }

	public:


			/// A distance which is used for the getClosestPoint() call. If the distance between
			/// your query object and the convex hull is bigger than
			/// the value of this member, the function only returns this distance.
			/// Otherwise it recursively checks its children
		hkReal	m_minDistanceToUseConvexHullForGetClosestPoints;

	protected:

		hkVector4	m_aabbHalfExtents;
		hkVector4	m_aabbCenter;
		bool m_useCachedAabb; //+default(false)

	public:

		hkArray<const hkpConvexShape*> m_childShapes;


};


#endif // HK_COLLIDE2_CONVEX_LIST_SHAPE_H

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
